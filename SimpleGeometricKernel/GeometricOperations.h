#pragma once

class Point2D;

class go {
public:
	static void move(Point2D& point, double dx, double dy);

	static double distance(Point2D& point1, Point2D& point2);
}; 

