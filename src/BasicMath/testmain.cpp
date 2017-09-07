#include <iostream>
#include <tuple>
using namespace std;

#include "BasicMath/Core.h"

template<size_t I>
size_t F() {
	return F<I + 1>() + F<I + 2>();
}

template<>
size_t F<5>() { return 1; }

template<>
size_t F<6>() { return 1; }

int main() {
	cout << F<3>() << endl;
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
	auto x = p1.at<0>();
	return 0;
}
