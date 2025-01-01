#pragma once
#include <vector>
#include "BasicShape.h"
#include <unordered_map>

class ShapeManager {
private:
    std::unordered_map<int, std::shared_ptr<BasicShape>> shapes; // Хранение фигур по их индексам
    int nextIndex = 0; // Индекс для следующей фигуры
public:
    template <typename T, typename = std::enable_if_t<std::is_base_of_v<BasicShape, T>>>
    int addBasicShape(const T& shape) {
        auto ptr = std::make_shared<T>(shape);
        if (ptr) {
            shapes[nextIndex] = ptr; 
            return nextIndex++; 
        }
        else {
            std::cerr << "Error: Attempting to add an invalid shape.\n";
            return -1;
        }
    }

    void removeBasicShape(size_t index) {
        if (shapes.erase(index) == 0) {
            cout << "index out of range\n";
        }
    }

    void drawAll(sf::RenderWindow& window,Font font) {
        for (const auto& [index, shape] : shapes) {
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
    }

    std::shared_ptr<BasicShape> getBasicShape(int index) const {
        auto it = shapes.find(index);
        return (it != shapes.end()) ? it->second : nullptr;
    }
};