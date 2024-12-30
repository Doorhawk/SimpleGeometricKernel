#pragma once
#include <stdexcept>

struct Point2D
{
public:
    double x;
    double y;
    double abs() {
        return sqrt(x * x + y * y);
    }
    Point2D() : x(0), y(0) {}
    Point2D(double x, double y) : x(x), y(y) {}
    Point2D(const Point2D& other) : x(other.x), y(other.y) {}
    Point2D& operator=(const Point2D& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
        }
        return *this;
    }
    Point2D operator+(const Point2D& other) const {
        return Point2D(x + other.x, y + other.y);
    }
    Point2D operator-(const Point2D& other) const {
        return Point2D(x - other.x, y - other.y);
    }
    Point2D operator*(double scalar) const {
        return Point2D(x * scalar, y * scalar);
    }
    Point2D operator/(double scalar) const {
        if (scalar != 0) {
            return Point2D(x / scalar, y / scalar);
        }
        else {
            throw std::invalid_argument("Division by zero.");
        }
    }
};