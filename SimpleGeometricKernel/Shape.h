#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class BasicShape {
public:
    virtual void draw(sf::RenderWindow& window) const = 0; // Метод отрисовки virtual => у каждого потомка свой должен быть оперделен
    virtual ~BasicShape() = default; // деструктор у каждого потомка свой по умолчанью
};

class Point2D :BasicShape {
    double x, y;
    Point2D(double x = 0, double y = 0) :x(x), y(y) {}
    void draw(sf::RenderWindow& window) const override {
        CircleShape point(3.f);
        point.setPosition(x, y);
        point.setFillColor(Color::Black);
        window.draw(point);
    }
};