#pragma once
#include <SFML/Graphics.hpp>
#include "GeometricOperations.h"
#include "global.h"
#include "climits"
#include <string>
#include <iostream>
#include "enums.h"
#include <array>
#include <sstream>

using namespace sf;
using namespace std;



class Point;
class Line;

class Vector {
private:
    double x, y;
    friend class go;
    friend class Point;
    friend class Line;
public:
    double abs() const;
    Vector normalize() const;
    Vector();
    Vector(double x, double y);
    Vector(const Vector& other);
    Vector(const Point& other);
    Vector& operator=(Point& other);
    Vector& operator=(const Vector& other);
    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    double operator*(const Vector& other) const;
    Vector operator*(double scalar) const;
    Vector operator/(double scalar) const;
};

class Depends : public std::enable_shared_from_this<Depends> {
private:
protected:
    std::vector<std::weak_ptr<Depends>> children;
    std::vector <std::weak_ptr<Depends>> parent;
    DependsTypes dependsType = DependsTypes::None;
    bool isUpdating = false;
    ShapeType type;
    int index = 0;
    Color color = Color::Black;
    void setType(const ShapeType newType);
public:
    Depends(const ShapeType type);
    ShapeType getType() const;
    virtual ~Depends();
    void onDelete();
    void setDependsType(DependsTypes dp);
    void setIndex(int _index);
    void setColor(Color color);
    Color getColor();
    // Установка родителя с указанием типа зависимости
    void setParent(DependsTypes _type, const std::vector <std::weak_ptr<Depends>>& _parent);
    // Добавление ребёнка
    void addChild(const std::shared_ptr<Depends>& child);
    string printFamilyInfo() const;
    void removeExpiredChildren();
    // Уведомление ребёнка о том, что родитель удалён
    void onParentDeleted();
    // Уведомление детей об изменении
    void notifyChildren();
    DependsTypes getDependsType();
    virtual void update() = 0;
    virtual void init() = 0;
};
class BasicShape : public Depends {
private:
    bool valid;
protected:
public:
    BasicShape(const ShapeType type);
    virtual void draw(sf::RenderWindow& window, int num, sf::Font& font) const = 0; // Метод отрисовки virtual => у каждого потомка свой должен быть оперделен
    virtual std::string printInf() const = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void rotate(const Point& center, double angle) = 0;
    virtual ~BasicShape() = default; // деструктор у каждого потомка свой по умолчанью
    void setInvalid();
    void setValid();
    bool getValid();
};

class Point : public BasicShape {
private:
    double x, y;
    friend class go;
    friend class Line;
    friend class Segment;
    friend class Circle;
    friend class Sector;
    friend class Poligon;
protected:
    double dependsX;
    double dependsY;
public:
    Point(double x = 0, double y = 0);
    Point(const Point& other);
    Point(const Vector& other);
    void draw(sf::RenderWindow& window, int num, Font& font) const override;
    std::string printInf() const;
    void move(double dx, double dy) override;
    void rotate(const Point& center, double angle) override;
    Point& operator=(const Point& other);
    Point operator+(const Point& other) const;
    Point operator+(const Vector& other) const;
    Point operator-(const Vector& other) const;
    Point operator-(const Point& other) const;
    Point operator*(double scalar) const;
    bool operator==(const Point& other) const;
    double getX() const;
    double getY() const;
    void setX(double newx);
    void setY(double newy);
    void update() override;
    void init() override;
};
class Line : public virtual BasicShape {
protected:
    std::shared_ptr<Point> p1, p2;
    friend class go;
    friend class Point;
    double fun(double x) const;
    Line(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2);

public:
    // Фабричный метод для создания объекта
    static std::shared_ptr<Line> create(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2);
    virtual std::string printInf() const override;
    void move(double dx, double dy) override;
    void rotate(const Point& center, double angle) override;
    Line& operator=(const Line& other);
    Point getStart() const;
    Point getEnd() const;
    virtual void draw(sf::RenderWindow& window, int num, sf::Font& font) const override;
    void update() override;
    void init() override;
};


