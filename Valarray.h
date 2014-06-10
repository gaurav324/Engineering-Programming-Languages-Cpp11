// this file intentionally left blank
#ifndef _VALARRAY_H
#define _VALARRAY_H 1

#include "Vector.h"
#include <complex>
#include <cmath>

#include "EPL_traits.h"

#endif /* _VALARRAY_H */

using namespace std::rel_ops;
using namespace std;

////////////////////////////////////////////////////////////////
// Standard code for choosing between two types using between 
// two types.
////////////////////////////////////////////////////////////////

// Get ranking among int, float and double.
template <uint32_t rank>
struct SType;

template <> struct SType<1> { using type = int;  };
template <> struct SType<2> { using type = float; };
template <> struct SType<3> { using type = double; };

// If complex type, then type would be promoted to complex.
// Otherwise just same as stype.
template <typename T, bool complexity>
struct CType;

template <typename T> struct CType<T, true> {
	using type = std::complex<T>;
};

template <typename T> struct CType<T, false> {
	using type = T;
};

template <typename T, bool complexity> 
using CompType = typename CType<T, complexity>::type;

// This is how we choose.
template<typename T1, typename T2>
struct choose {
    static const uint64_t t1_rank = EPL_traits<T1>::SRank;
    static const uint64_t t2_rank = EPL_traits<T2>::SRank;
    static const uint64_t max_rank = t1_rank > t2_rank ? t1_rank : t2_rank;
    using stype = typename SType<max_rank>::type;

    static const bool complexity = EPL_traits<T1>::CRank || EPL_traits<T2>::CRank;
    using type = CompType<stype, complexity>;
};

////////////////////////////////////////////////////////////////
// Convert between types. This may not be required, because
// we may be able to directly convert between types. But I am 
// still keeping this.
////////////////////////////////////////////////////////////////
template <typename To, typename From, bool is_to_complex, bool is_from_complex>
struct convert;

template<typename To, typename From> 
struct convert<To, From, true, true> {
    From val;
    convert(From _v) { val = _v; }
    To getValue() {
        return To{static_cast<typename SType<EPL_traits<To>::SRank>::type>(val.real()), static_cast<typename SType<EPL_traits<To>::SRank>::type>(val.imag())};
    }
};

template<typename To, typename From> 
struct convert<To, From, true, false> {
    From val;
    convert(From _v) { val = _v; }
    To getValue() {
        return To{static_cast<typename SType<EPL_traits<To>::SRank>::type>(val)};
    }
};

template<typename To, typename From> 
struct convert<To, From, false, false> {
    From val;
    convert(From _v) { val = _v; }
    To getValue() {
        return static_cast<To>(val);
    }
};

template<typename To, typename From, typename R=convert<To, From, EPL_traits<To>::CRank, EPL_traits<From>::CRank>> 
struct promote : public R {
    promote(From _v) : R(_v) {}
};

////////////////////////////////////////////////////////////////
// Struct to encapsulate all the scalar values.
////////////////////////////////////////////////////////////////
template<typename T>
struct scalar {
    T val;
    scalar(T _v) : val(_v) {}
    
    T operator[](uint64_t index) const {
        return val;
    }
   
    uint64_t size() const {
        return -1;
    }
};

////////////////////////////////////////////////////////////////
// Iterator for all proxies. If T is constant, then this iterator
// would act just as const_iterator.
////////////////////////////////////////////////////////////////
template<typename OperationProxy, typename T>
class MyIterator {
public:
    using iterator = MyIterator<OperationProxy, T>;

    OperationProxy* proxy;
    uint64_t index;

    MyIterator(OperationProxy* _p, int _i) : proxy(_p), index(_i) {}

    T operator*() {
        return proxy->operator[](index);
    }
 
    // Pre-increment operator.
    MyIterator& operator++() {
        ++index;
        return *this;
    }

    // Post increment operator.
    MyIterator operator++(int) {
        iterator t{*this};
        ++index;
        return t;
    }

