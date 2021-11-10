#include <bits/stdc++.h>
#include <gl/freeglut.h>

// ウィンドウの初期位置と初期サイズ
#define INIT_X_POS 128
#define INIT_Y_POS 128
#define INIT_WIDTH 512
#define INIT_HEIGHT 512

// ウィンドウの縦横サイズ
unsigned int window_width, window_height;

// 画像の表示範囲
double left, right, bottom, top;

// 座標値参照の際のインデックス
#define X 0
#define Y 1

// 点
#define MAX_NUM_POINTS 10000
double point[MAX_NUM_POINTS][2];
unsigned int num_points;

// 表示モード
#define DISPLAY_POINTS 0
#define DISPLAY_CONES 1
unsigned int display_mode = DISPLAY_POINTS;

// 円周率
#define PI 3.141592653589793

// OpenGL関係の初期設定
void initGL(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // 背景色は白
	glEnable(GL_DEPTH_TEST);              // デプスバッファ機能を使用
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glDisable(GL_LIGHTING);               // 照明は用いない
}

// ボロノイ母点の生成
// [0, window_width]x[0, window_height]範囲内にランダムにnum個の点を生成し，これを母点とする
void genPoints(unsigned int num) {
	double x, y;

	// numがMAX_NUM_POINTSを超える場合には，それ以下に修正
	if (num > MAX_NUM_POINTS) {
		printf("Too many points to generate.");
		num = MAX_NUM_POINTS;
	}

	// 点はウィンドウ全体にばらまかれるように生成
	num_points = 0;
	while (true) {
		if (num_points >= num) {
			left = bottom = 0.0;
			right = (double)window_width;
			top = (double)window_height;
			return;
		}
		x = window_width * ((double)rand() / (double)RAND_MAX);
		y = window_height * ((double)rand() / (double)RAND_MAX);
		point[num_points][X] = x;
		point[num_points][Y] = y;
		num_points++;
	}
}

// ボロノイ母点の表示
void displayPoints(void) {
	unsigned int i;
	
	// 点のサイズ指定
	glPointSize(4.0f);  
	glBegin(GL_POINTS);
	for (i = 0; i < num_points; i++) {
		glColor3d(1.0, 0.0, 0.0);
		glVertex3d(point[i][X], point[i][Y], 0.0);
	}
	glEnd();
}

// ボロノイ母点下に配置された円錐形の描画
void displayCone(double peak_point[])
// double peak_point[]: xy平面上の点の座標 これを頂点とするxy平面の下側に描く
{
	int i;
	double x, y, radius;

	// ウィンドウ内に中心をもつ円がウィンドウを覆いつくすために必要な半径
	radius = static_cast<double>(sqrt((double)(window_width * window_width + window_height * window_height)) * 1.1);

	// peak_pointを頂点とする円錐を描く
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(peak_point[X], peak_point[Y], 0.0);
	for (i = 0; i <= 360; i++) {
		x = radius * cos((i % 360) / 180.0 * PI);
		y = radius * sin((i % 360) / 180.0 * PI);
		glVertex3d(peak_point[X] + x, peak_point[Y] + y, (-radius));
	}
	glEnd();
}

// 指定したIDに対応した色(r, g, b)を与える処理
void IDToColor(unsigned int id)
// unsigned int id; ID値
{
	GLubyte r, g, b;
	unsigned int tmp;
	r = (GLubyte)(id / 65536); // 65536 = 256 * 256
	tmp = id % 65536;
	g = (GLubyte)(tmp / 256);
	b = (GLubyte)(tmp % 256);
	glColor3ub(r, g, b);
}

// 色から対応するIDを得る処理
unsigned int colorToID(GLubyte r, GLubyte g, GLubyte b)
// GLubyte r, g, b 色データ
{
	return(r * 65536 + g * 256 + b);
}

// 円錐形状群の表示
void displayCones(void)
{
	unsigned int i;
	for (i = 0; i < num_points; i++)
	{
		IDToColor(i);
		displayCone(point[i]);
	}
}

// 表示
void display(void) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left, right, bottom, top, -1.0, 2000.0);
	glViewport(0, 0, window_width, window_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// モード(今後増えてく)
	switch (display_mode) {
		case DISPLAY_POINTS:
			displayPoints();
			break;
		case DISPLAY_CONES:
			displayCones();
			displayPoints();
			break;
		default:
			break;
	}
	glFlush();
}

// キー処理
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'q':
		case 'Q':
		case '\033': // Esc
			exit(0);

			// ランダムな点群の生成
		case 'g':
		case 'G':
			genPoints(1000);
			display_mode = DISPLAY_POINTS;
			glutPostRedisplay();
			break;
			// ボロノイ図の生成
		case 'v':
		case 'V':
			display_mode = DISPLAY_CONES;
			glutPostRedisplay();
			break;
		default:
			break;
	}
}

// ウィンドウサイズの変更に対応するコールバック関数
void resize(int width, int height) {
	unsigned int old_width, old_height;
	double d;

	// 現在のウィンドウサイズを確保
	old_width = window_width;
	old_height = window_height;

	// ウィンドウサイズを大域変数にしまいなおす
	window_width = width;
	window_height = height;

	// ウィンドウサイズの変更分を実サイズに置き換えて描画範囲を補正
	d = ((int)window_width - (int)old_width) * 0.5;
	right += d;
	left -= d;
	d = ((int)window_height - (int)old_height) * 0.5;
	top += d;
	bottom -= d;
}

int main(int argc, char *argv[]) 
{
	glutInitWindowPosition(INIT_X_POS, INIT_Y_POS);
	glutInitWindowSize(INIT_WIDTH, INIT_HEIGHT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Voronoi Diagram");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	initGL();
	glutMainLoop();
	return 0;
}