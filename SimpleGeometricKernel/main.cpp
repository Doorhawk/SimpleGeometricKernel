#include <SFML/Graphics.hpp>
#include "BasicShape.h"
#include <iostream>
#include "WindowManager.h"
#include "GeometricOperations.h"
#include "global.h"
#include <thread>
#include "enums.h"
#include "CommandManager.h"
#include <format>
#include "Logger.h"


template <typename... Args>
// проверяет корректность ввода чисел
bool inputValidation(Args&... args) {
    (std::cin >> ... >> args);  
    std::ostringstream oss;
    ((oss << args << " "), ...);  // Конкатенация всех аргументов через пробел
    if (std::cin.fail()) {
        std::cout << "Error! invalid input,enter numbers" << std::endl;
        LOG_G_ERROR("Invalid input, enter numbers");
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        LOG_INFO("command", "error");
        return false;
    }
    LOG_INFO("command", oss.str());
    return true;
}
// читает строку и разбивает ее на массив индексов, проверяет на кооректность ввод
// "str = "1,2,3,4" -> vec = {1, 2, 3, 4}"
bool parseIndices(std::vector<int>& indices) {
    std::string input;
    std::getline(std::cin, input); // Читаем всю строку

   

    std::stringstream ss(input);
    std::string token;

    while (std::getline(ss, token, ',')) { // Разбиваем по запятой
        try {
            int index = std::stoi(token);
            indices.push_back(index);
        }
        catch (const std::exception&) {
            std::cout << "Error! Invalid input: " << token << std::endl;
            LOG_G_ERROR("Error! Invalid input"+ token);
            LOG_INFO("command", "error");
            return false;
        }
    }
    LOG_INFO("command", input);
    return !indices.empty();
}


float global::size = 1.f;

using namespace sf;

