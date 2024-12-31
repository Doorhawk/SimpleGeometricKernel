#pragma once
#include <SFML/Graphics.hpp>
#include "GeometricOperations.h"
#include "global.h"
#include "climits"
#include <string>

using namespace sf;




class BasicShape {
public:
    virtual void draw(sf::RenderWindow& window, int num, sf::Font font) const = 0; // Метод отрисовки virtual => у каждого потомка свой должен быть оперделен
    virtual ~BasicShape() = default; // деструктор у каждого потомка свой по умолчанью
};

class Point : public BasicShape {
private:
    double x, y;
    friend class go;
    friend class Line;
public:
    Point(double x = 0, double y = 0) :x(x), y(y) {}
    Point(const Point& other) :x(other.x), y(other.y) {}
    void draw(sf::RenderWindow& window, int num, Font font) const override {
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
    Line& operator=(const Line& other) {
        if (this != &other) {
            p1 = other.p1;
            p2 = other.p2;
        }
        return *this;
    }
    void draw(sf::RenderWindow& window, int num, sf::Font font) const override {
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
        text.setString("l" + std::to_string(num));
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