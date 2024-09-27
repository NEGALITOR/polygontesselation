#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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

    glClear(GL_COLOR_BUFFER_BIT);  /*clear the window */

    if (!COLORS_DEFINED) {
       red   = 255;
       green = 0; 
       blue  = 0;
    }

    glColor3ub( red, green, blue ); 

     
    glFlush(); /* clear buffers */
}

int countNodes()
{
	VectorPoint* curr = head;

	if (curr == NULL)
	{
		//printf("0 | ");
		return 0;
	}

	int count = 1;

	while (curr != tail)
	{
		count++;
		curr = curr->next;
	}
	//printf("%d | ", count);
	return count;
}

GLfloat determinent(GLfloat matrix[2][2])
{
	GLfloat det = (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
	printf("Determinent: %.4f\n", det);
	return (det);
}

int checkIntersect(VectorPoint* vec)
{
	VectorPoint* curr = head;

	GLfloat x1 = tail->p[0], x2 = vec->p[0], x3, x4, a;
	GLfloat y1 = tail->p[1], y2 = vec->p[1], y3, y4, b;
	GLfloat ua, ub;

	GLfloat A[2][2];
	GLfloat Ai[2][2];

	A[0][0] = x2 - x1;
	A[1][0] = y2 - y1;

	for (int i = 0; i < countNodes()-1; i++)
	{
		x3 = curr->p[0];
		x4 = curr->next->p[0];
		y3 = curr->p[1];
		y4 = curr->next->p[1];
		printf("Line %d\n-----------\n", i+1);
		printf("x1: %.0f y1: %.0f | x2: %.0f y2: %.0f | x3: %.0f y3: %.0f | x4: %.0f y4: %.0f\n\n", x1, y1, x2, y2, x3, y3, x4, y4);

		A[0][1] = -(x4 - x3);
		A[1][1] = -(y4 - y3);

		printf("Matrix A: %.0f %.0f\n          %.0f %.0f\n", A[0][0], A[0][1], A[1][0], A[1][1]);

		//Ai = A;
		memcpy(Ai, A, 4 * sizeof(GLfloat));

		GLfloat detA = determinent(A);

		a = x3 - x1;
		b = y3 - y1;
		printf("a: %.0f, b: %.0f\n", a, b);

		Ai[0][0] = a;
		Ai[1][0] = b;
		printf("Matrix A1: %.0f %.0f\n          %.0f %.0f\n", Ai[0][0], Ai[0][1], Ai[1][0], Ai[1][1]);
		ua = determinent(Ai) / detA;


		//Ai = A;
		memcpy(Ai, A, 4 * sizeof(GLfloat));
		Ai[0][1] = a;
		Ai[1][1] = b;
		printf("Matrix A2: %.0f %.0f\n          %.0f %.0f\n", Ai[0][0], Ai[0][1], Ai[1][0], Ai[1][1]);
		ub = determinent(Ai) / detA;

		printf("ua: %f, ub: %f\n\n", ua, ub);

		curr = curr->next;

		if ((ua > 0 && ua < 1) && (ub > 0 && ub < 1))
			return 1;
	}
	
	
	return 0;

}

void drawPoly(int x, int y)
{
	glColor3f(red, green, blue);

	//VectorPoint vec;
	//printf("| %p |\n", &vec);

	printf("\n-----------------------------------------\n");

	if (head == NULL) 
	{
		head = (VectorPoint*) malloc(sizeof(VectorPoint));
		tail = head;
		//printf("Pointer: %d\n", *head);
		//printf("Pointer: %d\n", *tail);

		head->next = head;
		head->prev = head;
		//tail->next = head;
		//tail->prev = head;

		head->p[0] = x;
		head->p[1] = WINDOW_MAX - y;

		//printf("Pointer: %d\n", head->next);
		//printf("Pointer: %d\n", head->prev);

		printf("updated head\n");
	}
	else
	{
		VectorPoint* vec = (VectorPoint*) malloc(sizeof(VectorPoint));

		vec->p[0] = x;
		vec->p[1] = WINDOW_MAX - y;

		// Check Intersection
		if (countNodes() > 2 && checkIntersect(vec) == 1)
		{
			printf("INTERSECTED!!!!!\n\n");
			return;
		}

		tail->next = vec;
		tail->next->prev = tail;
		tail = tail->next;
		
		
		printf("updated tail\n");
		printf("%p %p\n\n", (void *) tail->prev, (void *) tail);
	}

	
	
	printf("Clicked Point: %d     %d\n", x, y);
	printf("Point: %.0f    %.0f\n", tail->p[0], tail->p[1]);
	//printf("Head: %.0f    %.0f\nTail Prev: %.0f    %.0f | Tail Curr %.0f    %.0f | Tail Next %.0f    %.0f\n", 
	//		head->p[0], head->p[1], tail->prev->p[0], tail->prev->p[1], tail->p[0], tail->p[1], tail->next->p[0], tail->next->p[1]);
	printf("Head: %.0f    %.0f\nTail Prev: %.0f    %.0f | Tail Curr %.0f    %.0f\n", 
			head->p[0], head->p[1], tail->prev->p[0], tail->prev->p[1], tail->p[0], tail->p[1]);
	printf("-----------------------------------------\n\n");
	
	glBegin(GL_POINTS);
		glVertex2fv(tail->p);
	glEnd();

	if (head != tail)
	{
		glBegin(GL_LINES);
			glVertex2fv(tail->prev->p);
			glVertex2fv(tail->p);
		glEnd();
	}
	

    glFlush();
}

void connectEnds()
{
	if (countNodes() < 3)
	{
		printf("Not enough verticies to connect\n");
		return;
	}

	tail->next = head;
	head->prev = tail;

	glBegin(GL_LINES);
		glVertex2fv(tail->p);
		glVertex2fv(head->p);
	glEnd();

	glFlush();
}

void clearBox()
{
	VectorPoint* curr = head;
	VectorPoint* next;

	if (head == NULL) return;
	do {
		next = curr->next;
		free(curr);
		curr = next;
	} while(curr != head);

	head = NULL;
	tail = NULL;



    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		drawPoly(x, y);
		
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// Connect last point with first point
		connectEnds();
		
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		// TODO

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
