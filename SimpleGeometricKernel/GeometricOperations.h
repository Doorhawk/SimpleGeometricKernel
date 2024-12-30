#pragma once

class Point2D;
class Line;

class go {
public:
	static void move(Point2D& point, double dx, double dy);
	static void move(Line& line, double dx, double dy);

	static void rotate(Point2D& point, Point2D& center, double angle);
	static void rotate(Line& line, Point2D& center, double angle);

	static double distance(Point2D& point1, Point2D& point2);
}; 

