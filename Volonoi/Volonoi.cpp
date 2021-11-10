#include <bits/stdc++.h>
#include <gl/freeglut.h>

// �E�B���h�E�̏����ʒu�Ə����T�C�Y
#define INIT_X_POS 128
#define INIT_Y_POS 128
#define INIT_WIDTH 512
#define INIT_HEIGHT 512

// �E�B���h�E�̏c���T�C�Y
unsigned int window_width, window_height;

// �摜�̕\���͈�
double left, right, bottom, top;

// ���W�l�Q�Ƃ̍ۂ̃C���f�b�N�X
#define X 0
#define Y 1

// �_
#define MAX_NUM_POINTS 10000
double point[MAX_NUM_POINTS][2];
unsigned int num_points;

// �\�����[�h
#define DISPLAY_POINTS 0
#define DISPLAY_CONES 1
unsigned int display_mode = DISPLAY_POINTS;

// �~����
#define PI 3.141592653589793

// OpenGL�֌W�̏����ݒ�
void initGL(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // �w�i�F�͔�
	glEnable(GL_DEPTH_TEST);              // �f�v�X�o�b�t�@�@�\���g�p
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glDisable(GL_LIGHTING);               // �Ɩ��͗p���Ȃ�
}

// �{���m�C��_�̐���
// [0, window_width]x[0, window_height]�͈͓��Ƀ����_����num�̓_�𐶐����C������_�Ƃ���
void genPoints(unsigned int num) {
	double x, y;

	// num��MAX_NUM_POINTS�𒴂���ꍇ�ɂ́C����ȉ��ɏC��
	if (num > MAX_NUM_POINTS) {
		printf("Too many points to generate.");
		num = MAX_NUM_POINTS;
	}

	// �_�̓E�B���h�E�S�̂ɂ΂�܂����悤�ɐ���
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

// �{���m�C��_�̕\��
void displayPoints(void) {
	unsigned int i;
	
	// �_�̃T�C�Y�w��
	glPointSize(4.0f);  
	glBegin(GL_POINTS);
	for (i = 0; i < num_points; i++) {
		glColor3d(1.0, 0.0, 0.0);
		glVertex3d(point[i][X], point[i][Y], 0.0);
	}
	glEnd();
}

// �{���m�C��_���ɔz�u���ꂽ�~���`�̕`��
void displayCone(double peak_point[])
// double peak_point[]: xy���ʏ�̓_�̍��W ����𒸓_�Ƃ���xy���ʂ̉����ɕ`��
{
	int i;
	double x, y, radius;

	// �E�B���h�E���ɒ��S�����~���E�B���h�E�𕢂��������߂ɕK�v�Ȕ��a
	radius = static_cast<double>(sqrt((double)(window_width * window_width + window_height * window_height)) * 1.1);

	// peak_point�𒸓_�Ƃ���~����`��
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(peak_point[X], peak_point[Y], 0.0);
	for (i = 0; i <= 360; i++) {
		x = radius * cos((i % 360) / 180.0 * PI);
		y = radius * sin((i % 360) / 180.0 * PI);
		glVertex3d(peak_point[X] + x, peak_point[Y] + y, (-radius));
	}
	glEnd();
}

// �w�肵��ID�ɑΉ������F(r, g, b)��^���鏈��
void IDToColor(unsigned int id)
// unsigned int id; ID�l
{
	GLubyte r, g, b;
	unsigned int tmp;
	r = (GLubyte)(id / 65536); // 65536 = 256 * 256
	tmp = id % 65536;
	g = (GLubyte)(tmp / 256);
	b = (GLubyte)(tmp % 256);
	glColor3ub(r, g, b);
}

// �F����Ή�����ID�𓾂鏈��
unsigned int colorToID(GLubyte r, GLubyte g, GLubyte b)
// GLubyte r, g, b �F�f�[�^
{
	return(r * 65536 + g * 256 + b);
}

// �~���`��Q�̕\��
void displayCones(void)
{
	unsigned int i;
	for (i = 0; i < num_points; i++)
	{
		IDToColor(i);
		displayCone(point[i]);
	}
}

// �\��
void display(void) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left, right, bottom, top, -1.0, 2000.0);
	glViewport(0, 0, window_width, window_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ���[�h(���㑝���Ă�)
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

// �L�[����
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'q':
		case 'Q':
		case '\033': // Esc
			exit(0);

			// �����_���ȓ_�Q�̐���
		case 'g':
		case 'G':
			genPoints(1000);
			display_mode = DISPLAY_POINTS;
			glutPostRedisplay();
			break;
			// �{���m�C�}�̐���
		case 'v':
		case 'V':
			display_mode = DISPLAY_CONES;
			glutPostRedisplay();
			break;
		default:
			break;
	}
}

// �E�B���h�E�T�C�Y�̕ύX�ɑΉ�����R�[���o�b�N�֐�
void resize(int width, int height) {
	unsigned int old_width, old_height;
	double d;

	// ���݂̃E�B���h�E�T�C�Y���m��
	old_width = window_width;
	old_height = window_height;

	// �E�B���h�E�T�C�Y����ϐ��ɂ��܂��Ȃ���
	window_width = width;
	window_height = height;

	// �E�B���h�E�T�C�Y�̕ύX�������T�C�Y�ɒu�������ĕ`��͈͂�␳
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