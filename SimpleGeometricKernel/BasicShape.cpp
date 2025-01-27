#include "BasicShape.h"

double Vector::abs() const {
        return sqrt(x * x + y * y);
    }
Vector Vector::normalize() const {
    double length = abs();
    if (length != 0) {
        return *this / length;
    }
    cerr << "in vec lenth = 0" << endl;
    return *this;
}
Vector::Vector() : x(0), y(0) {}
Vector::Vector(double x, double y) : x(x), y(y) {}
Vector::Vector(const Vector& other) : x(other.x), y(other.y) {}
Vector::Vector(const Point& other) : x(other.getX()), y(other.getY()) {}
Vector& Vector::operator=(const Vector& other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
    }
    return *this;
}
Vector& Vector::operator=(Point& other) {
    x = other.getX();
    y = other.getY();
    return *this;
}
Vector Vector::operator+(const Vector& other) const {
    return Vector(x + other.x, y + other.y);
}
Vector Vector::operator-(const Vector& other) const {
    return Vector(x - other.x, y - other.y);
}
double Vector::operator*(const Vector& other) const {
    return x * other.x + y * other.y;
}
Vector Vector::operator*(double scalar) const {
    return Vector(x * scalar, y * scalar);
}
Vector Vector::operator/(double scalar) const {
    if (scalar != 0) {
        return Vector(x / scalar, y / scalar);
    }
    else {
        throw std::invalid_argument("Division by zero.");
    }
}


