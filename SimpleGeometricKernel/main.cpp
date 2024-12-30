#include <SFML/Graphics.hpp>
#include "BasicShape.h"
#include <iostream>
#include "WindowManager.h"
#include "GeometricOperations.h"
#include "global.h"

float global::size = 1.f;
float global::linesize = 1.f;

using namespace sf;

template <typename T, typename... Args>
std::shared_ptr<T> ms(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

int main()
{

    ShapeManager sm;


    Point2D p1 = { 0,0 };
    Point2D p2 = { 1,1 };
    Line line(p1, p2);
    Line line1(p1, p2);
    
    go::move(line1, 100, 0);
    go::move(p2, 100, 100);

    std::cout<<go::distance(p1, p2);

    


    sm.addBasicShape(line);
    sm.addBasicShape(line1);
    sm.addBasicShape(p1);
    sm.addBasicShape(p2);


    WindowManager window(sm);
    window.show();


   

    return 0;
}