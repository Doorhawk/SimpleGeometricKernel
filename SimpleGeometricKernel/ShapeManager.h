#pragma once
#include <vector>
#include "BasicShape.h"

class ShapeManager {
private:
    std::vector<std::shared_ptr<BasicShape>> BasicShapes;

public:
    template <typename T, typename = std::enable_if_t<std::is_base_of_v<BasicShape, T>>>
    void addBasicShape(const T& shape) {
        BasicShapes.push_back(std::make_shared<T>(shape)); // Создаём shared_ptr и добавляем
    }

    void removeBasicShape(size_t index) {
        if (index < BasicShapes.size()) {
            BasicShapes.erase(BasicShapes.begin() + index);
        }
    }

    void drawAll(sf::RenderWindow& window,Font font) {
        int i = 0;
        for (const auto& BasicShape : BasicShapes) {
            BasicShape->draw(window,i++,font);
        }
    }

    std::shared_ptr<BasicShape> getBasicShape(size_t index) {
        return index < BasicShapes.size() ? BasicShapes[index] : nullptr;
    }
};