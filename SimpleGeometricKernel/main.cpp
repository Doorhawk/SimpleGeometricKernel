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
                
            }
        }
        else if (command == "rotate") {
            std::string shapeType;
            std::cin >> shapeType; // Определяем тип фигуры (point или line)
            int index;
            std::cin >> index;

            auto shape = shapeManager.getBasicShape(index);
            if (!shape) {
                std::cout << "Invalid index.\n";
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

            double angle;
            std::cin >> angle; // Считываем угол

            if (shapeType == "point") {
                auto point = std::dynamic_pointer_cast<Point>(shape);
                if (point) {
                    go::rotate(*point, center, angle);
                    std::cout << "Point rotated successfully.\n";
                }
                else {
                    std::cout << "Shape at index " << index << " is not a point.\n";
                }
            }
            else if (shapeType == "line") {
                auto line = std::dynamic_pointer_cast<Line>(shape);
                if (line) {
                    go::rotate(*line, center, angle);
                    std::cout << "Line rotated successfully.\n";
                }
                else {
                    std::cout << "Shape at index " << index << " is not a line.\n";
                }
            }
            else {
                std::cout << "Invalid shape type. Use 'point' or 'line'.\n";
            }
        }
        else if (command == "exit") {
            std::cout << "Exiting program...\n";
            exit(0);
        }
        else {
            std::cout << "Unknown command.\n";
        }
    }

}

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