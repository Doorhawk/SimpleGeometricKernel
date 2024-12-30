#pragma once
#include <SFML/Graphics.hpp>
#include "GeometricOperations.h"
#include "global.h"

using namespace sf;

class BasicShape {
public:
    virtual void draw(sf::RenderWindow& window) const = 0; // Метод отрисовки virtual => у каждого потомка свой должен быть оперделен
    virtual ~BasicShape() = default; // деструктор у каждого потомка свой по умолчанью
};

class Point2D : public BasicShape {
private:
    double x, y;
    friend class go;
    friend class Line;
public:
    Point2D(double x = 0, double y = 0) :x(x), y(y) {}
    Point2D(const Point2D& other) :x(other.x), y(other.y) {}
    void draw(sf::RenderWindow& window) const override {
        float r = 3.f * global::size;
        CircleShape point(r);
        point.setPosition(x-r, y-r);
        point.setFillColor(Color::Black);
        window.draw(point);
    }
};

class Line : public BasicShape {
private:
    Point2D p1,p2;
    friend class go;
    double fun(double x) {
        return (x - p1.x) * (p2.y - p1.y) / (p2.x - p1.x) + p1.y;
    }
public:
    Line() :p1({ 0,0 }), p2({1,1}) {}
    Line(Point2D p1, Point2D p2) :p1(p1), p2(p2) {}
    Line(const Line& other) :p1(other.p1), p2(other.p2) {}
    void draw(sf::RenderWindow& window) const override {
        float size = global::linesize;
        float ysize = (size - p1.x) * (p2.y - p1.y) / (p2.x - p1.x) + p1.y;
        float y_size = (-size - p1.x) * (p2.y - p1.y) / (p2.x - p1.x) + p1.y;
        sf::VertexArray line(sf::Lines);
        line.append(sf::Vertex(sf::Vector2f(size, ysize), Color::Black)); // Левая граница
        line.append(sf::Vertex(sf::Vector2f(-size, y_size), Color::Black));  // Правая граница
        window.draw(line);
    }
};