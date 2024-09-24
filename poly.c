#include <GL/glut.h>
#include <stdio.h>



GLubyte red, green, blue;
int COLORS_DEFINED;

const int WINDOW_POSITION = 100;
const int WINDOW_MAX = 800;

const float WORLD_COORDINATE_MIN = 0.0;
const float WORLD_COORDINATE_MAX = 800.0;

typedef GLfloat point[2]; 


typedef struct VectorPoint
{
	point p;

	struct VectorPoint* next;
	struct VectorPoint* prev;
}VectorPoint;


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
	glClearColor(1.0, 1.0, 1.0, 1.0); // white background 
	glColor3f(1.0, 0.0, 0.0); // draw in red
	glPointSize(10.0);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(WORLD_COORDINATE_MIN, WORLD_COORDINATE_MAX, 
				WORLD_COORDINATE_MIN, WORLD_COORDINATE_MAX);
	glMatrixMode(GL_MODELVIEW);

}

void display(void)
{
	/* define a point data type */
	VectorPoint vec;
    

    glClear(GL_COLOR_BUFFER_BIT);  /*clear the window */

    if (!COLORS_DEFINED) {
       red   = 255;
       green = 0; 
       blue  = 0;
    }

    glColor3ub( red, green, blue ); 

	/*
    // define point

    vec.p[0] = 100; 
    vec.p[1] = 100;
   
    // plot new point 

    glBegin(GL_POINTS);
        glVertex2fv(vec.p); 
    glEnd();

	*/

     
    glFlush(); /* clear buffers */
}



void drawBox(int x, int y)
{
	VectorPoint vec;

	glColor3f(red, green, blue);

	vec.p[0] = x;
    vec.p[1] = WINDOW_MAX - y;

	glBegin(GL_POINTS);
        glVertex2fv(vec.p); 
    glEnd();

    glFlush();
}

void clearBox()
{
    glClear(GL_COLOR_BUFFER_BIT); 
    glFlush();
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		printf("%d	%d\n", x, y);
		drawBox(x, y);
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// TODO
		//		Connect last point with first point
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		// TODO
		//		Clear linked list

		printf ("%d   %d\n", x, y);
        clearBox();
	}
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 'q' || key == 'Q') exit(0);

	if (key == 'f' || key == 'F')
	{
		// TODO
		//		Draw filled polygon without tesselate
	}
	if (key == 't' || key == 'T')
	{
		// TODO
		//		Tesselate w/ areas of each triangle
	}
	if (key == 'p' || key == 'P')
	{
		// TODO
		//		Fill and Tesselate
	}
	if (key == 'l' || key == 'L')
	{
		// TODO
		//		Restore to initial state (no fill and no tes)
	}

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
