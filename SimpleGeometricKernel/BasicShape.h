#pragma once
#include <SFML/Graphics.hpp>
#include "GeometricOperations.h"
#include "global.h"
#include "climits"
#include <string>
#include <iostream>

using namespace sf;
using namespace std;

class Vector;

class BasicShape {
private:
public:
    virtual void draw(sf::RenderWindow& window, int num, sf::Font& font) const = 0; // Метод отрисовки virtual => у каждого потомка свой должен быть оперделен
    virtual void printInf() const = 0; 
    virtual void move(double dx, double dy) = 0; 
    virtual void rotate(const Point& center, double angle) = 0;
    virtual ~BasicShape() = default; // деструктор у каждого потомка свой по умолчанью
};

class Point : public BasicShape {
private:
    double x, y;
    friend class go;
    friend class Line;
    friend class Circle;
    friend class Poligon;
public:
    Point(double x = 0, double y = 0) :x(x), y(y) {}
    Point(const Point& other) :x(other.x), y(other.y) {}
    void draw(sf::RenderWindow& window, int num, Font& font) const override {
        float r = 3.f * global::size;
        CircleShape point(r);
        point.setPosition(x-r, y-r);
        point.setFillColor(Color::Black);
        window.draw(point);

        Text text;
        text.setFont(font);
        text.setScale(1* global::size, -1* global::size);
        text.setPosition(x - 25 * global::size, y + 15 * global::size);
        text.setString("p"+std::to_string(num));
        text.setCharacterSize(15);
        text.setFillColor(Color::Black);
        window.draw(text);
    }
    void printInf() const {
        cout << "point: (" << x << ", " << y << ")"<<endl;
    }
    void move(double dx, double dy) {
        x += dx;
        y += dy;
    }
    void rotate(const Point& center, double angle) {
        x -= center.x;
        y -= center.y;

        double newx = cos(angle) * x - sin(angle) * y;
        double newy = sin(angle) * x + cos(angle) * y;

        x = newx + center.x;
        y = newy + center.y;
    }
    Point& operator=(const Point& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
        }
        return *this;
    }
    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }
    Point operator+(const Vector& other) const;
    Point operator-(const Vector& other) const;
    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }
    Point operator*(double scalar) const {
        return(Point(x * scalar, y * scalar));
    }
    bool operator==(const Point& other) const {
        return (x == other.x) && (y == other.y);
    }
    double getX() const {
        return x;
    }
    double getY() const {
        return y;
    }

};

class Vector {
private:
    double x, y;
    friend class go;
    friend class Point;
public:
    double abs() const {
        return sqrt(x * x + y * y);
    }
    Vector normalize() const {
        double length = abs();
        if (length != 0) {
            return *this / length;
        }
        throw std::invalid_argument("Cannot normalize a zero vector.");
    }
    Vector() : x(0), y(0) {}
    Vector(double x, double y) : x(x), y(y) {}
    Vector(const Vector& other) : x(other.x), y(other.y) {}
    Vector(const Point& other) : x(other.getX()), y(other.getY()) {}
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
        }
        return *this;
    }
    Vector& operator=(Point& other) {
        x = other.getX();
        y = other.getY();
        return *this;
    }
    Vector operator+(const Vector& other) const {
        return Vector(x + other.x, y + other.y);
    }
    Vector operator-(const Vector& other) const {
        return Vector(x - other.x, y - other.y);
    }
    double operator*(const Vector& other) const {
        return x * other.x + y * other.y;
    }
    Vector operator*(double scalar) const {
        return Vector(x * scalar, y * scalar);
    }
    Vector operator/(double scalar) const {
        if (scalar != 0) {
            return Vector(x / scalar, y / scalar);
        }
        else {
            throw std::invalid_argument("Division by zero.");
        }
    }
};


