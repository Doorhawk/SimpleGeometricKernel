#pragma once
#include <optional>
#include <vector>

class Point;
class Line;
class Circle;

class go {
private:
	static double precision;
public:
	static void setPrecision(double newPrecision);
	static double getPrecision();

	// перенесены в классы, здесь можно удалить
	static void move(Point& point, double dx, double dy);
	static void move(Line& line, double dx, double dy);
	static void rotate(Point& point, const Point& center, double angle);
	static void rotate(Line& line, const Point& center, double angle);
	static Line getParallel(const Line& line, const  Point& point);      // перенести в класс и добавить в интерфейс
	static Line getPerpendicular(const Line& line, const  Point& point); // перенести в класс и добавить в интерфейс
	//

	// перенос в классы не дает преимуществ и как-то логично чтобы были отдельно 
	static double distance(const Point& point1, const Point& point2);   
	static double distance(const Line& line, const Point& point);       
	static double distance(const Point& point, const Line& line);
	static std::vector<Point> findIntersection(const Line& line1, const Line& line2);
	static std::vector<Point> findIntersection(const Circle& circle, const Line& line);
	static std::vector<Point> findIntersection(const Line& line, const Circle& circle);
	static std::vector<Point> findIntersection(const Circle& circle1, const Circle& circle2);
	// на счет этой не уверен
	static Point findMiddle(const Point& point1,const Point& point2);

	//

	
}; 


