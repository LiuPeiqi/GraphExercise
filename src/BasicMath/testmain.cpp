#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

#include "BasicMath/Matrix.h"

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

