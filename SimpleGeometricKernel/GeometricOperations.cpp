#include "BasicShape.h"
#include "GeometricOperations.h"

void go::move(Point2D& point, double dx, double dy) {
	point.x += dx;
	point.y += dy;
}
void go::move(Line& line, double dx, double dy) {
	line.p1.x += dx;
	line.p2.x += dx;
	line.p1.y += dy;
	line.p2.y += dy;
}
double go::distance(Point2D& point1, Point2D& point2) {
	return sqrt((point1.x - point2.x)* (point1.x - point2.x)+ (point1.y - point2.y)* (point1.y - point2.y));
}