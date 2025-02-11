#include "BasicShape.h"
#include "GeometricOperations.h"



    



double go::precision = 1e-8;

void go::setPrecision(double newPrecision) {
	if (newPrecision <= 0) {
		throw std::invalid_argument("Precision must be positive.");
	}
	precision = newPrecision;
}
double go::getPrecision() {
	return precision;
}

//
//void go::move(Point& point, double dx, double dy) {
//	point.x += dx;
//	point.y += dy;
//}
//void go::move(Line& line, double dx, double dy) {
//	line.p1.x += dx;
//	line.p2.x += dx;
//	line.p1.y += dy;
//	line.p2.y += dy;
//}
//
//void go::rotate(Point& point, const Point& center, double angle) {
//
//	point.x -= center.x;
//	point.y -= center.y;
//
//	double newx = cos(angle) * point.x - sin(angle) * point.y;
//	double newy = sin(angle) * point.x + cos(angle) * point.y;
//
//	point.x = newx + center.x;
//	point.y = newy + center.y;
//}
//void go::rotate(Line& line, const Point& center, double angle) {
//
//	line.p1.x -= center.x;
//	line.p1.y -= center.y;
//
//	double newx = cos(angle) * line.p1.x - sin(angle) * line.p1.y;
//	double newy = sin(angle) * line.p1.x + cos(angle) * line.p1.y;
//
//	line.p1.x = newx + center.x;
//	line.p1.y = newy + center.y;
//
//	line.p2.x -= center.x;
//	line.p2.y -= center.y;
//
//	newx = cos(angle) * line.p2.x - sin(angle) * line.p2.y;
//	newy = sin(angle) * line.p2.x + cos(angle) * line.p2.y;
//
//	line.p2.x = newx + center.x;
//	line.p2.y = newy + center.y;
//}
//
//


double go::distance(const Point& point1,const Point& point2) {
	return sqrt((point1.x - point2.x)* (point1.x - point2.x)+ (point1.y - point2.y)* (point1.y - point2.y));
}
double go::distance(std::shared_ptr<Line> line, const Point& point) {
	// a = (x1 - x2,y1 - y2) - направление прямой
	// (a,w) 

	Vector a = line->p2 - line->p1; // - направление прямой
	Vector w = line->p1 - point; // - направление между точкой и первой точкой прямой
	double answ = abs(a.x * w.y - a.y * w.x); // Модуль определителя (векторное произведение в 2D)

	if (a.abs() == 0) {
		throw std::invalid_argument("The two points defining the line are the same.");
	}

	answ = answ / a.abs(); // Sp = h * abs(a); => h = Sp/abs(a)

	return answ;
}
double go::distance(const Point& point, std::shared_ptr<Line> line) {
	return go::distance(line, point);
}
double go::distance(std::shared_ptr<Circle> circle, const Point& point) {

	double distCentPoint = go::distance(point, circle->center);
	return  abs(distCentPoint - circle->radius);
}
double go::distance(const Point& point, std::shared_ptr<Circle> circle) {
	return go::distance(circle, point);
}
//
//Line go::getParallel(const Line& line, const  Point& point) {
//	
//	Point p1 = point;
//	Point p2 = { line.p2.x+point.x-line.p1.x,line.p2.y + point.y- line.p1.y};
//
//	return Line(p1, p2);
//	
//}
std::shared_ptr<Line> go::getPerpendicular(std::shared_ptr<Line> line, std::shared_ptr<Point> point) {
	Point p1 = (line->p1 - line->p2);
	p1 = { -p1.y,p1.x };
	return std::make_shared<Line>(Line((p1 + *point), *point));
}

