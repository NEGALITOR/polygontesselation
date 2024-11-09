// Taha Tas
// Ear Clipping Algorithm

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

typedef array<GLfloat, 4> point;
typedef array<point, 3> triangle;

vector<point> polygon;
vector<triangle> triangleList;
int closed = 0;

// Dot Product
int dot(point p1, point p2)
{
	return p1[0] * p2[0] + p1[1] * p2[1];
}

// Magnitude of a vector
double magnitude(point p1)
{
	return sqrt(p1[0]*p1[0] + p1[1]*p1[1]);
}

// Checks the winding order of a line
int checkWinding(point p1, point p2)
{
	return (p1[0] * p2[1] - p1[1] * p2[0]);
}

// Gets the interior angle of a line
double getInteriorAngle(point p1, point p2)
{
	return acos(dot(p1, p2) / (magnitude(p1) * magnitude(p2)));
}

// Determinent of a matrix
float determinent(float a, float b, float c, float d)
{
	return a*d - b*c;
}

// Gets the area of a triangle
double area(triangle tri)
{
	point a = tri[0];
	point b = tri[1];
	point c = tri[2];
	return 0.5 * fabs(a[0]*b[1] - a[0]*c[1] + b[0]*c[1] - b[0]*a[1] + c[0]*a[1] - c[0]*b[1]);
}

// Checks if the winding is clockwise or not
bool isClockwise(vector<point> poly)
{
	int sum = 0;
	int n = poly.size();
	for(int i = 0; i < n; i++)
		sum += (poly[(i+1)%n][0] - poly[i][0]) * (poly[(i+1)%n][1] + poly[i][1]);

	return sum > 0;
}

// Checks if two different lines intersect
bool intersect(point startPoint1, point endPoint1, point startPoint2, point endPoint2)
{
	int denom = determinent(endPoint1[0] - startPoint1[0], -(endPoint2[0] - startPoint2[0]), endPoint1[1] - startPoint1[1],  -(endPoint2[1] - startPoint2[1]));
	
	if(denom == 0) return false;

	float uA = determinent(startPoint2[0] - startPoint1[0], -(endPoint2[0] - startPoint2[0]), startPoint2[1] - startPoint1[1], -(endPoint2[1] - startPoint2[1])) / float(denom);
	float uB = determinent(endPoint1[0] - startPoint1[0], startPoint2[0] - startPoint1[0], endPoint1[1] - startPoint1[1], startPoint2[1] - startPoint1[1]) / float(denom);

	return uA > 0 && uA < 1 && uB > 0 && uB < 1;
}

// Checks if an ear line intersects with any other tess line
bool diagonalIntersect(vector<point> poly, int index)
{
	int n = poly.size();
	for(int i = 0; i < n; i++)
	{
		if(i == index) continue;
		if(i == (index + 2) % n) continue;
		if((i + 1) % n == index) continue;
		if((i + 1) % n == (index + 2) % n) continue;

		if(intersect(poly[index], poly[(index+2)%n], poly[i], poly[(i+1)%n]))
			return true;
	}
	return false;
}

// Checks intersections during drawing
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

		float detA = determinent(A[0][0], A[0][1], A[1][0], A[1][1]);

		a = x3 - x1;
		b = y3 - y1;

		Ai[0][0] = a;
		Ai[1][0] = b;

		ua = determinent(Ai[0][0], Ai[0][1], Ai[1][0], Ai[1][1]) / detA;

		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
				Ai[j][k] = A[j][k];
		
		Ai[0][1] = a;
		Ai[1][1] = b;

		ub = determinent(Ai[0][0], Ai[0][1], Ai[1][0], Ai[1][1]) / detA;

		

		if ((ua > 0 && ua < 1) && (ub > 0 && ub < 1)) return true;
	}

	return false;
}

// Checks num sign
int checkSign(int num)
{
	if (num > 0) return 1;
	if (num < 0) return -1;
	return 0;
}

// Determines if an ear can be made from the polygon
bool validEar(vector<point> poly, int index, int &windingOrder)
{
	int n = poly.size();

	// Check for CCW
	point lineOne = {	poly[index][0] - poly[(index+1)%n][0],
						poly[index][1] - poly[(index+1)%n][1],
						0,
						1	};

	point lineTwo = {	poly[(index+2)%n][0] - poly[(index+1)%n][0],
						poly[(index+2)%n][1] - poly[(index+1)%n][1],
						0,
						1	};

	windingOrder = checkWinding(lineOne, lineTwo);
	if (windingOrder >= 0)return false;

	// Check for intersection of the ear line
	if(diagonalIntersect(poly, index)) return false;

	// Check interior angle of the line/ear
	point nextLine = {	poly[(index+3)%n][0] - poly[(index+2)%n][0],
						poly[(index+3)%n][1] - poly[(index+2)%n][1],
						0,
						1	};

	point nextNextLine = {	poly[index][0] - poly[(index + 2)%n][0],
							poly[index][1] - poly[(index + 2)%n][1],
							0,
							1	};

	if(getInteriorAngle(nextNextLine, {-lineTwo[0], -lineTwo[1], 0, 1}) > getInteriorAngle(nextLine, {-lineTwo[0], -lineTwo[1], 0, 1}))
		if(checkSign(checkWinding(nextNextLine, lineTwo)) == checkSign(checkWinding(nextLine, lineTwo)))
			return false;

	return true;
}