class Circle : public virtual BasicShape {
private:
    std::shared_ptr<Point> center;
    std::shared_ptr<Point> onCircle;
    double radius = 0;
    friend class go;
    Circle(std::shared_ptr<Point> center, std::shared_ptr<Point> onCircle);
protected:
    
public:
    static std::shared_ptr<Circle> create(std::shared_ptr<Point> _center, std::shared_ptr<Point> _onCircle);
    virtual std::string printInf() const override;
    void move(double dx, double dy) override;
    virtual void rotate(const Point& _center, double angle) override;
    Circle& operator=(const Circle& other);
    virtual void draw(sf::RenderWindow& window, int num, sf::Font& font) const override;
    std::shared_ptr<Point> getCenter() const;
    std::shared_ptr<Point> getOnCircle() const;
    double getRadius() const;
    void update() override;
    void init() override;
    
};


//class Segment : public Line {
//private:
//    friend class go;
//public:
//    Segment() :Line(Point(0, 0), Point(1, 1)), BasicShape("segment") {}
//    Segment(Point p1, Point p2) : Line(p1, p2), BasicShape("segment") {}
//    Segment(const Segment& other) :Line(other.p1, other.p2), BasicShape("segment") {};
//    Segment(const Line& other) :Line(other.getStart(), other.getEnd()), BasicShape("segment") {};
//
//    void printInf() const override {
//        cout << "Sector: (" << p1.x << ", " << p1.y << ") " << p2.x << ", " << p2.y << ")" << endl;
//    }
//    vector<Segment> divide(const Point& point) const {
//        if (point == p1 || point == p2) {
//            return {};
//        }
//        if (abs(fun(point.x) - point.y) < go::getPrecision()) {
//            return { Segment(p1,point),Segment(point,p2) };
//        }
//        else {
//            return {};
//        }
//    }
//    void draw(sf::RenderWindow& window, int num, sf::Font& font) const override {
//
//        sf::VertexArray line(sf::Lines, 2);
//        line[0].position = sf::Vector2f(p1.x, p1.y);
//        line[1].position = sf::Vector2f(p2.x, p2.y);
//        line[0].color = Color::Black;
//        line[1].color = Color::Black;
//        window.draw(line);
//
//        Text text;
//        text.setFont(font);
//        text.setScale(1 * global::size, -1 * global::size);
//        Point mid = go::findMiddle(p1, p2);
//        text.setPosition(mid.x + 15 * global::size, mid.y - 15 * global::size);
//        text.setString("sg" + std::to_string(num));
//        text.setCharacterSize(15);
//        text.setFillColor(Color::Black);
//        window.draw(text);
//
//    }
//    
//};
//
//
//
//class Sector : public Circle {
//private:
//    double endAngle, startAngle;
//    friend class go;
//    // нужна проверка что разница углов не более 360, ну или если больше то вычитаем из каждого по 360 привести в норму
//public:
//    Sector() :Circle(Point(0, 0), 1),BasicShape("sector"), startAngle(0), endAngle(acos(-1)) {}
//    Sector(Point cen, double rad, double startAngle, double endAngle) :Circle(cen, rad), BasicShape("sector"), startAngle(startAngle), endAngle(endAngle) {}
//    Sector(Point cen, Point point, double startAngle, double endAngle) :Circle(cen, Vector(cen - point).abs()), BasicShape("sector"), startAngle(startAngle), endAngle(endAngle) {}
//    Sector(const Sector& other) :Circle(other.getCenter(), other.getRadius()), BasicShape("sector"), startAngle(other.startAngle), endAngle(other.endAngle) {}
//    void printInf() const override {
//        cout << "secrot: center (" << getCenter().x << ", " << getCenter().y << "), radius = " << getRadius() << ", endAngle = " << endAngle << ", startAngle = " << startAngle << endl;
//    }
//    void rotate(const Point& center, double angle) override {
//        
//        Point cen = getCenter();
//        cen.rotate(center, angle);
//        setCenter(cen);
//
//        startAngle += angle;
//        endAngle += angle;
//    }
//    Sector& operator=(const Sector& other) {
//        if (this != &other) {
//            setCenter(other.getCenter());
//            setRadius(other.getRadius());
//            endAngle = other.endAngle;
//            startAngle = other.startAngle;
//        }
//        return *this;
//    }
//    void draw(sf::RenderWindow& window, int num, sf::Font& font) const override {
//        int pointCount = 50;
//        // Создаем массив вершин (тип треугольный фан)
//        sf::VertexArray sector(sf::Lines, pointCount);
//
//
//        Point cen = getCenter();
//        double rad = getRadius();
//
//        // Вычисляем точки сектора
//        double angleStep = (endAngle - startAngle) / pointCount; // Шаг между углами
//        for (int i = 0; i < pointCount; ++i) {
//            sf::VertexArray line(sf::Lines, 2);
//
//            float angle = startAngle + i * angleStep;
//            float x1 = cen.x + rad * cos(angle);
//            float y1 = cen.y + rad * sin(angle);
//            line[0].position = sf::Vector2f(x1, y1);
//            angle = startAngle + (i + 1) * angleStep;
//            x1 = cen.x + rad * cos(angle);
//            y1 = cen.y + rad * sin(angle);
//            line[1].position = sf::Vector2f(x1, y1);
//
//
//
//            line[0].color = Color::Black;
//            line[1].color = Color::Black;
//
//            window.draw(line);
//        }
//
//
//        float x1 = cen.x + rad * cos(endAngle);
//        float y1 = cen.y + rad * sin(endAngle);
//        Text text;
//        text.setFont(font);
//        text.setScale(1 * global::size, -1 * global::size);
//        text.setPosition(x1, y1);
//        text.setString("sc" + std::to_string(num));
//        text.setCharacterSize(15);
//        text.setFillColor(Color::Black);
//        window.draw(text);
//    }
//    Point getStartPoint() const {
//        Point stp = getCenter() + Point(getRadius() * cos(startAngle), getRadius() * sin(startAngle));
//        return stp;
//    }
//    Point getEndPoint() const {
//        Point enp = getCenter() + Point(getRadius() * cos(endAngle), getRadius() * sin(endAngle));
//        return enp;
//    }
//   /* vector<Segment> divide(const Point& point) const {
//        double 
//    }*/
//};
//
//
//class Poligon : public BasicShape {
//private:
//    vector<Point> points;
//    friend class go;
//public:
//    Poligon() :points({}), BasicShape("poligon") {}
//    Poligon(vector<Point>& vec) :BasicShape("poligon"), points(vec) {}
//    Poligon(const Poligon& other) :BasicShape("poligon"),points(other.points) {}
//
//    void printInf() const override {
//        cout << "poligon: \n";
//        for(auto& p : points)
//            cout<< "\t( " << p.x << ", " << p.y << ")"<< endl;
//    }
//    void move(double dx, double dy) override {
//        for (auto& p : points) {
//            p.move(dx, dy);
//        }
//    }
//    void rotate(const Point& center, double angle) override {
//
//        for (auto& p : points) {
//            p.rotate(center, angle);
//        }
//        
//    }
//    Poligon& operator=(const Poligon& other) {
//        if (this != &other) {
//            for (auto& p : other.points) {
//                points.push_back(p);
//            }
//        }
//        return *this;
//    }
//    void draw(sf::RenderWindow& window, int num, sf::Font& font) const override {
//
//        double maxX = points[0].x;
//        double minY = points[0].y;
//
//        for (size_t i = 0; i < points.size(); i++) {
//            size_t nextIndex = (i + 1) % points.size(); // Индекс следующей точки (для замыкания)
//
//            sf::VertexArray line(sf::Lines, 2);
//            line[0].position = sf::Vector2f(points[i].x, points[i].y);
//            line[1].position = sf::Vector2f(points[nextIndex].x, points[nextIndex].y);
//            line[0].color = Color::Black;
//            line[1].color = Color::Black;
//            window.draw(line);
//
//            maxX = std::max(maxX, points[i].x);
//            minY = std::min(minY, points[i].y);
//        }
//        
//
//        Text text;
//        text.setFont(font);
//        text.setScale(1 * global::size, -1 * global::size);
//        text.setPosition(maxX, minY);
//        text.setString("pl" + std::to_string(num));
//        text.setCharacterSize(15);
//        text.setFillColor(Color::Black);
//        window.draw(text);
//    }
//    double getArea() {
//        int size = points.size();
//        if (size == 0)
//            return 0;
//        double answ = 0;
//        for (int i = 0; i < size - 1; i++) {
//            answ += (points[i].x * points[i + 1].y - points[i].y * points[i + 1].x);
//        }
//        answ += (points[size - 1].x * points[0].y - points[size - 1].y * points[0].x);
//        return abs(answ) / 2;
//    }
//    vector<Point> getPoints() {
//        return points;
//    }
//};

