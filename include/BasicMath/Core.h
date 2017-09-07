#ifndef GRAPH_EXERCISE_BASIC_MATH_CORE_H
#define GRAPH_EXERCISE_BASIC_MATH_CORE_H
#include <type_traits>
namespace lpq {

	template<typename ...ValueType>
	class BasicPoint;

	template<>
	class BasicPoint<> {
	public:
		//using ValueType = void;
		enum { dimension = 0 };
		using _Derived = BasicPoint<>;
		constexpr BasicPoint() noexcept {}
		constexpr BasicPoint(const BasicPoint&) noexcept {}
		constexpr BasicPoint(BasicPoint&&) noexcept{}

		_Derived operator+ (const _Derived& right) const noexcept { return _Derived(); }
		_Derived& operator+= (const _Derived& right) noexcept { return *this; }
		_Derived operator- (const _Derived& right) const noexcept { return _Derived(); }
		_Derived& operator-= (const _Derived& right) noexcept { return *this; }
		template<typename ScalerType>
		_Derived operator+ (ScalerType scaler) const noexcept { return _Derived(); }
		template<typename ScalerType>
		_Derived& operator+= (ScalerType scaler) noexcept { return *this; }
		template<typename ScalerType>
		_Derived operator- (ScalerType scaler) const noexcept { return _Derived(); }
		template<typename ScalerType>
		_Derived& operator-= (ScalerType scaler) noexcept { return *this; }
		template<typename ScalerType>
		_Derived operator* (ScalerType scaler) const noexcept { return _Derived(); }
		template<typename ScalerType>
		_Derived& operator*= (ScalerType scaler) noexcept { return *this; }
		template<typename ScalerType>
		_Derived operator/ (ScalerType scaler) const noexcept { return _Derived(); }
		template<typename ScalerType>
		_Derived& operator/= (ScalerType scaler) noexcept { return *this; }
		constexpr bool operator==(const _Derived&) const noexcept { return true; }
		constexpr bool operator<(const _Derived&) const noexcept { return true; }
		constexpr bool operator>(const _Derived&) const noexcept { return true; }
		constexpr bool operator<=(const _Derived&) const noexcept { return true; }
		constexpr bool operator>=(const _Derived&) const noexcept { return true; }
		constexpr bool operator!=(const _Derived&) const noexcept { return true; }
		constexpr unsigned char dot(const _Derived&) const noexcept{ return 0; }
	};

