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

VectorPoint* head;
VectorPoint* tail;


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
	//VectorPoint vec;
    

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
	glColor3f(red, green, blue);

	//VectorPoint vec;
	//printf("| %p |\n", &vec);

	if (head == NULL) 
	{
		head = (VectorPoint*) malloc(sizeof(VectorPoint));
		tail = head;
		//printf("Pointer: %d\n", *head);
		//printf("Pointer: %d\n", *tail);

		head->next = head;
		head->prev = head;
		tail->next = head;
		tail->prev = head;

		//printf("Pointer: %d\n", head->next);
		//printf("Pointer: %d\n", head->prev);

		printf("updated head\n");
	}
	else
	{
		tail->next = (VectorPoint*) malloc(sizeof(VectorPoint));
		tail->next->prev = tail;
		tail = tail->next;

		// Make Linked List Circular
		tail->next = head;
		head->prev = tail;
		
		printf("updated tail\n");
		printf("%p %p\n\n", (void *) tail->prev, (void *) tail);
	}

	

	tail->p[0] = x;
    tail->p[1] = WINDOW_MAX - y;

	
	printf("-----------------------------------------\n");
	printf("Clicked Point: %d     %d\n", x, y);
	printf("Point: %.0f    %.0f\n", tail->p[0], tail->p[1]);
	printf("Head: %.0f    %.0f\nTail Prev: %.0f    %.0f | Tail Curr %.0f    %.0f | Tail Next %.0f    %.0f\n", 
			head->p[0], head->p[1], tail->prev->p[0], tail->prev->p[1], tail->p[0], tail->p[1], tail->next->p[0], tail->next->p[1]);
	printf("-----------------------------------------\n\n");
	

	glBegin(GL_POINTS);
        glVertex2fv(tail->p); 
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
