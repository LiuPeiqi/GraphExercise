#include <array>
#include <algorithm>
#include <typeinfo>
#include <memory>
#include "gtest/gtest.h"

#include "BasicMath/Matrix.h"

TEST(Matrix, Point)
{
	ASSERT_EQ(sizeof(lpq::Point2Di), 8);
	ASSERT_EQ(sizeof(lpq::Point2Dl), 16);
	ASSERT_EQ(sizeof(lpq::Point2Df), 8);
	ASSERT_EQ(sizeof(lpq::Point2Dd), 16);
	ASSERT_EQ(sizeof(lpq::Point3Di), 12);
	ASSERT_EQ(sizeof(lpq::Point3Dl), 24);
	ASSERT_EQ(sizeof(lpq::Point3Df), 12);
	ASSERT_EQ(sizeof(lpq::Point3Dd), 24);
	ASSERT_EQ(sizeof(lpq::PointRGB), 3);
	ASSERT_EQ(sizeof(lpq::PointRGBf), 12);
	ASSERT_EQ(sizeof(lpq::PointARGB), 4);
	ASSERT_EQ(sizeof(lpq::PointARGBf), 16);
	ASSERT_EQ(sizeof(lpq::PointRGB[3]), 9);
	ASSERT_EQ(sizeof(lpq::make_point('a', 'b', 'c', 'd', 'e', 'f', 'g')), 7);
	constexpr auto is_same = std::is_same<lpq::Point3Dd, decltype(lpq::make_point(1.0, 1.0, 1.0))>::value;
	ASSERT_TRUE(is_same);
	lpq::Point3Di p0;
	const lpq::Point3Di p1(1, 2, 3);
	const lpq::Point3Di p2(5, 6, 7);
	const lpq::Point3Di p3(6, 8, 10);
	const lpq::Point3Di p4(4, 4, 4);
	EXPECT_EQ(p3.At<0>(), 6);
	EXPECT_EQ(p3.At<1>(), 8);
	EXPECT_EQ(p3.At<2>(), 10);
	EXPECT_EQ(p1, p1 + p0);
	EXPECT_EQ(p1, p1 - p0);
	EXPECT_EQ(p0, p0 * 1);
	EXPECT_EQ(p1, p1 * 1);
	EXPECT_EQ(p0, p0 / 1);
	EXPECT_EQ(p1, p1 / 1);
	auto one = (p4 - p3 + p2 + p1) / 4;
	EXPECT_EQ(one, p4 / 4);
	EXPECT_EQ(one + 3.0f, p4);
	EXPECT_EQ(one, p4 - 3);
	one += 3;
	EXPECT_TRUE(p4 == one);
	one -= p4;
	EXPECT_TRUE(p0 == one);
	one = p4 / 4;
	EXPECT_TRUE(p1 != one);
	one = p4 / 4.5;
	EXPECT_TRUE(p0 == one);
	one = p4 / 4;
	one *= 100L;
	one /= 33.333;
	EXPECT_EQ(one / 3, p4 / 4);
	EXPECT_EQ(one.Dot(one), 27);
	EXPECT_EQ((p0 - p4).Dot(p1), -24);
	EXPECT_EQ((-p4).Dot(p1), -24);
	auto tow = (p4 - p3 + p2 + p1) / 2;
	std::swap(tow, one);
	EXPECT_EQ(one * 2, p4);
	tow = one;
	one.At<0>() = 1;
	one.At<1>() = 1;
	one.At<2>() = 1;
	EXPECT_EQ(one * 2, tow);
}

TEST(Matrix, MatrixIterator) {
	lpq::BasicMatrix<int> mat;
	EXPECT_TRUE(mat.Empty());
	int data[]{ 1,2,3,4,5,6 };
	mat.Alloc(3, 2, data);
	lpq::BasicMatrix<int> mat2(mat.Clone());
	int data_t[] = { 1,3,5,2,4,6 };
	lpq::BasicMatrix<int> mat3(2, 3, data_t);
	mat3.Transpose();
	auto mat_iter = mat.BeginByRow();
	auto mat3_iter = mat3.BeginByRow();
	for (size_t i = 0; i < 6; ++i) {
		EXPECT_EQ(*mat_iter++, *mat3_iter++);
	}
	--mat_iter;
	--mat3_iter;
	for (size_t i = 0; i < 6; ++i) {
		EXPECT_EQ(*mat_iter--, *mat3_iter--);
	}
	EXPECT_TRUE(mat3 == mat);
	mat3.Arrange();
	EXPECT_TRUE(mat3 == mat);
	mat_iter = mat.BeginByRow();
	mat3_iter = mat3.BeginByRow();
	for (size_t i = 0; i < 6; ++i) {
		EXPECT_EQ(*mat_iter++, *mat3_iter++);
	}
	auto mat4 = mat2.Transpose().Clone();
	for (size_t i = 0; i < 2; ++i) {
		for (size_t j = 0; j < 3; ++j) {
			EXPECT_EQ(mat4.At(i, j), mat2.At(i, j));
		}
	}
	auto mat2_iter = mat2.BeginByRow();
	auto end = mat.EndByColumn();
	for (mat_iter = mat.BeginByColumn(); mat_iter != end; ++mat_iter) {
		EXPECT_EQ(*mat_iter, *mat2_iter);
		++mat2_iter;
	}
	std::array<int, 6> verify;
	std::copy(mat.BeginByRow(), mat.EndByRow(), verify.begin());
	for (size_t i = 0; i < 6; ++i) {
		EXPECT_EQ(verify[i], data[i]);
	}
	std::copy(mat.BeginByColumn(), mat.EndByColumn(), verify.begin());
	for (size_t i = 0; i < 6; ++i) {
		EXPECT_EQ(verify[i], data_t[i]);
	}
	mat_iter = mat.BeginByRow();
	++mat_iter;
	EXPECT_LT(mat.BeginByRow(), mat_iter);
	EXPECT_EQ(mat.BeginByRow(), (mat_iter + 3) - 4);
	mat_iter -= 1;
	EXPECT_EQ(mat.BeginByRow(), mat_iter);
	mat_iter += 6;
	EXPECT_EQ(mat.EndByRow(), mat_iter);
	EXPECT_EQ(mat.EndByColumn(), mat_iter);
	EXPECT_EQ(6, mat.EndByRow() - mat.BeginByRow());

	auto mat6 = lpq::BasicMatrix<double>(1, 6);
	auto mat6_iter = mat6.BeginByRow();
	for (size_t i = 0; i < 6; ++i) {
		*mat6_iter++ = i + 1;
	}
	mat6.Transpose();
	auto row_iter = mat6.BeginByRow();
	auto col_iter = mat6.BeginByColumn();
	for (size_t i = 0; i < 6; ++i) {
		EXPECT_EQ(*row_iter++, *col_iter++);
	}
	mat6.Arrange();
	row_iter = mat6.BeginByRow();
	col_iter = mat6.BeginByColumn();
	for (size_t i = 0; i < 6; ++i) {
		EXPECT_EQ(*row_iter++, *col_iter++);
	}

}