    // + operator.
    MyIterator operator+(int64_t n) {
        return iterator{proxy, index + n};
    }

    // - operator.
    MyIterator operator-(int64_t n) {
        return iterator{proxy, index - n};
    }

    // += operator.
    MyIterator& operator+=(int64_t n) {
        index += n;
        return *this;
    }

    // -= operator.
    MyIterator& operator-=(int64_t n) {
        index -= n;
        return *this;
    }

    // [] operator.
    T operator[](int64_t n) {
        return proxy->operator[](index + n);
    }
 
    // == operator
    bool operator == (const MyIterator& rhs) const {
        return this->index == rhs.index ? true : false;
    }

};

////////////////////////////////////////////////////////////////
// Unary Function Proxy. I think this could also be merged with
// normal proxy classes, by using some specialization. Either
// we can specialize by checking whether parent class is 
// std::unary_function or by encapsulating unary functions in
// one class and binary functions in another.
////////////////////////////////////////////////////////////////
template <typename L, typename R, typename UnaryOperation>
struct UnaryOperationProxy {
    using RetType = typename UnaryOperation::result_type;

    // Members.
    const Valarray<L, R>& val;
    UnaryOperation op;

    // Constructor.
    UnaryOperationProxy(const Valarray<L, R>& _v, UnaryOperation _op) : val(_v), op(_op) {}
    
    // Element at operator.
    RetType operator[](uint64_t index) const {
        return op(this->val[index]);
    } 

    // Return size.
    uint64_t size() const { return (uint64_t)val.size(); }

    //  Iterator related functions.
    using iterator = MyIterator<UnaryOperationProxy, L>;
    using const_iterator = MyIterator<UnaryOperationProxy, const L>;

    iterator begin() {
        return iterator(this, 0);
    }
    
    const_iterator begin() const {
        return const_iterator(this, 0);
    }

    iterator end() {
        return iterator(this, this->size());
    }

    const_iterator end() const {
        return const_iterator(this, this->size());
    }
};

////////////////////////////////////////////////////////////////
// Parent Proxy class for binary operations having two valarray
// arguments.
////////////////////////////////////////////////////////////////
template <typename BinaryOperation, typename LRetType, typename LType, typename RRetType, typename RType>
struct ParentProxy {
    using RetType = typename choose<LRetType, RRetType>::type;

    // Members.
    const Valarray<LRetType, LType>& left;
    const Valarray<RRetType, RType>& right;
    BinaryOperation op;

    // Constructor.
    ParentProxy(BinaryOperation _op, const Valarray<LRetType, LType>& _l, const Valarray<RRetType, RType>& _r) : op(_op), left(_l), right(_r) {}

    RetType operator[](uint64_t index) const {
        auto lValue = promote<RetType, LRetType>(this->left[index]);
        auto rValue = promote<RetType, RRetType>(this->right[index]);

        return this->op(lValue.getValue(), rValue.getValue());
    }

    // Chooose min size.
    uint64_t size() const { 
        return std::min((uint64_t)left.size(), (uint64_t)right.size()); 
    }
};

////////////////////////////////////////////////////////////////
// Parent class having one operand as valarray and another as
// a scalar. Scalar on the right side and Valarray on the left.
////////////////////////////////////////////////////////////////
template <typename BinaryOperation, typename LRetType, typename LType, typename RRetType>
struct ParentProxy<BinaryOperation, LRetType, LType, RRetType, scalar<RRetType>> {
    using RetType = typename choose<LRetType, RRetType>::type;

    // Members.
    const Valarray<LRetType, LType>& left;
    scalar<RRetType> right;
    BinaryOperation op;

    // This is for switching order or operations. Useful for subtraction and division.
    bool order;

    // Constructor.
    ParentProxy(BinaryOperation _op, const Valarray<LRetType, LType>& _l, scalar<RRetType> _r, bool _b) : op(_op), left(_l), right(_r), order(_b) {}

