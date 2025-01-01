#pragma once
#include <SFML/Graphics.hpp>
#include "GeometricOperations.h"
#include "global.h"
#include "climits"
#include <string>
#include <iostream>

using namespace sf;
using namespace std;



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
    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }
    double getX() const {
        return x;
    }
    double getY() const {
        return y;
    }

};

class Line : public BasicShape {
private:
    Point p1,p2;
    friend class go;
    double fun(double x) {
        return (x - p1.x) * (p2.y - p1.y) / (p2.x - p1.x) + p1.y;
    }
public:
    Line() :p1({ 0,0 }), p2({1,1}) {}
    Line(Point p1, Point p2) :p1(p1), p2(p2) {}
    Line(const Line& other) :p1(other.p1), p2(other.p2) {}
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
    Line& operator=(const Line& other) {
        if (this != &other) {
            p1 = other.p1;
            p2 = other.p2;
        }
        return *this;
    }
    void draw(sf::RenderWindow& window, int num, sf::Font& font) const override {
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

        Text text;
        text.setFont(font);
        text.setScale(1 * global::size, -1 * global::size);
        text.setPosition(p1.x + 15 * global::size, p1.y + 15 * global::size);
        text.setString("L" + std::to_string(num));
        text.setCharacterSize(15);
        text.setFillColor(Color::Black);
        window.draw(text);

    }
};

class Circle : public BasicShape{
private:
    Point cen;
    double rad;
    friend class go;
public:
    Circle() :cen({ 0,0 }), rad(1) {}
    Circle(Point cen, double rad) :cen(cen), rad(rad) {}
    Circle(const Circle& other) :cen(other.cen), rad(other.rad) {}
    void printInf() const override {
        cout << "circle: center (" << cen.x << ", " << cen.y << "), radius = "<<rad <<  endl;
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
        sf::CircleShape shape(rad); // Радиус круга — 100 пикселей
        shape.setPosition(cen.x-rad,cen.y-rad); // Устанавливаем позицию круга
        shape.setFillColor(sf::Color::Transparent); // Убираем заливку
        shape.setOutlineThickness(1.f*global::size); // Устанавливаем толщину контура
        shape.setOutlineColor(sf::Color::Black); // Устанавливаем цвет контура
        window.draw(shape);

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

class Vector {
private:
    double x, y;
    friend class go;
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