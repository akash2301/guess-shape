#include <string.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#define PI 3.14159265
#define SMALL_NUMBER 1e-5

using namespace std;

/*********************************************************************************/
const string notAValidShape = "Not a valid Shape";

class Coord {
	public:
		float x, y;

	Coord (float a, float b) {
		x = a;
		y = b;
	}

	Coord () {}
};

bool areValuesInCloseProximity (float a, float b, float maxDiff) {
	return abs (a - b) < maxDiff;
}

float computeAngle (Coord mid, Coord left, Coord right) {
	const float deg = 180.0f / PI;

	// x values of 2 points are same, subtract a small number to the non-mid point to avoid divide-by-zero
	if (left.x == mid.x) {
		left.x -= SMALL_NUMBER;
	}

	if (right.x == mid.x) {
		right.x -= SMALL_NUMBER;
	}

	float gradML = (left.y - mid.y) / (left.x - mid.x),
		gradMR = (mid.y - right.y) / (mid.x - right.x);
	float eq = abs ((gradML - gradMR) / (1+(gradMR*gradML)));

	return atan (eq) * deg;
}

float computeEdgeLength (Coord a, Coord b) {
	float l1 = b.x - a.x, l2 = b.y - a.y;
	return sqrt ((l1*l1) + (l2*l2));
}

vector< float > getInteriorAngles (vector< Coord >& coords) {
	vector< float > angles = vector< float > ();
	const unsigned int numOfCoords = coords.size ();

	// First point is a special case, handle it differently
	// Since the last point on list is same as 1st point (to ensure closed shape), use second-last point
	// as the right point of the first point.
	angles.push_back (computeAngle(coords [0], coords [1], coords [numOfCoords - 2]));

	// For rest of points, take the point, take 1 before it & 1 after it & compute angle (after gradients)
	for (int i = 1; i < numOfCoords - 1; i++) {
		angles.push_back (computeAngle (coords [i], coords [i-1], coords [i+1]));
	}

	return angles;
}

string identifyTriangle (vector< Coord >& coords) {
	const string triangle = "Triangle";
	const vector< float > interiorAngles = getInteriorAngles (coords);
	const float error = 10.0f;
	unsigned int flags = 0;

	if (areValuesInCloseProximity (interiorAngles [0], interiorAngles [1], error)) {
		flags |= 1;
	}

	if (areValuesInCloseProximity (interiorAngles [0], interiorAngles [2], error)) {
		flags |= 2;
	}

	if (areValuesInCloseProximity (interiorAngles [1], interiorAngles [2], error)) {
		flags |= 4;
	}

	if (!flags) {
		return "Scalene " + triangle;
	}

	if (flags == 7) {
		return "Equilateral " + triangle;
	}

	return "Isoceles " + triangle;
}


string identifyQuadrilateral (vector< Coord >& coords) {
	const vector< float > interiorAngles = getInteriorAngles (coords);
	const float error = 7.0f;

	// step 1: check if all interior angles are same (90 degree)
	/*if (!areValuesInCloseProximity (interiorAngles [0], 90.0f, error)) {
		return notAValidShape;
	}*/

	for (int i = 1; i < interiorAngles.size (); i++) {
		if (!areValuesInCloseProximity (interiorAngles [0], interiorAngles [i], error)) {
			return notAValidShape;
		}
	}

	// Step 2: Compute Aspect ratio to determine whether quad. is a square or rectangle
	const float firstEdgeLength = computeEdgeLength (coords [0], coords [1]),
		secondEdgeLength = computeEdgeLength (coords [1], coords [2]);

	if (areValuesInCloseProximity (firstEdgeLength, secondEdgeLength, 5.0f)) {
		return "Square";
	}

	return "Rectangle";
}

bool isRegularPentagon (vector< Coord >& coords) {
	const vector< float > interiorAngles = getInteriorAngles (coords);
	const float error = 20.0f;

	// step 1: check if all interior angles are same (90 degree)
	/*if (!areValuesInCloseProximity (interiorAngles [0], 90.0f, error)) {
		return notAValidShape;
	}*/

	for (int i = 1; i < interiorAngles.size (); i++) {
		if (!areValuesInCloseProximity (interiorAngles [0], interiorAngles [i], error)) {
			return false;
		}
	}

	return true;
}

bool isRegularHexagon (vector< Coord >& coords) {
	const vector< float > interiorAngles = getInteriorAngles (coords);
	const float error = 20.0f;

	// step 1: check if all interior angles are same (90 degree)
	/*if (!areValuesInCloseProximity (interiorAngles [0], 90.0f, error)) {
		return notAValidShape;
	}*/

	for (int i = 1; i < interiorAngles.size (); i++) {
		if (!areValuesInCloseProximity (interiorAngles [0], interiorAngles [i], error)) {
			return false;
		}
	}

	return true;
}


