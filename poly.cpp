#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <algorithm>

using namespace std;


GLubyte red, green, blue;
int COLORS_DEFINED;

const int WINDOW_POSITION = 100;
const int WINDOW_MAX = 800;

const float WORLD_COORDINATE_MIN = 0.0;
const float WORLD_COORDINATE_MAX = 800.0;

typedef array<GLint, 2> point;
typedef array<point, 3> triangle;

vector<point> polygon;
vector<triangle> triangleList;
int closed = 0;

void printVec(vector<point> poly)
{
	for (int i = 0; i < poly.size(); i++)
	{
		printf("%d %d | ", poly[i][0], poly[i][1]);
	}
	printf("\n");
}

void printTriList(vector<triangle> triList)
{
	for (int i = 0; i < triList.size(); i++)
	{
		printf("Triangle: %d %d | %d %d | %d %d\n", triList[i][0][0], triList[i][0][1], triList[i][1][0], triList[i][1][1], triList[i][2][0], triList[i][2][1]);
	}
	printf("\n");
}

void printArea(point p1, point p2, point p3)
{
	float areaOfTri = 0.5 * fabs( p1[0] * (p2[1] - p3[1]) + p2[0] * (p3[1] - p1[1]) + p3[0] * (p1[1] - p2[1]));
	printf("| Area of Triangle: (%3d, %3d) | (%3d, %3d) | (%3d, %3d) is %9.2f |\n", p1[0], p1[1], p2[0], p2[1], p3[0], p3[1], areaOfTri);
}

float crossProduct(point p1, point p2)
{
	return (p1[0] * p2[1]) - (p1[1] * p2[0]);
}

float dotProduct(point p1, point p2)
{
	return (p1[0] * p2[0]) + (p1[1] * p2[1]);
}

float getInteriorAngle(point p1, point p2)
{
	return atan2(crossProduct(p1, p2), dotProduct(p1, p2));
}

float determinent(float matrix[2][2])
{
	return (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
}

bool isIntersect(point p1, point p2)
{
	float x1 = p2[0], x2 = p1[0], x3, x4, a;
	float y1 = p2[1], y2 = p1[1], y3, y4, b;
	float ua, ub;

	float A[2][2];
	float Ai[2][2];

	A[0][0] = x2 - x1;
	A[1][0] = y2 - y1;

	for (int i = 0; i < polygon.size()-1; i++)
	{
		x3 = polygon[i][0];
		x4 = polygon[i+1][0];
		y3 = polygon[i][1];
		y4 = polygon[i+1][1];


		A[0][1] = -(x4 - x3);
		A[1][1] = -(y4 - y3);

		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
				Ai[j][k] = A[j][k];

		float detA = determinent(A);

		a = x3 - x1;
		b = y3 - y1;

		Ai[0][0] = a;
		Ai[1][0] = b;

		ua = determinent(Ai) / detA;

		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
				Ai[j][k] = A[j][k];
		
		Ai[0][1] = a;
		Ai[1][1] = b;

		ub = determinent(Ai) / detA;

		

		if ((ua > 0 && ua < 1) && (ub > 0 && ub < 1))
		{
			//printf("INTERSECTED!\n");
			return true;
		}
	}

	return false;
}

float getPolyArea(vector<point> poly)
{
	float area = 0;

	for (int i = 0; i < poly.size()-1; i++)
	{
		int x1 = polygon[i][0];
		int y1 = polygon[i][1];
		int x2 = polygon[(i+1) % poly.size()][0];
		int y2 = polygon[(i+1) % poly.size()][1];

		area += (x1 * y2 - x2 * y1);
	}
	return area/2;
}

float getTriArea(int x1, int y1, int x2, int y2, int x3, int y3)
{
	return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

vector<point> flipVectorWinding(vector<point> poly)
{
	if (getPolyArea(poly) < 0)
		reverse(poly.begin(), poly.end());
	return poly;
}

vector<point> filterPoints(vector<point> poly, triangle tri)
{
	vector<point> newPoints;
	for (int i = 0; i < poly.size(); i++)
		if (find(tri.begin(), tri.end(), poly[i]) == tri.end())
		{
			//printf("%d %d has been found! \n", poly[i][0], poly[i][1]);
			newPoints.push_back(poly[i]);
		}
	return newPoints;
}

bool isInTriangle(point p, point a, point b, point c)
{
	int ax = a[0];
	int ay = a[1];
	int bx = b[0];
	int by = b[1];
	int cx = c[0];
	int cy = c[1];
	int px = p[0];
	int py = p[1];

	float totalArea = getTriArea(ax, ay, bx, by, cx, cy);
	float areaA = getTriArea(px, py, bx, by, cx, cy);
	float areaB = getTriArea(ax, ay, px, py, cx, cy);
	float areaC = getTriArea(ax, ay, bx, by, px, py);
	
	return totalArea == areaA + areaB + areaC;
}


vector<triangle> tesselate()
{
	vector<point> indexList = polygon;
	vector<triangle> triList;
	int isAllTriFound = -1;
	bool inTriangle = false;
	triangleList.clear();
	int count = 0;


	indexList = flipVectorWinding(indexList);
	//printVec(polygon);
	//printVec(indexList);

	printf(" -------------------------------------------------------------------------\n");
	while (isAllTriFound != 0)
	{
		isAllTriFound = 0;
		

		for (int i = 1; i < indexList.size()-1; i++)
		{
			
			point p1 = indexList[i-1];
			point p2 = indexList[i];
			point p3 = indexList[(i+1) % indexList.size()];
			

			//printf("p1: %d %d | ", p1[0], p1[1]);
			//printf("p2: %d %d | ", p2[0], p2[1]);
			//printf("p3: %d %d\n", p3[0], p3[1]);
			
			// Middle point to 1st End point
			point v1 = {p2[0] - p1[0], p2[1] - p1[1]};
			// 2nd End point to Middle point
			point v2 = {p3[0] - p2[0], p3[1] - p2[1]};

			//printf("v1: %d %d | ", v1[0], v1[1]);
			//printf("v2: %d %d\n", v2[0], v2[1]);

			float interiorAngle = getInteriorAngle(v1, v2);
			//printf("%d %d | %d %d | %d %d | %f\n", p1[0], p1[1], p2[0], p2[1], p3[0], p3[1], interiorAngle);

			//printf("Interior Angle: %f\n", interiorAngle);

			inTriangle = false;

			if (interiorAngle < 0)
			{
				//cout << "failed" << endl;
				continue;
			}
			if (interiorAngle == 0)
			{
				indexList.erase(indexList.begin() + i);
				continue;
			}
			else
			{
				triangle tri = {p1, p2, p3};

				vector<point> nonTriPoints = filterPoints(indexList, tri);

				//cout << endl;
				//printVec(indexList);
				//printVec(nonTriPoints);
				//cout << endl;

				//inTriangle = false;

				for (int j = 0; j < nonTriPoints.size(); j++)
				{
					if (isInTriangle(nonTriPoints[j], tri[0], tri[1], tri[2]))
					{
						inTriangle = true;
						//cout << inTriangle << endl;
					}
				}


				//printf("%d\n", inTriangle);
				if (inTriangle == true)
					continue;
				else
				{
					glBegin(GL_LINES);
						glVertex2iv(tri[0].data());
						glVertex2iv(tri[2].data());
					glEnd();
					glFlush();

					count++;
					printf("|%3d", count);
					printArea(tri[0], tri[1], tri[2]);

					triList.push_back(tri);
					indexList.erase(indexList.begin() + i);
					isAllTriFound++;
					//printf("%d\n", isAllFound);
					break;
				}
				
			}
			

		}
		
	}
	printf(" -------------------------------------------------------------------------\n\n");
	return triList;
}

void fillPolygon(vector<point> poly)
{	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBegin(GL_POLYGON);
		for (int i = 0; i < poly.size(); i++)
		{
			glVertex2iv(poly[i].data());
		}
	glEnd();

	glFlush();
	
}

void fillTessPolygon(vector<triangle> triList)
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < triList.size(); i++)
	{
		glBegin(GL_POLYGON);
			glVertex2iv(triList[i][0].data());
			glVertex2iv(triList[i][1].data());
			glVertex2iv(triList[i][2].data());
		
		glEnd();
	}

	glFlush();
}

