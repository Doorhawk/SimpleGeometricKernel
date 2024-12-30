#include <SFML/Graphics.hpp>
#include "BasicShape.h"
#include <iostream>
#include "WindowManager.h"
#include "GeometricOperations.h"
using namespace sf;

template <typename T, typename... Args>
std::shared_ptr<T> ms(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

int main()
{

    ShapeManager sm;

    auto point1 = ms<Point2D>(0,0);
    auto point2 = ms<Point2D>(0,0);
    sm.addBasicShape(point1);
    sm.addBasicShape(point2);

    go::move(*point1, 3, 4);
    std::cout<<go::distance(*point1, *point2);

    






    WindowManager window(sm);
    window.show();


   

    return 0;
}