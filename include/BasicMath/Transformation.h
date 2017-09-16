#ifndef GRAPH_EXERCHIS_BASIC_MATH_TRANSFORMATION_H
#define GRAPH_EXERCHIS_BASIC_MATH_TRANSFORMATION_H
#include <cmath>

#include "Matrix.h"
namespace lpq {
	template<typename PointType>
	void Shift(const PointType* beg, const PointType* end, const PointType& shift, PointType* dest) {
		for (; beg != end; ++beg) {
			*dest++ = *beg + shift;
		}
	}
	template<typename PointType>
	Matrix<typename PointType::ValueType> MakeShiftMatrix(const PointType& shift) {
		static_assert(_CHECK_POINTS_ARGS_TYPES<PointType>(), "Argements Types of Point is not same!");
		size_t rank = shift.dimension + 1;
		auto mat = Matrix<typename PointType::ValueType>::Identity(rank, rank);
		typename PointType::ValueType const * value_iter = reinterpret_cast<typename PointType::ValueType const *>(&shift);
		auto iter = mat.BeginByColumn(shift.dimension);
		for (size_t i = 0; i < rank - 1; ++i) {
			*iter = *value_iter;
			++iter;
			++value_iter;
		}
		return std::move(mat);
	}

	template<typename PointType>
	void Scale(const PointType* beg, const PointType* end, const PointType& ref_point, const PointType& scale, PointType* dest) {
		auto origin = ref_point;
		Shift(beg, end, -origin, dest);
		auto dest_end = dest + (end - beg);
		for (auto iter = dest; iter < dest_end; ++iter) {
			iter->Scale(scale);
		}
		Shift(dest, dest_end, origin, dest);
	}
	template<typename PointType>
	Matrix<typename PointType::ValueType> MakeScaleMatrix(const PointType& scale) {
		static_assert(_CHECK_POINTS_ARGS_TYPES<PointType>(), "Argements Types of Point is not same!");
		size_t rank = scale.dimension + 1;
		auto mat = Matrix<typename PointType::ValueType>::Identity(rank, rank);
		typename PointType::ValueType const* value_iter = reinterpret_cast<typename PointType::ValueType const*>(&scale);
		auto iter = mat.BeginByRow();
		for (size_t i = 0; i < rank - 1; ++i) {
			*iter = *value_iter;
			iter += (rank + 1);
			++value_iter;
		}
		return std::move(mat);
	}

	enum RotateDirectory {
		XRotate = 0x1,
		YRotate = 0x2,
		ZRotate = 0x4
	};
	template<typename ValueType, typename RadianType, RotateDirectory which>
	Matrix<ValueType> RotationMatrix(RadianType alpha, bool homogeneous) {
		auto cos_alpha = static_cast<RadianType>(std::cos(alpha));
		auto sin_alpha = static_cast<RadianType>(std::sin(alpha));
		std::array<RadianType, 4> cache{ cos_alpha, sin_alpha, -sin_alpha, cos_alpha };
		auto cache_iter = cache.data();
		auto mat = Matrix<ValueType>();
		size_t rank = 0x7;
		constexpr int mask = 0x7 ^ which;//X == 0001b; Y == 0010b; Z == 0100b;
		if (homogeneous) {
			mat = Matrix<ValueType>::Identity(4, 4);
			rank |= 0x8;
		}
		else {
			mat = Matrix<ValueType>::Identity(3, 3);
		}
		auto iter = mat._data();
		for (size_t i = 1; i & rank; i <<= 1) {
			for (size_t j = 1; j & rank; j <<= 1) {
				if ((i&mask) && (j&mask)) {
					*iter = *cache_iter++;
				}
				++iter;
			}
		}
		return std::move(mat);
	}

}
#endif // !GRAPH_EXERCHIS_BASIC_MATH_TRANSFORMATION_H
