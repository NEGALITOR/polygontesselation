#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <GL/glut.h>

using std::cout;

void placeRandom();

GLsizei wh = 900 ; // initial height of window
GLsizei ww = 900 ; // initial width of window

void Displaydot ( void ) {
    glClear ( GL_COLOR_BUFFER_BIT );

	double linepts[2];
	double vertices[8][2];
	double theta = 22.5;
	double r = 400;

	for (int i = 0; i < 8; i++)
	{
		vertices[i][0] = r * cos(theta * M_PI/180);
		vertices[i][1] = r * sin(theta * M_PI/180);
		theta = theta + 45.0;
		printf(" %f8.2  %f8.3 \n", vertices[i][0], vertices[i][1]);
	}

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
		for (int i = 0; i < 8; i++)
		{
			linepts[0] = 450 + vertices[i][0];
			linepts[1] = 450 + vertices[i][1];
			glVertex2f(linepts[0], linepts[1]);
		}
	glEnd();
    glFlush();
}

void MyInit ( void ) {
    cout << glGetString(GL_VERSION) <<"\n";
    cout << glGetString(GL_VENDOR) <<"\n";
    glClearColor ( 1.0, 1.0, 1.0, 0.0 ); //white background
    glColor3f(0.0f, 1.0f, 0.0f); // green drawing colour
    glPointSize(10.0); // 10 pixel dot!
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity ( ) ;
    gluOrtho2D ( 0.0, (GLdouble)ww, 0.0, (GLdouble)wh );
}

int main(int argc, char **argv) {
    glutInit ( &argc, argv );
    glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize ( ww, wh ); // window size
    glutInitWindowPosition ( 180, 90 ); // & position on screen
    glutCreateWindow ( "Display a Dot" );
    MyInit ( );
    glutDisplayFunc ( Displaydot );
    glutMainLoop ( );
}

