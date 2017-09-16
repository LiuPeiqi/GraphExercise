#include <array>
#include <algorithm>
#include <typeinfo>
#include <memory>
#include "gtest/gtest.h"

#include "BasicMath/Matrix.h"
using namespace lpq;
TEST(Transformation, Convert) {
	auto p3 = make_point(1.0f, 2.0f, 3.0f);
	auto& p2 = p3.ConvertTo<Point2Df>();
	p2.At<1>() = 3;
	EXPECT_EQ(p3, make_point(1.0f, 3.0f, 3.0f));
	auto& p1 = BasicPoint<float>::ConvertTo(p3);
	p1.At<0>() = 0;
	EXPECT_EQ(p3, make_point(0.0f, 3.0f, 3.0f));
	auto mat = ReferenceByMatrix(p3);
	mat.At(0, 2) = 0.0f;
	EXPECT_EQ(p3, make_point(0.0f, 3.0f, 0.0f));
	auto mat2 = MakeHomogeneousMatrix(p3);
	EXPECT_EQ(ReferenceByPoint<Point4D<float>>(mat2), make_point(0.0f, 3.0f, 0.0f, 1.0f));
}
#include "BasicMath/Transformation.h"
TEST(Transformation, Transofrmation) {
	auto p1 = make_point(1, 1, 1);
	auto* pp1 = &p1;
	Shift(pp1, pp1 + 1, make_point(-1, -1, -1), pp1);
	EXPECT_EQ(make_point(0, 0, 0), p1);
	auto shift = MakeShiftMatrix(make_point(1, 2, 3));
	int verify[]{ 1,0,0,1, 0,1,0,2, 0,0,1,3, 0,0,0,1 };
	auto ver_mat = Matrix<int>(4, 4, verify);
	EXPECT_EQ(ver_mat, shift);
	Shift(pp1, pp1 + 1, make_point(-1, -1, -1), pp1);
	Scale(pp1, pp1 + 1, make_point(0, 0, 0), make_point(2, 2, 2), pp1);
	EXPECT_EQ(p1, make_point(-2, -2, -2));
	auto scale = MakeScaleMatrix(make_point(2, 2, 2));
	int verify2[]{ 2,0,0,0, 0,2,0,0, 0,0,2,0, 0,0,0,1 };
	auto ver_mat2 = Matrix<int>(4, 4, verify2);
	EXPECT_EQ(ver_mat2, scale);
	auto rotate_x = RotationMatrix<double, double, RotateDirectory::XRotate>(1, true);
	auto rotate_y = RotationMatrix<double, double, RotateDirectory::YRotate>(1, true);
	auto rotate_z = RotationMatrix<double, double, RotateDirectory::ZRotate>(1, true);

}
