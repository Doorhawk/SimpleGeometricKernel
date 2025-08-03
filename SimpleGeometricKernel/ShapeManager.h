#pragma once
#include <vector>
#include "BasicShape.h"
#include <unordered_map>
#include <mutex>
#include <condition_variable>
#include "WindowManager.h"
#include "Logger.h"



class ShapeManager {
private:
    std::unordered_map<int, std::shared_ptr<BasicShape>> shapes;
    std::mutex shapesMutex;
    std::condition_variable cv;
    bool allDrowed = true;
    bool shapesModified = true;
    int nextIndex = 0;
    friend class WindowManager;
    Color defaultColor = Color::Black;
public:
    template <typename T, typename = std::enable_if_t<std::is_base_of_v<BasicShape, T>>>
    std::shared_ptr<BasicShape> addBasicShape(const T& shape) {
        LOG_G_DEBUG("addBasicShape() - start");
        shapesModified = false;
        std::unique_lock<std::mutex> lock(shapesMutex);
        cv.wait(lock, [this] { return allDrowed; });

        auto ptr = std::make_shared<T>(shape);
        if (ptr) {
            LOG_G_INFO("Shape added successfully, index = " + std::to_string(nextIndex));
            shapes[nextIndex] = ptr;
            shapes[nextIndex]->setIndex(nextIndex);
            shapes[nextIndex]->setColor(defaultColor);
            shapesModified = true;
            cv.notify_all();
            return shapes[nextIndex++];
        }
        else {
            LOG_G_ERROR("Failed to add shape: nullptr created");
            shapesModified = true;
            cv.notify_all();
            return nullptr;
        }
    }

    template <typename T, typename = std::enable_if_t<std::is_base_of_v<BasicShape, T>>>
    std::shared_ptr<BasicShape> addBasicShape(std::shared_ptr<T> shape) {
        LOG_G_DEBUG("addBasicShape(shared_ptr) - start");
        shapesModified = false;
        std::unique_lock<std::mutex> lock(shapesMutex);
        cv.wait(lock, [this] { return allDrowed; });

        if (shape) {
            LOG_G_INFO("Shape added successfully, index = " + std::to_string(nextIndex));
            shapes[nextIndex] = shape;
            shapes[nextIndex]->setIndex(nextIndex);
            shapes[nextIndex]->setColor(defaultColor);
            shapesModified = true;
            cv.notify_all();
            return shapes[nextIndex++];
        }
        else {
            LOG_G_ERROR("Failed to add shape: received nullptr");
            shapesModified = true;
            cv.notify_all();
            return nullptr;
        }
    }

    void removeAllBasicShape() {
        LOG_G_INFO("Removing all shapes");
        shapesModified = false;
        std::unique_lock<std::mutex> lock(shapesMutex);
        cv.wait(lock, [this] { return allDrowed; });

        shapes.clear();
        nextIndex = 0;
        shapesModified = true;
        cv.notify_all();
    }

    bool removeBasicShape(size_t index) {
        shapesModified = false;
        std::unique_lock<std::mutex> lock(shapesMutex);
        cv.wait(lock, [this] { return allDrowed; });

        LOG_G_DEBUG("Attempting to remove shape with index = " + std::to_string(index));
        try {
            auto shape = getBasicShape(index);

            removeChildrenRecursively(shape);

            shapes.erase(index);
            LOG_G_INFO("Shape removed successfully, index = " + std::to_string(index));
        }
        catch (...) {
            LOG_G_WARNING("Failed to remove shape: index " + std::to_string(index) + " out of range");
            shapesModified = true;
            cv.notify_all();
            return false;
        }

        shapesModified = true;
        cv.notify_all();
        return true;
    }
    

    void removeChildrenRecursively(const std::shared_ptr<Depends>& shape) {
        // Копируем список детей, чтобы избежать изменения вектора во время удаления
        auto childrenCopy = shape->getChildren();

        for (const auto& weakChild : childrenCopy) {
            if (auto child = weakChild.lock()) {
                removeChildrenRecursively(child);  // Рекурсивно удаляем детей
                shapes.erase(child->getIndex());  // Удаляем ребёнка из ShapeManager
            }
        }
    }
    void drawAll(sf::RenderWindow& window, Font font) {
        std::unique_lock<std::mutex> lock(shapesMutex);
        cv.wait(lock, [this] { return shapesModified; });

        allDrowed = false;
        for (const auto& [index, shape] : shapes) {
            if (!shape->getValid()) continue;
            try {
                if (shape) {
                    shape->draw(window, index, font);
                }
                else {
                    LOG_G_WARNING("Encountered a nullptr in BasicShapes. Skipping.");
                }
            }
            catch (const std::exception& e) {
                LOG_G_ERROR("Exception during draw: " + std::string(e.what()) + ". Skipping.");
            }
            catch (...) {
                LOG_G_ERROR("Unknown exception during draw. Skipping.");
            }
        }
        allDrowed = true;
        cv.notify_all();
    }

    std::shared_ptr<BasicShape> getBasicShape(int index) const {
        LOG_G_DEBUG("Get shape, index = " + std::to_string(index));
        auto it = shapes.find(index);
        if (it != shapes.end() && it->second->getValid()) {
            LOG_G_DEBUG("Shape found, index = " + std::to_string(index));
            return it->second;
        }
        else {
            LOG_G_WARNING("Shape not found, index = " + std::to_string(index) + " out of range");
            throw std::invalid_argument("Out of range");
        }
    }

    void setDefaultColor(Color color) {
        defaultColor = color;
    }
    void setAllColor(Color color) {
        for (const auto& [index, shape] : shapes) {
            if (!shape->getValid()) continue;
            try {
                if (shape) {
                    shape->setColor(color);
                }
                else {
                    LOG_G_WARNING("cant set new color");
                }
            }
            catch (...) {
                LOG_G_ERROR("Unknown exception during set color. Skipping.");
            }
        }
    }
};
