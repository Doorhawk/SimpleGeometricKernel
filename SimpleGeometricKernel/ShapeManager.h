#pragma once
#include <vector>
#include "BasicShape.h"

class ShapeManager {
private:
    std::vector<std::shared_ptr<BasicShape>> BasicShapes;

public:
    void addBasicShape(const std::shared_ptr<BasicShape>& BasicShape) {
        BasicShapes.push_back(BasicShape);
    }

    void removeBasicShape(size_t index) {
        if (index < BasicShapes.size()) {
            BasicShapes.erase(BasicShapes.begin() + index);
        }
    }

    void drawAll(sf::RenderWindow& window) {
        for (const auto& BasicShape : BasicShapes) {
            BasicShape->draw(window);
        }
    }

    std::shared_ptr<BasicShape> getBasicShape(size_t index) {
        return index < BasicShapes.size() ? BasicShapes[index] : nullptr;
    }
};