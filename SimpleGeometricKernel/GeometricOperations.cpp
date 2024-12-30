#include "BasicShape.h"
#include "GeometricOperations.h"

void go::move(Point2D& point, double dx, double dy) {
	point.x += dx;
	point.y += dy;
}
double go::distance(Point2D& point1, Point2D& point2) {
	return sqrt((point1.x - point2.x)* (point1.x - point2.x)+ (point1.y - point2.y)* (point1.y - point2.y));
}