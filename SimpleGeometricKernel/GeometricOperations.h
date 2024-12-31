#pragma once

class Point;
class Line;

class go {
public:
	static void move(Point& point, double dx, double dy);
	static void move(Line& line, double dx, double dy);

	static void rotate(Point& point, const Point& center, double angle);
	static void rotate(Line& line, const Point& center, double angle);

	static Line getParallel(const Line& line, const  Point& point);
	static Line getPerpendicular(const Line& line, const  Point& point);

	static double distance(const Point& point1, const Point& point2);
	static double distance(const Line& line, const Point& point);
}; 

