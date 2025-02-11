#pragma once
#include <optional>
#include <vector>
#include <climits>
class Point;
class Line;
class LineSimple;
class Circle;

class go {
private:
	static double precision;
public:
	static void setPrecision(double newPrecision);
	static double getPrecision();

	// перенесены в классы, здесь можно удалить
	//static void move(Point& point, double dx, double dy);
	//static void move(Line& line, double dx, double dy);
	//static void rotate(Point& point, const Point& center, double angle);
	//static void rotate(Line& line, const Point& center, double angle);
	//static Line getParallel(const Line& line, const  Point& point);      // перенести в класс и добавить в интерфейс
	std::shared_ptr<Line> getPerpendicular(std::shared_ptr<Line> line, std::shared_ptr<Point> point); // перенести в класс и добавить в интерфейс
	////

	//// перенос в классы не дает преимуществ и как-то логично чтобы были отдельно 
	static double distance(const Point& point1, const Point& point2);   
	static double distance(std::shared_ptr<Line> line, const Point& point);
	static double distance(const Point& point, std::shared_ptr<Line> line);
	static double distance(const Point& point, std::shared_ptr<Circle> circle);
	static double distance(std::shared_ptr<Circle> circle, const Point& point);
	static std::vector<Point> findIntersection(Line line1, Line line2);

	

	//static std::vector<Point> findIntersection(const Circle& circle, const Line& line);
	//static std::vector<Point> findIntersection(const Line& line, const Circle& circle);
	//static std::vector<Point> findIntersection(const Circle& circle1, const Circle& circle2);
	//// ...
	static Point findMiddle(const Point& point1,const Point& point2);
	//static double findAngle(const Line& line, const Line& line1);
	//static double findAngle(const Point& point1, const Point& point2, const Point& point3);
	

	//

	
}; 




