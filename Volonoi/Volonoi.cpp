#include <iostream>
#include <math.h>
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

// OpenGL�֌W�̏����ݒ�
void initGL(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glDisable(GL_LIGHTING);
}

// �{���m�C��_�̐���
// [0, window_width]x[0, window_height]��͎��Ȃ��Ƀ����_����num�̓_�𐶐����C������_�Ƃ���
void genPoints(unsigned int num) {
	double x, y;

	// num��MAX_NUM_POINTS�𒴂���ꍇ�ɂ́C����ȉ��ɏC��
	if (num > MAX_NUM_POINTS) {
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

// �\�����[�h
#define DISPLAY_POINTS 0
unsigned int display_mode = DISPLAY_POINTS;

// �\��
void display(void) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentitiy();
	glOrtho(left, right, bottom, top, -1.0, 2000.0);
	glViewPort(0, 0, window_width, window_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ���[�h(���㑝���Ă�)
	switch (display_mode) {
		case DISPLAY_POINTS:
			displayPoints();
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
		case '\033':
			exit(0);

			// �����_���ȓ_�Q�̐���
		case 'g':
		case 'G':
			genPoints(1000);
			display_mode = DISPLAY_POINTS;
			glutPostRedeisplay();
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

int main(int argc, char *argv[]) {
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