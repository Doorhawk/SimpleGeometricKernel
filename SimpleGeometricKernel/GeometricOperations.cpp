#include "BasicShape.h"
#include "GeometricOperations.h"

void go::move(Point& point, double dx, double dy) {
	point.x += dx;
	point.y += dy;
}
void go::move(Line& line, double dx, double dy) {
	line.p1.x += dx;
	line.p2.x += dx;
	line.p1.y += dy;
	line.p2.y += dy;
}

void go::rotate(Point& point, const Point& center, double angle) {

	point.x -= center.x;
	point.y -= center.y;

	double newx = cos(angle) * point.x - sin(angle) * point.y;
	double newy = sin(angle) * point.x + cos(angle) * point.y;

	point.x = newx + center.x;
	point.y = newy + center.y;
}
void go::rotate(Line& line, const Point& center, double angle) {

	line.p1.x -= center.x;
	line.p1.y -= center.y;

	double newx = cos(angle) * line.p1.x - sin(angle) * line.p1.y;
	double newy = sin(angle) * line.p1.x + cos(angle) * line.p1.y;

	line.p1.x = newx + center.x;
	line.p1.y = newy + center.y;

	line.p2.x -= center.x;
	line.p2.y -= center.y;

	newx = cos(angle) * line.p2.x - sin(angle) * line.p2.y;
	newy = sin(angle) * line.p2.x + cos(angle) * line.p2.y;

	line.p2.x = newx + center.x;
	line.p2.y = newy + center.y;
}


double go::distance(const Point& point1,const Point& point2) {
	return sqrt((point1.x - point2.x)* (point1.x - point2.x)+ (point1.y - point2.y)* (point1.y - point2.y));
}
double go::distance(const Line& line, const Point& point) {
	// a = (x1 - x2,y1 - y2) - направление прямой
	// (a,w) 

	Vector a = line.p2 - line.p1; // - направление прямой
	Vector w = line.p1 - point; // - направление между точкой и первой точкой прямой
	double answ = abs(a.x * w.y - a.y * w.x); // Модуль определителя (векторное произведение в 2D)

	if (a.abs() == 0) {
		throw std::invalid_argument("The two points defining the line are the same.");
	}

	answ = answ / a.abs(); // Sp = h * abs(a); => h = Sp/abs(a)

	return answ;
}

Line go::getParallel(const Line& line, const  Point& point) {
	
	Point p1 = point;
	Point p2 = { line.p2.x+point.x-line.p1.x,line.p2.y + point.y- line.p1.y};

	return Line(p1, p2);
	
}
Line go::getPerpendicular(const Line& line, const  Point& point) {
	Point p1 = (line.p1 - line.p2);
	p1 = { -p1.y,p1.x };
	return Line(p1 + point, point);
}