// Tessellates the polygon
vector<triangle> tessellate()
{
	vector<triangle> triList;

	vector<point> poly = polygon;

	// Reverse if not CCW
	if(isClockwise(poly))
		reverse(poly.begin(), poly.end());

	int n = poly.size();
	while(n > 3)
	{
		// Triangle List logic
		for(int i = 0; i < n; i++)
		{
			int windingOrder;
			if(validEar(poly, i, windingOrder))
			{
				triangle tri = {poly[i], poly[(i+1)%n], poly[(i+2)%n]};
				triList.push_back(tri);

				poly.erase(poly.begin() + (i + 1)%n);
				n--;
				break;
			}
			else if(windingOrder == 0)
			{
				poly.erase(poly.begin() + (i + 1)%n);
				n--;
				break;
			}
		}
	}
	
	triangle finalTriangle = {poly[0], poly[1], poly[2]};
	triList.push_back(finalTriangle);

	return triList;
}

// Draw all the triangles when tesselated
void drawTesselation(vector<triangle> triList)
{
	if(triList.empty())
		triList = tessellate();

	

	printf("\n -------------------------------------------------------------------------\n");
	for(int i = 0; i < triList.size(); i++)
	{
		float areaOfTri = area(triList[i]);
		printf("|%3d", i+1);
		printf("| Area of Triangle: (%3.0f, %3.0f) | (%3.0f, %3.0f) | (%3.0f, %3.0f) is %9.2f |\n", triList[i][0][0], triList[i][0][1], triList[i][1][0], triList[i][1][1], triList[i][2][0], triList[i][2][1], areaOfTri);

		glBegin(GL_LINES);
			glVertex2fv(triList[i][0].data());
			glVertex2fv(triList[i][1].data());

			glVertex2fv(triList[i][1].data());
			glVertex2fv(triList[i][2].data());

			glVertex2fv(triList[i][2].data());
			glVertex2fv(triList[i][0].data());
		glEnd();
	}
	printf(" -------------------------------------------------------------------------\n\n");

	glFlush();
  

}

// Fills the polygon without tesselation
void fillPolygon(vector<point> poly)
{	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBegin(GL_POLYGON);
		for (int i = 0; i < poly.size(); i++)
		{
			glVertex2fv(poly[i].data());
		}
	glEnd();

	glFlush();
	
}

// Fills polygon with tesselation
void fillTessPolygon(vector<triangle> triList)
{
	
	glClear(GL_COLOR_BUFFER_BIT);

	if (triList.empty())
	{
		triList.clear();
		triList = tessellate();
	}

	
	for (int i = 0; i < triList.size(); i++)
	{
		glBegin(GL_POLYGON);
			glVertex2fv(triList[i][0].data());
			glVertex2fv(triList[i][1].data());
			glVertex2fv(triList[i][2].data());
		
		glEnd();
	}

	glFlush();
}

// Restores original lines and vertices w/o tesselation and fill
void restoreOriginal(vector<point> poly)
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < poly.size(); i++)
	{
		glBegin(GL_POINTS);
			glVertex2fv(poly[i].data());
		glEnd();
		glBegin(GL_LINES);
			glVertex2fv(poly[i].data());
			glVertex2fv(poly[(i+1) % poly.size()].data());
		glEnd();
	}

	glFlush();
}

// Connect the two ends with left click and determine if intersection occurs
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

	printf("\nConnected Ends.\n");
	closed = 1;

	glBegin(GL_LINES);
		glVertex2fv(polygon.back().data());
		glVertex2fv(polygon.front().data());
	glEnd();

	glFlush();

}

// Manages right click. Allows placement of vertices with lines/dots connections
// Checks for intersections and adds the vertex to the index list
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
	printf("Point Added: (%.0f, %.0f)\n", p[0], p[1]);

	glBegin(GL_POINTS);
		glVertex2fv(polygon.back().data());
	glEnd();

	if (polygon.front().data() != polygon.back().data())
	{
		glBegin(GL_LINES);
			glVertex2fv(polygon[polygon.size() - 2].data());
			glVertex2fv(polygon.back().data());
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

// Mouse interactions
// Right Click: Place a vertex on screen
// Left Click: Connect the ends with a lin
// Middle Click: Reset
void mouse(int button, int state, int x, int y)
{

	if (closed == 0 && button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		drawPoly(x, y);
	}
	if (closed == 0 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
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

		printf("Cleared Polygon.\n");
	}

}


// Keyboard Interactions
// Q: Quit
// F: Fill polygon w/o tess
// T: Tessellate polygon
// P: Fill tessellated polygon
// L: Restore to original polygon w/o tesselation and fill
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
		triangleList = tessellate();
		drawTesselation(triangleList);
	}

	if (key == 'p' || key == 'P')
	{
		fillTessPolygon(triangleList);
		printf("Filled w/ Tesselation.\n\n");
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