class Line : public BasicShape {
private:
    Point p1,p2;
    bool isSegment;
    friend class go;
    double fun(double x) const{
        return (x - p1.x) * (p2.y - p1.y) / (p2.x - p1.x) + p1.y;
    }
public:
    Line() :p1({ 0,0 }), p2({1,1}),isSegment(false) {}
    Line(Point p1, Point p2,bool isSegment = false) :p1(p1), p2(p2),isSegment(isSegment) {}
    Line(const Line& other) :p1(other.p1), p2(other.p2),isSegment(other.isSegment) {}
    void printInf() const override {
        cout << "line: (" << p1.x << ", " << p1.y << ") " << p2.x << ", " << p2.y << ")" <<endl;
    }
    void move(double dx, double dy) override {
        p1.x += dx;
        p2.x += dx;
        p1.y += dy;
        p2.y += dy;
    }
    void rotate(const Point& center, double angle) override {
        p1.x -= center.x;
        p1.y -= center.y;

        double newx = cos(angle) * p1.x - sin(angle) * p1.y;
        double newy = sin(angle) * p1.x + cos(angle) * p1.y;

        p1.x = newx + center.x;
        p1.y = newy + center.y;

        p2.x -= center.x;
        p2.y -= center.y;

        newx = cos(angle) * p2.x - sin(angle) * p2.y;
        newy = sin(angle) * p2.x + cos(angle) * p2.y;

        p2.x = newx + center.x;
        p2.y = newy + center.y;
    }
    Line getParallel(const Point& point) const {
        Point p = point;
        Point q = { p2.x + point.x - p1.x,p2.y + point.y - p1.y };
        return Line(p, q);
    }
    Line getPerpendicular(const Point& point) const {
        Point p = (p2 - p1);
        p = { -p.y,p.x };
        return Line(p + point, point);
    }
    Line& operator=(const Line& other) {
        if (this != &other) {
            p1 = other.p1;
            p2 = other.p2;
        }
        return *this;
    }
    void draw(sf::RenderWindow& window, int num, sf::Font& font) const override {
        if (isSegment) {
            sf::VertexArray line(sf::Lines, 2);
            line[0].position = sf::Vector2f(p1.x, p1.y);
            line[1].position = sf::Vector2f(p2.x, p2.y);
            line[0].color = Color::Black;
            line[1].color = Color::Black;
            window.draw(line);
        }
        else {
            View view = window.getView();
            float size = view.getSize().x + abs(view.getCenter().x) * 2 + abs(view.getCenter().y) * 2;
            sf::VertexArray line(sf::Lines);
            if ((p2.x - p1.x) != 0) {
                float ysize = (size - p1.x) * (p2.y - p1.y) / (p2.x - p1.x) + p1.y;
                float y_size = (-size - p1.x) * (p2.y - p1.y) / (p2.x - p1.x) + p1.y;
                line.append(sf::Vertex(sf::Vector2f(size, ysize), Color::Black)); // Левая граница
                line.append(sf::Vertex(sf::Vector2f(-size, y_size), Color::Black));  // Правая граница
            }
            else {
                line.append(sf::Vertex(sf::Vector2f(p1.x, size), Color::Black)); // Левая граница
                line.append(sf::Vertex(sf::Vector2f(p1.x, -size), Color::Black));
            }
            window.draw(line);
        }

        
        Text text;
        text.setFont(font);
        text.setScale(1 * global::size, -1 * global::size);
        Point mid = go::findMiddle(p1, p2);
        text.setPosition(mid.x + 15 * global::size, mid.y + 15 * global::size);
        text.setString("L" + std::to_string(num));
        text.setCharacterSize(15);
        text.setFillColor(Color::Black);
        window.draw(text);

    }
    vector<Line> divide(const Point& point) const {
        if (!isSegment)
            return {};
        if (point == p1 || point == p2) {
            return {};
        }
        if (fun(point.x) < point.y + go::getPrecision() && fun(point.x) > point.y - go::getPrecision()) {
            return { Line(p1,point,true),Line(point,p2,true) };
        }
        else {
            return {};
        }
    }
    bool getIsSegment(){
        return isSegment;
    }
};

