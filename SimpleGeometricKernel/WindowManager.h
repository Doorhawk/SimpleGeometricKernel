#pragma once
#include <SFML/Graphics.hpp>
#include "ShapeManager.h"
#include "global.h"
using namespace sf;

class WindowManager {
private:
    ShapeManager& shapeManager;
    RenderWindow window;
    View view;
    bool isDragging;
    Vector2f prevMousePos;
    Font font;

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
            global::size *= zoomFactor;
            view.zoom(zoomFactor);
        }

    }
    void drawAxes() {

        float length = view.getSize().x+abs(view.getCenter().x)*2+ abs(view.getCenter().y)*2;
        sf::Color color = sf::Color(0,0,0,50);
        float tickSize = 10.0f*global::size;
        float tickSpacing = 10.0f;

        sf::VertexArray axes(sf::Lines);

        // Ось X
        axes.append(sf::Vertex(sf::Vector2f(-length / 2, 0), color)); // Левая граница
        axes.append(sf::Vertex(sf::Vector2f(length / 2, 0), color));  // Правая граница

        // Ось Y
        axes.append(sf::Vertex(sf::Vector2f(0, -length / 2), color)); // Верхняя граница
        axes.append(sf::Vertex(sf::Vector2f(0, length / 2), color));  // Нижняя граница

        //// Добавляем штрихи для оси X
        //for (float x = -length / 2; x <= length / 2; x += tickSpacing) {
        //    if (x != 0) { // Пропускаем центр
        //        axes.append(sf::Vertex(sf::Vector2f(x, -tickSize / 2), color));
        //        axes.append(sf::Vertex(sf::Vector2f(x, tickSize / 2), color));
        //    }
        //}

        //// Добавляем штрихи для оси Y
        //for (float y = -length / 2; y <= length / 2; y += tickSpacing) {
        //    if (y != 0) { // Пропускаем центр
        //        axes.append(sf::Vertex(sf::Vector2f(-tickSize / 2, y), color));
        //        axes.append(sf::Vertex(sf::Vector2f(tickSize / 2, y), color));
        //    }
        //}

        // Рисуем массив вершин
        window.draw(axes);
    }
public:
    WindowManager(ShapeManager& shapeManager) : shapeManager(shapeManager) {
        window.create(VideoMode(800, 800), "Scene");
        view.setSize(800.f, -800.f); // Инвертируем ось Y
        //view.zoom(1./80);
        view.setCenter(0.f, 0.f); // Центрируем вид
        isDragging = false;
        if (!font.loadFromFile("arialmt.ttf")) {
            std::cout << "font errror";
        }
    }

    void show() {
        while (window.isOpen()) {
            
            Event event;
            while (window.pollEvent(event)) {
                controls(event);
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

            shapeManager.drawAll(window,font);
            drawAxes();

            window.display();
        }
    }
};