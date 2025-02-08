#pragma once

#include <string>
enum class DependsTypes {
    None,
    Parallel,
    Perpendicular,
    MedianPerpendicular,
    Bisectrix,
    BelongsToLine,
    IntersectionLineLine
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

