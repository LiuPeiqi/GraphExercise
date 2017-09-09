#ifndef GRAPH_EXERCISE_BASIC_MATH_CORE_H
#define GRAPH_EXERCISE_BASIC_MATH_CORE_H
#include <cassert>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>
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
		_Derived& operator=(const _Derived&)noexcept { return *this; }

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
		_Derived operator- () const noexcept { return _Derived(); }
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
		constexpr unsigned char Dot(const _Derived&) const noexcept{ return 0; }
		void swap(_Derived&) noexcept {}
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
			At() noexcept { return value; }
		template<size_t Index> typename std::enable_if<(Index + 1) < dimension, typename _Base::ValueType>::type&
			At() noexcept { return _Base::At<Index>(); }
		template<size_t Index> ValueType At() const noexcept { return const_cast<_Derived*>(this)->At<Index>(); }

		_Derived operator+ (const _Derived& right) const noexcept {
			return _Derived(_Base::operator+(right._GetRest()), value + right.value);
		}
		_Derived& operator+=(const _Derived& right) noexcept {
			value += right.value;
			_GetRest() += right._GetRest();
			return *this;
		}
		_Derived operator- (const _Derived& right) const noexcept {
			return _Derived(_Base::operator-(right._GetRest()), value - right.value);
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
		_Derived operator- () const noexcept {
			return _Derived(_GetRest().operator-(), -value);
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

		_This Dot(const _Derived& right) const noexcept{
			return value * right.value + _Base::Dot(right._GetRest());
		}
		void swap(_Derived& other) noexcept { std::swap(value, other.value); _Base::swap(other._GetRest()); }
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
	using PointRGBf = Point3D<float> ;
	using PointARGB = Point4D<unsigned char>;
	using PointARGBf = Point4D<float>;

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
	class BasicMatrix {//if not change org data ptr, it will not delete memory;
	public:
		BasicMatrix():data(nullptr), alloc_addr(nullptr), row(0), col(0), col_step(0){}
		BasicMatrix(size_t row, size_t column, ValueType* data_addr) : 
			data(data_addr), alloc_addr(nullptr), row(row), col(column), col_step(1) {}
		BasicMatrix(size_t row, size_t column) :BasicMatrix(row, column, new ValueType[row*column]) { alloc_addr = data; }
		BasicMatrix(const BasicMatrix& other) :BasicMatrix(other.row, other.col){/*copy at pos*/}
		BasicMatrix(BasicMatrix&& other):alloc_addr(nullptr) noexcept { swap(other); }
		~BasicMatrix() { delete[] alloc_addr; }
		void swap(BasicMatrix& right) noexcept{
			std::swap(data, right.data);
			std::swap(alloc_addr, right.alloc_addr);
			std::swap(row, right.row);
			std::swap(col, right.col);
			std::swap(col_step, right.col_step);
		}

		bool Empty() const noexcept { return 0 == row || 0 == col; }

		BasicMatrix& Transpose() {
			if (Empty()) { return *this; }
			col_step = col_step == 1 ? row : 1;
			std::swap(row, col);
			return *this;
		}
		BasicMatrix& Arrange() {
			if (Empty()) { return *this; }
			if (1 == col_step) { return *this; }
			ValueTypePtr new_data = new ValueType[row * col];
			col_step = 1;
			std::swap(row, col);
			ValueTypePtr iter = new_data;
			for (size_t i = 0; i < col; ++i) {
				ValueTypePtr org_it = data + i;
				for (size_t j = 0; j < row; ++j) {
					std::swap(*iter, *org_it);
					++iter;
					org_it += col;
				}
			}
			std::swap(new_data, data);
			delete[] new_data;
			return *this;
		}

		ValueType& At(size_t row, size_t column) noexcept  {
			std::assert((row < this->row) && (column < this->col));
			size_t step = this->col;
			if (1 != col_step) {
				std::swap(row, column);
				step = this->row;
			}
			return data[row*step + column];
		}
		const ValueType& At(size_t row, size_t column) const noexcept { return const_cast<const ValueType&>(At(row, column)); }
		
		template<typename ValueType> class IteratorImp;
		template<typename ValueType>
		class Iterator :public std::iterator<std::random_access_iterator_tag, ValueType> {
		private:
			friend class BasicMatrix<ValueType>;
			std::unique_ptr<IteratorImp<ValueType>> _iterator;
			Iterator(IteratorImp<ValueType>* inter_iter):_iterator(inter_iter){}
		public:
			using _Self = Iterator<ValueType>;
			Iterator(const _Self &right):
				_iterator(right._iterator.make_clone_ptr()){}
			Iterator(_Self && right) { std::swap(_iterator, right._iterator); }
			_Self& operator=(_Self tmp) { std::swap(iterator, tmp._iterator); return *this; }
			_Self& operator++() { _iterator->operator++(); return *this; }
			_Self& operator--() { _iterator->operator--(); return *this; }
			_Self operator++(int) { auto tmp = *this; _iterator->operator++(); return tmp; }
			_Self operator--(int) { auto tmp = *this; _iterator->operator--(); return tmp; }
			reference operator*() const { return _iterator->operator*(); }
			pointer operator->() const { return _iterator->operator->(); }
			bool operator==(const _Self& right) const { return _iterator->operator==(right._iterator); }
			bool operator!=(const _Self& right) const { return _iterator->operator!=(right._iterator); }
			bool operator<(const _Self& right) const { return _iterator->operator<(right._iterator); }
			bool operator>=(const _Self& right) const { return _iterator->operator>=(right._iterator); }
			bool operator>(const _Self& right) const { return _iterator->operator>(right._iterator); }
			bool operator<=(const _Self& right) const { return _iterator->operator<=(right._iterator); }
			reference operator[](difference_type n) const { return _iterator->operator[](n); }
			_Self& operator+=(difference_type n) { _iterator->operator+=(n); return *this; }
			_Self& operator-=(difference_type n) { _iterator->operator-=(n); return *this; }
			_Self operator+(difference_type n) const { _Self tmp(_iterator->make_clone_ptr()); tmp._iterator->operator+=(n); return tmp; }
			_Self operator-(difference_type n) const { return operator+(-n); }
			difference_type operator-(const _Self& before) { return _iterator->operator-(before._iterator); }
		};
		using iterator = Iterator<ValueType>;
		iterator RowBegin() const {
			if (1 == col_step) {
				return iterator(new IteratorImp<ValueType>(data));
			}
			else {
				return iterator(new TransposeIteratorImp<ValueType>(data, data + row*col, data, col));
			}
		}
		iterator RowEnd()const {
			if (1 == col_step) {
				return iterator(new IteratorImp<ValueType>(data + row*col));
			}
			else {
				return iterator(new TransposeIteratorImp<ValueType>(data, data + row*col, data + row*col, col));
			}
		}
		iterator ColBegin() const {
			if (1 != col_step) {
				return iterator(new IteratorImp<ValueType>(data));
			}
			else {
				return iterator(new TransposeIteratorImp<ValueType>(data, data + row*col, data, col));
			}
		}
		iterator ColEnd()const {
			if (1 != col_step) {
				return iterator(new IteratorImp<ValueType>(data + row*col));
			}
			else {
				return iterator(new TransposeIteratorImp<ValueType>(data, data + row*col, data + row*col, col));
			}
		}
	protected:
		using ValueTypePtr = ValueType*;
		ValueTypePtr data, alloc_addr;
		size_t row, col;
		int col_step;
		//------------------iterator------------------------//
		template<typename ValueType>
		class IteratorImp : public std::iterator<std::random_access_iterator_tag, ValueType> {
			friend class BasicMatrix<ValueType>;
		protected:
			pointer _cur;
			IteratorImp(pointer cur) :_cur(cur) {}
		public:
			using _Self = IteratorImp<ValueType>;
			virtual _Self* make_clone_ptr() const { return new _Self(*this); }
			virtual _Self& operator++() { ++_cur; return *this; }
			virtual _Self& operator--() { --_cur; return *this; }
			virtual _Self operator++(int) { auto tmp = *this; ++(*this); return tmp; }
			virtual _Self operator--(int) { auto tmp = *this; --(*this); return tmp; }
			virtual reference operator*() const { return *_cur; }
			virtual pointer operator->() const { return _cur; }
			virtual bool operator==(const _Self& right) const { return _cur == right._cur; }
			virtual bool operator!=(const _Self& right) const { return !operator==(right); }
			virtual bool operator<(const _Self& right) const { return _cur < right._cur; }
			virtual bool operator>=(const _Self& right) const { return !operator<(right); }
			virtual bool operator>(const _Self& right) const { return _cur > right._cur; }
			virtual bool operator<=(const _Self& right) const { return !operator>(right); }
			virtual reference operator[](difference_type n) const { return _cur[n]; }
			virtual _Self& operator+=(difference_type n) { _cur += n; return *this; }
			virtual _Self& operator-=(difference_type n) { _cur -= n; return *this; }
			virtual _Self operator+(difference_type n) const { return IteratorImp(_cur + n); }
			virtual _Self operator-(difference_type n) const { return IteratorImp(_cur - n); }
			virtual difference_type operator-(const _Self& before) { return _cur - before._cur; }
		};
		/*----------------------transpose iterator--------------------*/
		template<typename ValueType>
		class TransposeIteratorImp :public IteratorImp<ValueType> {
			friend class BasicMatrix<ValueType>;
		protected:
			pointer _beg, _end;
			difference_type _step, _count;
			TransposeIteratorImp(pointer beg, pointer end, pointer cur, difference_type step) :
				Iterator(cur), _beg(beg), _end(end), _step(col), _count(0){}
		public:
			using _Self = TransposeIteratorImp<ValueType>;
			using _Base = IteratorImp<ValueType>;
			_Base* make_clone_ptr() const override { return new _Self(*this); }
			_Base& operator++()override {  
				if ((_cur + _step) < _end) {
					_cur += step;
					return *this;
				}
				if (_count < _step) {
					_cur = _beg + _count++;
					return *this;
				}
				_cur = _end;
				return *this;
			}
			_Base& operator--()override {
				if ((_cur - step) >= _beg) {
					_cur -= step;
					return *this;
				}
				if (_count >= 0) {
					_cur = _beg + --_count + step;
					return *this;
				}
				_cur = _beg - 1;
				return *this;
			}
			_Self operator++(int) override { auto tmp = *this; ++(*this); return tmp; }
			_Self operator--(int) override { auto tmp = *this; --(*this); return tmp; }
			reference operator[](difference_type n) const override {
				TransposeIteratorImp iter(*this);
				iter.operator+=(n);
				return *iter;
			}
			_Base& operator+=(difference_type n) override {
				auto offset = n * step;
				if ((_cur + offset) < _end) {
					_cur += offset;
					return *this;
				}
				auto row = static_cast<difference_type>((_end - _beg) / _step);
				auto distance = _cur - _beg;
				auto i = static_cast<difference_type>(distance / _step);
				auto offset_j = static_cast<difference_type>(n / row);
				auto offset_i = n%row;
				if ((i + offset_i) >= row) {
					_cur = _end;
					return *this;
				}
				auto j = distance % _step;
				_cur = _beg + ((i + offset_i) * _step + j + offset_j);
				return *this;
			}
			_Self& operator-=(difference_type n)override { return operator+=(-n); }
			_Self operator+(difference_type n) const override { _Self tmp(*this); tmp += n; return tmp; }
			_Self operator-(difference_type n) const override { _Self tmp(*this); tmp -= n; return tmp; }
			difference_type operator-(const _Self& before) const override{
				auto GetRowImp = [](auto b, auto col) {return [=](auto e) {return static_cast<difference_type>((e - b) / col); }; };
				auto GetColImp = [](auto b, auto col) {return [=](auto e) {return static_cast<difference_type>((e - b) % col); }; };
				auto GetRow = GetRowImp(_beg, _step);
				auto GetCol = GetColImp(_beg, _step);
				auto row = GetRow(_end);
				auto CvtColIndex = [=row, &](auto cur) {return GetCol(cur) * row + GetRow(cur); };
				return CvtColIndex(_cur) - CvtColIndex(before._cur);
			}

		};

	};

}

namespace std {
	template<typename ...T>
	void swap(lpq::BasicPoint<T...>& left, lpq::BasicPoint<T...>& right) noexcept {
		left.swap(right);
	}

	template<typename T>
	void swap(lpq::BasicMatrix<T>& left, lpq::BasicMatrix<T>& right) noexcept {
		left.swap(right);
	}
}
#endif // !GRAPH_EXERCISE_BASIC_MATH_CORE_H
