#pragma once
#include <string>



enum class DependsTypes {
    None,
    Parallel,
    Perpendicular,
    MedianPerpendicular,
    Bisectrix,
    BelongsToLine,
    IntersectionLineLine,
    Circle3points,
    Circle2points // center and onCircle
};



enum class wMode {
    cameraMove,
    figureMove
};

enum ShapeType {
    st_point = 0,
    st_line,
    st_circle
};


