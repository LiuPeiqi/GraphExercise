#ifndef GRAPH_EXERCISE_BASIC_MATH_CORE_H
#define GRAPH_EXERCISE_BASIC_MATH_CORE_H
#include <type_traits>
namespace lpq {
	template<typename ...ValueType>
	class BasicPoint;

	template<>
	class BasicPoint<> {
	public:
		enum { dimension = 0 };
		using _Derived = BasicPoint<>;
		constexpr BasicPoint() noexcept {}
		constexpr BasicPoint(const BasicPoint&) noexcept {}
		//constexpr BasicPoint(BasicPoint&&) noexcept{}

		_Derived operator+ (const _Derived& right) noexcept { return _Derived(); }
		_Derived operator- (const _Derived& right) noexcept { return _Derived(); }

	};

	template<typename _This, typename ...Rest>
	class BasicPoint<_This, Rest...>
		: private BasicPoint<Rest...>{
		using _Base = BasicPoint<Rest...>;
		_Base& _GetRest() { return *this; }
		const _Base& _GetRest() const { return *this; }
		BasicPoint(_This _value, _Base _base) noexcept : _Base(_base), value(_value) {}
	public:
		enum { dimension = (1 + sizeof...(Rest)) };
		using _Derived = BasicPoint<_This, Rest...>;
		_This value;
		BasicPoint() noexcept : _Base(), value{} {}
		BasicPoint(Rest ..._rest, _This _value) noexcept : _Base(_rest...), value(_value) {}
		BasicPoint(const _Derived&) noexcept = default;
		BasicPoint(_Derived&&) noexcept = default;
		_Derived& operator=(const _Derived&)noexcept = default;
		~BasicPoint() noexcept = default;
		template<size_t Index> typename std::enable_if<(Index + 1) == dimension, _This>::type at() { return value; }
		template<size_t Index> typename std::enable_if<(Index + 1) < dimension, decltype(_Base::at<Index>())>::type at(){ 
			return _GetRest().at<Index>(); 
		}
		//template<> _This& at<1>() { return value; }

		_Derived operator+ (const _Derived& right) noexcept {
			return _Derived(value + right.value, _GetRest().operator+(right._GetRest()));
		}

		_Derived operator- (const _Derived& right) noexcept {
			return _Derived(value - right.value, _GetRest().operator-(right._GetRest()));
		}

	};

	template<typename ValueType>
	using Point2D = BasicPoint<typename ValueType, typename ValueType>;

	template<typename ValueType>
	using Point3D = BasicPoint<ValueType, ValueType, ValueType>;

	using Point2Di = Point2D<int>;
	using Point2Dl = Point2D<long long>;
	using Point2Df = Point2D<float>;
	using Point2Dd = Point2D<double>;

	using Point3Di = Point3D<int>;
	using Point3Dl = Point3D<long long>;
	using Point3Df = Point3D<float>;
	using Point3Dd = Point3D<double>;

	using PointRGB = Point3D<unsigned char>;
	using PointARGB = BasicPoint<unsigned char, unsigned char, unsigned char, unsigned char>;
}
#endif // !GRAPH_EXERCISE_BASIC_MATH_CORE_H