template <typename T, typename... Args>
std::shared_ptr<T> ms(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

const double PI = acos(-1);


void commandProcessor(CommandManager& commandManager, std::atomic<bool>& isRunning) {
    std::string command;
    while (isRunning) {
        LOG_INFO("command", "\n");
        std::cout << "-> ";
        std::cin >> command;
        LOG_INFO("command", command);
        if (command == "add") {
            std::cin >> command;
            LOG_INFO("command",command);
            if (command == "point") {
                float x, y;
                if (!inputValidation(x, y)) { continue; };
                try {
                    
                    commandManager.addPoint(x, y);
                    std::cout << std::format("Point created ({}, {})\n", x, y);
                    LOG_G_INFO("Point created at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
                }
                catch (std::invalid_argument const& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                    LOG_G_ERROR("Failed to add point: " + std::string(ex.what()));
                }
                
            }
            else if (command == "line") {
                cin >> command;
                LOG_INFO("command",command);
                if (command == "p") {
                    int i, j;
                    if (!inputValidation(i, j)) { continue; };
                    
                    try {
                        commandManager.addLine(i, j);
                        std::cout << "Line created between points " << i << " and " << j << ".\n";
                        LOG_G_INFO("Line created between points " + std::to_string(i) + " and " + std::to_string(j));
                    }
                    catch (std::invalid_argument const& ex) {
                        std::cout << "Error: " << ex.what() << "\n";
                        LOG_G_ERROR("Failed to create line between points " + std::to_string(i) + " and " + std::to_string(j) + ": " + std::string(ex.what()));
                    }
                }
                else if (command == "c") {
                    float x, y, x1, y1;
                    if (!inputValidation(x, y,x1,y1)) { continue; };
                    try {
                        commandManager.addLine(x, y, x1, y1);
                        std::cout << std::format("Line created between coords {}, {} and {}, {}\n", x, y, x1, y1);
                        LOG_G_INFO("Line created between coordinates (" + std::to_string(x) + ", " + std::to_string(y) + ") and (" + std::to_string(x1) + ", " + std::to_string(y1) + ")");
                    }
                    catch (std::invalid_argument const& ex) {
                        std::cout << "Error: " << ex.what() << "\n";
                        LOG_G_ERROR("Failed to create line between coordinates (" + std::to_string(x) + ", " + std::to_string(y) + ") and (" + std::to_string(x1) + ", " + std::to_string(y1) + "): " + std::string(ex.what()));
                    }
                }
                else {
                    std::cout << "Invalid center type. Use 'c' for coordinates or 'p' for point index.\n";
                    LOG_G_WARNING("Invalid command = {"+command+"}. Expected 'c' or 'p'. ");
                    continue;
                }
            }
            else if (command == "circle") {
                cin >> command;
                LOG_INFO("command",command);
                if (command == "p") {
                    int i, j;
                    if (!inputValidation(i, j)) { continue; };

                    try {
                        commandManager.addCircle(i, j);
                        std::cout << "Circle created between points " << i << " and " << j << ".\n";
                        LOG_G_INFO("Circle created, center =  " + std::to_string(i) + ", onCircle = " + std::to_string(j));
                    }
                    catch (std::invalid_argument const& ex) {
                        std::cout << "Error: " << ex.what() << "\n";
                        LOG_G_ERROR("Failed to create Circle with center = " + std::to_string(i) + ", onCircle = " + std::to_string(j) + ": " + std::string(ex.what()));
                    }
                }
                else if (command == "c") {
                    float x, y, x1, y1;
                    if (!inputValidation(x, y, x1, y1)) { continue; };
                    try {
                        commandManager.addCircle(x, y, x1, y1);
                        std::cout << std::format("Circle created between coords {}, {} and {}, {}\n", x, y, x1, y1);
                        LOG_G_INFO("Circle created, center = (" + std::to_string(x) + ", " + std::to_string(y) + "), onCircle = (" + std::to_string(x1) + ", " + std::to_string(y1) + ")");
                    }
                    catch (std::invalid_argument const& ex) {
                        std::cout << "Error: " << ex.what() << "\n";
                        LOG_G_ERROR("Failed to create Circle with center = (" + std::to_string(x) + ", " + std::to_string(y) + "), onCircle = (" + std::to_string(x1) + ", " + std::to_string(y1) + "): " + std::string(ex.what()));
                    }
                }
                else if (command == "3p") {
                    int i, j, k;
                    if (!inputValidation(i, j, k)) { continue; };
                    try {
                        commandManager.addCircle3points(i, j,k);
                        std::cout << "Circle created points " << i << ", " << j <<"," <<k<< "\n";
                        LOG_G_INFO("Circle created, 3 points " + std::to_string(i) + ", " + std::to_string(j)+", "+ std::to_string(k));
                    }
                    catch (std::invalid_argument const& ex) {
                        std::cout << "Error: " << ex.what() << "\n";
                        LOG_G_ERROR("Failed to create Circle with 3 points " + std::to_string(i) + ", " + std::to_string(j) + ", " + std::to_string(k) + ": " + std::string(ex.what()));
                    }
                }
                else {
                    std::cout << "Invalid center type. Use 'c' for coordinates or 'p' for point index or '3p'.\n";
                    LOG_G_WARNING("Invalid command = {" + command + "}. Expected 'c' or 'p'. ");
                    continue;
                }
            }
            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Unknown command.\n";
                LOG_G_WARNING("Unknown command: " + command);
            }
        }
        else if (command == "delete") {
            std::vector<int> indices;
            if (!parseIndices(indices)) { continue;}
            try {
                for (int index : indices) {
                    if (commandManager.deleteShape(index))
                        std::cout << "Delete shape " << index << ".\n";
                    else{
                        std::cout << "Delete shape error " << ".\n";
                        LOG_G_ERROR("Failed to delete shape");
                    }
                        
                }
            }
            catch (std::invalid_argument const& ex) {
                std::cout << "Error: " << ex.what() << "\n";
                LOG_G_ERROR("Failed to delete shape " + std::string(ex.what()));
            }
            
        }
        else if (command == "deleteAll") {
            try {
                commandManager.deleteAllShapes();
                std::cout << "Delete all shape " << ".\n";
            }
            catch (std::invalid_argument const& ex) {
                std::cout << "Error: " << ex.what() << "\n";
                LOG_G_ERROR("Failed to delete all shapes " + std::string(ex.what()));
            }
            
        }
        else if (command == "move") {
            int index = 0;
            if (!inputValidation(index)) { continue; };
            std::cin >> command;
            LOG_INFO("command",command);
            if (command != "to") {
                std::cout << "missed \"to\"" << endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            double x, y;
            if (!inputValidation(x,y)) { continue; };
            try {
                commandManager.moveShape(index, x, y);
                std::cout << "Move shape "<<index<<" to "<<x<<", "<<y << "\n";
                LOG_G_INFO("Move shape " + to_string(index) + " to " + to_string(x) + ", " + to_string(y) + "\n");
            }
            catch (std::invalid_argument const& ex) {
                std::cout << "Error: " << ex.what() << "\n";
                LOG_G_ERROR("Failed to move shape" + std::string(ex.what()));
            }
        }
        else if (command == "rotate") {
            int index = 0;
            if (!inputValidation(index)) { continue; };

            std::string tmp;
            std::cin >> tmp;
            LOG_INFO("command",tmp);
            if (tmp != "around"&& tmp != "ar") {
                std::cout << "missed \"around\"\n";
                LOG_G_WARNING("missed \"around\" in rotate command");
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            std::string centerType;
            std::cin >> centerType; // Определяем тип центра (c или p)
            LOG_INFO("command",centerType);
            if (centerType == "c") { // Центр задан координатами
                float x, y;
                if (!inputValidation(x,y)) { continue; };
                std::cin >> tmp;
                LOG_INFO("command",tmp);
                if (tmp != "by") {
                    std::cout << "missed \"by\"\n";
                    LOG_G_WARNING("missed \"by\" in rotate command");
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }
                double angle;
                std::cin >> angle; // Считываем угол
                if (!inputValidation(angle)) { continue; };
                angle = angle / 180 * PI;

                try {
                    commandManager.rotateShape(index, x, y, angle);
                    std::cout << "Rotate shape " << index << " around " << x << ", " << y <<" by "<<angle << "\n";
                    LOG_G_INFO("Rotate shape " + to_string(index) + " around " + to_string(x) + ", " + to_string(y) + " by " + to_string(angle) + "\n");
                }
                catch (std::invalid_argument const& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                    LOG_G_ERROR("Failed to rotate shape" + std::string(ex.what()));
                }
            }
            else if (centerType == "p") { // Центр задан индексом точки
                int centerIndex;
                std::cin >> centerIndex;
                if (!inputValidation(centerIndex)) { continue; };
                std::cin >> tmp;
                if (tmp != "by") {
                    std::cout << "missed \"by\"\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }
                double angle;
                std::cin >> angle; // Считываем угол
                if (!inputValidation(angle)) { continue; };
                // degree to rad
                angle = angle / 180 * PI;

                try {
                    commandManager.rotateShape(index, centerIndex, angle);
                    std::cout << "Rotate shape " << index << " around point " << centerIndex << " by " << angle << "\n";
                    LOG_G_INFO("Rotate shape " + to_string(index) + " around index =" + to_string(centerIndex) + " by " + to_string(angle) + "\n");
                }
                catch (std::invalid_argument const& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                    LOG_G_ERROR("Failed to rotate shape" + std::string(ex.what()));
                }
               
            }
            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid center type. Use 'c' for coordinates or 'p' for point index.\n";
                LOG_G_WARNING("Invalid center type. Use 'c' for coordinates or 'p' for point index.");
                continue;
            }
        }
        else if (command == "parallel"|| command == "ll") {
            int index = 0;
            if (!inputValidation(index)) { continue; };
            std::string pointType = "";
            std::cin >> pointType; // Определяем тип центра (c или p)
            LOG_INFO("command",pointType);
            if (pointType == "c") { // Центр задан координатами
                float x, y;
                if (!inputValidation(x,y)) { continue; };
                
                try {
                    commandManager.addParallelLine(index, x, y);
                    std::cout << "add Parallel line to line " << index << ", point " << x << ", " << y<<endl;
                    LOG_G_INFO(std::format("add Parallel line to line index = {}, point ({}, {})",index,x,y));
                }
                catch (std::invalid_argument const& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                    LOG_G_ERROR("Failed to add parallel " + std::string(ex.what()));
                }
            }
            else if (pointType == "p") { // Центр задан индексом точки
                int pointIndex;
                if (!inputValidation(pointIndex)) { continue; };
                try {
                    commandManager.addParallelLine(index, pointIndex);
                    std::cout << "add Parallel line to line " << index << ", point " << pointIndex <<endl;
                    LOG_G_INFO(std::format("add Parallel line to line index = {}, point index = {}", index, pointIndex));
                }
                catch (std::invalid_argument const& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                    LOG_G_ERROR("Failed to add parallel " + std::string(ex.what()));
                }
                
            }
            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid point type. Use 'c' for coordinates or 'p' for point index.\n";
                LOG_G_WARNING("Invalid point type. Use 'c' for coordinates or 'p' for point index.");
                continue;
            }
        }
        else if (command == "perpendicular" || command == "pp") {

            int index = 0;
            if (!inputValidation(index)) { continue; };
            std::string pointType = "";
            std::cin >> pointType; // Определяем тип центра (c или p)
            LOG_INFO("command",pointType);
            if (pointType == "c") { // Центр задан координатами
                float x, y;
                if (!inputValidation(x, y)) { continue; };

                try {
                    commandManager.addPerpendicularLine(index, x, y);
                    std::cout << "add Perpendicular line to line " << index << ", point " << x << ", " << y << endl;
                    LOG_G_INFO(std::format("add Perpendicular line to line index = {}, point ({}, {})", index, x, y));
                }
                catch (std::invalid_argument const& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                    LOG_G_ERROR("Failed to add Perpendicular " + std::string(ex.what()));
                }
            }
            else if (pointType == "p") { // Центр задан индексом точки
                int pointIndex;
                if (!inputValidation(pointIndex)) { continue; };
                try {
                    commandManager.addPerpendicularLine(index, pointIndex);
                    std::cout << "add Perpendicular line to line " << index << ", point  " << pointIndex << endl;
                    LOG_G_INFO(std::format("add add Perpendicular line to line index = {}, point index = {}", index, pointIndex));
                }
                catch (std::invalid_argument const& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                    LOG_G_ERROR("Failed to add Perpendicular " + std::string(ex.what()));
                }

            }
            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid center type. Use 'c' for coordinates or 'p' for point index.\n";
                LOG_G_WARNING("Invalid point type. Use 'c' for coordinates or 'p' for point index.");
                continue;
            }
        }
        else if (command == "medianPerpendicular" || command == "mp") {
            
            std::string pointType;
            std::cin >> pointType; // Определяем тип центра (c или p)
            LOG_INFO("command",pointType);
            if (pointType == "c") { // Центр задан координатами
                float x = 0, y = 0;
                float x1 = 0, y1 = 0;
                if (!inputValidation(x, y, x1, y1)) { continue; };
                try {
                    commandManager.addMedianPerpendicular(x, y, x1, y1);
                    std::cout << std::format("add Median Perpendicular ({}, {}), ({}, {})\n", x, y, x1, y1);
                    LOG_G_INFO(std::format("add Median Perpendicular ({}, {}), ({}, {})", x, y, x1, y1));
                }
                catch (std::invalid_argument const& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                    LOG_G_ERROR("Failed to add Median Perpendicular " + std::string(ex.what()));
                }
               
            }
            else if (pointType == "p") { // Центр задан индексом точки
                int point1Index = 0,point2Index= 0;
                if (!inputValidation(point1Index, point2Index)) { continue; };
                try {
                    commandManager.addMedianPerpendicular(point1Index,point2Index);
                    std::cout << std::format("add Median Perpendicular point index {}, {}\n", point1Index, point2Index);
                    LOG_G_INFO(std::format("add Median Perpendicular point index {}, {}", point1Index, point2Index));
                }
                catch (std::invalid_argument const& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                    LOG_G_ERROR("Failed to add Median Perpendicular " + std::string(ex.what()));
                }
            }
            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid point type. Use 'c' for coordinates or 'p' for point index.\n";
                LOG_G_WARNING("Invalid point type. Use 'c' for coordinates or 'p' for point index.");
                continue;
            }
                
        }
        else if (command == "bisectrix" || command == "bi") {

            std::string pointType;
            std::cin >> pointType; // Определяем тип центра (c или p)
            LOG_INFO("command",pointType);
            if (pointType == "c") { // Центр задан координатами
                float x = 0, y = 0;
                float x1 = 0, y1 = 0;
                float x2 = 0, y2 = 0;
                if (!inputValidation(x, y, x1, y1, x2, y2)) { continue; };
                try {
                    //commandManager.addBisectrix(x, y, x1, y1);
                    std::cout << std::format("add bisectrix ({}, {}), ({}, {}), ({}, {})\n", x, y, x1, y1,x2,y2);
                    LOG_G_INFO(std::format("add bisectrix ({}, {}), ({}, {}), ({}, {})", x, y, x1, y1,x2,y2));
                }
                catch (std::invalid_argument const& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                    LOG_G_ERROR("Failed to add bisectrix " + std::string(ex.what()));
                }

            }
            else if (pointType == "p") { // Центр задан индексом точки
                int point1Index = 0, point2Index = 0, point3Index = 0;
                if (!inputValidation(point1Index, point2Index, point3Index)) { continue; };
                try {
                    commandManager.addBisectrix(point1Index, point2Index,point3Index);
                    std::cout << std::format("add bisectrix point index {}, {}, {}\n", point1Index, point2Index, point3Index);
                    LOG_G_INFO(std::format("add Median Perpendicular point index {}, {}, {}", point1Index, point2Index, point3Index));
                }
                catch (std::invalid_argument const& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                    LOG_G_ERROR("Failed to add bisectrix " + std::string(ex.what()));
                }
            }
            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid point type. Use 'c' for coordinates or 'p' for point index.\n";
                LOG_G_WARNING("Invalid point type. Use 'c' for coordinates or 'p' for point index.");
                continue;
            }

            }
        else if (command == "intersection" || command == "inter") {
            int index1 = 0, index2 = 0;
            string type1, type2;
            cin >> type1;
            LOG_INFO("command", type1);
            if (!inputValidation(index1)) { continue; };
            cin >> type2;
            LOG_INFO("command", type2);
            if (!inputValidation(index2)) { continue; };

            if (type1 == "line" && type2 == "line") {

                try {
                    commandManager.intersection(st_line, index1, st_line, index2);
                    std::cout << std::format("add intersection Line {} - Line {}\n",index1,index2);
                    LOG_G_INFO(std::format("add intersection Line {} - Line {}", index1, index2));
                }
                catch (std::invalid_argument const& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                    LOG_G_ERROR("Failed to add intersection line line " + std::string(ex.what()));
                }
            }
            else {
                std::cout << "not for this type\n";
                LOG_G_WARNING("cant intersection type: "+ type1+", "+ type2);
            }

        }
        else if (command == "belong") {
            int index;
            if (!inputValidation(index)) { continue; };
            
            std::string pointType;
            std::cin >> pointType; // Определяем тип центра (c или p)
            LOG_INFO("command",pointType);
            shared_ptr<Point> point;
            if (pointType == "c") { // Центр задан координатами
                float x, y;
                if (!inputValidation(x,y)) { continue; };
                try {
                    commandManager.addPointBelong(index, x, y);
                    std::cout << std::format("add Point Belone Line {}\n", index);
                    LOG_G_INFO(std::format("add Point Belone Line {}", index));
                }
                catch (std::invalid_argument const& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                    LOG_G_ERROR("Failed to add belong point " + std::string(ex.what()));
                }
            }
            else if (pointType == "p") { // Центр задан индексом точки
                int pointIndex;
                if (!inputValidation(pointIndex)) { continue; };
                try {
                    commandManager.addPointBelong(index, pointIndex);
                    std::cout << std::format("add Point Belone Line {}\n", index);
                    LOG_G_INFO(std::format("add Point Belone Line {}", index));
                }
                catch (std::invalid_argument const& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                    LOG_G_ERROR("Failed to add belong point " + std::string(ex.what()));
                }
            }
            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid point type. Use 'c' for coordinates or 'p' for point index.\n";
                LOG_G_WARNING("Invalid point type. Use 'c' for coordinates or 'p' for point index.");
                continue;
            }

        }
        else if (command == "inf") {
            int index1;
            
            if (!inputValidation(index1)) { continue; };
            try {
                commandManager.getInf(index1);
                LOG_G_INFO("get info shape index ="+to_string(index1));
            }
            catch (std::invalid_argument const& ex) {
                std::cout << "Error: " << ex.what() << "\n";
                LOG_G_ERROR("Failed to get inf " + std::string(ex.what()));
            }
        }
        else if (command == "color") {
            
            double r, g, b, a;
            if (!inputValidation(r, g, b, a)) { continue; };
            std::cin >> command;
            LOG_INFO("command",command);
            if (command != "to") {
                std::cout << "missed \"to\"" << endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            std::vector<int> indices;
            if (!parseIndices(indices)) { continue; }
            try {
                for (int index : indices) {
                    commandManager.setColor(Color(r, g, b, a), index);
                    std::cout << "color shape " << index << " to " << r << ", " << g << ", " << b << ", " << a << "\n";
                    LOG_G_INFO("color shape " + to_string(index) + " to " + to_string(r) + ", "
                        + to_string(g) + ", "
                        + to_string(b) + ", "
                        + to_string(a) + "\n");
                }
            }
            catch (std::invalid_argument const& ex) {
                std::cout << "Error: " << ex.what() << "\n";
                LOG_G_ERROR("Failed to set shape shape" + std::string(ex.what()));
            }
            }
        else if (command == "colorDef") {
            double r, g, b, a;
            if (!inputValidation(r, g, b, a)) { continue; };
            try {
                commandManager.setDefaultColor(Color(r, g, b, a));
                std::cout << "default color = " << r << ", " << g << ", " << b << ", " << a << "\n";
                LOG_G_INFO("default color = " + to_string(r) + ", "
                    + to_string(g) + ", "
                    + to_string(b) + ", "
                    + to_string(a) + "\n");
            }
            catch (std::invalid_argument const& ex) {
                std::cout << "Error: " << ex.what() << "\n";
                LOG_G_ERROR("Failed to set default color" + std::string(ex.what()));
            }
        }
        else if (command == "colorAll") {
            double r, g, b, a;
            if (!inputValidation(r, g, b, a)) { continue; };
            try {
                commandManager.setAllColor(Color(r, g, b, a));
                std::cout << "All color = " << r << ", " << g << ", " << b << ", " << a << "\n";
                LOG_G_INFO("All color = " + to_string(r) + ", "
                    + to_string(g) + ", "
                    + to_string(b) + ", "
                    + to_string(a));
            }
            catch (std::invalid_argument const& ex) {
                std::cout << "Error: " << ex.what() << "\n";
                LOG_G_ERROR("Failed to set All color" + std::string(ex.what()));
            }
        }
        else if (command == "exit") {
            std::cout << "Exiting program...\n";
            isRunning = false;
            LOG_G_INFO("Exit command processor");
        }
        else {
             std::cin.clear();
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             std::cout << "Unknown command.\n";
             LOG_G_WARNING("Unknown command: "+ command);
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
   

    LOG_IN_CONSOLE(false);
    LOG_G_SET_FORMAT("[%L] %T - %M\n");
    LOG_G_INFO("____________________________START__________________________");

    LOG_SET_FORMAT("command", "%M ");
    LOG_INFO("command", "_______start_______\n");

    // важное, добавть инфалидность себе и детям если например точка была 
    // пересчением прямых а они стали параллельны, или если точка пересечение окружности и прямой то когда пересечения нет делаем точку и ее детей инвалидами и не рисуем

    std::atomic<bool> isRunning{ true };

    ShapeManager shapeManager;

    CommandManager commandManager(shapeManager);

    std::thread commandThread(commandProcessor, std::ref(commandManager), std::ref(isRunning));

    WindowManager windowManager(shapeManager, isRunning);
    
    windowManager.show();
    commandThread.join();


    LOG_INFO("command", "\n________end________\n");
    LOG_G_INFO("_____________________________END____________________________");
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