#pragma once
#include <SFML/Graphics.hpp>
#include "ShapeManager.h"
using namespace sf;

class WindowManager {
private:
    ShapeManager& shapeManager;
    RenderWindow window;
    View view;
    bool isDragging;
    Vector2f prevMousePos;

    void controls(Event event) {

        if (event.type == Event::Closed) {
            window.close();
        }

        // Удержание мыши для перемещения
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            isDragging = true;
            prevMousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);
        }
        if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
            isDragging = false;
        }

        // Масштабирование через колёсико мыши
        if (event.type == Event::MouseWheelScrolled) {
            float zoomFactor = (event.mouseWheelScroll.delta > 0) ? 0.9f : 1.1f;
            view.zoom(zoomFactor);
        }

    }

public:
    WindowManager(ShapeManager& shapeManager) : shapeManager(shapeManager) {
        window.create(VideoMode(800, 600), "Scene");
        view.setSize(800.f, -600.f); // Инвертируем ось Y
        view.setCenter(0.f, 0.f); // Центрируем вид
        isDragging = false;
    }

    void show() {
        while (window.isOpen()) {
            
            Event event;
            while (window.pollEvent(event)) {
                controls(event);;
            }
            // Перемещение вида при удержании мыши
            if (isDragging) {
                Vector2f currentMousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);
                Vector2f offset = prevMousePos - currentMousePos;
                view.move(offset);
            }
            // Устанавливаем обновлённый вид
            window.setView(view);

            window.clear(Color::White);

            shapeManager.drawAll(window);

            window.display();
        }
    }
};