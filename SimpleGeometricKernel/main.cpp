#include <SFML/Graphics.hpp>
#include "BasicShape.h"
#include <iostream>
#include "WindowManager.h"
#include "GeometricOperations.h"
#include "global.h"
#include <thread>
#include "enums.h"

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
        std::cout << "-> ";
        std::cin >> command;

        if (command == "add") {
            std::cin >> command;
            if (command == "point") {
                float x, y;
                std::cin >> x >> y;
                shapeManager.addBasicShape(Point(x, y));
            }
            else if (command == "line") {
                cin >> command;
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
                        shapeManager.addBasicShape(Line::create(point1, point2));
                        std::cout << "Line created between points " << i << " and " << j << ".\n";
                    }
                    else {
                        std::cout << "Both shapes must be points.\n";
                    }
                }
                else if (command == "c") {
                    float x, y, x1, y1;
                    std::cin >> x >> y >> x1 >> y1;

                    auto p1 = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x, y)));
                    auto p2 = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x1, y1)));
                    if(p1&&p2)
                        shapeManager.addBasicShape(Line::create(p1,p2));
                }
                else {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid center type. Use 'c' for coordinates or 'p' for point index.\n";
                    continue;
                }
            }
            /*else if (command == "segment") {
                cin >> command;
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
                    shapeManager.addBasicShape(Segment(Point(x, y), Point(x1, y1)));
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
            else if (command == "sector") {
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
                double rad;
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
                        rad = vec.abs();
                    }
                    else {
                        std::cout << "Both shapes must be points.\n";
                    }
                }
                else if (command == "c") {
                    double radius;
                    cin >> radius;
                    // Создаем прямую между точками
                    rad = radius;
                }
                else {
                    std::cout << "Invalid center type. Use 'c' for coordinates or 'p' for point index.\n";
                    continue;
                }

                double angle1, angle2;
                cin >> angle1>>angle2;
                // добавитть проверку введения углов
                shapeManager.addBasicShape(Sector(center, rad, angle1/180*acos(-1), angle2 /180 * acos(-1)));

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
            }*/
        }
        else if (command == "delete") {
            int i;
            std::cin >> i;
            shapeManager.removeBasicShape(i);
        }
        else if (command == "deleteAll") {
            shapeManager.removeAllBasicShape();
        }
        else if (command == "move") {
            int index1;
            std::cin >> index1;
            std::cin >> command;
            if (command != "to") {
                std::cout << "missed \"to\"" << endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
            if (tmp != "around"&& tmp != "ar") {
                std::cout << "missed \"around\"\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid center type. Use 'c' for coordinates or 'p' for point index.\n";
                continue;
            }

            std::cin >> tmp;
            if (tmp != "by") {
                std::cout << "missed \"by\"\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            double angle;
            std::cin >> angle; // Считываем угол
            // degree to rad
            angle = angle / 180 * PI;

            shape->rotate(center, angle);
        }
        else if (command == "parallel"|| command == "ll") {

            int index;
            cin >> index;
            auto shapeLine = shapeManager.getBasicShape(index);
            if (!shapeLine) {
                cout << "invalid index\n";
            }

            std::string centerType;
            std::cin >> centerType; // Определяем тип центра (c или p)

            shared_ptr<Point> point;
            if (centerType == "c") { // Центр задан координатами
                float x, y;
                std::cin >> x >> y;
                point = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x, y)));
            }
            else if (centerType == "p") { // Центр задан индексом точки
                int centerIndex;
                std::cin >> centerIndex;

                auto centerShape = shapeManager.getBasicShape(centerIndex);
                point = std::dynamic_pointer_cast<Point>(centerShape);

                if (!point) {
                    std::cout << "Shape at index " << centerIndex << " is not a point.\n";
                    continue;
                }
            }
            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid center type. Use 'c' for coordinates or 'p' for point index.\n";
                continue;
            }

            if (shapeLine->getType() == "line") {
                auto line = std::dynamic_pointer_cast<Line>(shapeLine);
                if (!line) {
                    cout << "not line at " << index << endl;
                    continue;
                }

                //auto p1 = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(point));
                //auto p2 = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(*point + Point(10,10)));
                auto p2 = std::make_shared<Point>(*point + Point(10,10));
                if (p2) {
                    auto parallelLine = shapeManager.addBasicShape(Line::create(point, p2));
                    parallelLine->setParent(DependsTypes::Parallel, line);
                }
                    

            }
            /*else if (shapeLine->getType() == "segment") {
                auto sec = std::dynamic_pointer_cast<Segment>(shapeLine);
                if (!sec) {
                    cout << "not line at " << index << endl;
                    continue;
                }
                shapeManager.addBasicShape(sec->getParallel(point));
            }*/


            }
        else if (command == "perpendicular"|| command == "pr") {

            int index;
            cin >> index;
            auto shapeLine = shapeManager.getBasicShape(index);
            if (!shapeLine) {
                cout << "invalid index\n";
            }

            std::string centerType;
            std::cin >> centerType; // Определяем тип центра (c или p)

            shared_ptr<Point> point;
            if (centerType == "c") { // Центр задан координатами
                float x, y;
                std::cin >> x >> y;
                point = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x, y)));
            }
            else if (centerType == "p") { // Центр задан индексом точки
                int centerIndex;
                std::cin >> centerIndex;

                auto centerShape = shapeManager.getBasicShape(centerIndex);
                point = std::dynamic_pointer_cast<Point>(centerShape);

                if (!point) {
                    std::cout << "Shape at index " << centerIndex << " is not a point.\n";
                    continue;
                }
            }
            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid center type. Use 'c' for coordinates or 'p' for point index.\n";
                continue;
            }

            if (shapeLine->getType() == "line") {
                auto line = std::dynamic_pointer_cast<Line>(shapeLine);
                if (!line) {
                    cout << "not line at " << index << endl;
                    continue;
                }

                //auto p1 = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(point));
                //auto p2 = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(*point + Point(10,10)));
                auto p2 = std::make_shared<Point>(*point + Point(10, 10));
                if (p2) {
                    auto parallelLine = shapeManager.addBasicShape(Line::create(point, p2));
                    parallelLine->setParent(DependsTypes::Perpendicular, line);
                }


            }
            /*else if (shapeLine->getType() == "segment") {
                auto sec = std::dynamic_pointer_cast<Segment>(shapeLine);
                if (!sec) {
                    cout << "not line at " << index << endl;
                    continue;
                }
                shapeManager.addBasicShape(sec->getParallel(point));
            }*/


            }
        else if (command == "belong") {

            int index;
            cin >> index;
            auto shapeLine = shapeManager.getBasicShape(index);
            if (!shapeLine) {
                cout << "invalid index\n";
            }

            std::string centerType;
            std::cin >> centerType; // Определяем тип центра (c или p)

            shared_ptr<Point> point;
            if (centerType == "c") { // Центр задан координатами
                float x, y;
                std::cin >> x >> y;
                point = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x, y)));
            }
            else if (centerType == "p") { // Центр задан индексом точки
                int centerIndex;
                std::cin >> centerIndex;

                auto centerShape = shapeManager.getBasicShape(centerIndex);
                point = std::dynamic_pointer_cast<Point>(centerShape);

                if (!point) {
                    std::cout << "Shape at index " << centerIndex << " is not a point.\n";
                    continue;
                }
            }
            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid center type. Use 'c' for coordinates or 'p' for point index.\n";
                continue;
            }

            if (shapeLine->getType() == "line") {
                auto line = std::dynamic_pointer_cast<Line>(shapeLine);
                if (!line) {
                    cout << "not line at " << index << endl;
                    continue;
                }

                point->setParent(DependsTypes::BelongsToLine, line);
                

            }
            /*else if (shapeLine->getType() == "segment") {
                auto sec = std::dynamic_pointer_cast<Segment>(shapeLine);
                if (!sec) {
                    cout << "not line at " << index << endl;
                    continue;
                }
                shapeManager.addBasicShape(sec->getParallel(point));
            }*/


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
        else if (command == "exit") {
            std::cout << "Exiting program...\n";
            exit(0);
        }
        else {
             std::cin.clear();
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             std::cout << "Unknown command.\n";
        }
       /* else if (command == "addMarkers") {
            int i;
            std::cin >> i;
            auto shape1 = shapeManager.getBasicShape(i);
            if (!shape1) {
                std::cout << "Invalid indices.\n";
                continue;
            }
            string type = shape1->getType();
            if (type == "segment") {
                auto seg = std::dynamic_pointer_cast<Segment>(shape1);
                if (seg) {
                    // Создаем прямую между точками
                    shapeManager.addBasicShape(seg->getStart());
                    shapeManager.addBasicShape(seg->getEnd());
                }
                else {
                    std::cout << "It must be segment.\n";
                }
            }
            else if (type == "circle") {
                auto cir = std::dynamic_pointer_cast<Circle>(shape1);
                if (cir) {
                    // Создаем прямую между точками
                    shapeManager.addBasicShape(cir->getCenter());
                }
                else {
                    std::cout << "It must be circle.\n";
                }

            }
            else if (type == "sector") {
                
                auto sec = std::dynamic_pointer_cast<Sector>(shape1);

                if (sec) {
                    shapeManager.addBasicShape(sec->getCenter());
                    shapeManager.addBasicShape(sec->getStartPoint());
                    shapeManager.addBasicShape(sec->getEndPoint());
                    // и ещё добавить определение токек начала и конца дуги
                }
                else {
                    std::cout << "It must be circle.\n";
                }
            }
            else if (type == "poligon") {
                
                auto poli = std::dynamic_pointer_cast<Poligon>(shape1);

                if (poli) {
                    for(Point& po:poli->getPoints())
                        shapeManager.addBasicShape(po);
                    // и ещё добавить определение токек начала и конца дуги
                }
                else {
                    std::cout << "It must be poligon.\n";
                }
            }
            else {
                cout << "No for this type\n";
            }
        }
        
        else if (command == "intersection") {
            int index1, index2;
            std::cin >> index1>>index2;
            auto shape1 = shapeManager.getBasicShape(index1);
            auto shape2 = shapeManager.getBasicShape(index2);
            if (!shape1 || !shape2) {
                std::cout << "Invalid indices.\n";
                continue;
            }
            std::string shapeType1, shapeType2;
            shapeType1 = shape1->getType();
            shapeType2 = shape2->getType();
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
            else if (shapeType1 == "circle" && shapeType2 == "circle") {
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
            int index1, index2;
            std::cin >> index1 >> index2;
            auto shape1 = shapeManager.getBasicShape(index1);
            auto shape2 = shapeManager.getBasicShape(index2);
            if (!shape1 || !shape2) {
                std::cout << "Invalid indices.\n";
                continue;
            }
            std::string shapeType1, shapeType2;
            shapeType1 = shape1->getType();
            shapeType2 = shape2->getType();
            if (shapeType1 == "point" && shapeType2 == "line") {
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
            int index1;
            std::cin >> index1;
            auto shape1 = shapeManager.getBasicShape(index1);
            string shapeType = shape1->getType();
            if (!shape1) {
                std::cout << "Invalid indices.\n";
                continue;
            }
            if (shapeType == "point") {
                auto point = std::dynamic_pointer_cast<Point>(shape1);
                if (point) {
                    shapeManager.addBasicShape(*point);
                }
            }
            else if (shapeType == "line") {
                auto line = std::dynamic_pointer_cast<Line>(shape1);
                if (line) {
                    shapeManager.addBasicShape(*line);
                }
            }
            else if (shapeType == "circle") {
                auto circle = std::dynamic_pointer_cast<Circle>(shape1);
                if (circle) {
                    shapeManager.addBasicShape(*circle);
                }
            }
            else if (shapeType == "poligon") {
                auto poligon = std::dynamic_pointer_cast<Poligon>(shape1);
                if (poligon) {
                    shapeManager.addBasicShape(*poligon);
                }
            }
            else {
                std::cout << "Unsupported shapes " << shapeType << endl;
            }
        }
        
        else if (command == "perpendicular") {

            int index;
            cin >> index;
            auto shapeLine = shapeManager.getBasicShape(index);
            if (!shapeLine) {
                cout << "invalid index\n";
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
            if (shapeLine->getType() == "line") {
                auto line = std::dynamic_pointer_cast<Line>(shapeLine);
                if (!line) {
                    cout << "not line at " << index << endl;
                    continue;
                }
                shapeManager.addBasicShape(line->getPerpendicular(point));
            }
            else if (shapeLine->getType() == "segment") {
                auto sec = std::dynamic_pointer_cast<Segment>(shapeLine);
                if (!sec) {
                    cout << "not line at " << index << endl;
                    continue;
                }
                shapeManager.addBasicShape(sec->getPerpendicular(point));
            }
            
        }
        else if (command == "middle") {
            string type = "";
            cin >> type;
            if (type == "c") { // Центр задан координатами
                float x, y, x1,y1;
                std::cin >> x >> y>>x1>>y1;
                shapeManager.addBasicShape(go::findMiddle(Point(x, y), Point(x1, y1)));
            }
            else if (type == "p") { // Центр задан индексом точки
                int i1;
                std::cin >> i1;
                int i2;
                std::cin >> i2;

                auto shape1 = shapeManager.getBasicShape(i1);
                auto shape2 = shapeManager.getBasicShape(i2);
                

                if (!shape1||!shape2) {
                    std::cout << "Invalid indices.\n";
                    continue;
                }
                auto point1 = dynamic_pointer_cast<Point>(shape1);
                auto point2 = dynamic_pointer_cast<Point>(shape2);
                if (point1 && point2) {
                    shapeManager.addBasicShape(go::findMiddle(*point1, *point2));
                }
                else {
                    cout << "one is not a point\n";
                }

            }
            else {
                std::cout << "Invalid center type. Use 'c' for coordinates or 'p' for point index.\n";
                continue;
            }
        }
        else if (command == "area") {
            int index;
            cin >> index;
            auto shape = shapeManager.getBasicShape(index);
            if (!shape) {
                cout << "Index error";
                continue;
            }
            auto poligon = dynamic_pointer_cast<Poligon>(shape);
            if (poligon) {
                cout << "Poligon " << index << " area = " << poligon->getArea() << endl;
            }
            else {
                cout << "this is not poligon\n";
            }
        }
        else if (command == "divide") {
            int index;
            cin >> index;
            auto shapeLine = shapeManager.getBasicShape(index);
            auto seg= std::dynamic_pointer_cast<Segment>(shapeLine);
            if (!seg) {
                cout << "not segment at " << index << endl;
                continue;
            }
            std::string Type;
            std::cin >> Type; // Определяем тип центра (c или p)

            Point point;
            if (Type == "c") { // Центр задан координатами
                float x, y;
                std::cin >> x >> y;
                shapeManager.addBasicShape(Point(x, y));
                point = { x, y };
            }
            else if (Type == "p") { // Центр задан индексом точки
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

            vector<Segment> segments = seg->divide(point);
            if (segments.empty()) {
                cout << "point the point does not belong to the segment or the point belongs to the edge\n";
                continue;
            }
            shapeManager.addBasicShape(segments[0]);
            shapeManager.addBasicShape(segments[1]);
            shapeManager.removeBasicShape(index);
        }
        else if (command == "angle") {
            cin >> command;
            if (command == "line") {
                int index1, index2;
                cin >> index1 >> index2;
                auto shape1 = shapeManager.getBasicShape(index1);
                auto shape2 = shapeManager.getBasicShape(index2);
                if (!shape1 || !shape2) {
                    cout << "Invalid index"<< endl;
                    continue;
                }
                auto line1 = dynamic_pointer_cast<Line>(shape1);
                auto line2 = dynamic_pointer_cast<Line>(shape2);
                if (!line1 || !line2) {
                    cout << "is not line on " << index1 << " " << index2 << endl;
                    continue;
                }
                cout<<"angle = "<<go::findAngle(*line1, *line2)*180/acos(-1)<<endl;

            }
            else if (command == "point") {
                int index1, index2,index3;
                cin >> index1 >> index2>>index3;
                auto shape1 = shapeManager.getBasicShape(index1);
                auto shape2 = shapeManager.getBasicShape(index2);
                auto shape3 = shapeManager.getBasicShape(index3);
                if (!shape1 || !shape2||!shape3) {
                    cout << "Invalid index" << endl;
                    continue;
                }
                auto p1 = dynamic_pointer_cast<Point>(shape1);
                auto p2 = dynamic_pointer_cast<Point>(shape2);
                auto p3 = dynamic_pointer_cast<Point>(shape3);
                if (!p1 || !p2||!p3) {
                    cout << "is not point on " << index1 << " " << index2 << endl;
                    continue;
                }
                cout << "angle = " << go::findAngle(*p1, *p2,*p3) * 180 / acos(-1)<<endl;
            }
            else {
                cout << "no for this type\n";
            }
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
            
        }*/
       
    }

}


