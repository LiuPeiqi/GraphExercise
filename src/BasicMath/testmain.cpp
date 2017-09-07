#include <iostream>
#include <tuple>
using namespace std;

#include "BasicMath/Core.h"

template<typename _BackElem, typename ...ArgTypes>
void PrintPointImp(const lpq::BasicPoint<_BackElem, ArgTypes...>& point) {
	PrintPointImp<ArgTypes...>(point._GetRest());
	cout << "," << point.value;
}

template<typename Back>
void PrintPointImp(const lpq::BasicPoint<Back>& point) {
	cout << point.value;
}

template<typename PointType>
void PrintPoint(const PointType& point) {
	cout << "(";
	PrintPointImp(point);
	cout << ")" << endl;
}

int main() {
	auto sizeof_tuple = [](auto t) {cout << "sizeof:" << sizeof(t) << endl; };
	tuple<char> t1;
	//sizeof_tuple(t1);
	//tuple<short> t2;
	//sizeof_tuple(t2);
	//tuple<int> t3;
	//sizeof_tuple(t3);
	tuple<char, int> t4;
	std::get<1>(t4);
	//sizeof_tuple(t4);
	//tuple<int, char> t5;
	//sizeof_tuple(t5);
	//tuple<tuple<char, char, char>, tuple<char, char, char>> t6;
	//sizeof_tuple(t6);
	//tuple<tuple<char, char, char>, tuple<char, char, int>> t7;
	//sizeof_tuple(t7);
	lpq::Point2Di p1(1, 2);
	lpq::Point2Di p3(1, 2);
	lpq::Point3Di p2(1, 2, 3);
	auto p4 = p1 + p3;
	sizeof_tuple(p2);
	sizeof_tuple(p3);
	sizeof_tuple(p4);
	PrintPoint(p1);
	PrintPoint(p2);
	PrintPoint(p3);
	PrintPoint(p4);
	cout << "------" << endl;
	auto p5 = p1 + 2.0;
	PrintPoint(p5);
	auto p6 = p1 - 2.0;
	PrintPoint(p6);
	auto p7 = p1 * 2.0;
	PrintPoint(p7);
	auto p8 = p1 / 2.0;
	PrintPoint(p8);
	p1 += p1;
	PrintPoint(p1);
	p1 -= p1;
	PrintPoint(p1);
	p5 += -1;
	PrintPoint(p5);
	p5 -= -1;
	PrintPoint(p5);
	p5 *= -1;
	PrintPoint(p5);
	p5 /= -1;
	PrintPoint(p5);
	cout << p4.dot(p3) << endl;
	auto p = lpq::make_point(1, 2, 3);
	cout << (p1 == p3);
	return 0;
}
