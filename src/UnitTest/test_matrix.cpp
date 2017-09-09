#include <typeinfo>
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
