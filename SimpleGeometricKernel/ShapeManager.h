#pragma once
#include <vector>
#include "BasicShape.h"
#include <unordered_map>
#include <mutex>
#include <condition_variable>

class ShapeManager {
private:
    std::mutex shapesMutex;
    std::condition_variable cv;
    bool allDrowed = false;
    bool shapesModified = true;
    std::unordered_map<int, std::shared_ptr<BasicShape>> shapes; // ’ранение фигур по их индексам
    int nextIndex = 0; // »ндекс дл€ следующей фигуры
public:
    template <typename T, typename = std::enable_if_t<std::is_base_of_v<BasicShape, T>>>
    std::shared_ptr<BasicShape> addBasicShape(const T& shape) {
        shapesModified = false;
        std::unique_lock<std::mutex> lock(shapesMutex);
        cv.wait(lock, [this] { return allDrowed; }); // ∆дЄм завершени€ кадра
        auto ptr = std::make_shared<T>(shape);
        if (ptr) {
            shapes[nextIndex] = ptr;
            shapes[nextIndex]->setIndex(nextIndex);
            shapesModified = true;
            cv.notify_all();
            nextIndex++;
            return shapes[nextIndex-1];
            
        }
        else {
            std::cerr << "Error: Attempting to add an invalid shape.\n";
            shapesModified = true;
            cv.notify_all();
            return nullptr;
        }
    }
    template <typename T, typename = std::enable_if_t<std::is_base_of_v<BasicShape, T>>>
    std::shared_ptr<BasicShape> addBasicShape(std::shared_ptr<T> shape) {
        shapesModified = false;
        std::unique_lock<std::mutex> lock(shapesMutex);
        cv.wait(lock, [this] { return allDrowed; }); // ∆дЄм завершени€ кадра
        if (shape) {
            shapes[nextIndex] = shape;
            shapes[nextIndex]->setIndex(nextIndex);
            shapesModified = true;
            cv.notify_all();
            nextIndex++;
            return shapes[nextIndex - 1];
        }
        else {
            std::cerr << "Error: Attempting to add an invalid shape.\n";
            shapesModified = true;
            cv.notify_all();
            return nullptr;
        }
    }
    void removeAllBasicShape() {
        shapesModified = false;
        std::unique_lock<std::mutex> lock(shapesMutex);
        cv.wait(lock, [this] { return allDrowed; }); // ∆дЄм завершени€ кадра

        shapes.clear();
        nextIndex = 0;
        shapesModified = true;
        cv.notify_all();
    }
    bool removeBasicShape(size_t index) {
        shapesModified = false;
        std::unique_lock<std::mutex> lock(shapesMutex);
        cv.wait(lock, [this] { return allDrowed; }); // ∆дЄм завершени€ кадра

        try {
            shapes.erase(index);
        }
        catch(...){
            cout << "index out of range\n";
            shapesModified = true;
            cv.notify_all();
            return false;
        }

        /*if (shapes.erase(index) == 0) {
            cout << "index out of range\n";
            shapesModified = true;
            cv.notify_all();
            return false;
        }*/
        shapesModified = true;
        cv.notify_all();
        return true;
    }

    void drawAll(sf::RenderWindow& window,Font font) {
        std::unique_lock<std::mutex> lock(shapesMutex);
        cv.wait(lock, [this] { return shapesModified; });
        allDrowed = false;
        for (const auto& [index, shape] : shapes) {
            if (!shape->getValid())
                continue;
            try {
                if (shape) {
                    shape->draw(window, index, font);
                    //cout << "drowed"<<index<<endl;
                }
                else {
                    std::cerr << "Warning: Encountered a nullptr in BasicShapes. Skipping.\n";
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Exception during draw: " << e.what() << ". Skipping this shape.\n";
            }
            catch (...) {
                std::cerr << "Unknown exception during draw. Skipping this shape.\n";
            }
        }
        allDrowed = true;
        cv.notify_all();
    }

    std::shared_ptr<BasicShape> getBasicShape(int index) const {
        auto it = shapes.find(index);
        return (it != shapes.end()&&it->second->getValid()) ? it->second : nullptr;
    }
};