#include <GL/glut.h>
#include <stdio.h>

const int WINDOW_POSITION_X = 100;
const int WINDOW_POSITION_Y = 100;
const int WINDOW_MAX_X = 400;
const int WINDOW_MAX_Y = 400;

void myglutInit(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_MAX_X, WINDOW_MAX_Y);
	glutInitWindowPosition(WINDOW_POSITION_X, WINDOW_POSITION_Y);
	glutCreateWindow("Polygon Tesselation");
}

void myInit(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Insert gluOrtho2D
	glMatrixMode(GL_MODELVIEW);

}

void display(void)
{
	
}

void mouse(int button, int state, int x, int y)
{
	
}

void keyboard(unsigned char key, int x, int y)
{
		
}

int main(int argc, char** argv)
{
	myglutInit(argc,argv);
	myInit();

	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMainLoop();
}
