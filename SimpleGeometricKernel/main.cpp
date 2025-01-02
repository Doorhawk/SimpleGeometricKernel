#include <SFML/Graphics.hpp>
#include "BasicShape.h"
#include <iostream>
#include "WindowManager.h"
#include "GeometricOperations.h"
#include "global.h"
#include <thread>

float global::size = 1.f;

using namespace sf;

template <typename T, typename... Args>
std::shared_ptr<T> ms(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

const double PI = acos(-1);


void commandProcessor(ShapeManager& shapeManager) {
    std::string command;
    while (true) {
        std::cout << "Enter command: ";
        std::cin >> command;

        if (command == "add") {
            std::cin >> command;
            if (command == "point") {
                float x, y;
                std::cin >> x >> y;
                shapeManager.addBasicShape(Point(x, y));
            }
            else if (command == "line") {
                std::cin >> command;
                if (command == "p") {
                    int i, j;
                    std::cin >> i >> j;
                    auto shape1 = shapeManager.getBasicShape(i);
                    auto shape2 = shapeManager.getBasicShape(j);

                    if (!shape1 || !shape2) {
                        std::cout << "Invalid indices.\n";
                        continue;
                    }

                    // Попытка привести фигуры к точкам
                    auto point1 = std::dynamic_pointer_cast<Point>(shape1);
                    auto point2 = std::dynamic_pointer_cast<Point>(shape2);

                    if (point1 && point2) {
                        // Создаем прямую между точками
                        shapeManager.addBasicShape(Line(*point1, *point2));
                        std::cout << "Line created between points " << i << " and " << j << ".\n";
                    }
                    else {
                        std::cout << "Both shapes must be points.\n";
                    }
                }
                else if (command == "c") {
                    float x, y, x1, y1;
                    std::cin >> x >> y >> x1 >> y1;
                    shapeManager.addBasicShape(Line(Point(x, y),Point(x1,y1)));
                }
                else {
                    std::cout << "Invalid center type. Use 'c' for coordinates or 'p' for point index.\n";
                    continue;
                }
            }
            else if (command == "circle") {
                std::cin >> command;
                Point center;
                if (command == "p") {
                    int i;
                    std::cin >> i;
                    auto shape1 = shapeManager.getBasicShape(i);

                    if (!shape1) {
                        std::cout << "Invalid indices.\n";
                        continue;
                    }
                    // Попытка привести фигуры к точкам
                    auto point1 = std::dynamic_pointer_cast<Point>(shape1);

                    if (point1) {
                        center = *point1;
                    }
                    else {
                        std::cout << "Both shapes must be points.\n";
                    }
                }
                else if (command == "c") {
                    double x, y;
                    std::cin >> x >> y;
                    center = Point(x, y);
                }
                else {
                    std::cout << "Invalid center type. Use 'c' for coordinates or 'p' for point index.\n";
                    continue;
                }

                std::cin >> command;
                if (command == "p") {
                    int i;
                    std::cin >> i;
                    auto shape1 = shapeManager.getBasicShape(i);

                    if (!shape1) {
                        std::cout << "Invalid indices.\n";
                        continue;
                    }
                    // Попытка привести фигуры к точкам
                    auto point1 = std::dynamic_pointer_cast<Point>(shape1);

                    if (point1) {
                        Vector vec = (*point1 - center);
                        // Создаем прямую между точками
                        shapeManager.addBasicShape(Circle(center, vec.abs()));
                    }
                    else {
                        std::cout << "Both shapes must be points.\n";
                    }
                }
                else if (command == "c") {
                    double radius;
                    cin >> radius;
                    // Создаем прямую между точками
                    shapeManager.addBasicShape(Circle(center, radius));
                }
                else {
                    std::cout << "Invalid center type. Use 'c' for coordinates or 'p' for point index.\n";
                    continue;
                }

                
            }
            else if (command == "poligon") {
                int size = 0;
                std::cin >> size;
                std::cin >> command;
                vector<Point> points;
                if (command == "p") {
                    for (int i = 0; i < size; i++) {
                        int j = 0;
                        std::cin >> j;
                        auto shape = shapeManager.getBasicShape(j);
                        if (!shape) {
                            std::cout << "Invalid indices.\n";
                            continue;
                        }
                        auto point1 = std::dynamic_pointer_cast<Point>(shape);
                        if (point1) {
                            points.push_back(*point1);
                        }
                        else {
                            std::cout << "shapes must be points.\n";
                        }
                    }
                    shapeManager.addBasicShape(Poligon(points));
                    std::cout << "Poligon created.\n";
                }
                else if (command == "c") {
                    for (int i = 0; i < size; i++) {
                        float x, y;
                        std::cin >> x >> y;
                        //добавить проверку на верность введенных чи
                        points.push_back(Point(x, y));
                    }
                    shapeManager.addBasicShape(Poligon(points));
                    std::cout << "Poligon created.\n";
                }
                else {
                    std::cout << "Invalid center type. Use 'c' for coordinates or 'p' for point index.\n";
                    continue;
                }
            }
        }
        else if (command == "delete") {
            int i;
            std::cin >> i;
            shapeManager.removeBasicShape(i);
        }
        else if (command == "inf") {
            int index1;
            std::cin >> index1;

            auto shape1 = shapeManager.getBasicShape(index1);

            if (!shape1) {
                std::cout << "Invalid indices.\n";
                continue;
            }

            shape1->printInf();
        }
        else if (command == "move") {
            int index1;
            std::cin >> index1;
            std::cin >> command;
            if (command != "to"){
                std::cout << "missed \"to\"" << endl;
                continue;
            }
            double x, y;
            // проверить введено ли чесло а не строка!!!
            std::cin >> x >> y;
            auto shape1 = shapeManager.getBasicShape(index1);
            if (!shape1) {
                std::cout << "Invalid indices.\n";
                continue;
            }
            shape1->move(x, y);
        }
        else if (command == "rotate") {
            int index;
            std::cin >> index;

            auto shape = shapeManager.getBasicShape(index);
            if (!shape) {
                std::cout << "Invalid index.\n";
                continue;
            }
            //to make it easier to understand
            std::string tmp;
            std::cin >> tmp;
            if (tmp != "around") {
                std::cout << "missed \"around\"\n";
                continue;
            }

            std::string centerType;
            std::cin >> centerType; // Определяем тип центра (c или p)

            Point center;
            if (centerType == "c") { // Центр задан координатами
                float x, y;
                std::cin >> x >> y;
                center = { x, y };
            }
            else if (centerType == "p") { // Центр задан индексом точки
                int centerIndex;
                std::cin >> centerIndex;

                auto centerShape = shapeManager.getBasicShape(centerIndex);
                auto centerPoint = std::dynamic_pointer_cast<Point>(centerShape);

                if (centerPoint) {
                    center = *centerPoint;
                }
                else {
                    std::cout << "Shape at index " << centerIndex << " is not a point.\n";
                    continue;
                }
            }
            else {
                std::cout << "Invalid center type. Use 'c' for coordinates or 'p' for point index.\n";
                continue;
            }

            std::cin >> tmp;
            if (tmp != "an") {
                std::cout << "missed \"an\"\n";
                continue;
            }

            double angle;
            std::cin >> angle; // Считываем угол
            // degree to rad
            angle = angle / 180 * PI;

            shape->rotate(center, angle);
        }
        else if (command == "intersection") {
            std::string shapeType1, shapeType2;
            int index1, index2;

            // Чтение типа и индекса первого объекта
            std::cin >> shapeType1 >> index1;

            // Чтение типа и индекса второго объекта
            std::cin >> shapeType2 >> index2;

            auto shape1 = shapeManager.getBasicShape(index1);
            auto shape2 = shapeManager.getBasicShape(index2);

            // Проверка существования объектов
            if (!shape1 || !shape2) {
                std::cout << "Invalid indices.\n";
                continue;
            }

            // Проверка типа первого объекта
            if (shapeType1 == "line" && shapeType2 == "line") {
                auto line1 = std::dynamic_pointer_cast<Line>(shape1);
                auto line2 = std::dynamic_pointer_cast<Line>(shape2);

                if (line1 && line2) {
                    vector<Point> intersection = go::findIntersection(*line1, *line2);
                    if (!intersection.empty()) {
                        std::cout << "Intersection found at: (" << intersection[0].getX() << ", " << intersection[0].getY() << ")\n";
                        shapeManager.addBasicShape(intersection[0]);
                    }
                    else {
                        std::cout << "No intersection found.\n";
                    }
                }
                else {
                    std::cout << "One or both shapes are not lines.\n";
                }
            }
            else if (shapeType1 == "line" && shapeType2 == "circle") {
                auto line = std::dynamic_pointer_cast<Line>(shape1);
                auto circle = std::dynamic_pointer_cast<Circle>(shape2);

                if (line && circle) {
                    vector<Point> intersection = go::findIntersection(*line, *circle);
                    if (!intersection.empty()) {
                        for (auto& inter : intersection) {
                            std::cout << "Intersection found at: (" << inter.getX() << ", " << inter.getY() << ")\n";
                            shapeManager.addBasicShape(inter);
                        }
                    }
                    else {
                        std::cout << "No intersection found.\n";
                    }
                }
                else {
                    std::cout << "One or both shapes are not lines and circle.\n";
                }
            }
            else if (shapeType1 == "circle"&& shapeType2 == "line") {
                auto line = std::dynamic_pointer_cast<Line>(shape2);
                auto circle = std::dynamic_pointer_cast<Circle>(shape1);

                if (line && circle) {
                    vector<Point> intersection = go::findIntersection(*line, *circle);
                    if (!intersection.empty()) {
                        for (auto& inter : intersection) {
                            std::cout << "Intersection found at: (" << inter.getX() << ", " << inter.getY() << ")\n";
                            shapeManager.addBasicShape(inter);
                        }
                    }
                    else {
                        std::cout << "No intersection found.\n";
                    }
                }
                else {
                    std::cout << "One or both shapes are not circle and line.\n";
                }
            }
            else if (shapeType2 == "circle" && shapeType1 == "circle") {
                auto circle1 = std::dynamic_pointer_cast<Circle>(shape1);
                auto circle2 = std::dynamic_pointer_cast<Circle>(shape2);

                if (circle1 && circle2) {
                    vector<Point> intersection = go::findIntersection(*circle1, *circle2);
                    if (!intersection.empty()) {
                        for (auto& inter : intersection) {
                            std::cout << "Intersection found at: (" << inter.getX() << ", " << inter.getY() << ")\n";
                            shapeManager.addBasicShape(inter);
                        }
                    }
                    else {
                        std::cout << "No intersection found.\n";
                    }
                }
                else {
                    std::cout << "One or both shapes are not circles.\n";
                }
            }
            else {
                std::cout << "Unsupported combination of shapes: " << shapeType1 << " and " << shapeType2 << ".\n";
            }
        }
        else if (command == "distance") {
            std::string shapeType1, shapeType2;
            int index1, index2;

            // Чтение типа и индекса первого объекта
            std::cin >> shapeType1 >> index1;

            // Чтение типа и индекса второго объекта
            std::cin >> shapeType2 >> index2;

            auto shape1 = shapeManager.getBasicShape(index1);
            auto shape2 = shapeManager.getBasicShape(index2);

            // Проверка существования объектов
            if (!shape1 || !shape2) {
                std::cout << "Invalid indices.\n";
                continue;
            }
            // Проверка типа первого объекта
            else if (shapeType1 == "point" && shapeType2 == "line") {
                auto point = std::dynamic_pointer_cast<Point>(shape1);
                auto line = std::dynamic_pointer_cast<Line>(shape2);
                if (point && line) {
                    cout << "distance between " << index1 << " -> " << index2 << " = " << go::distance(*point, *line) << endl;
                }
                else {
                    std::cout << "One or both shapes are not lines.\n";
                }
            }
            else if (shapeType1 == "line" && shapeType2 == "point") {
                auto point = std::dynamic_pointer_cast<Point>(shape2);
                auto line = std::dynamic_pointer_cast<Line>(shape1);
                if (point && line) {
                    cout << "distance between " << index1 << " -> " << index2 << " = " << go::distance(*point, *line) << endl;;
                }
                else {
                    std::cout << "One or both shapes are not lines.\n";
                }
            }
            else if (shapeType1 == "point" && shapeType2 == "point") {
                auto point1 = std::dynamic_pointer_cast<Point>(shape1);
                auto point2 = std::dynamic_pointer_cast<Point>(shape2);
                if (point1 && point2) {
                    cout << "distance between " << index1 << " -> " << index2 << " = " << go::distance(*point1, *point2) << endl;
                }
                else {
                    std::cout << "One or both shapes are not lines.\n";
                }
            }
            else {
                std::cout << "Unsupported combination of shapes: " << shapeType1 << " and " << shapeType2 << ".\n";
            }
        }
        else if (command == "copy") {
            std::cin >> command;
            int index1;
            std::cin >> index1;
            auto shape1 = shapeManager.getBasicShape(index1);
            if (!shape1) {
                std::cout << "Invalid indices.\n";
                continue;
            }
            if (command == "point") {
                auto point = std::dynamic_pointer_cast<Point>(shape1);
                if (point) {
                    shapeManager.addBasicShape(*point);
                }
            }
            else if (command == "line") {
                auto line = std::dynamic_pointer_cast<Line>(shape1);
                if (line) {
                    shapeManager.addBasicShape(*line);
                }
            }
            else if (command == "circle") {
                auto circle = std::dynamic_pointer_cast<Circle>(shape1);
                if (circle) {
                    shapeManager.addBasicShape(*circle);
                }
            }
            else if (command == "poligon") {
                auto poligon = std::dynamic_pointer_cast<Poligon>(shape1);
                if (poligon) {
                    shapeManager.addBasicShape(*poligon);
                }
            }
            else {
                std::cout << "Unsupported shapes " <<command<< endl;
            }
        }
        else if (command == "parallel") {

            int index;
            cin >> index;
            auto shapeLine = shapeManager.getBasicShape(index);
            auto line = std::static_pointer_cast<Line>(shapeLine);
            if (!line) {
                cout << "not line at " << index<<endl;
                continue;
            }

            std::string centerType;
            std::cin >> centerType; // Определяем тип центра (c или p)

            Point point;
            if (centerType == "c") { // Центр задан координатами
                float x, y;
                std::cin >> x >> y;
                point = { x, y };
            }
            else if (centerType == "p") { // Центр задан индексом точки
                int centerIndex;
                std::cin >> centerIndex;

                auto centerShape = shapeManager.getBasicShape(centerIndex);
                auto centerPoint = std::dynamic_pointer_cast<Point>(centerShape);

                if (centerPoint) {
                    point = *centerPoint;
                }
                else {
                    std::cout << "Shape at index " << centerIndex << " is not a point.\n";
                    continue;
                }
            }
            else {
                std::cout << "Invalid center type. Use 'c' for coordinates or 'p' for point index.\n";
                continue;
            }

            shapeManager.addBasicShape(line->getParallel(point));
        }
        else if (command == "perpendicular") {

            int index;
            cin >> index;
            auto shapeLine = shapeManager.getBasicShape(index);
            auto line = std::static_pointer_cast<Line>(shapeLine);
            if (!line) {
                cout << "not line at " << index << endl;
                continue;
            }

            std::string centerType;
            std::cin >> centerType; // Определяем тип центра (c или p)

            Point point;
            if (centerType == "c") { // Центр задан координатами
                float x, y;
                std::cin >> x >> y;
                point = { x, y };
            }
            else if (centerType == "p") { // Центр задан индексом точки
                int centerIndex;
                std::cin >> centerIndex;

                auto centerShape = shapeManager.getBasicShape(centerIndex);
                auto centerPoint = std::dynamic_pointer_cast<Point>(centerShape);

                if (centerPoint) {
                    point = *centerPoint;
                }
                else {
                    std::cout << "Shape at index " << centerIndex << " is not a point.\n";
                    continue;
                }
            }
            else {
                std::cout << "Invalid center type. Use 'c' for coordinates or 'p' for point index.\n";
                continue;
            }
           
            shapeManager.addBasicShape(line->getPerpendicular(point));
        }
        else if (command == "area") {
            int index;
            cin >> index;
            auto shape = shapeManager.getBasicShape(index);
            if (!shape) {
                cout << "Index error";
                continue;
            }
            auto poligon = static_pointer_cast<Poligon>(shape);
            if (poligon) {
                cout << "Poligon " << index << " area = " << poligon->getArea() << endl;
            }
            else {
                cout << "this is not poligon\n";
            }
        }
        else if (command == "exit") {
            std::cout << "Exiting program...\n";
            exit(0);
        }
        else if (command == "help") {
                cout << "0 - all\n"
                    << "1 - help\n"
                    << "2 - add\n"
                    << "3 - delete\n"
                    << "4 - inf\n"
                    << "5 - move\n"
                    << "6 - rotate\n"
                    << "7 - inter\n"
                    << "8 - distance\n"
                    << "9 - exit\n"
                    << "10- ...\n";
                cout << "Enter num: ";
                int num;
                cin >> num;
                switch (num)
                {
                case 1:
                    cout << "help - Display this help message.\n";
                    break;
                case 2:
                    cout << "add [type] ... - Add a new geometric object.\n"
                        << "\tpoint x y - Add a point with coordinates(x, y).\n"
                        << "\tline[p, c] ... - Add a line.\n"
                        << "\t\tp i j - Create a line between points with indices i and j.\n"
                        << "\t\tc x y x1 y1 - Create a line between coordinates(x, y) and (x1, y1).\n";
                    break;
                case 3:
                    cout << "delete i - Remove the object at index i.\n";
                    break;
                case 4:
                    cout << "inf i - Display information about the object at index i.\n";
                    break;
                case 5:
                    cout << "move i to x y - Move the object at index i to a new position (x, y).\n";
                    break;
                case 6:
                    cout << "rotate i around [c, p] ... an angle - Rotate an object at index i.\n"
                        << "\tc x y - Rotate around coordinates(x, y).\n"
                        << "\tp j - Rotate around the point at index j.\n"
                        << "\tangle - Specify the rotation angle in degrees.\n";
                    break;
                case 7:
                    cout << "inter [type1] i [type2] j - Find the intersection between two shapes.\n"
                        << "\ttype1, type2 - Specify the types(e.g., line, point).\n"
                        << "\ti, j - Indices of the shapes to intersect.\n";
                    break;
                case 8:
                    cout << "distance [type1] i [type2] j - Calculate the distance between two shapes.\n"
                        << "\ttype1, type2 - Specify the types(e.g., line, point).\n"
                        << "\ti, j - Indices of the shapes.\n";
                    break;
                case 9:
                    cout << "exit - Exit the program.\n";
                    break;
                case 0:
                     cout << "help - Display this help message.\n";
                
                    cout << "add [type] ... - Add a new geometric object.\n"
                        << "\tpoint x y - Add a point with coordinates(x, y).\n"
                        << "\tline[p, c] ... - Add a line.\n"
                        << "\t\tp i j - Create a line between points with indices i and j.\n"
                        << "\t\tc x y x1 y1 - Create a line between coordinates(x, y) and (x1, y1).\n";
             
                    cout << "delete i - Remove the object at index i.\n";
                 
                    cout << "inf i - Display information about the object at index i.\n";
                  
                    cout << "move i to x y - Move the object at index i to a new position (x, y).\n";
                   
                    cout << "rotate i around [c, p] ... an angle - Rotate an object at index i.\n"
                        << "\tc x y - Rotate around coordinates(x, y).\n"
                        << "\tp j - Rotate around the point at index j.\n"
                        << "\tangle - Specify the rotation angle in degrees.\n";
                  
                    cout << "inter [type1] i [type2] j - Find the intersection between two shapes.\n"
                        << "\ttype1, type2 - Specify the types(e.g., line, point).\n"
                        << "\ti, j - Indices of the shapes to intersect.\n";
                   
                    cout << "distance [type1] i [type2] j - Calculate the distance between two shapes.\n"
                        << "\ttype1, type2 - Specify the types(e.g., line, point).\n"
                        << "\ti, j - Indices of the shapes.\n";
                  
                    cout << "exit - Exit the program.\n";
                   
                    break;
                default:
                    cout << "there is no such command\n";
                    break;
                }
            
        }
        else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Unknown command.\n";
        }
    }

}


// пересечение круга, дистанция до круга,
// пусь положение найзвания прямой круга точик завист от индекса и вращается 
// перпендигуляр и параллельные прямые в командПроцессор и в Класс лайн

// Пусть в GO будут только общие операции типа булевых, а прямая паралельная данной ну это применимо тольок к прямой

int main()
{



    ShapeManager shapeManager;


    std::thread commandThread(commandProcessor, std::ref(shapeManager));

    // Менеджер окна
    WindowManager windowManager(shapeManager);
    windowManager.show();
   
    commandThread.join();

    return 0;
}