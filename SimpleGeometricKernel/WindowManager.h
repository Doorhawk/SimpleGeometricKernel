#pragma once
#include <SFML/Graphics.hpp>
#include "ShapeManager.h"
#include "global.h"
#include "enums.h"
#include "Logger.h"
#include <iostream>
using namespace sf;



class WindowManager {
private:
    ShapeManager& shapeManager;
    RenderWindow window;
    View view;
    bool isDragging;
    Vector2f prevMousePos;
    Font font;
    wMode mode = wMode::figureMove;
    std::shared_ptr<BasicShape> selectedShape = nullptr; // Выбранная фигура
    Point startPose;
    int wHeight;
    int wWidth;
    std::atomic<bool>& isRunning;
    std::string infAboutSelectedShape = "";
    Color startColor = Color::Black;


    void controls(Event event) {

        if (event.type == Event::Closed) {
            isRunning = false;
            LOG_G_INFO("window is close");
            window.close();
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::M) {
                mode = wMode::figureMove;
                LOG_G_INFO("figureMove mod ON in window");

            }
            else if (event.key.code == sf::Keyboard::C) {
                mode = wMode::cameraMove;
                LOG_G_INFO("cameraMove mod ON in window");
            }
        }
        if(mode == wMode::cameraMove){
            // Удержание мыши для перемещения
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                isDragging = true;
                prevMousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);
            }
            if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
                isDragging = false;
            }
        }
        else if(mode == wMode::figureMove) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (selectedShape) {
                    selectedShape->setColor(startColor);
                    selectedShape = nullptr;
                    infAboutSelectedShape = "";
                }
                LOG_G_DEBUG("LBM Clicked");
                sf::Vector2f mousePos = getMouseWorldPosition();
                double minDistance = std::numeric_limits<double>::max();
                std::shared_ptr<BasicShape> PointMin = nullptr;
                // Ищем ближайшую фигуру к месту клика
                float threshold = view.getSize().x * 0.02f;
                for (const auto& [index, shape] : shapeManager.shapes) {
                    if (shape && shape->getType() == st_point) { // Проверяем только точки
                        double distance = getDistance(shape, mousePos.x, mousePos.y);
                        if (distance < minDistance && distance < threshold) {
                            selectedShape = shape;
                            minDistance = distance;
                            LOG_G_DEBUG("found the point you clicked on, index = "+to_string(index));
                        }
                    }
                }

                // Если точка не найдена, ищем ближайшую фигуру
                if (!selectedShape) {
                    minDistance = std::numeric_limits<double>::max(); // Сброс минимальной дистанции
                    for (const auto& [index, shape] : shapeManager.shapes) {
                        if (shape) {
                            double distance = getDistance(shape, mousePos.x, mousePos.y);
                            if (distance < minDistance && distance < threshold) {
                                selectedShape = shape;
                                minDistance = distance;
                                LOG_G_DEBUG("found the shape you clicked on, index = " + to_string(index));
                            }
                        }
                    }
                }

                // Если нашли фигуру, сохраняем её начальную позицию
                if (selectedShape) {
                    LOG_G_DEBUG(std::format("starting position = ({}, {})", mousePos.x, mousePos.y));
                    startPose = Point(mousePos.x, mousePos.y);
                    infAboutSelectedShape = selectedShape->printInf();
                    isDragging = true;
                    startColor = selectedShape->getColor();
                    selectedShape->setColor(Color::Blue);
                }
            }

            // Отпускание кнопки мыши
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                LOG_G_DEBUG("LBM Released");
                isDragging = false; // Останавливаем перемещение
                //selectedShape->setColor(Color::Black);
                //selectedShape = nullptr;
            }
        }

        // Масштабирование через колёсико мыши
        if (event.type == Event::MouseWheelScrolled) {
            float zoomFactor = (event.mouseWheelScroll.delta > 0) ? 0.9f : 1.1f;
            global::size *= zoomFactor;
            view.zoom(zoomFactor);
        }

    }
    double getDistance(std::shared_ptr<BasicShape> shape,float x, float y) {
        if (shape->getType() == st_point) {
            auto point = dynamic_pointer_cast<Point>(shape);
            if (point) {
                return go::distance(*point, Point(x, y))/1.2;
            }
        } else if (shape->getType() == st_line) {
            auto line = dynamic_pointer_cast<Line>(shape);
            if (line) {
                return go::distance(line, Point(x, y));
            }
        }
        else if (shape->getType() == st_circle) {
            auto circle = dynamic_pointer_cast<Circle>(shape);
            if (circle) {
                return go::distance(circle, Point(x, y));
            }
        }
        return 10000;
    }
    sf::Vector2f getMouseWorldPosition() {
        // Получаем текущую позицию мыши в оконных координатах
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        // Преобразуем оконные координаты в мировые
        return window.mapPixelToCoords(pixelPos);
    }
    void moveFigure() {
        if (isDragging && mode == wMode::cameraMove) {
            Vector2f currentMousePos = window.mapPixelToCoords(Mouse::getPosition(window), view);
            Vector2f offset = prevMousePos - currentMousePos;
            view.move(offset);
        }
        else if (isDragging && selectedShape) {
            sf::Vector2f mousePos = getMouseWorldPosition();
            double dx = mousePos.x - startPose.getX();
            double dy = mousePos.y - startPose.getY();

            selectedShape->move(dx, dy);

            // Обновляем начальную позицию для следующего шага
            startPose.move(dx, dy);
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

        window.draw(axes);

        string mod_name = "C/M to swirch:\n";
        switch (mode)  
        {
        case wMode::cameraMove:
            mod_name += "mode: camera";
            break;
        case wMode::figureMove:
            mod_name += "mode: figure";
            break;
        default:
            break;
        }


        window.setView(window.getDefaultView()); // Сбрасываем представление
        sf::Text text;
        text.setFont(font);                     // Устанавливаем шрифт
        text.setString(mod_name+"\n\n"+ infAboutSelectedShape);        // Устанавливаем текст
        text.setCharacterSize(14);              // Размер текста в пикселях
        text.setFillColor(sf::Color::Black);    // Цвет текста
        text.setScale(1, 1);
        text.setPosition(20,20);
        window.draw(text);
        window.setView(view);
    }
public:
    WindowManager(ShapeManager& shapeManager, std::atomic<bool>& isRunning) : shapeManager(shapeManager),isRunning(isRunning), startPose(0,0) {
        wHeight = 800;
        wWidth = 800;
        window.create(VideoMode(800, 800), "Scene");
        window.setFramerateLimit(60);
        view.setSize(800, -800); // Инвертируем ось Y
        view.setCenter(0.f, 0.f); // Центрируем вид
        global::size *= 0.1;
        view.zoom(0.1);
        isDragging = false;
        LOG_G_INFO("window is open");
        if (!font.loadFromFile("arialmt.ttf")) {
            std::cout << "font errror";
            LOG_G_ERROR("Failed to load font");
        }
    }

    void show() {
        while (window.isOpen()) {
            
            if (!isRunning)
                window.close();

            Event event;
            while (window.pollEvent(event)) {
                controls(event);
            }
            // Перемещение вида при удержании мыши
            
            // Устанавливаем обновлённый вид
            window.setView(view);

            window.clear(Color::White);

            moveFigure();
            shapeManager.drawAll(window,font);
            drawAxes();

            window.display();
        }
    }
    void setMode(wMode newmode) {
        mode = newmode;
    }
};