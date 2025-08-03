#pragma once
#include "ShapeManager.h"
#include "enums.h"



class CommandManager
{
	ShapeManager& shapeManager;
public:
	CommandManager(ShapeManager& shapeManager) :shapeManager(shapeManager) {}
	
	int addPoint(double x, double y) {
		return shapeManager.addBasicShape(Point(x, y))->getIndex();
	}

	void setDefaultColor(Color newColor) {
		shapeManager.setDefaultColor(newColor);
	}
	void setColor(Color newColor,int index){
		auto shape = shapeManager.getBasicShape(index);
		if (!shape) {
			throw std::invalid_argument("Invalid indices in move command");
		}
		shape->setColor(newColor);
	}
	void setAllColor(Color newColor) {
		shapeManager.setAllColor(newColor);
	}

	

	tuple<int,int,int> addLine(double x, double y,double x1,double y1,bool isSegment = false) {
		int i1 = 0, i2 = 0, i3 = 0;
		auto p1 = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x, y)));
		auto p2 = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x1, y1)));
		i1 = p1->getIndex();
		i2 = p2->getIndex();
		if (p1 && p2) {
			auto line2point = shapeManager.addBasicShape(Line(isSegment));
			i3 = line2point->getIndex();
			line2point->setParent(DependsTypes::Line2points, { p1,p2 });
			line2point->addbasePoint(p1, p2);
		}
		else
			throw std::invalid_argument("Error in creation point for line.");
		return { i1,i2,i3 };
		
	}
	int addLine(int index1, int index2, bool isSegment = false) {
        
		auto p1 = getPoint(index1);
		auto p2 = getPoint(index2);

		auto line2point = shapeManager.addBasicShape(Line(isSegment));
		line2point->setParent(DependsTypes::Line2points, { p1,p2 });
		line2point->addbasePoint(p1, p2);
		return line2point->getIndex();
	}
	
	// return index <point1 point2 circle>
	tuple<int, int, int> addCircle(double x, double y, double x1, double y1) {
		
		int i1 = 0, i2 = 0, i3 = 0;
		auto center = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x, y)));
		auto onCircle= std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x1, y1)));
		i1 = center->getIndex();
		i2 = onCircle->getIndex();
		if (center && onCircle) {
			auto circle2points = shapeManager.addBasicShape(Circle(Point(0, 0), Point(10, 10)));
			i3 = circle2points->getIndex();
			circle2points->setParent(DependsTypes::Circle2points, { center,onCircle });
			circle2points->addbasePoint(center,onCircle);
		}
		else
			throw std::invalid_argument("Error in creation point for circle ");
		return { i1,i2,i3 };
	}
	int addCircle(int index1, int index2) {
		auto center1 = getPoint(index1);
		auto onCircle1 = getPoint(index2);

		auto circle2points = shapeManager.addBasicShape(Circle(Point(0, 0), Point(10, 10)));
		circle2points->setParent(DependsTypes::Circle2points, { center1,onCircle1 });
		circle2points->addbasePoint(center1,onCircle1);

		return circle2points->getIndex();
	}
	int addCircle3points(int index1, int index2, int index3) {
		auto point1 = getPoint(index1);
		auto point2 = getPoint(index2);
		auto point3 = getPoint(index3);

		auto circle3points = shapeManager.addBasicShape(Circle(Point(0,0), Point(10,10)));
		circle3points->setParent(DependsTypes::Circle3points, { point1,point2,point3 });
		circle3points->addbasePoint(point1,point2,point3);

		return circle3points->getIndex();
	}

	bool deleteShape(int index) {
		return shapeManager.removeBasicShape(index);
	}
	void deleteAllShapes() {
		shapeManager.removeAllBasicShape();
	}

	void moveShape(int index, double x, double y) {
        
        auto shape = shapeManager.getBasicShape(index);
        if (!shape) {
			throw std::invalid_argument("Invalid indices in move command");
        }
        shape->move(x, y);
	}

	void rotateShape(int shapeIndex, double cx,double cy,double angle ) {
		auto shape = shapeManager.getBasicShape(shapeIndex);
		if (!shape) {
			throw std::invalid_argument("Invalid indices in rotate command");
		}
		shape->rotate(Point(cx,cy), angle);
	}
	void rotateShape(int shapeIndex, int centerPointIndex, double angle) {
		auto shape = shapeManager.getBasicShape(shapeIndex);
		if (!shape) {
			throw std::invalid_argument("Invalid shape indices in rotate command");
		}
		auto centerPoint = getPoint(centerPointIndex);
		shape->rotate(*centerPoint, angle);
	}

	void addParallelLine(int lineIndex,double x, double y) {
		auto line = getLine(lineIndex);
		auto point1 = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x, y)));
		
		auto parallelLine = shapeManager.addBasicShape(Line());
		parallelLine->setParent(DependsTypes::Parallel, { line,point1 });
		parallelLine->addbasePoint(point1);

	}
	void addParallelLine(int lineIndex, int pointIndex) {
		auto line = getLine(lineIndex);
		auto point1 = getPoint(pointIndex);

		auto parallelLine = shapeManager.addBasicShape(Line());
		parallelLine->setParent(DependsTypes::Parallel, { line,point1 });
		parallelLine->addbasePoint(point1);
	}

	void addPerpendicularLine(int lineIndex, double x, double y) {
		auto line = getLine(lineIndex);
		auto point1 = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x, y)));

		auto perpendicularLine = shapeManager.addBasicShape(Line());
		perpendicularLine->setParent(DependsTypes::Perpendicular, { line,point1 });
		perpendicularLine->addbasePoint(point1);
	}
	void addPerpendicularLine(int lineIndex, int pointIndex) {
		
		auto line = getLine(lineIndex);
		auto point1 = getPoint(pointIndex);


		auto perpendicularLine = shapeManager.addBasicShape(Line());
		perpendicularLine->setParent(DependsTypes::Perpendicular, { line,point1 });
		perpendicularLine->addbasePoint(point1);
	}

	void addMedianPerpendicular(double x, double y, double x1, double y1) {
		
		auto point = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x, y)));
		auto point1 = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x1, y1)));
		
		auto linePoint1 = std::make_shared<Point>(Point(0, 0));
		auto linePoint2 = std::make_shared<Point>(Point(10, 10));

		auto medianPerpendicular = shapeManager.addBasicShape(Line());
		medianPerpendicular->setParent(DependsTypes::MedianPerpendicular, { point1, point });

		auto midpoint = shapeManager.addBasicShape(Point());
		midpoint->setParent(DependsTypes::Midpoint, { point,point1 });
	}
	void addMedianPerpendicular(int index1, int index2) {
		
		auto point1 = getPoint(index1);
		auto point2 = getPoint(index2);
		
		auto linePoint1 = std::make_shared<Point>(Point(0, 0));
		auto linePoint2 = std::make_shared<Point>(Point(10, 10));

		auto medianPerpendicular = shapeManager.addBasicShape(Line());
		medianPerpendicular->setParent(DependsTypes::MedianPerpendicular, { point1, point2 });

		auto midpoint = shapeManager.addBasicShape(Point());
		midpoint->setParent(DependsTypes::Midpoint, { point1,point2 });
	}

	void addMidpoint(double x, double y, double x1, double y1) {
		auto point = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x, y)));
		auto point1 = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x1, y1)));

		auto midpoint = shapeManager.addBasicShape(Point());
		midpoint->setParent(DependsTypes::Midpoint, { point,point1 });
	}
	void addMidpoint(int index1, int index2) {
		auto point = getPoint(index1);
		auto point1 = getPoint(index2);

		auto midpoint = shapeManager.addBasicShape(Point());
		midpoint->setParent(DependsTypes::Midpoint, { point,point1 });
	}

	void addBisectrix(int index1, int index2, int index3) {
		auto point1 = getPoint(index1);
		auto point2 = getPoint(index2);
		auto point3 = getPoint(index3);

		auto bisectrix = shapeManager.addBasicShape(Line());
		bisectrix->setParent(DependsTypes::Bisectrix, { point1,point2,point3 });

	}

	void addPointBelong(int shapeIndex,double x, double y) {
		auto point = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x, y)));
		auto shape = shapeManager.getBasicShape(shapeIndex);
		if (!shape) {
			throw std::invalid_argument("Invalid indices " + to_string(shapeIndex));
		}

		if (shape->getType() == st_line) {
			auto line = std::dynamic_pointer_cast<Line>(shape);
			if (!line) {
				throw std::invalid_argument("Shape " + to_string(shapeIndex) + " not line");
			}
			point->setParent(DependsTypes::BelongsToLine, { line });
		}
		else if (shape->getType() == st_circle) {
			auto circle = std::dynamic_pointer_cast<Circle>(shape);
			if (!circle) {
				throw std::invalid_argument("Shape " + to_string(shapeIndex) + " not circle");
			}
			point->setParent(DependsTypes::BelongsToCircle, { circle });
		}
		else {
			throw std::invalid_argument("addPointBelong not for this shape type");
		}
	}
	void addPointBelong(int shapeIndex, int pointIndex) {
		auto point = getPoint(pointIndex);
		auto shape = shapeManager.getBasicShape(shapeIndex);
		if (!shape) {
			throw std::invalid_argument("Invalid indices " + to_string(shapeIndex));
		}

		if (shape->getType() == st_line) {
			auto line = std::dynamic_pointer_cast<Line>(shape);
			if (!line) {
				throw std::invalid_argument("Shape " + to_string(shapeIndex) + " not line");
			}
			point->setParent(DependsTypes::BelongsToLine, { line });
		}
		if (shape->getType() == st_circle) {
			auto circle = std::dynamic_pointer_cast<Circle>(shape);
			if (!circle) {
				throw std::invalid_argument("Shape " + to_string(shapeIndex) + " not circle");
			}
			point->setParent(DependsTypes::BelongsToCircle, { circle });
		}
		else {
			throw std::invalid_argument("addPointBelong not for this shape type");
		}
	}

	void addCircleCenter(int index) {
		auto circle = getCircle(index);

		auto point = shapeManager.addBasicShape(Point());
		point->setParent(DependsTypes::CircleCenter, { circle });
	}
	

	void intersection(ShapeType type1, int index1, ShapeType type2, int index2) {
		if (type1 == st_line&& type2 == st_line) {
			auto line1 = getLine(index1);
			auto line2 = getLine(index2);

			auto interPoint = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(0, 0)));

			interPoint->setParent(DependsTypes::IntersectionLineLine, { line1,line2 });
		}
		else if (type1 == st_line && type2 == st_circle) {
			auto line = getLine(index1);
			auto circle = getCircle(index2);

			auto interPoint1 = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(0, 0)));
			auto interPoint2 = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(0, 0)));

			interPoint1->setParent(DependsTypes::IntersectionLineCircle, { circle,line }, 1);
			interPoint2->setParent(DependsTypes::IntersectionLineCircle, { circle,line }, -1);
		}
		else if (type1 == st_circle && type2 == st_line) {
			auto line = getLine(index2);
			auto circle = getCircle(index1);

			auto interPoint1 = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(0, 0)));
			auto interPoint2 = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(0, 0)));

			interPoint1->setParent(DependsTypes::IntersectionLineCircle, { circle,line }, 1);
			interPoint2->setParent(DependsTypes::IntersectionLineCircle, { circle,line }, -1);
		}
		
		else {
			throw std::invalid_argument("not for this type");
		}
	}

	void getInf(int shapeIndex) {
		auto shape = shapeManager.getBasicShape(shapeIndex);
		if (!shape) {
			throw std::invalid_argument("Invalid indices " + to_string(shapeIndex));
		}
		std::cout<<shape->printInf();
	}
