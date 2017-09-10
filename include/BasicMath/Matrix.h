#ifndef GRAPH_EXERCISE_BASIC_MATH_CORE_H
#define GRAPH_EXERCISE_BASIC_MATH_CORE_H
#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>
#include <functional>
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
		using Matrix = BasicMatrix<ValueType>;

		/* construct and alloc and swap */

		BasicMatrix(size_t row, size_t column, ValueType* data_addr, std::shared_ptr<ValueType> alloc_addr) :
			data(data_addr), alloc_addr(alloc_addr), row(row), col(column), col_step(1) {}
		BasicMatrix() :BasicMatrix(0, 0, nullptr, nullptr) {}
		BasicMatrix(size_t row, size_t column, ValueType* data_addr) : BasicMatrix(row, column, data_addr, nullptr) {}
		BasicMatrix(size_t row, size_t column) :BasicMatrix(row, column, nullptr, 
			std::shared_ptr<ValueType>(new ValueType[row*column], std::default_delete<ValueType[]>())) {
			data = alloc_addr.get(); }
		BasicMatrix(const Matrix& other) = default;
		BasicMatrix(Matrix&& other) noexcept { swap(other); }
		~BasicMatrix() = default;
		Matrix& Alloc(size_t row, size_t column) {
			swap(Matrix(row, column));
			return *this;
		}
		Matrix& Alloc(size_t row, size_t column, ValueType* data_addr) {
			swap(Matrix(row, column, data_addr));
			return *this;
		}
		Matrix Clone() const{
			BasicMatrix tmp(row, col);
			ValueTypePtr _data = tmp.data;
			auto end = EndByRow();
			for (auto iter = BeginByRow(); iter != end; ++iter) {
				*_data++ = *iter;
			}
			return std::move(tmp);
		}
		Matrix& operator=(Matrix mat) {
			swap(mat);
			return *this;
		}
		bool operator==(const Matrix& right)const  {
			if (row != right.row || col != right.col) {
				return false;
			}
			auto end = EndByRow();
			auto right_iter = right.BeginByRow();
			for (auto iter = BeginByRow(); iter != end; ++iter) {
				if (*iter != *right_iter++) {
					return false;
				}
			}
			return true;
		}
		bool operator!=(const Matrix& right) const { return !operator==(right); }
		void swap(Matrix& right) noexcept{
			std::swap(data, right.data);
			std::swap(alloc_addr, right.alloc_addr);
			std::swap(row, right.row);
			std::swap(col, right.col);
			std::swap(col_step, right.col_step);
		}

		/* size info */

		bool Empty() const noexcept { return 0 == row || 0 == col; }
		std::pair<size_t, size_t> Size()const noexcept { return std::make_pair(row, col); }

		/* transpose */

		Matrix& Transpose() {
			if (Empty()) { return *this; }
			col_step = col_step == 1 ? col : 1;
			std::swap(row, col);
			return *this;
		}
		Matrix& Arrange() {
			if (Empty()) { return *this; }
			if (1 == col_step) { return *this; }
			auto new_addr = std::shared_ptr<ValueType>(new ValueType[row * col], std::default_delete<ValueType[]>());
			ValueTypePtr new_data = new_addr.get();
			col_step = 1;
			ValueTypePtr iter = new_data;
			for (size_t i = 0; i < row; ++i) {
				ValueTypePtr org_it = data + i;
				for (size_t j = 0; j < col; ++j) {
					*iter = std::move(*org_it);
					++iter;
					org_it += row;
				}
			}
			alloc_addr = new_addr;
			data = alloc_addr.get();
			return *this;
		}

		/* matrix index and iterator */

		ValueType& At(size_t row, size_t column) noexcept  {
			assert((row < this->row) && (column < this->col));
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
				_iterator(right._iterator->make_clone_ptr()){}
			Iterator(_Self && right) { std::swap(_iterator, right._iterator); }
			_Self& operator=(_Self tmp) { std::swap(_iterator, tmp._iterator); return *this; }
			_Self& operator++() { _iterator->operator++(); return *this; }
			_Self& operator--() { _iterator->operator--(); return *this; }
			_Self operator++(int) { auto tmp = *this; _iterator->operator++(); return tmp; }
			_Self operator--(int) { auto tmp = *this; _iterator->operator--(); return tmp; }
			reference operator*() const { return _iterator->operator*(); }
			pointer operator->() const { return _iterator->operator->(); }
			bool operator==(const _Self& right) const { return _iterator->operator==(*(right._iterator)); }
			bool operator!=(const _Self& right) const { return _iterator->operator!=(*(right._iterator)); }
			bool operator<(const _Self& right) const { return _iterator->operator<(*(right._iterator)); }
			bool operator>=(const _Self& right) const { return _iterator->operator>=(*(right._iterator)); }
			bool operator>(const _Self& right) const { return _iterator->operator>(*(right._iterator)); }
			bool operator<=(const _Self& right) const { return _iterator->operator<=(*(right._iterator)); }
			reference operator[](difference_type n) const { return _iterator->operator[](n); }
			_Self& operator+=(difference_type n) { _iterator->operator+=(n); return *this; }
			_Self& operator-=(difference_type n) { _iterator->operator-=(n); return *this; }
			_Self operator+(difference_type n) const { _Self tmp(_iterator->make_clone_ptr()); tmp._iterator->operator+=(n); return tmp; }
			_Self operator-(difference_type n) const { return operator+(-n); }
			difference_type operator-(const _Self& before) const { return _iterator->operator-(*(before._iterator)); }
		};
		using iterator = Iterator<ValueType>;
		iterator BeginByRow() const {
			if (1 == col_step || std::min(row, col) == 1){
				return iterator(new IteratorImp<ValueType>(data));
			}
			else {
				return iterator(new TransposeIteratorImp<ValueType>(data, data + row*col, data, col_step));
			}
		}
		iterator EndByRow()const {
			if (1 == col_step || std::min(row, col) == 1) {
				return iterator(new IteratorImp<ValueType>(data + row*col));
			}
			else {
				auto end = data + row*col;
				return iterator(new TransposeIteratorImp<ValueType>(data, end, end, col_step));
			}
		}
		iterator BeginByColumn() const {
			if (1 != col_step || std::min(row, col) == 1) {
				return iterator(new IteratorImp<ValueType>(data));
			}
			else {
				return iterator(new TransposeIteratorImp<ValueType>(data, data + row*col, data, col));
			}
		}
		iterator EndByColumn()const {
			if (1 != col_step || std::min(row, col) == 1) {
				return iterator(new IteratorImp<ValueType>(data + row*col));
			}
			else {
				auto end = data + row*col;
				return iterator(new TransposeIteratorImp<ValueType>(data, end, end, col));
			}
		}

		/* mathematics */

		Matrix operator+ (const Matrix& right) const {
			Matrix tmp;
			BinaryOperate<ValueType, ValueType>(*this, right, tmp, [](ValueType l, ValueType r) {return l + r; });
			return std::move(tmp);
		}
		template<typename ConstNumType>
		Matrix operator+ (ConstNumType n) const {
			Matrix tmp;
			UnaryOperate<ConstNumType>(*this, tmp, [=](ValueType l) {return static_cast<ValueType>(l + n); });
			return std::move(tmp);
		}
		Matrix operator- (const Matrix& right) const {
			Matrix tmp;
			BinaryOperate<ValueType, ValueType>(*this, right, tmp, [](ValueType l, ValueType r) {return l - r; });
			return std::move(tmp);
		}
		template<typename ConstNumType>
		Matrix operator- (ConstNumType n) const {
			Matrix tmp;
			UnaryOperate<ConstNumType>(*this, tmp, [=](ValueType l) {return static_cast<ValueType>(l - n); });
			return std::move(tmp);
		}
		template<typename ConstNumType>
		Matrix operator* (ConstNumType n) const {
			Matrix tmp;
			UnaryOperate<ConstNumType>(*this, tmp, [=](ValueType l) {return static_cast<ValueType>(l * n); });
			return std::move(tmp);
		}
		template<typename ConstNumType>
		Matrix operator/ (ConstNumType n) const {
			Matrix tmp;
			UnaryOperate<ConstNumType>(*this, tmp, [=](ValueType l) {return static_cast<ValueType>(l / n); });
			return std::move(tmp);
		}
		Matrix& operator+=(const Matrix& right) {
			BinaryOperate(*this, right, *this, [](ValueType l, ValueType r) {return l + r; });
			return *this;
		}
		template<typename ConstNumType>
		Matrix& operator+= (ConstNumType n) {
			UnaryOperate<ConstNumType>(*this, *this, [=](ValueType l) {return static_cast<ValueType>(l + n); });
			return *this;
		}
		Matrix& operator-=(const Matrix& right) {
			BinaryOperate<ValueType, ValueType>(*this, right, *this, [](ValueType l, ValueType r) {return l - r; });
			return *this;
		}
		template<typename ConstNumType>
		Matrix& operator-= (ConstNumType n)  {
			UnaryOperate<ConstNumType>(*this, *this, [=](ValueType l) {return static_cast<ValueType>(l - n); });
			return *this;
		}
		Matrix DotProduct(const Matrix& right) const {
			Matrix tmp;
			BinaryOperate<ValueType, ValueType>(*this, right, tmp, [](ValueType l, ValueType r) {return l * r; });
			return std::move(tmp);
		}
		Matrix& DotProductInSelf(const Matrix& right)  {
			BinaryOperate<ValueType, ValueType>(*this, right, *this, [](ValueType l, ValueType r) {return l * r; });
			return *this;
		}
		Matrix DotDivision(const Matrix& right) const {
			Matrix tmp;
			BinaryOperate<ValueType, ValueType>(*this, right, tmp, [](ValueType l, ValueType r) {return l / r; });
			return std::move(tmp);
		}
		Matrix& DotDivisionInSelf(const Matrix& right) {
			BinaryOperate<ValueType, ValueType>(*this, right, *this, [](ValueType l, ValueType r) {return l / r; });
			return *this;
		}
		Matrix operator*(const Matrix& right) const {
			assert(col == right.row);
			Matrix res(row, right.col);
			auto res_iter = res.BeginByRow();
			auto row_iter_backup = this->BeginByRow();
			for (size_t i = 0; i < res.row; ++i) {
				auto col_iter = right.BeginByColumn();
				auto row_iter = row_iter_backup;
				for (size_t j = 0; j < res.col; ++j) {
					row_iter = row_iter_backup;
					*res_iter = ValueType{};
					for (size_t k = 0; k < col; ++k) {
						(*res_iter) += (*row_iter * *col_iter);
						++row_iter;
						++col_iter;
					}
					++res_iter;
				}
				row_iter_backup = row_iter;
			}
			return std::move(res);
		}
		static Matrix Zero(size_t row, size_t col) {
			Matrix zero(row, col);
			std::fill_n(zero.data, row*col, ValueType{});
			return std::move(zero);
		}
		static Matrix One(size_t row, size_t col) {
			auto one = Zero(row, col);
			one += 1;
			return std::move(one);
		}
		static Matrix Identity(size_t row, size_t col) {
			auto identity = Zero(row, col);
			auto min_rank = std::min(row, col);
			auto iter = identity.data;
			for (size_t i = 0; i < min_rank; ++i) {
				*iter = 1;
				iter += col;
			}
		}
