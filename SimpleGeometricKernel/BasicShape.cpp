#include "BasicShape.h"


std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << "Point(" << p.getX() << ", " << p.getY() << ")";
}
std::ostream& operator<<(std::ostream& os, const Line& l) {
    return os << "Line(" << l.getStart().getX() << ", " << l.getStart().getY() << ")(" << l.getEnd().getX() << ", " << l.getEnd().getY();
}
std::ostream& operator<<(std::ostream& os, const Circle& c) {
    return os << "Circle(" << c.getCenter().getX() << ", " << c.getCenter().getY() << "), r = " << c.getRadius();
}

std::string toString(ShapeType shape) {
    switch (shape) {
    case st_point:  return "point";
    case st_line:   return "line";
    case st_circle: return "circle";
    }
    return "";
}
std::string toString(DependsTypes dp) {
    switch (dp)
    {
    case DependsTypes::None:                    return "None";
    case DependsTypes::Parallel:                return "Parallel";
    case DependsTypes::Perpendicular:           return "Perpendicular";
    case DependsTypes::MedianPerpendicular:     return "MedianPerpendicular";
    case DependsTypes::Bisectrix:               return "Bisectrix";
    case DependsTypes::BelongsToLine:           return "BelongsToLine";
    case DependsTypes::IntersectionLineLine:    return "IntersectionLineLine";
    case DependsTypes::Circle3points:           return "Circle3points";
    case DependsTypes::Circle2points:           return "Circle2pointsl";// center and onCircle
    case DependsTypes::Line2points:             return "Line2points";
    case DependsTypes::BelongsToCircle:         return "BelongsToCircle";
    case DependsTypes::IntersectionLineCircle:  return "IntersectionLineCircle";
    case DependsTypes::Midpoint:                return "Midpoint";
    case DependsTypes::CircleCenter:            return "CircleCenter";
    default:                                    return "Unknown";
    }
}

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
double Vector::getX() {
    return x;
}
double Vector::getY() {
    return y;
}