    RetType operator[](uint64_t index) const {
        auto lValue = promote<RetType, LRetType>(this->left[index]);
        auto rValue = promote<RetType, RRetType>(this->right[index]);

        if (order) {
            return this->op(lValue.getValue(), rValue.getValue());
        } else {
            return this->op(rValue.getValue(), lValue.getValue());
        }
    }

    // Choose one size.
    uint64_t size() const { 
        return (uint64_t)left.size();
    }
};

////////////////////////////////////////////////////////////////
// Class for valarrays which have two operands. Either it could
// extend from two valarray version of parentProxy or it can 
// extend from one valarry-one scalar version of ParentProxy.
////////////////////////////////////////////////////////////////
template <typename Operation, typename LRetType, typename LType, typename RRetType=LRetType, typename RType=scalar<RRetType>, typename R=ParentProxy<Operation, LRetType, LType, RRetType, RType>>
struct Proxy : public R {
    using RetType = typename choose<LRetType, RRetType>::type;

    // Constructors.
    Proxy(Operation _op, const Valarray<LRetType, LType>& _l, const Valarray<RRetType, RType>& _r) : R(_op, _l, _r) {}
    Proxy(Operation _op, const Valarray<LRetType, LType>& _l, scalar<RRetType> _r, bool order) : R(_op, _l, _r, order) {}
  
    using iterator = MyIterator<Proxy, RetType>;
    using const_iterator = MyIterator<Proxy, const RetType>;

    iterator begin() {
        return iterator(this, 0);
    }
    
    const_iterator begin() const {
        return const_iterator(this, 0);
    }

    iterator end() {
        return iterator(this, this->size());
    }

    const_iterator end() const {
        return const_iterator(this, this->size());
    }
};

////////////////////////////////////////////////////////////////
// Square root as a unary function.
////////////////////////////////////////////////////////////////
template <typename Arg, typename Result>
struct square_root : public std::unary_function<Arg, Result>  {
    Result operator() (const Arg& arg) const {
        return std::sqrt(arg);
    }
};

////////////////////////////////////////////////////////////////
// Valarray inherits from Vector by default. Otherwise, we would
// pass Proxy as its parent class.
////////////////////////////////////////////////////////////////
template <typename T, typename R=epl::Vector<T> >
struct Valarray : public R {
public:
    // This is how we explictly ask for inheriting constructors.
    // If we dont ask, constructors would not be inherited, which 
    // makes sense also because we could be adding more attributes
    // to the derived class.
    // But this does not seem to work when R is not vector.
    // using R::vector;
    Valarray() : R() {}

    explicit Valarray(uint64_t size) : R(size) {}

    Valarray(std::initializer_list<T> il) : R(il) {}

    // Constructor for proxy operting two valarrays.
    template <typename BinaryOperation, typename T1, typename R1, typename T2, typename R2>
    Valarray(BinaryOperation op, const Valarray<T1, R1>& x, const Valarray<T2, R2>& y) : R(op, x, y) {}

    template <typename BinaryOperation, typename T1, typename R1, typename T2>
    Valarray(BinaryOperation op, const Valarray<T1, R1>& x, scalar<T2> y, bool order=true) : R(op, x, y, order) {}

    // This is for UnaryOperation Proxy.
    template <typename L1, typename R1, typename UnaryOperation>
    Valarray(const Valarray<L1, R1>& rhs, UnaryOperation op) : R(rhs, op) {} 

    // Copy constructor.
    template <typename L1, typename R1>
    Valarray(const Valarray<L1, R1>& rhs) {
        for (int i=0; i < rhs.size(); ++i) {
            this->push_back((T)rhs[i]);
        }
    }

    // Assignment operator.
    Valarray<T, R>& operator=(const Valarray<T, R>& that) {
        if (this != &that) {
            int total_size = this->size();
            int temp = 0;
            while(temp != total_size) {
                ++temp;
                this->pop_back();
            }
            for (int i=0; i < that.size(); ++i) {
                this->push_back(that[i]);
            }
        }
        return *this;
    }