	template<typename _This, typename ...Rest>
	class BasicPoint<_This, Rest...>
		: private BasicPoint<Rest...>{
		using _Base = BasicPoint<Rest...>;
		_Base& _GetRest() { return *this; }
		BasicPoint(_Base _base, _This _value) noexcept : _Base(_base), value(_value) {}
	public:
		enum { dimension = (1 + sizeof...(Rest)) };
		using ValueType = _This;
		using _Derived = BasicPoint<_This, Rest...>;
		_This value;
		BasicPoint() noexcept : _Base(), value{} {}
		BasicPoint(Rest ..._rest, _This _value) noexcept : _Base(_rest...), value(_value) {}
		BasicPoint(const _Derived&) noexcept = default;
		BasicPoint(_Derived&&) noexcept = default;
		_Derived& operator=(const _Derived&)noexcept = default;
		~BasicPoint() noexcept = default;
		const _Base& _GetRest() const { return *this; }
		template<size_t Index> typename std::enable_if<(Index + 1) == dimension, ValueType>::type& 
			at() noexcept { return value; }
		template<size_t Index> typename std::enable_if<(Index + 1) < dimension, typename _Base::ValueType>::type&
			at() noexcept { return _Base::at<Index>(); }
		template<size_t Index> ValueType at() const noexcept { return at<Index>(); }

		_Derived operator+ (const _Derived& right) const noexcept {
			return _Derived(_Base::operator+(right._GetRest()), value + right.value);
		}
		_Derived& operator+=(const _Derived& right) noexcept {
			value += right.value;
			_GetRest() += right._GetRest();
			return *this;
		}
		_Derived operator- (const _Derived& right) const noexcept {
			return _Derived(_Base::.operator-(right._GetRest()), value - right.value);
		}
		_Derived& operator-=(const _Derived& right) noexcept {
			value -= right.value;
			_GetRest() -= right._GetRest();
			return *this;
		}
		template<typename ScalerType>
		_Derived operator+ (ScalerType scaler) const noexcept {
			return _Derived(_Base::operator+(scaler), static_cast<_This>(value + scaler));
		}
		template<typename ScalerType>
		_Derived& operator+= (ScalerType scaler) noexcept {
			value = static_cast<_This>(value + scaler);
			_Base::operator+=(scaler);
			return *this;
		}
		template<typename ScalerType>
		_Derived operator- (ScalerType scaler) const noexcept {
			return _Derived(_Base::operator-(scaler), static_cast<_This>(value - scaler));
		}
		template<typename ScalerType>
		_Derived& operator-= (ScalerType scaler) noexcept {
			value = static_cast<_This>(value - scaler);
			_Base::operator-=(scaler);
			return *this;
		}
		template<typename ScalerType>
		_Derived operator* (ScalerType scaler) const noexcept {
			return _Derived(_Base::operator*(scaler), static_cast<_This>(value * scaler));
		}
		template<typename ScalerType>
		_Derived& operator*= (ScalerType scaler) noexcept {
			value = static_cast<_This>(value * scaler);
			_Base::operator*=(scaler);
			return *this;
		}
		template<typename ScalerType>
		_Derived operator/ (ScalerType scaler) const noexcept {
			return _Derived(_Base::operator/(scaler), static_cast<_This>(value / scaler));
		}
		template<typename ScalerType>
		_Derived& operator/= (ScalerType scaler) noexcept {
			value = static_cast<_This>(value / scaler);
			_Base::operator/=(scaler);
			return *this;
		}
		bool operator==(const _Derived& right) const noexcept { 
			return value == right.value && _Base::operator==(right._GetRest());
		}
		bool operator!=(const _Derived& right) const noexcept { return !operator==(right); }

		_This dot(const _Derived& right) const noexcept{
			return value * right.value + _Base::dot(right._GetRest());
		}
	};

	template<typename ValueType>
	using Point2D = BasicPoint<typename ValueType, typename ValueType>;
	template<typename ValueType>
	using Point3D = BasicPoint<ValueType, ValueType, ValueType>;
	template<typename ValueType>
	using Point4D = BasicPoint<ValueType, ValueType, ValueType, ValueType>;

	using Point2Di = Point2D<int>;
	using Point2Dl = Point2D<long long>;
	using Point2Df = Point2D<float>;
	using Point2Dd = Point2D<double>;

	using Point3Di = Point3D<int>;
	using Point3Dl = Point3D<long long>;
	using Point3Df = Point3D<float>;
	using Point3Dd = Point3D<double>;

	using PointRGB = Point3D<unsigned char>;
	using PointARGB = Point4D<unsigned char>;

	template<typename First, typename Second, typename ...Rest>
	constexpr bool _CHECK_ARGS_TYPES() {
		return std::is_same<First, Second>::value && _CHECK_ARGS_TYPES<Second, Rest...>();
	}
	template<typename Last>
	constexpr bool _CHECK_ARGS_TYPES() {
		return true;
	}
	template<typename ...ArgTypes>
	BasicPoint<ArgTypes...> make_point(ArgTypes... args) {
		static_assert(_CHECK_ARGS_TYPES<ArgTypes...>(), "Argements Types of Point is not same!");
		return BasicPoint<ArgTypes...>(args...);
	}

	template<typename ValueType>
	class Matrix {//alloc memory and delete

	};

	template<typename ValueType, typename OrgType>
	class MatrixView {// use the OrgType memory

	};
}
#endif // !GRAPH_EXERCISE_BASIC_MATH_CORE_H
