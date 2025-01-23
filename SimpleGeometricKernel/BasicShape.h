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
enum class DependsTypes {
    None,
    Parallel,
    Perpendicular,
    BelongsToLine,
    Intersection
};
class Depends : public std::enable_shared_from_this<Depends> {
private:
protected:
    std::vector<std::weak_ptr<Depends>> children;
    std::weak_ptr<Depends> parent;
    DependsTypes dependsType = DependsTypes::None;
    bool isUpdating = false;
    string type;

    int index = 0;
    void setType(const string& newType) {
        type = newType;
    }

public:
    Depends(const std::string& type) : type(type) {}
    string getType() const {
        return type;
    }
    virtual ~Depends() = default;
    void setIndex(int _index) { index = _index; }
    // Установка родителя с указанием типа зависимости
    void setParent(DependsTypes _type, const std::shared_ptr<Depends>& _parent) {
        // Если уже есть родитель, отписываемся от него
        
        parent = _parent;
        dependsType = _type;

        // очистака невалидных детей
        
        // Регистрируемся как ребёнок у нового родителя
        if (_parent) {
            _parent->addChild(shared_from_this());
        }
        update();
    }

    // Добавление ребёнка
    void addChild(const std::shared_ptr<Depends>& child) {
        removeExpiredChildren();
        children.push_back(child);
    }
    void printFamilyInfo() const {
        
        // Выводим родителя, если он есть
        if (auto p = parent.lock()) {
            std::cout << "  Parent: " << p->type <<" "<< p->index << "\n";
        }
        else {
            std::cout << "  Parent: None\n";
        }

        // Выводим всех детей
        std::cout << "  Children: ";
        bool hasChildren = false;
        for (const auto& weakChild : children) {
            if (auto child = weakChild.lock()) {
                std::cout << child->type << " " << child->index << ",";
                hasChildren = true;
            }
        }

        if (!hasChildren) {
            std::cout << "None";
        }
        std::cout << "\n";
    }
    
    /*void removeChild(const std::shared_ptr<Depends>& child) {
        children.erase(std::remove_if(children.begin(), children.end(),
            [&child](const std::weak_ptr<Depends>& weakChild) {
                auto childPtr = weakChild.lock();
                return childPtr == child || !childPtr;
            }), children.end());
    }*/
    // Удаление пустые ссылки на детей ребёнка
    void removeExpiredChildren() {
        children.erase(
            std::remove_if(children.begin(), children.end(),
                [](const std::weak_ptr<Depends>& wptr) {
                    return wptr.expired();
                }),
            children.end());
    }
    // Уведомление ребёнка о том, что родитель удалён
    void onParentDeleted() {
        parent.reset();  // Сбрасываем родителя
    }

    // Уведомление детей об изменении
    void notifyChildren() {
        for (auto& weakChild : children) {
            if (auto childPtr = weakChild.lock()) {
                if(childPtr->isUpdating){
                   continue;
                }
                childPtr->update();
            }
        }
    }
    DependsTypes getDependsType() { return dependsType; }
    // Обновление на основе родителя
    virtual void update() = 0;
};


class BasicShape : public Depends {
private:
    