void Depends::setType(const string& newType) {
    type = newType;
}
Depends::Depends(const std::string& type) : type(type) {}
string Depends::getType() const {
    return type;
}
void Depends::setIndex(int _index) { index = _index; }
void Depends::setParent(DependsTypes _type, const std::shared_ptr<Depends>& _parent) {
    // Если уже есть родитель, отписываемся от него

    parent = _parent;
    dependsType = _type;

    // очистака невалидных детей

    // Регистрируемся как ребёнок у нового родителя
    if (_parent) {
        _parent->addChild(shared_from_this());
    }
    init();
    update();
}
void Depends::addChild(const std::shared_ptr<Depends>& child) {
    removeExpiredChildren();
    children.push_back(child);
}
void Depends::printFamilyInfo() const {

    // Выводим родителя, если он есть
    if (auto p = parent.lock()) {
        std::cout << "  Parent: " << p->type << " " << p->index << "\n";
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
void Depends::removeExpiredChildren() {
    children.erase(
        std::remove_if(children.begin(), children.end(),
            [](const std::weak_ptr<Depends>& wptr) {
                return wptr.expired();
            }),
        children.end());
}
void Depends::onParentDeleted() {
    parent.reset();  // Сбрасываем родителя
}
void Depends::notifyChildren() {
    for (auto& weakChild : children) {
        if (auto childPtr = weakChild.lock()) {
            if (childPtr->isUpdating) {
                continue;
            }
            childPtr->update();
        }
    }
}
void Depends::setDependsType(DependsTypes dp) {
    dependsType = dp;
}
Depends::~Depends() {
    // Уведомляем детей о том, что родитель удалён
    for (auto& childWeakPtr : children) {
        if (auto child = childWeakPtr.lock()) {
            child->onParentDeleted(); // Уведомляем ребёнка
            child->setDependsType(DependsTypes::None);
        }
    }

    // Очищаем список детей, удаляя просроченные ссылки
    removeExpiredChildren();
}
DependsTypes Depends::getDependsType() { return dependsType; }


BasicShape::BasicShape(const std::string& type) :Depends(type), valid(true) {}
void BasicShape::setInvalid() {
    valid = false;
}
void BasicShape::setValid() {
    valid = true;
}
bool BasicShape::getValid() {
    return valid;
}




Point::Point(double x, double y) :BasicShape("point"), x(x), y(y), dependsX(0), dependsY(0) {}
Point::Point(const Point& other) : BasicShape("point"), x(other.x), y(other.y), dependsX(0), dependsY(0) {}
Point::Point(const Vector& other):BasicShape("point"), x(other.x), y(other.y), dependsX(0), dependsY(0) {}
void Point::draw(sf::RenderWindow& window, int num, Font& font) const {
    float r = 3.f * global::size;
    CircleShape point(r);
    point.setPosition(x - r, y - r);
    point.setFillColor(Color::Black);
    window.draw(point);

    Text text;
    text.setFont(font);
    text.setScale(1 * global::size, -1 * global::size);
    text.setPosition(x - 25 * global::size, y + 15 * global::size);
    text.setString("p" + std::to_string(num));
    text.setCharacterSize(15);
    text.setFillColor(Color::Black);
    window.draw(text);
}
void Point::printInf() const {
    cout << "point: (" << x << ", " << y << ")" << endl;
    printFamilyInfo();
}
void Point::move(double dx, double dy) {
    if (dependsType == DependsTypes::None) {
        x += dx;
        y += dy;
    }
    else if (dependsType == DependsTypes::BelongsToLine) {
        x += dx;
        y += dy;
        init();
    }
    notifyChildren();
}
void Point::rotate(const Point& center, double angle) {
    if (dependsType == DependsTypes::None) {
        x -= center.x;
        y -= center.y;

        double newx = cos(angle) * x - sin(angle) * y;
        double newy = sin(angle) * x + cos(angle) * y;

        x = newx + center.x;
        y = newy + center.y;
    }
    else if (dependsType == DependsTypes::BelongsToLine) {
        cout << "can't rotate belongs point\n";
    }

    notifyChildren();
}
Point& Point::operator=(const Point& other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
    }
    return *this;
}
Point Point::operator+(const Point& other) const {
    return Point(x + other.x, y + other.y);
}
Point Point::operator+(const Vector & other) const {
    return Point(x + other.x, y + other.y);
}
Point Point::operator-(const Vector& other) const {
    return Point(x - other.x, y - other.y);
}
Point Point::operator-(const Point& other) const {
    return Point(x - other.x, y - other.y);
}
Point Point::operator*(double scalar) const {
    return(Point(x * scalar, y * scalar));
}
bool Point::operator==(const Point& other) const {
    return (x == other.x) && (y == other.y);
}
double Point::getX() const {
    return x;
}
double Point::getY() const {
    return y;
}
void Point::setX(double newx) {
    x = newx;
    notifyChildren();
}
void Point::setY(double newy) {
    y = newy;
    notifyChildren();
}
void Point::update() {
    if (dependsType == DependsTypes::None) {

    }
    else if (dependsType == DependsTypes::BelongsToLine) {
        
        auto line = std::dynamic_pointer_cast<Line>(parent.lock());
        if (!line)
            throw std::invalid_argument("Parent of point not line ");
        Vector vec = (*line->p2 - *line->p1);
        double abs = vec.abs();
        vec = vec.normalize();

        x = abs*dependsX*vec.x + line->p1->x;
        y = abs*dependsX*vec.y + line->p1->y;
    }
    else {

    }
    notifyChildren();
}
void Point::init() {
    if (dependsType == DependsTypes::None) {
        
    }
    else if (dependsType == DependsTypes::BelongsToLine) {
       
        auto line = std::dynamic_pointer_cast<Line>(parent.lock());
        if (!line)
            throw std::invalid_argument("Parent of point not line ");
        Point p1 = (*line->p1 - *line->p2);
        p1 = { -p1.y,p1.x };
        Point p2 = p1 + *this;
        p1 = *this;
        Point q1 = line->getStart();
        Point q2 = line->getEnd();

        double A1 = p2.x - p1.x;
        double B1 = -(q2.x - q1.x);
        double C1 = q1.x - p1.x;

        double A2 = p2.y - p1.y;
        double B2 = -(q2.y - q1.y);
        double C2 = q1.y - p1.y;

        double det = A1 * B2 - A2 * B1;
        if (std::abs(det) < go::getPrecision()) {
            throw std::invalid_argument("point bolings line error");
            return;
        }
        double t = (C1 * B2 - C2 * B1) / det;
        // Вычисляем точку пересечения
        Point intersection;
        x = p1.x + t * (p2.x - p1.x);
        y = p1.y + t * (p2.y - p1.y);

        Point midle = go::findMiddle(q1, q2);
        Vector toI(*this - q1);
        Vector toP2(q2 - q1);

        dependsX = toI.abs()/toP2.abs();

        if ((toI.normalize() - toP2.normalize()).abs() < go::getPrecision()) {

        }
        else {
            dependsX = -dependsX;
        }
    }
}

Line::Line(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2)
    : BasicShape("line"), p1(std::move(p1)), p2(std::move(p2)) {}
double Line::fun(double x) const {
    return 0;
}
// Фабричный метод для создания объекта
std::shared_ptr<Line> Line::create(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2) {
    auto line = std::shared_ptr<Line>(new Line(p1, p2));
    line->p1->addChild(line);
    line->p2->addChild(line);
    return line;
}
void Line::printInf() const {
    cout << "line " << index << " : (" << p1->x << ", " << p1->y << "), (" << p2->x << ", " << p2->y << ")" << endl;
    printFamilyInfo();
}
void Line::move(double dx, double dy) {
    if (p1->getDependsType() == DependsTypes::BelongsToLine ||
        p2->getDependsType() == DependsTypes::BelongsToLine) {
        cout << "can't rotate line wirch have belong point\n";
    }
    else {
        isUpdating = true;
        p1->move(dx, dy);
        p2->move(dx, dy);
        isUpdating = false;
        notifyChildren();
    }
    
}
void Line::rotate(const Point& center, double angle) {
    if (dependsType == DependsTypes::Parallel) {
        cout << "can't rotate parallel line\n";
    }
    else if (dependsType == DependsTypes::Perpendicular) {
        cout << "can't rotate perpendicular line\n";
    }
    else if (p1->getDependsType() == DependsTypes::BelongsToLine ||p2->getDependsType() == DependsTypes::BelongsToLine) {
        cout << "can't rotate line wirch have belong point\n";
    }
    else {
        isUpdating = true;
        p1->rotate(center, angle);
        p2->rotate(center, angle);
        isUpdating = false;
        notifyChildren();
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
Line& Line::operator=(const Line& other) {
    if (this != &other) {
        p1 = other.p1;
        p2 = other.p2;
    }
    return *this;
}
Point Line::getStart() const {
    return *p1;
}
Point Line::getEnd() const {
    return *p2;
}
void Line::draw(sf::RenderWindow& window, int num, sf::Font& font) const {

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
void Line::update() {

    if (dependsType == DependsTypes::None) {

    }
    else if (dependsType == DependsTypes::Parallel) {
        auto line = std::dynamic_pointer_cast<Line>(parent.lock());
        if (!line)
            throw std::invalid_argument("Parent of parallel line not line ");
        Vector vec = { *line->p2 - *line->p1};
        vec = vec.normalize() * 10;
        (*p2) = *p1 + vec;
    }
    else if (dependsType == DependsTypes::Perpendicular) {

        auto line = std::dynamic_pointer_cast<Line>(parent.lock());
        if (!line)
            throw std::invalid_argument("Parent of perpendicular line not line ");
        Vector vec = *line->p2 - *line->p1;
        vec = { -(vec.y),vec.x};
        vec = vec.normalize() * 10;
        (*p2) = *p1 + vec;

    }
    else {

    }

    notifyChildren();
    //std::cout << "Line updated based on points.\n";
}
void Line::init() {

}