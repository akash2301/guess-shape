#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#define PI 3.14159265
#define SMALL_NUMBER 1e-5

using namespace std;

class Coord {
	public:
		float x, y;

	Coord (float a, float b) {
		x = a;
		y = b;
	}
};

bool areFloatValuesSimilar (float a, float b) {
	return abs (a - b) < 5.0f;
}

float computeAngle (Coord mid, Coord left, Coord right) {
	const float deg = 180.0f / PI;

	// x values of 2 points are same, subtract a small number to the non-mid point to avoid divide-by-zero
	if (left.x == mid.x) {
		left.x -= SMALL_NUMBER;
	}

	if (right.x - mid.x) {
		right.x -= SMALL_NUMBER;
	}

	float gradML = (left.y - mid.y) / (left.x - mid.x),
		gradMR = (mid.y - right.y) / (mid.x - right.x);
	float eq = abs ((gradML - gradMR) / (1+(gradMR*gradML)));

	//return min (angle, 180 - angle);
	return atan (eq) * deg;
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
	unsigned int flags = 0;

	if (areFloatValuesSimilar (interiorAngles [0], interiorAngles [1])) {
		flags |= 1;
	}

	if (areFloatValuesSimilar (interiorAngles [0], interiorAngles [2])) {
		flags |= 2;
	}

	if (areFloatValuesSimilar (interiorAngles [1], interiorAngles [2])) {
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
	return "ss";
}

bool isRegularPentagon (vector< Coord >& coords) {
	return true;
}

bool isRegularHexagon (vector< Coord >& coords) {
	return true;
}


string detectShapeDecisionTree (vector< Coord >& coords) {
	const string notAValidShape = "Not a valid Shape";

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

	coords.push_back (Coord (10, 10));
	coords.push_back (Coord (10, 40));
	coords.push_back (Coord (50, 10));
	coords.push_back (Coord (50, 40));

	cout << detectShapeDecisionTree (coords) << endl;
}