    bool valid;
protected:
    
public:
    BasicShape(const std::string& type) :Depends(type), valid(true) {}
    virtual void draw(sf::RenderWindow& window, int num, sf::Font& font) const = 0; // Метод отрисовки virtual => у каждого потомка свой должен быть оперделен
    virtual void printInf() const = 0; 
    virtual void move(double dx, double dy,bool updateChild = true) = 0; //bool updateChild = true УДАЛИТЬ ХУЙНЯ
    virtual void rotate(const Point& center, double angle, bool updateChild = true) = 0;
    virtual ~BasicShape() = default; // деструктор у каждого потомка свой по умолчанью
    void setInvalid() {
        valid = false;
    }
    void setValid() {
        valid = true;
    }
    bool getValid() {
        return valid;
    }
    
    


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
public:
    Point(double x = 0, double y = 0) :BasicShape("point"),x(x), y(y) {}
    Point(const Point& other) : BasicShape("point"),x(other.x), y(other.y) {}
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
    void printInf() const override {
        cout << "point: (" << x << ", " << y << ")" << endl;
        printFamilyInfo();
    }
    void move(double dx, double dy, bool updateChild = true) override {
        x += dx;
        y += dy;
        if(updateChild)
            notifyChildren();
    }
    void rotate(const Point& center, double angle, bool updateChild = true) override {
        x -= center.x;
        y -= center.y;

        double newx = cos(angle) * x - sin(angle) * y;
        double newy = sin(angle) * x + cos(angle) * y;

        x = newx + center.x;
        y = newy + center.y;

        if (updateChild)
            notifyChildren();
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
    double setX(double newx, bool updateChild = true) {
        x = newx;
        if(updateChild)
            notifyChildren();
    }
    double setY(double newy, bool updateChild = true) {
        y = newy;
        if (updateChild)
            notifyChildren();
    }
    void update() override {
        // Реагируем на изменения точек
        std::cout << "point updated based on points.\n";
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


class Line : public virtual BasicShape {
protected:
    std::shared_ptr<Point> p1, p2;
    friend class go;
    double fun(double x) const{
        return (x - p1->x) * (p2->y - p1->y) / (p2->x - p1->x) + p1->y;
    }
    Line(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2)
        : BasicShape("line"), p1(std::move(p1)), p2(std::move(p2)) {}

public:
    // Фабричный метод для создания объекта
    static std::shared_ptr<Line> create(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2) {
        auto line = std::shared_ptr<Line>(new Line(p1, p2));
        line->p1->addChild(line);
        line->p2->addChild(line);
        return line;
    }
    virtual void printInf() const override {
        cout << "line " <<index<<" : (" << p1->x << ", " << p1->y << "), (" << p2->x << ", " << p2->y << ")" <<endl;
        printFamilyInfo();
    }
    void move(double dx, double dy,bool updateChild = true) override {
        isUpdating = true;
        p1->move(dx,dy);
        p2->move(dx,dy);
        isUpdating = false;
        notifyChildren();
    }
    void rotate(const Point& center, double angle, bool updateChild = true) override {
        if (dependsType != DependsTypes::Parallel) {
            isUpdating = true;
            p1->rotate(center, angle);
            p2->rotate(center, angle);
            isUpdating = false;
            notifyChildren();
        }
        else {
            cout << "cant rotate paralllel line\n";
        }
        
    }
    /*void updateParallel(const Point& point) const {
        Point p = point;
        Point q = { p2->x + point.x - p1->x,p2->y + point.y - p1->y };
        return Line(p, q);
    }
    Line getPerpendicular(const Point& point) const {
        Point p = (p2 - p1);
        p = { -p.y,p.x };
        return Line(p + point, point);
    }*/
    Line& operator=(const Line& other) {
        if (this != &other) {
            p1 = other.p1;
            p2 = other.p2;
        }
        return *this;
    }
    Point getStart() const {
        return *p1;
    }
    Point getEnd() const {
        return *p2;
    }
    virtual void draw(sf::RenderWindow& window, int num, sf::Font& font) const override {
       
            View view = window.getView();
            float size = view.getSize().x + abs(view.getCenter().x) * 2 + abs(view.getCenter().y) * 2;
            sf::VertexArray line(sf::Lines);
            if ((p2->x - p1->x) != 0) {
                float ysize = (size - p1->x) * (p2->y - p1->y) / (p2->x - p1->x) + p1->y;
                float y_size = (-size - p1->x) * (p2->y - p1->y) / (p2->x - p1->x) + p1->y;
                line.append(sf::Vertex(sf::Vector2f(size, ysize), Color::Black)); // Левая граница
                line.append(sf::Vertex(sf::Vector2f(-size, y_size), Color::Black));  // Правая граница
            }
            else {
                line.append(sf::Vertex(sf::Vector2f(p1->x, size), Color::Black)); // Левая граница
                line.append(sf::Vertex(sf::Vector2f(p1->x, -size), Color::Black));
            }
            window.draw(line);
        

        
        Text text;
        text.setFont(font);
        text.setScale(1 * global::size, -1 * global::size);
        Point mid = go::findMiddle(*p1, *p2);
        text.setPosition(mid.x + 15 * global::size, mid.y + 15 * global::size);
        text.setString("L" + std::to_string(num));
        text.setCharacterSize(15);
        text.setFillColor(Color::Black);
        window.draw(text);

    }
    void update() override {

        if (dependsType == DependsTypes::None) {

        }
        else if(dependsType == DependsTypes::Parallel){
            auto line = std::dynamic_pointer_cast<Line>(parent.lock());
            if (!line)
                throw std::invalid_argument("Parent of parallel line not line ");
            (*p2) = { line->p2->x + p1->x - line->p1->x,line->p2->y + p1->y - line->p1->y };
        }
        else {

        }
        
        notifyChildren();
        std::cout << "Line updated based on points.\n";
    }
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
//class Circle : public virtual BasicShape{
//private:
//    Point cen;
//    double rad;
//    friend class go;
//protected:
//    void setCenter(const Point& newCenter) {
//        cen = newCenter;
//    }
//    void setRadius(double newRadius) {
//        rad = newRadius;
//    }
//public:
//    Circle() : BasicShape("circle"),cen({ 0,0 }), rad(1) {}
//    Circle(Point cen, double rad) :BasicShape("circle"),cen(cen), rad(rad)  {}
//    Circle(Point _cen, Point _point):BasicShape("circle") {
//        cen = _cen;
//        rad = Vector(cen - _point).abs();
//    }
//    Circle(const Circle& other) :cen(other.cen), rad(other.rad) {}
//    virtual void printInf() const override {
//        cout << "circle: center (" << cen.x << ", " << cen.y << "), radius = "<<rad <<  endl;
//   
//    }
//    void move(double dx, double dy) override {
//        cen.move(dx, dy);
//    }
//    virtual void rotate(const Point& center, double angle) override {
//        cen.rotate(center, angle);
//    }
//    Circle& operator=(const Circle& other) {
//        if (this != &other) {
//            cen = other.cen;
//            rad = other.rad;
//        }
//        return *this;
//    }
//    virtual void draw(sf::RenderWindow& window, int num, sf::Font& font) const override {
//        
//        sf::CircleShape shape(rad); // Радиус круга
//        int pointAtCircle = std::min(100, std::max(20, static_cast<int>(20 / global::size))); // min 20 -> max 100
//        shape.setPointCount(pointAtCircle); // точек на круг
//        shape.setPosition(cen.x-rad,cen.y-rad); // Устанавливаем позицию круга
//        shape.setFillColor(sf::Color::Transparent); // Убираем заливку
//        shape.setOutlineThickness(1.f*global::size); // Устанавливаем толщину контура
//        shape.setOutlineColor(sf::Color::Black); // Устанавливаем цвет контура
//        window.draw(shape);
//
//        Text text;
//        text.setFont(font);
//        text.setScale(1 * global::size, -1 * global::size);
//        text.setPosition(cen.x - rad, cen.y -rad);
//        text.setString("c" + std::to_string(num));
//        text.setCharacterSize(15);
//        text.setFillColor(Color::Black);
//        window.draw(text);
//    }
//    Point getCenter() const {
//        return cen;
//    }
//    double getRadius() const {
//        return rad;
//    }
//};
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

