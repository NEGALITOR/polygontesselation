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

	struct VectorPoint* prev;
	struct VectorPoint* next;

}VectorPoint;

typedef struct TriangleList
{
	point tri[3];

	struct TriangleList* prev;
	struct TriangleList* next;

}TriangleList;


VectorPoint* head;
VectorPoint* tail;
VectorPoint* copy = NULL;
TriangleList* tlHead = NULL;
int closed = 0;


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

void printVPNodes(VectorPoint** currHead)
{
	if (currHead == NULL) return;

	VectorPoint* curr = *currHead;

	//printf("PRINTING LIST\n----------------------------------------------------");
	while (curr != NULL)
	{
		printf("\n| %.0f %.0f | %p %p %p |\n", curr->p[0], curr->p[1], curr->prev, curr, curr->next);
		curr = curr->next;
	}
	//printf("----------------------------------------------------\n\n");
}

void printTLNodes(TriangleList** currHead)
{
	if (currHead == NULL) return;

	TriangleList* curr = *currHead;

	while (curr != NULL)
	{
		//printf("\n| %p %p %p | %p %p %p |\n\n", curr->tri[0], curr->tri[1], curr->tri[1], curr->prev, curr, curr->next);
		printf("\n| (%.0f, %.0f), (%.0f, %.0f), (%.0f, %.0f) | %p %p %p |\n\n", curr->tri[0][0], curr->tri[0][1], curr->tri[1][0], curr->tri[1][1], curr->tri[2][0], curr->tri[2][1], curr->prev, curr, curr->next);
		curr = curr->next;
	}
}

int countVPNodes(VectorPoint** currHead)
{
	VectorPoint* curr = *currHead;

	if (curr == NULL) return 0;
	int count = 0;

	while (curr != NULL)
	{
		count++;
		curr = curr->next;
	}
	//printf("CountVP: %d\n", count);
	return count;
}

int countTLNodes(TriangleList** currHead)
{
	TriangleList* curr = *currHead;

	if (curr == NULL) return 0;
	int count = 0;

	while (curr != NULL)
	{
		count++;
		curr = curr->next;
	}
	//printf("%d | ", count);
	return count;
}

VectorPoint* copyList(VectorPoint* curr, VectorPoint* prevNode)
{
	if (curr == NULL) return NULL;

	VectorPoint* newNode = (VectorPoint*) malloc(sizeof(VectorPoint));
	memcpy(newNode->p, curr->p, sizeof(point));
	newNode->prev = prevNode;

	newNode->next = copyList(curr->next, newNode);

	return newNode;

}

void clearVPList(VectorPoint** currHead)
{
	if (currHead == NULL) return;

	VectorPoint* curr = *currHead;
	VectorPoint* next;

	//printf("-----------------------------------------\n");
	//printVPNodes(currHead);

	while (curr != NULL)
	{
		next = curr->next;
		printf("Freeing: %f %f\n", curr->p[0], curr->p[1]);
		free(curr);
		curr = next;
	}
	//printf("-----------------------------------------\n\n");

	

}

void clearTLList(TriangleList** currHead)
{
	if (currHead == NULL) return;

	
	TriangleList* curr = *currHead;
	TriangleList* next;

	//printf("-----------------------------------------\n");
	printTLNodes(currHead);

	while (curr != NULL)
	{
		next = curr->next;
		printf("Freeing: %f %f | %f %f | %f %f \n", curr->tri[0][0], curr->tri[0][1], curr->tri[1][0], curr->tri[1][1], curr->tri[2][0], curr->tri[2][1]);
		free(curr);
		curr = next;
	}
	//printf("-----------------------------------------\n\n");

	*currHead = NULL;

}

void printArea(point zero, point one, point two)
{
	float areaOfTri = 0.5 * fabs( zero[0] * (one[1] - two[1]) + one[0] * (two[1] - zero[1]) + two[0] * (zero[1] - one[1]));
	printf("Area of Triangle (%.0f, %.0f) (%.0f, %.0f) (%.0f, %.0f) is %.2f\n", zero[0], zero[1], one[0], one[1], two[0], two[1], areaOfTri);

	
}