    // Assignment operator for valarray of different type.
    // In this one, we cannot compare this != that and that 
    // is not required either.
    template <typename L1, typename R1>
    Valarray<T, R>& operator=(const Valarray<L1, R1>& that) {
        int total_size = this->size();
        int temp = 0;
        while(temp != total_size) {
            ++temp;
            this->pop_back();
        }
        for (int i=0; i < that.size(); ++i) {
            this->push_back((T)that[i]);
        }
        return *this;
    }

    // Assignment a constant value to all the members of the valarray.
    Valarray<T, R>& operator=(const T& val) {
        for (int i=0; i < this->size(); ++i) {
            this->operator[](i) = val;
        }
    }

    // Accumulate all the values using binary operator.
    template<class BinaryOperation>
    T accumulate(BinaryOperation op, T init) {
        for (int i=0; i < this->size();  ++i) {
            init = op(init, this->operator[](i));
        }
        return init;
    }

    // Sums all the values.
    T sum() {
        T result{};
        return accumulate(std::plus<T>(), result);
    }

    // Applies Unary operation to all the members of the valarray.
    template<typename UnaryOperation>
    Valarray<typename UnaryOperation::result_type, UnaryOperationProxy<T, R, UnaryOperation>> 
    apply(UnaryOperation op) const {
        return Valarray<typename UnaryOperation::result_type, UnaryOperationProxy<T, R, UnaryOperation>>(*this, op);
    }

    // CompType takes base type. For float, it would evaluate to choose<float, CompType<double, false>>
    // For complex<float>, it would evaluate to choose<complex<float>, CompType<double, true>>
    template <typename T1=typename choose<T, CompType<double, EPL_traits<T>::CRank> >::type>
    Valarray<T1, UnaryOperationProxy<T, R, square_root<T, T1>>>
    sqrt() {
        return apply(square_root<T, T1>());
    }
};

// Printout ValArray.
template<typename T, typename R>
std::ostream& operator<<(std::ostream& out, const Valarray<T, R>& val) {
    const char* prefix = "";
    for (int i=0; i < val.size(); ++i) {
        out << prefix << val.operator[](i);
        prefix = ", ";
    }
    out << endl;
    return out;
}

// Unary negation.
template <typename T1, typename R1>
Valarray<T1, UnaryOperationProxy<T1, R1, std::negate<T1>> >
operator-(const Valarray<T1, R1>& x) {
    return x.apply(std::negate<T1>());
}

// Operator +. 
template <typename T1, typename R1, typename T2, typename R2, typename RetType=typename choose<T1, T2>::type>
Valarray<RetType, Proxy<std::plus<RetType>, T1, R1, T2, R2> > 
operator+(const Valarray<T1, R1>& x, const Valarray<T2, R2>& y) {
    return Valarray<RetType, Proxy<std::plus<RetType>, T1, R1, T2, R2>>(std::plus<RetType>(), x, y);
}

template <typename T1, typename R1, typename T2, typename RetType=typename choose<T1, T2>::type>
Valarray<RetType, Proxy<std::plus<RetType>, T1, R1, T2>> 
operator+(const Valarray<T1, R1>& x, T2 y) {
    return Valarray<RetType, Proxy<std::plus<RetType>, T1, R1, T2>>(std::plus<RetType>(), x, scalar<T2>(y), true);
}

template <typename T1, typename T2, typename R2, typename RetType=typename choose<T1, T2>::type>
Valarray<RetType, Proxy<std::plus<RetType>, T2, R2, T1> > 
operator+(T1 y, const Valarray<T2, R2>& x) {
    return Valarray<RetType, Proxy<std::plus<RetType>, T2, R2, T1>>(std::plus<RetType>(), x, scalar<T1>(y), false);
}