string detectShapeDecisionTree (vector< Coord >& coords) {
	switch (coords.size ()) {
		case 0: case 3:
			return notAValidShape;
		case 1:
			return "Point";
		case 2:
			return "Straight Line";
		case 4:
			return identifyTriangle (coords);
		case 5:
			return identifyQuadrilateral (coords);
		case 6:
			return (isRegularPentagon (coords) ? "Pentagon" : notAValidShape);
		case 7:
			return (isRegularHexagon (coords) ? "Hexagon" : notAValidShape);
		default:
			return "circle";
	}
}

/*********************************************************************************/

Coord w;
Coord s;

vector< Coord > coords = vector< Coord > ();
int mouseClickFlag = 0,userFlag=0,finalcoords;
/* Initialize OpenGL Graphics */
void initGL() {
   glClearColor(0.0,1.0,1.0,0);
	glColor3f(0.0,0.0,0.0);
	gluOrtho2D(0 , 640 , 0 , 480);
}



void displayText( float x, float y, int r, int g, int b, const char *string ) {
	int j = strlen( string );

	glColor3f( r, g, b );
	glRasterPos2f( x, y );
	for( int i = 0; i < j; i++ ) {
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, string[i] );
	}
	glFlush();
}
void lineDraw(Coord p, Coord q){
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2.5);
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINES);
	glVertex2f(p.x,p.y);
	glVertex2f(q.x,q.y);
	glEnd();
	glFlush();
}

void onMouseClickCallback(int button, int state, int x, int y) {	
if(userFlag==1){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    		y = 480 - y;
		Coord mycoord = Coord(x,y);
		coords.push_back(mycoord);
		cout<<mycoord.x<<" "<<mycoord.y<<"\n";
		cout<<coords[0].x<<" "<<coords[0].y;
		glPointSize(5);
		glColor3f(1.0,0.0,0.0);
		glBegin(GL_POINTS);
		glVertex2d(x, y);
		glEnd();
		glFlush();
		switch(mouseClickFlag){
			case 0: mouseClickFlag=1;
				w.x=x;
				w.y=y;
				break;
			case 1: mouseClickFlag=2;
				s.x=x;
				s.y=y;
				lineDraw(w,s);
				break;
			case 2: w.x=s.x;
				w.y=s.y;
				s.x=x;
				s.y=y;
				mouseClickFlag=2;
				lineDraw(w,s);
				break;
				
		}				

    	
	}
}
else
{}
}



void keyboard(unsigned char key, int x, int y)
{
	if (key == 32) {
	if(userFlag == 0)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glutSwapBuffers();
		userFlag=1;

	} else if(userFlag==1){
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
	finalcoords = coords.size();
	coords[finalcoords-1].x=coords[0].x;
	coords[finalcoords-1].y=coords[0].y;

	cout << endl << "*********THE SHAPE IS**********" << endl << detectShapeDecisionTree (coords) << endl;
	
	}
	}

}

void display() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
   glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
   displayText(220,450,0,0,0,"Mini Project in OpenGL");
   displayText(265,430,0,0,0,"on the topic");
   displayText(185,410,0,0,0,"Shape Detection - Open Challenge");
   displayText(225,370,0,0,0,"under the guidance of");
   displayText(235,340,0,0,0,"Mr. Pankaj Badoni");
   displayText(225,320,0,0,0,"(Assistant Proffessor)");
   displayText(135,300,0,0,0,"School of Computer Science and Engineering");
   displayText(255,210,0,0,0,"Submitted by");
   displayText(120,160,0,0,0,"Vatsal Jain");
   displayText(400,160,0,0,0,"Raghav Dua");

}

int main(int argc, char** argv) {
   Coord w;
   Coord s;
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_SINGLE); // Enable single buffered mode
   glutInitWindowSize(640, 480);   // Set the window's initial width & height
   glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
   glutCreateWindow("Object Guessing Algorithm");          // Create window with the given title
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   glutKeyboardFunc(keyboard);
   glutMouseFunc(onMouseClickCallback);
   initGL();                       // Our own OpenGL initialization
   glutMainLoop();                 // Enter the infinite event-processing loop
   return 0;
}

/*
int main () {
	vector< Coord > coords = vector< Coord > ();

	/* Pentagon
	coords.push_back (Coord (550,450));
	coords.push_back (Coord (455,519));
	coords.push_back (Coord (491,631));
	coords.push_back (Coord (609,631));
	coords.push_back (Coord (645,519));
	coords.push_back (Coord (550,450));
	*

	/* Hexagon
	coords.push_back (Coord (600,463));
	coords.push_back (Coord (500,463));
	coords.push_back (Coord (450,550));
	coords.push_back (Coord (500,637));
	coords.push_back (Coord (600,637));
	coords.push_back (Coord (650,550));
	coords.push_back (Coord (600,463));
	*

	/* Quadrilateral
	coords.push_back (Coord (200,200));
	coords.push_back (Coord (500,200));
	coords.push_back (Coord (500,330));
	coords.push_back (Coord (200,330));
	coords.push_back (Coord (200,200));
	*

	/* Triangle
	coords.push_back (Coord (550,450));
	coords.push_back (Coord (463,600));
	coords.push_back (Coord (637,600));
	coords.push_back (Coord (550,450));
	*

	cout << detectShapeDecisionTree (coords) << endl;
}
*/