std::vector<Point> go::findIntersection(Line line1, Line line2) {
	double A1 = line1.p2.x - line1.p1.x;
	double B1 = -(line2.p2.x - line2.p1.x);
	double C1 = line2.p1.x - line1.p1.x;

	double A2 = line1.p2.y - line1.p1.y;
	double B2 = -(line2.p2.y - line2.p1.y);
	double C2 = line2.p1.y - line1.p1.y;

	double det = A1 * B2 - A2 * B1;

	double t = 0;
	if (std::abs(det) < precision)
		return {};
	else
		t = (C1 * B2 - C2 * B1) / det;

	// Вычисляем точку пересечения
	Point intersection;
	intersection.x = line1.p1.x + t * (line1.p2.x - line1.p1.x);
	intersection.y = line1.p1.y + t * (line1.p2.y - line1.p1.y);

	return { intersection };
	/*
 * To find the intersection of two lines defined by two pairs of points:
 * Line 1: P1(x1, y1), P2(x2, y2)
 * Line 2: Q1(x3, y3), Q2(x4, y4)
 *
 * Lines are parameterized as follows:
 * 1. P(t) = P1 + t(P2 - P1)
 *    Expanded: x = x1 + t(x2 - x1), y = y1 + t(y2 - y1)
 * 2. Q(u) = Q1 + u(Q2 - Q1)
 *    Expanded: x = x3 + u(x4 - x3), y = y3 + u(y4 - y3)
 *
 * The goal is to solve for t and u where P(t) = Q(u), i.e., the intersection point.
 *
 * Step-by-step process:
 *
 * 1. Create two equations based on the parameterization:
 *    x1 + t(x2 - x1) = x3 + u(x4 - x3)
 *    y1 + t(y2 - y1) = y3 + u(y4 - y3)
 *
 * 2. Rearrange the equations:
 *    t(x2 - x1) - u(x4 - x3) = x3 - x1
 *    t(y2 - y1) - u(y4 - y3) = y3 - y1
 *
 * 3. Define coefficients:
 *    A1 = x2 - x1, B1 = -(x4 - x3), C1 = x3 - x1
 *    A2 = y2 - y1, B2 = -(y4 - y3), C2 = y3 - y1
 *
 *    This gives a system of linear equations:
 *    A1 * t + B1 * u = C1
 *    A2 * t + B2 * u = C2
 *
 * 4. Solve using Cramer's rule:
 *    Determinant: D = A1 * B2 - A2 * B1
 *
 *    If D == 0, the lines are parallel or coincident.
 *    Otherwise, solve for t and u:
 *    Dt = C1 * B2 - C2 * B1
 *    Du = A1 * C2 - A2 * C1
 *
 *    t = Dt / D
 *    u = Du / D
 *
 * 5. Find the intersection point:
 *    Substitute t (or u) into the parameterized equation for one of the lines:
 *    x = x1 + t(x2 - x1)
 *    y = y1 + t(y2 - y1)
 *
 * Notes:
 * - Use a precision threshold to check if D is close to 0, indicating parallel or overlapping lines.
 * - Return std::optional<Point> to handle cases where the lines do not intersect.
 */
}

//
//std::vector<Point> go::findIntersection(const Circle& circle, const Line& line) {
//	if (go::distance(circle.cen, line)>circle.rad+precision) {
//		return {};
//	}
//	else if (go::distance(circle.cen, line) >= circle.rad) {
//		Line perpend = line.getPerpendicular(circle.cen);
//		vector<Point> inter = go::findIntersection(perpend, line);
//		return { inter[0] };
//	}
//	else {
//		Line perpend = line.getPerpendicular(circle.cen);
//		vector<Point> inter = go::findIntersection(perpend, line);
//
//		double dist = sqrt(circle.rad * circle.rad - Vector(circle.cen - inter[0]).abs()* Vector(circle.cen - inter[0]).abs());
//		Vector vec = line.p2 - line.p1;
//		vec = vec.normalize();
//		vec = vec * dist;
//
//		return { inter[0] + vec,inter[0] - vec };
//	}
//	return {};
//}
//std::vector<Point> go::findIntersection(const Line& line, const Circle& circle) {
//	return findIntersection(circle, line);
//}
//std::vector<Point> go::findIntersection(const Circle& circle1, const Circle& circle2) {
//	double d = go::distance(circle1.cen, circle2.cen);
//	if ( d > circle1.rad +circle2.rad+ precision||d< abs(circle1.rad - circle2.rad) - precision) {
//		return {};
//	}
//	else if (d >= circle1.rad + circle2.rad|| d <= abs(circle1.rad - circle2.rad)) {
//		Vector vec = (circle2.cen - circle1.cen);
//		vec = vec.normalize();
//		vec = vec * circle1.rad;
//		return { circle1.cen+vec };
//	}
//	else {
//		double a = (circle1.rad * circle1.rad - circle2.rad * circle2.rad + d * d) / (2 * d);
//		double h = sqrt(circle1.rad * circle1.rad - a * a);
//		Point p3 = circle1.cen + (circle2.cen - circle1.cen) * (a/d);
//		
//		Point in1 = { p3.x + h * (circle2.cen.y - circle1.cen.y) / d, p3.y - h * (circle2.cen.x - circle1.cen.x) / d };
//		Point in2 = { p3.x - h * (circle2.cen.y - circle1.cen.y) / d, p3.y + h * (circle2.cen.x - circle1.cen.x) / d };
//		return { in1,in2 };
//	}
//	return {};
//}
//
Point go::findMiddle(const Point& point1, const Point& point2) {
	return Point((point2.x + point1.x) / 2, (point2.y + point1.y) / 2);
}
//double go::findAngle(const Line& line1, const Line& line2) {
//	Vector vec1 = line1.p1 - line1.p2;
//	Vector vec2 = line2.p1 - line2.p2;
//	double angle = acos(vec1 * vec2 / vec1.abs() / vec2.abs());
//	return min(angle, acos(-1)-angle);
//}
//double go::findAngle(const Point& point1, const Point& point2, const Point& point3) {
//	return findAngle(Line(point1, point2), Line(point3, point2));
//}