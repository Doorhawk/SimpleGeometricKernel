#include "GeometricOperations.h"
#include "unittest.h"




TEST_GROUP(GeometricOperations, go_distance_p_p) {
	Point p1(0, 0);
	Point p2(1, 1);
	double dis = go::distance(p1, p2);
	EQVAL_EPS(dis, sqrt(2),go::getPrecision());

	p1 = { 1,1 };
	p2 = { 1,1 };
	dis = go::distance(p1, p2);
	EQVAL_EPS(dis,0, go::getPrecision());
}
TEST_GROUP(GeometricOperations, go_distance_l_p) {
	Point p(2, 2);
	Line l(Point(0, 2), Point(2, 0));
	double dis = go::distance(p,l);
	EQVAL_EPS(dis, sqrt(2),go::getPrecision());
	dis = go::distance(l, p);
	EQVAL_EPS(dis, sqrt(2), go::getPrecision());
	p = { 1,1 };
	dis = go::distance(l, p);
	EQVAL_EPS(dis, 0, go::getPrecision());
}
TEST_GROUP(GeometricOperations, go_intersection_l_l) {
	Line l1(Point(0, 0), Point(0, 1));
	Line l2(Point(1, 0), Point(1, 1));

	CHECK(go::findIntersection(l1, l2).empty());

	l1 = Line(Point(-1, 1), Point(1, -1));
	l2 = Line(Point(1, 1), Point(-1, -1));

	auto inter = go::findIntersection(l1, l2);
	CHECK(!inter.empty());
	CHECK(inter.size()==1);
	EQVAL_EPS(inter[0].getX(), 0, go::getPrecision());
	EQVAL_EPS(inter[0].getY(), 0, go::getPrecision());

}
TEST_GROUP(GeometricOperations, go_distance_c_p) {
	Point p(2, 2);
	Circle c(Point(0, 0), Point(2, 2));
	EQVAL_EPS(go::distance(p, c), 0, go::getPrecision());
	EQVAL_EPS(go::distance(c, p), 0, go::getPrecision());

	p = { 3,3 };
	EQVAL_EPS(go::distance(c, p), sqrt(2), go::getPrecision());
	EQVAL_EPS(go::distance(p, c), sqrt(2), go::getPrecision());

	p = { 1,1 };
	EQVAL_EPS(go::distance(c, p), sqrt(2), go::getPrecision());
	EQVAL_EPS(go::distance(p, c), sqrt(2), go::getPrecision());
}
TEST_GROUP(GeometricOperations, go_intersection_l_c) {
	Line l1(Point(0, 0), Point(0, 1));
	Circle c(Point(2, 0), Point(3,0));

	CHECK(go::findIntersection(l1, c).empty());
	CHECK(go::findIntersection(c, l1).empty());


	c = Circle(Point(1, 0), Point(0, 0));

	auto inter1 = go::findIntersection(l1, c);
	auto inter2 = go::findIntersection(c, l1);

	CHECK(!inter1.empty());
	CHECK(!inter2.empty());

	CHECK(inter1.size() == 1);
	CHECK(inter2.size() == 1);

	EQVAL_EPS(inter1[0].getX(), 0, go::getPrecision());
	EQVAL_EPS(inter1[0].getY(), 0, go::getPrecision());
	EQVAL_EPS(inter2[0].getX(), 0, go::getPrecision());
	EQVAL_EPS(inter2[0].getY(), 0, go::getPrecision());

	c = Circle(Point(0, 0), Point(1, 0));

	inter1 = go::findIntersection(l1, c);
	inter2 = go::findIntersection(c, l1);

	CHECK(!inter1.empty());
	CHECK(!inter2.empty());

	CHECK(inter1.size() == 2);
	CHECK(inter2.size() == 2);

	EQVAL_EPS(inter1[0].getX(), 0, go::getPrecision());
	EQVAL_EPS(inter1[0].getY(), 1, go::getPrecision());
	EQVAL_EPS(inter2[0].getX(), 0, go::getPrecision());
	EQVAL_EPS(inter2[0].getY(), 1, go::getPrecision());
	EQVAL_EPS(inter1[1].getX(), 0, go::getPrecision());
	EQVAL_EPS(inter1[1].getY(), -1, go::getPrecision());
	EQVAL_EPS(inter2[1].getX(), 0, go::getPrecision());
	EQVAL_EPS(inter2[1].getY(), -1, go::getPrecision());

}
TEST_GROUP(GeometricOperations, go_middle_point) {
	Point p1(-1, -1);
	Point p2(1, 1);
	Point mid = go::findMiddle(p1, p2);
	EQVAL_EPS(mid.getX(), 0, go::getPrecision());
	EQVAL_EPS(mid.getY(), 0, go::getPrecision());

	mid = go::findMiddle(p2, p1);
	EQVAL_EPS(mid.getX(), 0, go::getPrecision());
	EQVAL_EPS(mid.getY(), 0, go::getPrecision());
}
TEST_GROUP(GeometricOperations, go_angle_3point) {
	Point p1(0, 1);
	Point p2(0, 0);
	Point p3(1, 0);
	double angle = go::findAngle(p1, p2, p3);
	EQVAL_EPS(angle, acos(-1)/2, go::getPrecision());

	angle = go::findAngle(p2, p1, p3);
	EQVAL_EPS(angle, acos(-1) / 4, go::getPrecision());
	
	p1 = Point(0, sqrt(3));
	p2 = Point(0, 0);
	p3 = Point(1, 0);

	angle = go::findAngle(p2, p1, p3);
	EQVAL_EPS(angle, acos(-1) / 6, go::getPrecision());

	angle = go::findAngle(p2, p3, p1);
	EQVAL_EPS(angle, acos(-1) / 3, go::getPrecision());
}
TEST_DATA_GROUP(GeometricOperations, go_precision, (std::vector<double>{1, 1e-10,1e10}), double prec) {
	double oldprec = go::getPrecision();

	go::setPrecision(prec);
	double newprec = go::getPrecision();
	go::setPrecision(oldprec);

	EQVAL(newprec, prec);
}
TEST_GROUP(GeometricOperations, go_precision_expect) {
	EXPECT_THROW(go::setPrecision(0), std::invalid_argument);
	EXPECT_THROW(go::setPrecision(-1), std::invalid_argument);
}
