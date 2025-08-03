#pragma once
#include <string>



enum class DependsTypes {
    None,
    Parallel,
    Perpendicular,
    MedianPerpendicular,
    CircleCenter,
    Midpoint,
    Bisectrix,
    BelongsToLine,
    BelongsToCircle,
    IntersectionLineLine,
    IntersectionLineCircle,
    Circle3points,
    Circle2points, // center and onCircle
    Line2points
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


