#include <SFML/Graphics.hpp>
#include "BasicShape.h"
#include <iostream>
#include "WindowManager.h"
#include "GeometricOperations.h"
#include "global.h"

float global::size = 1.f;

using namespace sf;

template <typename T, typename... Args>
std::shared_ptr<T> ms(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

const double PI = acos(-1);

int main()
{

    ShapeManager sm;


    Point l1(0, 0);
    Point l2(100,100);

    Point p2 = { -0,100 };
  
    Line line(l1,l2);

    double a = 0;

    Line line1;
    

    std::cout << go::distance(line, p2) << std::endl;

    //sm.addBasicShape(line1);
    sm.addBasicShape(line);
    
    sm.addBasicShape(l1);
    sm.addBasicShape(p2);
    sm.addBasicShape(l2);
    


    WindowManager window(sm);
    window.show();


   

    return 0;
}