// Operator -. 
template <typename T1, typename R1, typename T2, typename R2, typename RetType=typename choose<T1, T2>::type>
Valarray<RetType, Proxy<std::minus<RetType>, T1, R1, T2, R2> > 
operator-(const Valarray<T1, R1>& x, const Valarray<T2, R2>& y) {
    return Valarray<RetType, Proxy<std::minus<RetType>, T1, R1, T2, R2>>(std::minus<RetType>(), x, y);
}

template <typename T1, typename R1, typename T2, typename RetType=typename choose<T1, T2>::type>
Valarray<RetType, Proxy<std::minus<RetType>, T1, R1, T2>> 
operator-(const Valarray<T1, R1>& x, T2 y) {
    return Valarray<RetType, Proxy<std::minus<RetType>, T1, R1, T2>>(std::minus<RetType>(), x, scalar<T2>(y), true);
}

template <typename T1, typename T2, typename R2, typename RetType=typename choose<T1, T2>::type>
Valarray<RetType, Proxy<std::minus<RetType>, T2, R2, T1> > 
operator-(T1 y, const Valarray<T2, R2>& x) {
    return Valarray<RetType, Proxy<std::minus<RetType>, T2, R2, T1>>(std::minus<RetType>(), x, scalar<T1>(y), false);
}

// Operator *. 
template <typename T1, typename R1, typename T2, typename R2, typename RetType=typename choose<T1, T2>::type>
Valarray<RetType, Proxy<std::multiplies<RetType>, T1, R1, T2, R2> > 
operator*(const Valarray<T1, R1>& x, const Valarray<T2, R2>& y) {
    return Valarray<RetType, Proxy<std::multiplies<RetType>, T1, R1, T2, R2>>(std::multiplies<RetType>(), x, y);
}

template <typename T1, typename R1, typename T2, typename RetType=typename choose<T1, T2>::type>
Valarray<RetType, Proxy<std::multiplies<RetType>, T1, R1, T2>> 
operator*(const Valarray<T1, R1>& x, T2 y) {
    return Valarray<RetType, Proxy<std::multiplies<RetType>, T1, R1, T2>>(std::multiplies<RetType>(), x, scalar<T2>(y), true);
}

template <typename T1, typename T2, typename R2, typename RetType=typename choose<T1, T2>::type>
Valarray<RetType, Proxy<std::multiplies<RetType>, T2, R2, T1> > 
operator*(T1 y, const Valarray<T2, R2>& x) {
    return Valarray<RetType, Proxy<std::multiplies<RetType>, T2, R2, T1>>(std::multiplies<RetType>(), x, scalar<T1>(y), false);
}

// Operator /. 
template <typename T1, typename R1, typename T2, typename R2, typename RetType=typename choose<T1, T2>::type>
Valarray<RetType, Proxy<std::divides<RetType>, T1, R1, T2, R2> > 
operator/(const Valarray<T1, R1>& x, const Valarray<T2, R2>& y) {
    return Valarray<RetType, Proxy<std::divides<RetType>, T1, R1, T2, R2>>(std::divides<RetType>(), x, y);
}

template <typename T1, typename R1, typename T2, typename RetType=typename choose<T1, T2>::type>
Valarray<RetType, Proxy<std::divides<RetType>, T1, R1, T2>> 
operator/(const Valarray<T1, R1>& x, T2 y) {
    return Valarray<RetType, Proxy<std::divides<RetType>, T1, R1, T2>>(std::divides<RetType>(), x, scalar<T2>(y), true);
}

template <typename T1, typename T2, typename R2, typename RetType=typename choose<T1, T2>::type>
Valarray<RetType, Proxy<std::divides<RetType>, T2, R2, T1> > 
operator/(T1 y, const Valarray<T2, R2>& x) {
    return Valarray<RetType, Proxy<std::divides<RetType>, T2, R2, T1>>(std::divides<RetType>(), x, scalar<T1>(y), false);
}