protected:
		using ValueTypePtr = ValueType*;
		ValueTypePtr data;
		std::shared_ptr<ValueType> alloc_addr;
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
			virtual reference operator*() const { return *_cur; }
			virtual pointer operator->() const { return _cur; }
			virtual bool operator==(const _Self& right) const { 
				return _cur == right._cur; 
			}
			virtual bool operator!=(const _Self& right) const { return !operator==(right); }
			virtual bool operator<(const _Self& right) const { return _cur < right._cur; }
			virtual bool operator>=(const _Self& right) const { return !operator<(right); }
			virtual bool operator>(const _Self& right) const { return _cur > right._cur; }
			virtual bool operator<=(const _Self& right) const { return !operator>(right); }
			virtual reference operator[](difference_type n) const { return _cur[n]; }
			virtual _Self& operator+=(difference_type n) { _cur += n; return *this; }
			virtual _Self& operator-=(difference_type n) { _cur -= n; return *this; }
			virtual difference_type operator-(const _Self& before) const { return _cur - before._cur; }
		};
		//--------------transpose iterator------------------//
		template<typename ValueType>
		class TransposeIteratorImp :public IteratorImp<ValueType> {
			friend class BasicMatrix<ValueType>;
		protected:
			pointer _beg, _end;
			difference_type _step, _count;
			TransposeIteratorImp(pointer beg, pointer end, pointer cur, difference_type step) :
				IteratorImp(cur), _beg(beg), _end(end), _step(step), _count(1){}
		public:
			using _Self = TransposeIteratorImp<ValueType>;
			using _Base = IteratorImp<ValueType>;
			_Base* make_clone_ptr() const override { return new _Self(*this); }
			_Base& operator++()override {  
				if ((_cur + _step) < _end) {
					_cur += _step;
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
				assert(_cur <= _end);
				if (_cur == _end) {
					--_cur;
					return *this;
				}
				if ((_cur - _step) >= _beg) {
					_cur -= _step;
					return *this;
				}
				if (_count > 1) {
					_cur = _end - (_step - --_count) -1;
					return *this;
				}
				_cur = _beg - 1;
				return *this;
			}
			reference operator[](difference_type n) const override {
				TransposeIteratorImp iter(*this);
				iter.operator+=(n);
				return *iter;
			}
			_Base& operator+=(difference_type n) override {
				auto offset = n * _step;
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
			_Base& operator-=(difference_type n)override { return operator+=(-n); }
			difference_type operator-(const _Base& before) const override{
				auto GetRowImp = [](auto b, auto col) {return [=](auto e) {return static_cast<difference_type>((e - b) / col); }; };
				auto GetColImp = [](auto b, auto col) {return [=](auto e) {return static_cast<difference_type>((e - b) % col); }; };
				auto GetRow = GetRowImp(_beg, _step);
				auto GetCol = GetColImp(_beg, _step);
				auto row = GetRow(_end);
				auto CvtColIndex = [&](auto cur) {return GetCol(cur) * row + GetRow(cur); };
				return CvtColIndex(_cur) - CvtColIndex(before._cur);
			}

		};
		//--------------------------------------------------//
		template<typename T1, typename T2>
		static void BinaryOperate(const Matrix& left, const Matrix& right, Matrix& dest, std::function<ValueType(T1, T2)> op) {
			assert(left.Size() == right.Size());
			if (dest.Size() != left.Size()) {
				dest.Alloc(left.row, left.col);
			}
#define _SCL_SECURE_NO_WARNINGS
			std::transform(left.BeginByRow(), left.EndByRow(), right.BeginByRow(), dest.BeginByRow(), op);
		}
		template<typename ConstNumType>
		static void UnaryOperate(const Matrix& src, Matrix& dest, std::function<ValueType(ConstNumType)> op) {
			if (dest.Size() != src.Size()) {
				dest.Alloc(src.row, src.col);
			}
#define _SCL_SECURE_NO_WARNINGS
			std::transform(src.BeginByRow(), src.EndByRow(), dest.BeginByRow(), op);
		}
	};

}

namespace std {
	template<typename ...ConstNumType>
	void swap(lpq::BasicPoint<ConstNumType...>& left, lpq::BasicPoint<ConstNumType...>& right) noexcept {
		left.swap(right);
	}

	template<typename ConstNumType>
	void swap(lpq::BasicMatrix<ConstNumType>& left, lpq::BasicMatrix<ConstNumType>& right) noexcept {
		left.swap(right);
	}
}
#endif // !GRAPH_EXERCISE_BASIC_MATH_CORE_H