std::vector<std::weak_ptr<Depends>> Depends::getChildren() const {
    return children;
}
void Depends::setType(const ShapeType newType) {
    type = newType;
}
Depends::Depends(const ShapeType type) : type(type) {}
ShapeType Depends::getType() const {
    return type;
}
void Depends::setColor(Color newColor) {
    color = newColor;
    oldColor = newColor;
}
void Depends::setCildrenColor(Color newcolor, bool setNew) {
    if(setNew){
        oldColor = color;
        color = newcolor;
    }
    else {
        color = oldColor;
    }
    
    for (auto& weakChild : children) {
        if (auto childPtr = weakChild.lock()) {
            childPtr->setCildrenColor(newcolor,setNew);
        }
    }
}
void Depends::setIndex(int _index) { index = _index; }
int Depends::getIndex() { return index; }
void Depends::setParent(DependsTypes _type,const std::vector <std::weak_ptr<Depends>>& _parent, double val) {
    // Если уже есть родитель, отписываемся от него

    parent = _parent;
    dependsType = _type;
    valDepends = val;
    // очистака невалидных детей

    // Регистрируемся как ребёнок у нового родителя
    if (!parent.empty()) {
        for (auto& weakP : parent) {
            if (auto parentPtr = weakP.lock()) {
                parentPtr->addChildren(shared_from_this());
            }
        }
        
    }
    init();
    update();
}
string Depends::getFamilyInfo() const {

    std::ostringstream oss;

    oss << "Depends: " << toString(dependsType) << endl;

    if (!parent.empty()) {
        oss << "  Parent: ";
        for (auto& weakP : parent) {
            if (auto parentPtr = weakP.lock()) {
                oss << toString(parentPtr->type)<<" " << parentPtr->index<<", ";
            }
        }
        oss << endl;
    }
    else {
        oss << "  Parent: \n";
    }

    // Выводим всех детей
    oss << "  Children: ";
    bool hasChildren = false;
    for (const auto& weakChild : children) {
        if (auto child = weakChild.lock()) {

            oss << toString(child->type) << " " << child->index << ", ";
            hasChildren = true;
        }
    }
    if (!hasChildren) {
        oss << " ";
    }
    oss << "\n";

    return oss.str();
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
    parent.clear();  // Сбрасываем родителя
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
//Depends::~Depends() {
//
//    setInvalid();
//    onDelete();
//}
//void Depends::onDelete() {
//    // Удаляем текущий объект из родителей
//    for (auto& weakParent : parent) {
//        if (auto sharedParent = weakParent.lock()) {
//            sharedParent->removeChild(shared_from_this());
//        }
//    }
//
//    // Копируем `children`, чтобы не менять оригинальный вектор во время итерации
//    auto childrenCopy = children;
//
//    for (auto& childWeakPtr : childrenCopy) {
//        if (auto child = childWeakPtr.lock()) {
//            child->onDelete();  // Рекурсивное удаление
//        }
//    }
//
//    // Обнуляем данные
//    dependsType = DependsTypes::None;
//    children.clear();
//    parent.clear();
//    // НАДО УДАЛЯТЬ БЕЛОНГОВ!!!!!!!!!!!!!!!!!!!!!!!!!!!
//}
DependsTypes Depends::getDependsType() { return dependsType; }
Color Depends::getColor() {
    return color;
}
void Depends::setInvalid() {
    valid = false;
    for (auto& weakChild : children) {
        if (auto childPtr = weakChild.lock()) {
            childPtr->setInvalid();
        }
    }
}
void Depends::setValid() {
    valid = true;
    for (auto& weakChild : children) {
        if (auto childPtr = weakChild.lock()) {
            childPtr->setValid();
        }
    }
}
bool Depends::getValid() {
    return valid;
}
void Depends::removeChild(const std::shared_ptr<Depends>& child) {
    children.erase(
        std::remove_if(children.begin(), children.end(),
            [&child](const std::weak_ptr<Depends>& weakChild) {
                return weakChild.lock() == child;  // Удаляем, если указатель совпадает
            }),
        children.end()
    );
}


BasicShape::BasicShape(const ShapeType type) :Depends(type) {}
bool BasicShape::movebasePoint(double dx, double dy) {
    for (auto& weakBelong : basePoint) {
        if (auto belongPtr = weakBelong.lock()) {
            if (!belongPtr->canMove())
                return false;
        }
    }
    for (auto& weakBelong : basePoint) {
        if (auto belongPtr = weakBelong.lock()) {
            belongPtr->move(dx, dy);
        }
    }
    return true;
}
void BasicShape::removeExpiredbasePoint() {
    basePoint.erase(
        std::remove_if(basePoint.begin(), basePoint.end(),
            [](const std::weak_ptr<BasicShape>& wptr) {
                return wptr.expired();
            }),
        basePoint.end());

}
std::string BasicShape::getBasePointInf() const {
    ostringstream oss;
    oss << " base point: ";
    if (!basePoint.empty()) {
        for (auto& weakBelong : basePoint) {
            if (auto belongPtr = weakBelong.lock()) {
                oss << toString(belongPtr->getType()) << " " << belongPtr->getIndex() << ", ";
            }
        }
    }
    else {
        oss << " ";
    }
    oss << "\n";
    return oss.str();
}

Point::Point(double x, double y) :BasicShape(st_point), x(x), y(y) {}
Point::Point(const Point& other) : BasicShape(st_point), x(other.x), y(other.y) {}
Point::Point(const Vector& other):BasicShape(st_point), x(other.x), y(other.y) {}
void Point::draw(sf::RenderWindow& window, int num, Font& font) const {
    float r = 3.f * global::size;
    CircleShape point(r);
    point.setPosition(x - r, y - r);
    point.setFillColor(color);
    window.draw(point);

    Text text;
    text.setFont(font);
    text.setScale(1 * global::size, -1 * global::size);
    text.setPosition(x - 25 * global::size, y + 15 * global::size);
    text.setString("p" + std::to_string(num));
    text.setCharacterSize(15);
    text.setFillColor(color);
    window.draw(text);
}
std::string Point::printInf() const {
    ostringstream oss;
    oss << "point: "<< index <<" (" << x << ", " << y << ")" << endl<< getFamilyInfo()<< getBasePointInf();
    return oss.str();
}
bool Point::canMove() {
    if (dependsType == DependsTypes::None) {
        return true;
    }
    else if (dependsType == DependsTypes::BelongsToLine) {
        return false;
    }
    else if (
            dependsType == DependsTypes::IntersectionLineLine||
            dependsType == DependsTypes::IntersectionLineCircle||
            dependsType == DependsTypes::Midpoint||
            dependsType == DependsTypes::CircleCenter
        ) {
        return false;
    }
    else {
        return false;
    }
}
void Point::move(double dx, double dy) {
    
    if (dependsType == DependsTypes::IntersectionLineLine||
        dependsType == DependsTypes::IntersectionLineCircle||
        dependsType == DependsTypes::Midpoint||
        dependsType == DependsTypes::CircleCenter) {
        return;
    }
    isUpdating = true;
    x += dx;
    y += dy;
    if (dependsType == DependsTypes::BelongsToLine||
        dependsType == DependsTypes::BelongsToCircle) {
        init();
    }
    notifyChildren();
    isUpdating = false;
}
void Point::rotate(const Point& center, double angle) {
    if (dependsType == DependsTypes::None) {
        
    }
    else if (dependsType == DependsTypes::BelongsToLine) {
        cout << "can't rotate belongs point\n";
        return;
    }
    else if (dependsType == DependsTypes::IntersectionLineLine) {
        cout << "cant rotate intersection point\n";
        return;
    }
    x -= center.x;
    y -= center.y;
    double newx = cos(angle) * x - sin(angle) * y;
    double newy = sin(angle) * x + cos(angle) * y;
    x = newx + center.x;
    y = newy + center.y;

    isUpdating = true;
    notifyChildren();
    isUpdating = false;
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
bool Point::operator!=(const Point& other) const {
    return (x != other.x) || (y != other.y);
}
double Point::getX() const {
    return x;
}
double Point::getY() const {
    return y;
}
void Point::setPos(double newx, double newy) {
    if (dependsType == DependsTypes::IntersectionLineLine||
        dependsType == DependsTypes::IntersectionLineCircle||
        dependsType == DependsTypes::CircleCenter||
        dependsType == DependsTypes::Midpoint) {
        return;
    }
    isUpdating = true;
    x = newx;
    y = newy;
    if (dependsType == DependsTypes::BelongsToLine ||
        dependsType == DependsTypes::BelongsToCircle) {
        init();
    }
    notifyChildren();
    isUpdating = false;
}
void Point::update() {
    if (dependsType == DependsTypes::None) {

    }
    else if (dependsType == DependsTypes::BelongsToLine) {
        if (parent.empty())
            throw std::invalid_argument("update point with BelongsToLine - error: parent.empty() = true");
        if (parent.size()!=1)
            throw std::invalid_argument("update point with BelongsToLine - error: parent.size() != 1");

        auto line = std::dynamic_pointer_cast<Line>(parent[0].lock());
        if (!line)
            throw std::invalid_argument("Parent of point not line ");
        Vector vec = (line->p2 - line->p1);
        double abs = vec.abs();
        vec = vec.normalize();

        x = abs*valDepends*vec.x + line->p1.x;
        y = abs*valDepends*vec.y + line->p1.y;
    }
    else if (dependsType == DependsTypes::IntersectionLineLine) {
        if (parent.empty())
            throw std::invalid_argument("update point with IntersectionLineLine - error: parent.empty() = true");
        if (parent.size() != 2)
            throw std::invalid_argument("update point with IntersectionLineLine - error: parent.size() != 2");
        auto line1 = std::dynamic_pointer_cast<Line>(parent[0].lock());
        auto line2 = std::dynamic_pointer_cast<Line>(parent[1].lock());
        if (!line1||!line2)
            throw std::invalid_argument("Parent of point not line ");
        std::vector<Point> inter = go::findIntersection(*line1, *line2);

        if (!inter.empty()) {
            x = inter[0].x;
            y = inter[0].y;
            if (!valid) {
                setValid();
            }
        }
        else {
            setInvalid();
        }
    }
    else if (dependsType == DependsTypes::BelongsToCircle) {
        if (parent.empty())
            throw std::invalid_argument("update point with BelongsToLine - error: parent.empty() = true");
        if (parent.size() != 1)
            throw std::invalid_argument("update point with BelongsToLine - error: parent.size() != 1");
        auto circle = std::dynamic_pointer_cast<Circle>(parent[0].lock());
        if (!circle)
            throw std::invalid_argument("Parent of point not circle ");
        Point cen = circle->getCenter();
        double rad = circle->getRadius();

        Point newpos = Point(cen.x + rad, cen.y);
        newpos.rotate(cen, valDepends);
        *this = newpos;
    }
    else if (dependsType == DependsTypes::IntersectionLineCircle) {
        if(parent.empty())
            throw std::invalid_argument("update point with IntersectionLineCircle - error: parent.empty() = true");
        if (parent.size() != 2)
            throw std::invalid_argument("update point with IntersectionLineCircle - error: parent.size() != 2");
        auto circle = std::dynamic_pointer_cast<Circle>(parent[0].lock());
        auto line = std::dynamic_pointer_cast<Line>(parent[1].lock());
        if (!circle)
            throw std::invalid_argument("Parent of point not circle");
        if (!line)
            throw std::invalid_argument("Parent of point not line");

        vector<Point> inter = go::findIntersection(*circle, *line);
        if (inter.empty()) {
            if(valid)
                setInvalid();
        }
        else if (inter.size() == 1) {
            if (valDepends > 0){
                if (!valid)
                    setValid();
                *this = inter[0];
            }
            else
                if(valid)
                    setInvalid();
        }
        else {
            if (!valid)
                setValid();
            *this = valDepends > 0 ? inter[0] : inter[1];
        }
    }
    else if (dependsType == DependsTypes::Midpoint) {
        if (parent.empty())
            throw std::invalid_argument("update point with Midpoint - error: parent.empty() = true");
        if (parent.size() != 2)
            throw std::invalid_argument("update point with Midpoint - error: parent.size() != 2");
        auto point1 = std::dynamic_pointer_cast<Point>(parent[0].lock());
        auto point2 = std::dynamic_pointer_cast<Point>(parent[1].lock());

        *this = go::findMiddle(*point1, *point2);
    }
    else if (dependsType == DependsTypes::CircleCenter) {
        if (parent.empty())
            throw std::invalid_argument("update point with CircleCenter - error: parent.empty() = true");
        if (parent.size() != 1)
            throw std::invalid_argument("update point with CircleCenter - error: parent.size() != 1");
        auto circle = std::dynamic_pointer_cast<Circle>(parent[0].lock());
        if (!circle)
            throw std::invalid_argument("Parent of point not circle");

        *this = circle->getCenter();
    }
    else {

    }
    isUpdating = true;
    notifyChildren();
    isUpdating = false;
}
void Point::init() {
    if (dependsType == DependsTypes::None) {
        
    }
    else if (dependsType == DependsTypes::BelongsToLine) {
        if (parent.empty())
            throw std::invalid_argument("update point with BelongsToLine - error: parent.empty() = true");
        if (parent.size() != 1)
            throw std::invalid_argument("update point with BelongsToLine - error: parent.size() != 1");
        auto line = std::dynamic_pointer_cast<Line>(parent[0].lock());
        if (!line)
            throw std::invalid_argument("Parent of point not line ");

        Line pp;
        pp.toPerpendicular(*line, *this);
        vector<Point> inter = go::findIntersection(pp, *line);

        if(inter.empty())
            throw std::invalid_argument("inter is empty in belongsline");

        x = inter[0].x;
        y = inter[0].y;

        Point midle = go::findMiddle(line->p1, line->p2);
        Vector toI(*this - line->p1);
        Vector toP2(line->p2 - line->p1);

        valDepends = toI.abs()/toP2.abs();

        // направление в сторону прямой или нет
        if ((toI.normalize() - toP2.normalize()).abs() < go::getPrecision()) {
            
        }
        else {
            valDepends = -valDepends;
        }
    }
    else if (dependsType == DependsTypes::BelongsToCircle) {
        if (parent.empty())
            throw std::invalid_argument("update point with BelongsToLine - error: parent.empty() = true");
        if (parent.size() != 1)
            throw std::invalid_argument("update point with BelongsToLine - error: parent.size() != 1");
        auto circle = std::dynamic_pointer_cast<Circle>(parent[0].lock());
        if (!circle)
            throw std::invalid_argument("Parent of point not line ");
        Point cen = circle->getCenter();
        double rad = circle->getRadius();

        Vector vec = *this - cen;
        vec = vec.normalize();
        if (vec.abs() < go::getPrecision())
            vec = Vector(1, 0);
        *this = cen + vec * rad;
        Point point3 = cen + Vector(1, 0) * rad;
        valDepends = go::findAngle(*this, cen, point3);
        if (this->y <= cen.y)
            valDepends = -valDepends;
    }
    
}


Line::Line(Point p1, Point p2): BasicShape(st_line), p1(p1), p2(p2), isSegment(false) {}
Line::Line(): BasicShape(st_line), p1(Point(0,0)), p2(Point(10,10)), isSegment(false) {}
Line::Line(bool isSegment) : BasicShape(st_line), p1(Point(0, 0)), p2(Point(10, 10)), isSegment(isSegment) {}
double Line::fun(double x) const {
    return 0;
}
std::string Line::printInf() const {
    ostringstream oss;
    oss << "line " << index << " : (" << p1.x << ", " << p1.y << "), (" << p2.x << ", " << p2.y << ")" << endl
        <<getFamilyInfo() << getBasePointInf();
    return oss.str();
}
void Line::setSegment(bool is) {
    isSegment = is;
}
bool Line::canMove() {
    return true;
}
void Line::move(double dx, double dy) {
    
    if (dependsType == DependsTypes::MedianPerpendicular) {
        cout << "can't move middle perpendicular\n";
        return;
    }
    else if (dependsType == DependsTypes::Bisectrix) {
        cout << "can't move middle Bisectrix\n";
        return;
    }
    isUpdating = true;
    if (!movebasePoint(dx, dy)) {
        isUpdating = false;
        return;
    }
    p1.move(dx, dy);
    p2.move(dx, dy);
    notifyChildren();
    isUpdating = false;
    
}
void Line::rotate(const Point& center, double angle) {
    if (dependsType == DependsTypes::Parallel) {
        cout << "can't rotate parallel line\n";
    }
    else if (dependsType == DependsTypes::Perpendicular) {
        cout << "can't rotate perpendicular line\n";
    }
    else if (dependsType == DependsTypes::MedianPerpendicular) {
        cout << "can't rotate middle perpendicular\n";
    }
    else {
        isUpdating = true;
        p1.rotate(center, angle);
        p2.rotate(center, angle);
        isUpdating = false;
        notifyChildren();
    }

}
Line& Line::operator=(const Line& other) {
    if (this != &other) {
        p1 = other.p1;
        p2 = other.p2;
    }
    return *this;
}
Point Line::getStart() const {
    return p1;
}
Point Line::getEnd() const {
    return p2;
}
void Line::draw(sf::RenderWindow& window, int num, sf::Font& font) const {
    
    View view = window.getView();
    float size = view.getSize().x + abs(view.getCenter().x) * 2 + abs(view.getCenter().y) * 2;
    sf::VertexArray line(sf::Lines);
    if (isSegment) {
        line.append(sf::Vertex(sf::Vector2f(p1.x, p1.y), color)); // Левая граница
        line.append(sf::Vertex(sf::Vector2f(p2.x, p2.y), color));
    }
    else {
        
        if ((p2.x - p1.x) != 0) {
            float ysize = (size - p1.x) * (p2.y - p1.y) / (p2.x - p1.x) + p1.y;
            float y_size = (-size - p1.x) * (p2.y - p1.y) / (p2.x - p1.x) + p1.y;
            line.append(sf::Vertex(sf::Vector2f(size, ysize), color)); // Левая граница
            line.append(sf::Vertex(sf::Vector2f(-size, y_size), color));  // Правая граница
        }
        else {
            line.append(sf::Vertex(sf::Vector2f(p1.x, size), color)); // Левая граница
            line.append(sf::Vertex(sf::Vector2f(p1.x, -size), color));
        }
       
    }
    window.draw(line);


    Text text;
    text.setFont(font);
    text.setScale(1 * global::size, -1 * global::size);
    Point mid = go::findMiddle(p1, p2);
    text.setPosition(mid.x + 15 * global::size, mid.y + 15 * global::size);
    if(isSegment)
        text.setString("S" + std::to_string(num));
    else
        text.setString("L" + std::to_string(num));
    text.setCharacterSize(15);
    text.setFillColor(color);
    window.draw(text);

}
void Line::toMedianPerpendicular(const Point& point1, const Point& point2) {
    Point midle = go::findMiddle(point1, point2);
    p1 = midle;
    Vector vec = point1 - point2;
    vec = { -(vec.y),vec.x };
    vec = vec.normalize() * 10;
    p2 = p1 + vec;
}
void Line::toParallel(const Line& line, const Point& point) {
    Vector vec = { line.p2 - line.p1 };
    vec = vec.normalize() * 10;
    p2 = point + vec;
    p1 = point;
}
void Line::toPerpendicular(const Line& line, const Point& point) {
    Vector vec = line.p2 - line.p1;
    vec = { -(vec.y),vec.x };
    vec = vec.normalize() * 10;
    p2 = point + vec;
    p1 = point;
}
void Line::toBisectrix(const Point& point1, const Point& point2, const Point& point3) {

    Vector vec1 = (point1 - point2);
    Vector vec2 = (point3 - point2);

    vec1 = vec1.normalize();
    vec2 = vec2.normalize();

    p1 = point2 - (vec1 + vec2) * 10;
    p2 = point2 + (vec1 + vec2) * 10;
}
void Line::to2Points(const Point& point1, const Point& point2) {
    p1 = point1;
    p2 = point2;
}
void Line::update() {

    if (dependsType == DependsTypes::None) {

    }
    else if (dependsType == DependsTypes::Line2points) {
        if (parent.empty())
            throw std::invalid_argument("update line with 2point - error: parent.empty() == true");
        if (parent.size() != 2)
            throw std::invalid_argument("update line with 2point - error: parent.size() != 2");
        auto point1 = std::dynamic_pointer_cast<Point>(parent[0].lock());
        auto point2 = std::dynamic_pointer_cast<Point>(parent[1].lock());
        
        to2Points(*point1, *point2);

    }
    else if (dependsType == DependsTypes::Parallel) {
        if (parent.empty())
            throw std::invalid_argument("update line with Parallel - error: parent.empty() == true");
        if (parent.size() != 2)
            throw std::invalid_argument("update line with Parallel - error: parent.size() != 2");
        auto line = std::dynamic_pointer_cast<Line>(parent[0].lock());
        auto point = std::dynamic_pointer_cast<Point>(parent[1].lock());
        if (!line)
            throw std::invalid_argument("Parent 1 of parallel line not line ");
        if (!point)
            throw std::invalid_argument("Parent 2 of parallel line not point ");
        toParallel(*line, *point);
    }
    else if (dependsType == DependsTypes::Perpendicular) {
        if (parent.empty())
            throw std::invalid_argument("update line with Perpendicular - error: parent.empty() == true");
        if (parent.size() != 2)
            throw std::invalid_argument("update line with Perpendicular - error: parent.size() != 2");
        auto line = std::dynamic_pointer_cast<Line>(parent[0].lock());
        auto point = std::dynamic_pointer_cast<Point>(parent[1].lock());
        if (!line)
            throw std::invalid_argument("Parent 1 of perpendicular line not line ");
        if (!point)
            throw std::invalid_argument("Parent 2 of perpendicular line not point ");
        toPerpendicular(*line, *point);

    }
    else if (dependsType == DependsTypes::MedianPerpendicular) {
        if (parent.empty())
            throw std::invalid_argument("update line with MedianPerpendicular - error: parent.empty() == true");
        if (parent.size() != 2)
            throw std::invalid_argument("update line with MedianPerpendicular - error: parent.size() != 2");
        auto point1 = std::dynamic_pointer_cast<Point>(parent[0].lock());
        auto point2 = std::dynamic_pointer_cast<Point>(parent[1].lock());

        toMedianPerpendicular(*point1, *point2);

    }
    else if (dependsType == DependsTypes::Bisectrix) {
        if (parent.empty())
            throw std::invalid_argument("update line with Bisectrix - error: parent.empty() == true");
        if (parent.size() != 3)
            throw std::invalid_argument("update line with Bisectrix - error: parent.size() != 3");
        auto point1 = std::dynamic_pointer_cast<Point>(parent[0].lock());
        auto point2 = std::dynamic_pointer_cast<Point>(parent[1].lock());
        auto point3 = std::dynamic_pointer_cast<Point>(parent[2].lock());

        toBisectrix(*point1, *point2, *point3);
    }
    else {

    }
    isUpdating = true;
    notifyChildren();
    isUpdating = false;
    //std::cout << "Line updated based on points.\n";
}
void Line::init() {

}







Circle::Circle(Point center, Point onCircle) : BasicShape(st_circle), center(center), onCircle(onCircle) {
    radius = go::distance(center, onCircle);
}

std::string Circle::printInf() const  {
    ostringstream oss;
    oss << "circle " << index << " : center (" << center.x << ", " << center.y << "), radius = " << radius << endl
        <<getFamilyInfo() << getBasePointInf();
    return oss.str();
}
bool Circle::canMove() {
    return true;
}
void Circle::move(double dx, double dy)  {
    
    
    isUpdating = true;
    if (!movebasePoint(dx, dy)) {
        isUpdating = false;
        return;
    }
    center.move(dx, dy);
    onCircle.move(dx, dy);
    notifyChildren();
    isUpdating = false;
}
void Circle::rotate(const Point& _center, double angle) {
    
    isUpdating = true;
    center.rotate(_center, angle);
    onCircle.rotate(_center, angle);
    notifyChildren();
    isUpdating = false;
}
Circle& Circle::operator=(const Circle& other) {
    if (this != &other) {
        center = other.center;
        onCircle = other.onCircle;
        radius = other.radius;
    }
    return *this;
}
void Circle::draw(sf::RenderWindow& window, int num, sf::Font& font) const {

    sf::CircleShape shape(radius); // Радиус круга
    //int pointAtCircle = std::min(100, std::max(20, static_cast<int>(20 / global::size))); // min 20 -> max 100
    int pointAtCircle = 100;
    shape.setPointCount(pointAtCircle); // точек на круг
    shape.setPosition(center.x - radius, center.y - radius); // Устанавливаем позицию круга
    shape.setFillColor(sf::Color::Transparent); // Убираем заливку
    shape.setOutlineThickness(1.f * global::size); // Устанавливаем толщину контура
    shape.setOutlineColor(color); // Устанавливаем цвет контура
    window.draw(shape);

    Text text;
    text.setFont(font);
    text.setScale(1 * global::size, -1 * global::size);
    text.setPosition(center.x - radius, center.y - radius);
    text.setString("c" + std::to_string(num));
    text.setCharacterSize(15);
    text.setFillColor(color);
    window.draw(text);
}
Point Circle::getCenter() const {
    return center;
}
Point Circle::getOnCircle() const {
    return onCircle;
}
double Circle::getRadius() const {
    return radius;
}
void Circle::update() {
    if (dependsType == DependsTypes::None) {
        radius = go::distance(center, onCircle);
    }
    else if (dependsType == DependsTypes::Circle3points) {
        if (parent.empty())
            throw std::invalid_argument("update circle with Circle3points - error: parent.empty() == true");
        if (parent.size() != 3)
            throw std::invalid_argument("update circle with Circle3points - error: parent.size() != 3");

        auto point1 = std::dynamic_pointer_cast<Point>(parent[0].lock());
        auto point2 = std::dynamic_pointer_cast<Point>(parent[1].lock());
        auto point3 = std::dynamic_pointer_cast<Point>(parent[2].lock());

        Line line1;
        Line line2;

        line1.toMedianPerpendicular(*point1, *point2);
        line2.toMedianPerpendicular(*point2, *point3);

        std::vector<Point> inter = go::findIntersection(line1, line2);

        if (!inter.empty()) {
            
            center = inter[0];
            onCircle = *point2;
            radius = go::distance(center, onCircle);

            if (!valid) {
                setValid();
            }
        }
        else {
            setInvalid();
        }

    }
    else if (dependsType == DependsTypes::Circle2points) {
        if (parent.empty())
            throw std::invalid_argument("update circle with Circle2points - error: parent.empty() == true");
        if (parent.size() != 2)
            throw std::invalid_argument("update circle with Circle2points - error: parent.size() != 2");

        auto point1 = std::dynamic_pointer_cast<Point>(parent[0].lock());
        auto point2 = std::dynamic_pointer_cast<Point>(parent[1].lock());

        center = *point1;
        onCircle = *point2;

        radius = go::distance(center, onCircle);
    }
    isUpdating = true;
    notifyChildren();
    isUpdating = false;
}
void Circle::init() {

}