void restoreOriginal(vector<point> poly)
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < poly.size(); i++)
	{
		glBegin(GL_POINTS);
			glVertex2iv(poly[i].data());
		glEnd();
		glBegin(GL_LINES);
			glVertex2iv(poly[i].data());
			glVertex2iv(poly[(i+1) % poly.size()].data());
		glEnd();
	}

	glFlush();
}

void connectEnds()
{
	if (polygon.size() < 3) 
	{
		printf("Not enough verticies to connect ends.\n");
		return;
	}
	if (isIntersect(polygon.back(), polygon.front())) 
	{
		printf("First and last connection intersects another line.\n");
		return;
	}

	closed = 1;

	glBegin(GL_LINES);
		glVertex2iv(polygon.back().data());
		glVertex2iv(polygon.front().data());
	glEnd();

	glFlush();

}


void drawPoly(int x, int y)
{
	glColor3f(red, green, blue);

	point p;
	p[0] = x;
	p[1] = WINDOW_MAX-y;

	//printf("Point Clicked: %d %d\n", p[0], p[1]);

	if (!polygon.empty() && polygon.size() > 2 && isIntersect(p, polygon.back()))
	{
		printf("Lines intersect.\n");
		return;
	}

	polygon.push_back(p);

	glBegin(GL_POINTS);
		glVertex2iv(polygon.back().data());
	glEnd();

	if (polygon.front().data() != polygon.back().data())
	{
		glBegin(GL_LINES);
			glVertex2iv(polygon[polygon.size() - 2].data());
			glVertex2iv(polygon.back().data());
		glEnd();
	}

	glFlush();
}

// Input and Output Device Interactions
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
		connectEnds();
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		polygon.clear();
		triangleList.clear();
		glClear(GL_COLOR_BUFFER_BIT);
		glFlush();
		closed = 0;
	}

}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'q' || key == 'Q') exit(0);

	if (closed == 0) return;

	if (key == 'f' || key == 'F') 
	{
		fillPolygon(polygon);
		printf("Filled w/o Tesselation.\n\n");
	}
        
	if ((key == 't' || key == 'T'))
	{
		triangleList.clear();
		triangleList = tesselate();
		//printTriList(triangleList);
	}

	if (key == 'p' || key == 'P')
	{
		if (triangleList.empty())
		{
			triangleList.clear();
			triangleList = tesselate();
		}
		
		fillTessPolygon(triangleList);
		printf("Filled w/ Tesselation.\n\n");
		//printTriList(triangleList);
	}

	if (key == 'l' || key == 'L') 
	{
		restoreOriginal(polygon);
		printf("Restored Original Polygon Outline.\n\n");
	}
    
}

// OpenGL and GLUT Initialization
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

int main(int argc, char** argv)
{
	myglutInit(argc,argv);
	myInit();

	
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMainLoop();
}
