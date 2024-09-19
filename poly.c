#include <GL/glut.h>
#include <stdio.h>

struct points 
{

};

GLubyte red, green, blue;
int COLORS_DEFINED;

const int WINDOW_POSITION = 100;
const int WINDOW_MAX = 800;

const float WORLD_COORDINATE_MIN = 0.0;
const float WORLD_COORDINATE_MAX = 800.0;


void myglutInit(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_MAX, WINDOW_MAX);
	glutInitWindowPosition(WINDOW_POSITION, WINDOW_POSITION);
	glutCreateWindow("Polygon Tesselation");
}

void myInit(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(WORLD_COORDINATE_MIN, WORLD_COORDINATE_MAX, 
				WORLD_COORDINATE_MIN, WORLD_COORDINATE_MAX);
	glMatrixMode(GL_MODELVIEW);

}

void display(void)
{
	/* define a point data type */

    typedef GLfloat point[2];     

    point p; /* A point in 2-D space */

    glClear(GL_COLOR_BUFFER_BIT);  /*clear the window */

    if (!COLORS_DEFINED) {
       red   = 255;
       green = 0; 
       blue  = 0;
    }

    glColor3ub( red, green, blue ); 

    /* define point */

    p[0] = 100; 
    p[1] = 100;
   
    /* plot new point */

    glBegin(GL_POINTS);
        glVertex2fv(p); 
    glEnd();

     
    glFlush(); /* clear buffers */
}

void mouse(int button, int state, int x, int y)
{
	
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 'q' || key == 'Q') exit(0);

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