// при изменении линии она меняет положение точек те обновляют состояни линии 2 раза не круто 

// линия делится на 2 луча, луч делится на отрезок и луч
// добавит везде геттреры и сеттеры
// правельные мнооугольникик как центр точка в угле(центре стороны) и радиус
// прямоугольник по 2м точкам, пеергрузка определения у полигна
// сделат функцию удаления дубликатов, для этого в базовый класс над одобавить переменную овчечающую за тип обьекта
// модифицировать круг до дуги и сделать разбиение круга на дуги точкой
// пуст линия и отрезок будут разными классыми, потому что с помощью отречков моджно создавать другие фигуры(сложные контуры) а главнное ээфективно реализовать пересечение фигур друг с другом
// логичнее наверное что полигон это группа отрезков
// добавить функцию разбития полигона фигуры на составные -> линии и точки, для дальнейшего взаимодействия 
// добавить ещё способы задания сектора кроме как углами
// добавить функцию ищущую угол между прямыми (или по трем точкам
// фукция проверябщее принадлежит ли точка фигуре(прямой, кругу и тд)
// при divide полигон он должен удалять и разбиваться на линии, и уж не обходимая линии разбиваться на 2 наверное
// или похуй... функиця создания полигона из отрезков мб
// функия декстрой которая разбивает полигон на отрезки и точки
// 
// 
// сложная схема сдулать так чтобы
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




/*
add point 0 0
add line c 0 0 10 10
delete 0
add point 10 0
add line p 1 4
pr 3 p 4
ll 3 p 4
move 3 to 1 1
move 4 to 1 1
move 4 to 1 1
move 4 to 1 1
delete 6
delete 5
delete 7
delete 2
*/