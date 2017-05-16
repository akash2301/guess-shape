#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#define PI 3.14159265
#define SMALL_NUMBER 1e-5

using namespace std;
const string notAValidShape = "Not a valid Shape";

class Coord {
	public:
		float x, y;

	Coord (float a, float b) {
		x = a;
		y = b;
	}
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
	const float error = 4.0f;
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
	const float error = 4.0f;

	// step 1: check if all interior angles are same (90 degree)
	/*if (!areValuesInCloseProximity (interiorAngles [0], 90.0f, error)) {
		return notAValidShape;
	}*/
	cout << interiorAngles [0] << " " << interiorAngles [1] << " " << interiorAngles [2] << " " << interiorAngles [3] << endl;
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
	const float error = 4.0f;

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
	const float error = 4.0f;

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

int main () {
	vector< Coord > coords = vector< Coord > ();

	/* Pentagon
	coords.push_back (Coord (550,450));
	coords.push_back (Coord (455,519));
	coords.push_back (Coord (491,631));
	coords.push_back (Coord (609,631));
	coords.push_back (Coord (645,519));
	coords.push_back (Coord (550,450));
	*/

	/* Hexagon
	coords.push_back (Coord (600,463));
	coords.push_back (Coord (500,463));
	coords.push_back (Coord (450,550));
	coords.push_back (Coord (500,637));
	coords.push_back (Coord (600,637));
	coords.push_back (Coord (650,550));
	coords.push_back (Coord (600,463));
	*/

	/* Quadrilateral
	coords.push_back (Coord (200,200));
	coords.push_back (Coord (500,200));
	coords.push_back (Coord (500,330));
	coords.push_back (Coord (200,330));
	coords.push_back (Coord (200,200));
	*/

	coords.push_back (Coord (550,450));
	coords.push_back (Coord (463,600));
	coords.push_back (Coord (637,600));
	coords.push_back (Coord (550,450));

	cout << detectShapeDecisionTree (coords) << endl;
}