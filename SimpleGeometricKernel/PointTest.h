#include "BasicShape.h"
#include "unittest.h"

TEST_GROUP(point, point_define_default) {
	Point p;
	EQVAL(p.getX(), 0);
	EQVAL(p.getY(), 0);
}
TEST_DATA_GROUP(point, point_define_double, (std::vector<double>{-1, 0, 1, 1 / 3.}), double val) {
	Point p(val, val);
	EQVAL(p.getX(), val);
	EQVAL(p.getY(), val);
}
TEST_GROUP(point, point_operator_eqveqv) {
	Point p1(1 / 3., 1 / 3.);
	Point p2(1 / 3., 1 / 3.);
	CHECK(p1 == p2);
}
TEST_GROUP(point, point_operator_noteqv) {
	Point p1(1, 1.);
	Point p2(0, 1);
	CHECK(p1 != p2);

	p1 = Point(1, 1);
	p2 = Point(1, 0);
	CHECK(p1 != p2);
}
TEST_GROUP(point, point_define_point) {
	Point p1(10, -1/3.);
	Point p2(p1);
	EQVAL(p1, p2);
}
TEST_GROUP(point, point_define_vector) {
	Vector vec(10, -1/3.);
	Point p(vec);
	EQVAL(p.getX(), vec.getX());
	EQVAL(p.getY(), vec.getY());
}
TEST_GROUP(point, point_move) {
	Point p(1, 1);
	p.move(1 / 3., -1);
	EQVAL(p.getX(), 1+1/3.);
	EQVAL(p.getY(), 1-1);
}
TEST_GROUP(point, point_rotate) {
	Point p(0, 0);
	Point center(1, 0);
	p.rotate(center, acos(-1) / 2);
	p.rotate(center, acos(-1) / 2);
	p.rotate(center, acos(-1) / 2);
	p.rotate(center, acos(-1) / 2);
	EQVAL_EPS(p.getX(), 0, go::getPrecision());
	EQVAL_EPS(p.getY(), 0, go::getPrecision());

	p = Point(1, 0);
	p.rotate(Point(0,0), acos(-1.)/6);
	double answ = sqrt(3.0) / 2;
	EQVAL_EPS(p.getX(), answ, go::getPrecision());
	EQVAL_EPS(p.getY(), 0.5, go::getPrecision());
}
TEST_GROUP(point, point_operator_eqv) {
	Point p(1 / 3., -1 / 3);
	Point p1 = p;
	EQVAL(p.getX(), p1.getX());
	EQVAL(p.getY(), p1.getY());
}
TEST_GROUP(point, point_operator_plus_point) {
	Point p1(1 / 3., -1 / 3);
	Point p2(-1 / 3., 1 / 3);
	Point p3 = p1 + p2;
	EQVAL(p3.getX(), 0);
	EQVAL(p3.getY(), 0);
}
TEST_GROUP(point, point_operator_minus_point) {
	Point p1(1 / 3., 1 / 3);
	Point p2(1 / 3., 1 / 3);
	Point p3 = p1 - p2;
	EQVAL(p3.getX(), 0);
	EQVAL(p3.getY(), 0);
}
TEST_GROUP(point, point_operator_plus_vec) {
	Point p1(1.28, -1 / 3);
	Vector vec(-1.28, 1 / 3);
	Point p3 = p1 + vec;
	EQVAL(p3.getX(), 0);
	EQVAL(p3.getY(), 0);
}
TEST_GROUP(point, point_operator_minus_vec) {
	Point p1(2.3, 1 / 3);
	Vector vec(2.3, 1 / 3);
	Point p3 = p1 - vec;
	EQVAL(p3.getX(), 0);
	EQVAL(p3.getY(), 0);
}
TEST_GROUP(point, point_operator_multipy_p_sc) {
	Point p1(2,3);
	EQVAL((p1*2).getX(),4);
	EQVAL_EPS((p1*3.3).getY(),9.9,go::getPrecision());
}
TEST_GROUP(point, point_setpos) {
	Point p(0, 0);
	p.setPos(1, 1);
	EQVAL(p.getX(), 1);
	EQVAL(p.getY(), 1);
	p.setPos(1/2., 1/3.);
	EQVAL(p.getX(), 1 / 2.);
	EQVAL(p.getY(), 1 / 3.);

}
TEST_GROUP(point, point_update_none) {
	
	Point point(0, 0);
	CHECK(point.getDependsType() == DependsTypes::None);
	point.update();
	EQVAL(point, Point(0, 0));
}
TEST_GROUP(point, point_belongs_to_line) {

	auto point = make_shared<Point>(Point(1, 0));
	auto line = make_shared<Line>(Line(Point(0, 1), Point(0, -1)));
	point->setParent(DependsTypes::BelongsToLine, { line });
	CHECK(point->getDependsType() == DependsTypes::BelongsToLine);
	EQVAL(*point, Point(0, 0));
	line->move(0, 1);
	EQVAL(*point, Point(0, 1));
	line->rotate(Point(0, 0), -acos(-1) / 2);
	EQVAL_EPS(point->getX(), 1, go::getPrecision());
	EQVAL_EPS(point->getY(), 0, go::getPrecision());

	EXPECT_THROW_MSG(point->setParent(DependsTypes::BelongsToLine, {}),
		std::invalid_argument, "update point with BelongsToLine - error: parent.empty() = true");
	EXPECT_THROW_MSG(point->setParent(DependsTypes::BelongsToLine, { line,line }),
		std::invalid_argument, "update point with BelongsToLine - error: parent.size() != 1");
	auto point2 = make_shared<Point>(Point(1, 0));
	EXPECT_THROW_MSG(point->setParent(DependsTypes::BelongsToLine, { point2 }),
		std::invalid_argument, "Parent of point not line ");
	
}
TEST_GROUP(point, point_intersection_line_line) {

	auto point = make_shared<Point>(Point(0, 0));
	auto line1 = make_shared<Line>(Line(Point(0, 1), Point(0, -1)));
	auto line2 = make_shared<Line>(Line(Point(1, 0), Point(-1, 0)));
	point->setParent(DependsTypes::IntersectionLineLine, { line1,line2 });
	CHECK(point->getDependsType() == DependsTypes::IntersectionLineLine);
	EQVAL(*point, Point(0, 0));
	line1->move(2, 0);
	EQVAL(*point, Point(2, 0));

	EXPECT_THROW_MSG(point->setParent(DependsTypes::IntersectionLineLine, {}),
		std::invalid_argument, "update point with IntersectionLineLine - error: parent.empty() = true");
	EXPECT_THROW_MSG(point->setParent(DependsTypes::IntersectionLineLine, { line1 }),
		std::invalid_argument, "update point with IntersectionLineLine - error: parent.size() != 2");
	auto point2 = make_shared<Point>(Point(1, 0));
	EXPECT_THROW_MSG(point->setParent(DependsTypes::IntersectionLineLine, { point2,point2 }),
		std::invalid_argument, "Parent of point not line ");


	auto point1 = make_shared<Point>(Point(0, 0));
	auto line11 = make_shared<Line>(Line(Point(0, 0), Point(0, 1)));
	auto line21 = make_shared<Line>(Line(Point(1, 0), Point(1, 1)));
	point1->setParent(DependsTypes::IntersectionLineLine, { line11,line21 });
	CHECK(!point1->getValid());
}
TEST_GROUP(point, point_belongs_to_circle) {

	/*auto point = make_shared<Point>(Point(0, 0));
	auto line1 = make_shared<Line>(Line(Point(0, 1), Point(0, -1)));
	auto line2 = make_shared<Line>(Line(Point(1, 0), Point(-1, 0)));
	point->setParent(DependsTypes::IntersectionLineLine, { line1,line2 });
	CHECK(point->getDependsType() == DependsTypes::IntersectionLineLine);
	EQVAL(*point, Point(0, 0));
	line1->move(2, 0);
	EQVAL(*point, Point(2, 0));

	TEST_EXPECT_THROW_MSG(point->setParent(DependsTypes::IntersectionLineLine, {}),
		std::invalid_argument, "update point with IntersectionLineLine - error: parent.empty() = true");
	TEST_EXPECT_THROW_MSG(point->setParent(DependsTypes::IntersectionLineLine, { line1 }),
		std::invalid_argument, "update point with IntersectionLineLine - error: parent.size() != 2");
	auto point2 = make_shared<Point>(Point(1, 0));
	TEST_EXPECT_THROW_MSG(point->setParent(DependsTypes::IntersectionLineLine, { point2,point2 }),
		std::invalid_argument, "Parent of point not line ");


	auto point1 = make_shared<Point>(Point(0, 0));
	auto line11 = make_shared<Line>(Line(Point(0, 0), Point(0, 1)));
	auto line21 = make_shared<Line>(Line(Point(1, 0), Point(1, 1)));
	point1->setParent(DependsTypes::IntersectionLineLine, { line11,line21 });
	CHECK(!point1->getValid());*/
}