class Circle : public BasicShape{
private:
    Point cen;
    double rad;
    double endAngle,startAngle;
    friend class go;
public:
    Circle() :cen({ 0,0 }), rad(1), startAngle(0), endAngle(360){}
    Circle(Point cen, double rad) :cen(cen), rad(rad), startAngle(0), endAngle(360) {}
    Circle(Point _cen, Point _point){
        cen = _cen;
        rad = Vector(cen - _point).abs();
        startAngle = 0;
        endAngle = 360;
    }
    Circle(const Circle& other) :cen(other.cen), rad(other.rad), startAngle(other.startAngle), endAngle(other.endAngle) {}
    void printInf() const override {
        cout << "circle: center (" << cen.x << ", " << cen.y << "), radius = "<<rad <<  endl;
        // добавить про углы
    }
    void move(double dx, double dy) override {
        cen.x += dx;
        cen.y += dy;
    }
    void rotate(const Point& center, double angle) override {
        cen.x -= center.x;
        cen.y -= center.y;

        double newx = cos(angle) * cen.x - sin(angle) * cen.y;
        double newy = sin(angle) * cen.x + cos(angle) * cen.y;

        cen.x = newx + center.x;
        cen.y = newy + center.y;
    }
    Circle& operator=(const Circle& other) {
        if (this != &other) {
            cen = other.cen;
            rad = other.rad;
        }
        return *this;
    }
    void draw(sf::RenderWindow& window, int num, sf::Font& font) const override {
        int pointCount = 50;
        // Создаем массив вершин (тип треугольный фан)
        sf::VertexArray sector(sf::Lines, pointCount);

       
        
        // Вычисляем точки сектора
        float angleStep = (endAngle - startAngle) / pointCount; // Шаг между углами
        for (int i = 0; i < pointCount; ++i) {
            sf::VertexArray line(sf::Lines, 2);
            size_t nextIndex = (i + 1) % pointCount; // Индекс следующей точки (для замыкания)

            float angle = startAngle + i * angleStep;
            float x1 = cen.x + rad * std::cos(angle * acos(-1) / 180.0f);
            float y1 = cen.y + rad * std::sin(angle * acos(-1) / 180.0f);
            line[0].position = sf::Vector2f(x1, y1);
            angle = startAngle + (i+1) * angleStep;
            x1 = cen.x + rad * std::cos(angle * acos(-1) / 180.0f);
            y1 = cen.y + rad * std::sin(angle * acos(-1) / 180.0f);
            line[1].position = sf::Vector2f(x1,y1);
            
            
            
            line[0].color = Color::Black;
            line[1].color = Color::Black;

            window.draw(line);
        }

    


        //sf::CircleShape shape(rad); // Радиус круга
        //int pointAtCircle = std::min(100, std::max(20, static_cast<int>(20 / global::size))); // min 20 -> max 100
        //shape.setPointCount(pointAtCircle); // точек на круг
        //shape.setPosition(cen.x-rad,cen.y-rad); // Устанавливаем позицию круга
        //shape.setFillColor(sf::Color::Transparent); // Убираем заливку
        //shape.setOutlineThickness(1.f*global::size); // Устанавливаем толщину контура
        //shape.setOutlineColor(sf::Color::Black); // Устанавливаем цвет контура
        //window.draw(shape);

        Text text;
        text.setFont(font);
        text.setScale(1 * global::size, -1 * global::size);
        text.setPosition(cen.x - rad, cen.y -rad);
        text.setString("c" + std::to_string(num));
        text.setCharacterSize(15);
        text.setFillColor(Color::Black);
        window.draw(text);
    }
};

class Poligon : public BasicShape {
private:
    vector<Point> points;
    friend class go;
public:
    Poligon() :points({}) {}
    Poligon(vector<Point>& vec) : points(vec) {}
    Poligon(const Poligon& other) :points(other.points) {}

    void printInf() const override {
        cout << "poligon: \n";
        for(auto& p : points)
            cout<< "\t( " << p.x << ", " << p.y << ")"<< endl;
    }
    void move(double dx, double dy) override {
        for (auto& p : points) {
            p.x += dx;
            p.y += dy;
        }
    }
    void rotate(const Point& center, double angle) override {

        for (auto& p : points) {
            p.x -= center.x;
            p.y -= center.y;

            double newx = cos(angle) * p.x - sin(angle) * p.y;
            double newy = sin(angle) * p.x + cos(angle) * p.y;

            p.x = newx + center.x;
            p.y = newy + center.y;
        }
        
    }
    Poligon& operator=(const Poligon& other) {
        if (this != &other) {
            for (auto& p : other.points) {
                points.push_back(p);
            }
        }
        return *this;
    }
    void draw(sf::RenderWindow& window, int num, sf::Font& font) const override {

        double maxX = points[0].x;
        double minY = points[0].y;

        for (size_t i = 0; i < points.size(); i++) {
            size_t nextIndex = (i + 1) % points.size(); // Индекс следующей точки (для замыкания)

            sf::VertexArray line(sf::Lines, 2);
            line[0].position = sf::Vector2f(points[i].x, points[i].y);
            line[1].position = sf::Vector2f(points[nextIndex].x, points[nextIndex].y);
            line[0].color = Color::Black;
            line[1].color = Color::Black;
            window.draw(line);

            maxX = std::max(maxX, points[i].x);
            minY = std::min(minY, points[i].y);
        }
        

        Text text;
        text.setFont(font);
        text.setScale(1 * global::size, -1 * global::size);
        text.setPosition(maxX, minY);
        text.setString("pl" + std::to_string(num));
        text.setCharacterSize(15);
        text.setFillColor(Color::Black);
        window.draw(text);
    }
    double getArea() {
        int size = points.size();
        if (size == 0)
            return 0;
        double answ = 0;
        for (int i = 0; i < size - 1; i++) {
            answ += (points[i].x * points[i + 1].y - points[i].y * points[i + 1].x);
        }
        answ += (points[size - 1].x * points[0].y - points[size - 1].y * points[0].x);
        return abs(answ) / 2;
    }
};