private:
	std::shared_ptr<Point> getPoint(int index) {
		auto shape = shapeManager.getBasicShape(index);
		if (!shape) {
			throw std::invalid_argument("Invalid indices "+to_string(index));
			return nullptr;
		}
		auto point = dynamic_pointer_cast<Point>(shape);
		if (!point) {
			throw std::invalid_argument("Shape "+to_string(index)+" not point");
			return nullptr;
		}
		return point;
	}
	std::shared_ptr<Line> getLine(int index) {
		auto shape = shapeManager.getBasicShape(index);
		if (!shape) {
			throw std::invalid_argument("Invalid indices " + to_string(index));
			return nullptr;
		}
		auto line = dynamic_pointer_cast<Line>(shape);
		if (!line) {
			throw std::invalid_argument("Shape " + to_string(index) + " not line");
			return nullptr;
		}
		return line;
	}
	std::shared_ptr<Circle> getCircle(int index) {
		auto shape = shapeManager.getBasicShape(index);
		if (!shape) {
			throw std::invalid_argument("Invalid indices " + to_string(index));
			return nullptr;
		}
		auto circle = dynamic_pointer_cast<Circle>(shape);
		if (!circle) {
			throw std::invalid_argument("Shape " + to_string(index) + " not circle");
			return nullptr;
		}
		return circle;
	}
};
