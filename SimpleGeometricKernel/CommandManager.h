#pragma once
#include "ShapeManager.h"
#include "enums.h"



class CommandManager
{
	ShapeManager& shapeManager;
public:
	CommandManager(ShapeManager& shapeManager) :shapeManager(shapeManager) {}
	
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

	void addPoint(double x, double y) {
		shapeManager.addBasicShape(Point(x, y));
	}

	void addLine(double x, double y,double x1,double y1) {
		auto p1 = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x, y)));
		auto p2 = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x1, y1)));
		if (p1 && p2) {
			auto line2point = shapeManager.addBasicShape(Line());
			std::vector <std::weak_ptr<Depends>> parents = { p1,p2 };
			line2point->setParent(DependsTypes::Line2points, parents);
			line2point->addbasePoint(p1, p2);
		}
		else
			throw std::invalid_argument("Error in creation point for line.");
	}
	void addLine(int index1, int index2) {
        
		auto p1 = getPoint(index1);
		auto p2 = getPoint(index2);

		auto line2point = shapeManager.addBasicShape(Line());
		std::vector <std::weak_ptr<Depends>> parents = { p1,p2 };
		line2point->setParent(DependsTypes::Line2points, parents);
		line2point->addbasePoint(p1, p2);
        
	}

	void addCircle(double x, double y, double x1, double y1) {
		auto center = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x, y)));
		auto onCircle= std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x1, y1)));
		if (center && onCircle) {
			auto circle2points = shapeManager.addBasicShape(Circle(Point(0, 0), Point(10, 10)));
			std::vector <std::weak_ptr<Depends>> parents = { center,onCircle };
			circle2points->setParent(DependsTypes::Circle2points, parents);
			circle2points->addbasePoint(center,onCircle);
		}
		else
			throw std::invalid_argument("Error in creation point for circle ");
	}
	void addCircle(int index1, int index2) {
		auto center1 = getPoint(index1);
		auto onCircle1 = getPoint(index2);

		auto circle2points = shapeManager.addBasicShape(Circle(Point(0, 0), Point(10, 10)));
		std::vector <std::weak_ptr<Depends>> parents = { center1,onCircle1};
		circle2points->setParent(DependsTypes::Circle2points, parents);
		circle2points->addbasePoint(center1,onCircle1);
	}
	void addCircle3points(int index1, int index2, int index3) {
		auto point1 = getPoint(index1);
		auto point2 = getPoint(index2);
		auto point3 = getPoint(index3);

		auto circle3points = shapeManager.addBasicShape(Circle(Point(0,0), Point(10,10)));
		std::vector <std::weak_ptr<Depends>> parents = { point1,point2,point3 };
		circle3points->setParent(DependsTypes::Circle3points, parents);
		circle3points->addbasePoint(point1,point2,point3);
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
		std::vector <std::weak_ptr<Depends>> parents = { line,point1 };
		parallelLine->setParent(DependsTypes::Parallel, parents);
		parallelLine->addbasePoint(point1);

	}
	void addParallelLine(int lineIndex, int pointIndex) {
		auto line = getLine(lineIndex);
		auto point1 = getPoint(pointIndex);

		auto parallelLine = shapeManager.addBasicShape(Line());
		std::vector <std::weak_ptr<Depends>> parents = { line,point1 };
		parallelLine->setParent(DependsTypes::Parallel, parents);
		parallelLine->addbasePoint(point1);
	}

	void addPerpendicularLine(int lineIndex, double x, double y) {
		auto line = getLine(lineIndex);
		auto point1 = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x, y)));

		auto perpendicularLine = shapeManager.addBasicShape(Line());
		std::vector <std::weak_ptr<Depends>> parents = { line,point1 };
		perpendicularLine->setParent(DependsTypes::Perpendicular, parents);
		perpendicularLine->addbasePoint(point1);
	}
	void addPerpendicularLine(int lineIndex, int pointIndex) {
		
		auto line = getLine(lineIndex);
		auto point1 = getPoint(pointIndex);


		auto perpendicularLine = shapeManager.addBasicShape(Line());
		std::vector <std::weak_ptr<Depends>> parents = { line,point1 };
		perpendicularLine->setParent(DependsTypes::Perpendicular, parents);
		perpendicularLine->addbasePoint(point1);
	}

	void addMedianPerpendicular(double x, double y, double x1, double y1) {
		auto point = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x, y)));
		auto point1 = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(x1, y1)));

		auto linePoint1 = std::make_shared<Point>(Point(0, 0));
		auto linePoint2 = std::make_shared<Point>(Point(10, 10));

		auto medianPerpendicular = shapeManager.addBasicShape(Line());
		std::vector <std::weak_ptr<Depends>> parents = { point1, point };
		medianPerpendicular->setParent(DependsTypes::MedianPerpendicular, parents);
	}
	void addMedianPerpendicular(int index1, int index2) {
		
		auto point1 = getPoint(index1);
		auto point2 = getPoint(index2);
		
		auto linePoint1 = std::make_shared<Point>(Point(0, 0));
		auto linePoint2 = std::make_shared<Point>(Point(10, 10));

		auto medianPerpendicular = shapeManager.addBasicShape(Line());
		std::vector <std::weak_ptr<Depends>> parents = { point1, point2 };
		medianPerpendicular->setParent(DependsTypes::MedianPerpendicular, parents);
	}

	void addBisectrix(int index1, int index2, int index3) {
		auto point1 = getPoint(index1);
		auto point2 = getPoint(index2);
		auto point3 = getPoint(index3);

		auto bisectrix = shapeManager.addBasicShape(Line());
		std::vector <std::weak_ptr<Depends>> parents = { point1,point2,point3};
		bisectrix->setParent(DependsTypes::Bisectrix, parents);

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
			std::vector <std::weak_ptr<Depends>> parent = { line };
			point->setParent(DependsTypes::BelongsToLine, parent);
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
			std::vector <std::weak_ptr<Depends>> parent = { line };
			point->setParent(DependsTypes::BelongsToLine, parent);
		}
		else {
			throw std::invalid_argument("addPointBelong not for this shape type");
		}
	}

	

	void intersection(ShapeType type1, int index1, ShapeType type2, int index2) {
		if (type1 == st_line&& type2 == st_line) {
			auto line1 = getLine(index1);
			auto line2 = getLine(index2);

			auto inderPoint = std::dynamic_pointer_cast<Point>(shapeManager.addBasicShape(Point(0, 0)));

			std::vector <std::weak_ptr<Depends>> parents = { line1,line2 };
			inderPoint->setParent(DependsTypes::IntersectionLineLine, parents);
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
};
