#pragma once
#include <optional>


class Point;
class Line;

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
	//

	// Нужны здесь, перенос в классы неудобен потому то функция взаимодействует с 2мя обьектами и в классе придется для каждой пары делать
	static double distance(const Point& point1, const Point& point2);   
	static double distance(const Line& line, const Point& point);       
	static double distance(const Point& point, const Line& line);

	static std::optional<Point> findIntersection(const Line& line1, const Line& line2);
	//

	// надо перенести 
	static Line getParallel(const Line& line, const  Point& point);      // перенести в класс и добавить в интерфейс
	static Line getPerpendicular(const Line& line, const  Point& point); // перенести в класс и добавить в интерфейс
	//
}; 