GLfloat determinent(GLfloat matrix[2][2])
{
	GLfloat det = (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
	//printf("Determinent: %.4f\n", det);
	return (det);
}

int checkIntersect(VectorPoint* startVec, VectorPoint* endVec)
{
	VectorPoint* curr = head;
	int line = 1;

	GLfloat x1 = endVec->p[0], x2 = startVec->p[0], x3, x4, a;
	GLfloat y1 = endVec->p[1], y2 = startVec->p[1], y3, y4, b;
	GLfloat ua, ub;

	GLfloat A[2][2];
	GLfloat Ai[2][2];

	A[0][0] = x2 - x1;
	A[1][0] = y2 - y1;

	
	while (curr->next != NULL)
	{
		x3 = curr->p[0];
		x4 = curr->next->p[0];
		y3 = curr->p[1];
		y4 = curr->next->p[1];

		//printf("Line %d\n-----------\n", line);
		//printf("x1: %.0f y1: %.0f | x2: %.0f y2: %.0f | x3: %.0f y3: %.0f | x4: %.0f y4: %.0f\n\n", x1, y1, x2, y2, x3, y3, x4, y4);

		A[0][1] = -(x4 - x3);
		A[1][1] = -(y4 - y3);

		//printf("Matrix A: %.0f %.0f\n          %.0f %.0f\n", A[0][0], A[0][1], A[1][0], A[1][1]);

		//Ai = A;
		memcpy(Ai, A, 4 * sizeof(GLfloat));

		GLfloat detA = determinent(A);

		a = x3 - x1;
		b = y3 - y1;
		//printf("a: %.0f, b: %.0f\n", a, b);

		Ai[0][0] = a;
		Ai[1][0] = b;
		//printf("Matrix A1: %.0f %.0f\n          %.0f %.0f\n", Ai[0][0], Ai[0][1], Ai[1][0], Ai[1][1]);
		ua = determinent(Ai) / detA;


		memcpy(Ai, A, 4 * sizeof(GLfloat));
		Ai[0][1] = a;
		Ai[1][1] = b;
		//printf("Matrix A2: %.0f %.0f\n          %.0f %.0f\n", Ai[0][0], Ai[0][1], Ai[1][0], Ai[1][1]);
		ub = determinent(Ai) / detA;

		//printf("ua: %f, ub: %f\n\n", ua, ub);

		curr = curr->next;
		line++;

		if ((ua > 0 && ua < 1) && (ub > 0 && ub < 1))
		{
			//printf("INTERSECTED!\n");
			return 1;
		}
	}
	
	
	return 0;

}

void connectEnds()
{
	if (countVPNodes(&head) < 3)
	{
		printf("Not enough verticies to connect\n");
		return;
	}
	if (checkIntersect(head, tail) == 1)
	{
		printf("Final connection intersects.\n");
		return;
	}

	//tail->next = head;
	//head->prev = tail;
	closed = 1;

	glBegin(GL_LINES);
		glVertex2fv(tail->p);
		glVertex2fv(head->p);
	glEnd();

	glFlush();
}

void drawPoly(int x, int y)
{
	glColor3f(red, green, blue);

	//VectorPoint vec;
	//printf("| %p |\n", &vec);

	//printf("-----------------------------------------\n");

	if (head == NULL) 
	{
		head = (VectorPoint*) malloc(sizeof(VectorPoint));
		tail = head;
		//printf("Pointer: %d\n", *head);
		//printf("Pointer: %d\n", *tail);

		head->next = NULL;
		head->prev = NULL;
		//tail->next = head;
		//tail->prev = head;

		head->p[0] = x;
		head->p[1] = WINDOW_MAX - y;


		//printf("Clicked Point: %d     %d\n", x, y);
		//printf("Point: %.0f    %.0f\n", tail->p[0], tail->p[1]);

		//printf("updated head\n");
	}
	else
	{
		VectorPoint* vec = (VectorPoint*) malloc(sizeof(VectorPoint));

		vec->p[0] = x;
		vec->p[1] = WINDOW_MAX - y;

		// Check Intersection
		if (countVPNodes(&head) > 2 && checkIntersect(vec, tail) == 1)
		{
			//printf("-----------------------------------------\n");
			//printf("INTERSECTED!!!!!\n");
			//printf("-----------------------------------------\n\n");
			printf("Lines intersect.\n");
			return;
		}

		tail->next = vec;
		tail->next->prev = tail;
		tail = tail->next;
		tail->next = NULL;
		

		//printf("Point: %.0f    %.0f\n", tail->p[0], tail->p[1]);

		/*
		printf("updated tail\n");

		printf("Head: %.0f    %.0f\nTail Prev: %.0f    %.0f | Tail Curr %.0f    %.0f\n", 
			head->p[0], head->p[1], tail->prev->p[0], tail->prev->p[1], tail->p[0], tail->p[1]);
		
		printf("%p %p\n\n", (void *) tail->prev, (void *) tail);
		*/
	}

	
	
	//printVPNodes(head);
	//printf("-----------------------------------------\n\n");
	

	//printVPNodes(head);
	//printf("\n\nNum Nodes: %d\n\n", countVPNodes(head));
	
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


void fillPolygon(VectorPoint* currHead, TriangleList* triHead)
{
	//if (countVPNodes(&currHead) < 3) return;

	glClear(GL_COLOR_BUFFER_BIT);

	//printf("%.0f\n", triHead->tri[0]->p[0]);
	if (triHead == NULL)
	{
		printf("entered\n");
		VectorPoint* curr = currHead;
	
		glBegin(GL_POLYGON);
			do {
				glVertex2fv(curr->p);
				curr = curr->next;
			} while(curr != head && curr != NULL);
		glEnd();

		
	}
	else
	{
		printf("entered 2\n");
		TriangleList* curr = triHead;

		//printTLNodes(&curr);
		
		while (curr != NULL)
		{
			printf("fill in tri\n");
			printf("%.0f ",curr->tri[0][0]);
			printf("%.0f ",curr->tri[1][0]);
			printf("%.0f\n",curr->tri[2][0]);

			glBegin(GL_POLYGON);
				glVertex2fv(curr->tri[0]);
				glVertex2fv(curr->tri[1]);
				glVertex2fv(curr->tri[2]);
			glEnd();
			curr = curr->next;
		}
		
	}
	
	glFlush();
}

void restoreOriginal(VectorPoint* currHead)
{
	glClear(GL_COLOR_BUFFER_BIT);

	VectorPoint* curr = currHead;

	while (curr->next != NULL)
	{
		glBegin(GL_POINTS);
			glVertex2fv(curr->p);
		glEnd();
		glBegin(GL_LINES);
			glVertex2fv(curr->p);
			glVertex2fv(curr->next->p);
		glEnd();

		curr = curr->next;
	}
	glBegin(GL_POINTS);
		glVertex2fv(curr->p);
	glEnd();

	glBegin(GL_LINES);
		glVertex2fv(tail->p);
		glVertex2fv(head->p);
	glEnd();
	

	glFlush();
}


void addTLElement(TriangleList** currHead, point zero, point one, point two)
{
	/*
	TriangleList* triElement = (TriangleList*) malloc(sizeof(TriangleList));
	triElement->tri[0] = zero;
	triElement->tri[1] = one;
	triElement->tri[2] = two;
	
	triElement->next = NULL;
	triElement->prev = NULL;
	
	if (currHead == NULL)
	{
		currHead = &triElement;
		printf("\nNew currHead: | (%.0f, %.0f), (%.0f, %.0f), (%.0f, %.0f) | %p %p %p |\n\n", (*currHead)->tri[0]->p[0], (*currHead)->tri[0]->p[1], (*currHead)->tri[1]->p[0], (*currHead)->tri[1]->p[1], (*currHead)->tri[2]->p[0], (*currHead)->tri[2]->p[1], (*currHead)->prev, (*currHead), (*currHead)->next);

	}
	else
	{
		TriangleList* curr = *currHead;
	
		while (curr->next != NULL)
		{
			curr = curr->next;
		}

		curr->next = triElement;
		printf("\n%p %p %p\n", curr->next->tri[0], curr->next->tri[1], curr->next->tri[2]);
		curr->next->prev = curr;
		//printf("\ntlHead: | (%.0f, %.0f), (%.0f, %.0f), (%.0f, %.0f) | %p %p %p |\n\n", currHead->tri[0]->p[0], currHead->tri[0]->p[1], currHead->tri[1]->p[0], currHead->tri[1]->p[1], currHead->tri[2]->p[0], currHead->tri[2]->p[1], currHead->prev, currHead, currHead->next);
		
	}
	*/

	TriangleList* triElement = (TriangleList*) malloc(sizeof(TriangleList));

	
	memcpy(triElement->tri[0], zero, 1 * sizeof(point));
	memcpy(triElement->tri[1], one, 1 * sizeof(point));
	memcpy(triElement->tri[2], two, 1 * sizeof(point));
	
	
	
	/*
	triElement->tri[0] = zero;
	triElement->tri[1] = one;
	triElement->tri[2] = two;

	
	triElement->tri[0][0] = zero[0];
	triElement->tri[0][1] = zero[1];
	triElement->tri[1][0] = one[0];
	triElement->tri[1][1] = one[1];
	triElement->tri[2][0] = two[0];
	triElement->tri[2][1] = two[1];
	

	memcpy(&(triElement->tri[0][0]), &(zero[0]), 1 * sizeof(GLfloat));
	memcpy(&(triElement->tri[1][0]), &(one[0]), 1 * sizeof(GLfloat));
	memcpy(&(triElement->tri[2][0]), &(two[0]), 1 * sizeof(GLfloat));
	memcpy(&(triElement->tri[0][1]), &(zero[1]), 1 * sizeof(GLfloat));
	memcpy(&(triElement->tri[1][1]), &(one[1]), 1 * sizeof(GLfloat));
	memcpy(&(triElement->tri[2][1]), &(two[1]), 1 * sizeof(GLfloat));
	*/

	//printf("\nPoint Vals: | (%.0f, %.0f), (%.0f, %.0f), (%.0f, %.0f) |\n", zero[0], zero[1], one[0], one[1], two[0], two[1]);
	//printf("\nTriElement: | (%.0f, %.0f), (%.0f, %.0f), (%.0f, %.0f) | %p %p %p |\n", triElement->tri[0][0], triElement->tri[0][1], triElement->tri[1][0], triElement->tri[1][1], triElement->tri[2][0], triElement->tri[2][1], triElement->prev, triElement, triElement->next);

	triElement->next = NULL;
	triElement->prev = NULL;

	if (*currHead == NULL)
	{
		*currHead = triElement;
		//printf("\nNew currHead: | (%.0f, %.0f), (%.0f, %.0f), (%.0f, %.0f) | %p %p %p |\n", (*currHead)->tri[0][0], (*currHead)->tri[0][1], (*currHead)->tri[1][0], (*currHead)->tri[1][1], (*currHead)->tri[2][0], (*currHead)->tri[2][1], (*currHead)->prev, (*currHead), (*currHead)->next);

	}
	else
	{
		TriangleList* curr = *currHead;

		while (curr->next != NULL)
		{
			curr = curr->next;
		}

		curr->next = triElement;
		//printf("\n%p %p %p\n", curr->next->tri[0], curr->next->tri[1], curr->next->tri[2]);
		curr->next->prev = curr;
		//printf("\ncurr currhead: | (%.0f, %.0f), (%.0f, %.0f), (%.0f, %.0f) | %p %p %p |\n", (*currHead)->tri[0][0], (*currHead)->tri[0][1], (*currHead)->tri[1][0], (*currHead)->tri[1][1], (*currHead)->tri[2][0], (*currHead)->tri[2][1], (*currHead)->prev, (*currHead), (*currHead)->next);
		//printf("\ncurr : | (%.0f, %.0f), (%.0f, %.0f), (%.0f, %.0f) | %p %p %p |\n\n\n\n", curr->tri[0][0], curr->tri[0][1], curr->tri[1][0], curr->tri[1][1], curr->tri[2][0], curr->tri[2][1], curr->prev, curr, curr->next);
	}
	printArea(zero, one, two);

	//printTLNodes(currHead);
	//printf("-----------------------");
	//printTLNodes(&tlHead);


}

void tesselatePoly(VectorPoint** copyHead, TriangleList** triHead, VectorPoint* currPass)
{

	
	
	if (*copyHead == NULL && *triHead == NULL)
	{
		*copyHead = copyList(head, NULL);
		printf("%p | %p\n", *copyHead, *triHead);
		//printf("%.0f %.0f\n", (*copyHead)->p[0], (*copyHead)->p[1]);
		//printVPNodes(copyHead);
	}

	int numNodes = countVPNodes(copyHead);
	//printf("Num Nodes in Copy: %d\n", numNodes);
	//int numNodeshead = countVPNodes(&head);
	//printf("%d\n", numNodeshead);
	
	if (numNodes < 3) return;

	/*if (numNodes == 3)
	{
		VectorPoint* curr = (*copyHead)->next;


		addTLElement(triHead, curr->prev, curr, curr->next);
		printf("%d\n", countTLNodes(triHead));
		clearVPList(copyHead);

		return;
	}*/

	VectorPoint* curr;
	if (currPass != NULL)
	{
		//printf("the curr has been passed\n");
		curr = currPass;
	}
	else
		curr = (*copyHead)->next;

	VectorPoint* next;

	if (curr->next == NULL)
	{
		clearVPList(copyHead);
		*copyHead = NULL;
		//printf("copy count: %d\n", countVPNodes(copyHead));
		printf("Unable to tesselate further.\n");
		return;
	}
	

	
	//printf("\nComparing: (%.0f, %.0f), (%.0f, %.0f), (%.0f, %.0f)\n", curr->prev->p[0], curr->prev->p[1], curr->p[0], curr->p[1], curr->next->p[0], curr->next->p[1]);
	GLfloat l1x = curr->prev->p[0] - curr->p[0];
	GLfloat l1y = curr->prev->p[1] - curr->p[1];
	GLfloat l2x = curr->next->p[0] - curr->p[0];
	GLfloat l2y = curr->next->p[1] - curr->p[1];
	GLfloat crossProduct = (l1x * l2y)  - (l2x * l1y);
	
	int isIntersect = checkIntersect(curr->prev, curr->next);
	if (crossProduct > 0)
	{
		curr = curr->next;
		tesselatePoly(copyHead, triHead, curr);
	}
	else if (crossProduct < 0 && isIntersect == 0)
	{
		red   = 255;
       	green = 255; 
      	blue  = 0;

		glBegin(GL_LINES);
			glVertex2fv(curr->prev->p);
			glVertex2fv(curr->next->p);
		glEnd();

		red   = 255;
       	green = 0; 
      	blue  = 0;

		addTLElement(triHead, curr->prev->p, curr->p, curr->next->p);
		//printTLNodes(triHead);
		//printf("TL Count: %d\n", countTLNodes(triHead));

		curr->prev->next = curr->next;
		curr->next->prev = curr->prev;
		//printf("VP Count: %d\n", countVPNodes(copyHead));

		next = curr->next;
		//free(curr);
		curr = next;
		

		glFlush();
		printf("\nTESSELATED\n\n");

		if (countVPNodes(copyHead) == 3)
		{
			addTLElement(triHead, curr->prev->p, curr->p, curr->next->p);
			printTLNodes(triHead);
			clearVPList(copyHead);
			*copyHead = NULL;
			printf("TL Count: %d\n", countTLNodes(triHead));
			printf("VP Count: %d\n", countVPNodes(copyHead));
			

			printf("\nTesselation Finished.\n");
		}

	}
	else if (crossProduct == 0)
	{
		curr->prev->next = curr->next;
		curr->next->prev = curr->prev;
		//printf("VP Count: %d\n", countVPNodes(copyHead));

		next = curr->next;
		free(curr);
		curr = next;
	}
	else if (isIntersect == 1)
	{
		next = curr->next;
		curr = next;
		tesselatePoly(copyHead, triHead, curr);
	}

	//printTLNodes(triHead);
	//printf("-----------------------");
	//printTLNodes(&tlHead);
	
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



void mouse(int button, int state, int x, int y)
{
	
	if (closed == 0 && button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
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

        clearVPList(&head);
		head = NULL;
		tail = NULL;

		//clearVPList(&copy);
		clearTLList(&tlHead);
		copy = NULL;
		tlHead = NULL;
		
		closed = 0;


		glClear(GL_COLOR_BUFFER_BIT);
    	glFlush();
	}
}

void keyboard(unsigned char key, int x, int y)
{
	if (closed == 0) return;

	if (key == 'q' || key == 'Q') exit(0);

	if (key == 'f' || key == 'F')
	{
		fillPolygon(head, NULL);
	}
	if ((key == 't' || key == 'T'))
	{
		// TODO
		//		Tesselate w/ areas of each triangle

		glClear(GL_COLOR_BUFFER_BIT);
		restoreOriginal(head);

		//copy = copyList(head, NULL);

		clearVPList(&copy);
		clearTLList(&tlHead);
		do{
			tesselatePoly(&copy, &tlHead, NULL);
		} while (copy != NULL);
		//printf("TL Count at 3: %d\n", countTLNodes(tlHead));
		//printf("VP Count at 3: %d\n", countVPNodes(copy));
	}
	if (key == 'p' || key == 'P')
	{
		// TODO
		//		Fill and Tesselate
		if (tlHead == NULL)
		{
			copy = copyList(head, NULL);
			while (copy != NULL)
				tesselatePoly(&copy, &tlHead, NULL);
		}
			
		//printVPNodes(&head);
		//printf("\n-----------------------------\n");
		//printTLNodes(&tlHead);
		//countTLNodes(&tlHead);
		fillPolygon(NULL, tlHead);



	}
	if (key == 'l' || key == 'L')
	{
		// TODO
		//		Restore to initial state (no fill and no tes)
		restoreOriginal(head);
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
