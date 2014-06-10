#include <cstdint>
#include <iostream>
#include <vector>

#define _trick_
#define _2nd_attempt_

struct identity {
    double operator()(double in) const { return in; }
};

template <typename Fun>
double integrate(Fun f, double start, double end, double incr) {
    double result = 0.0;
    for (double x = start; x < end; x += incr) {
        result += f(x) * incr;
    }
    return result;
}

#ifdef _1st_attempt_
struct MulProxy {
    identity left;
    identity right;
    MulProxy(identity _l, identity _r) : left(_l), right(_r) {}
    double operator()(double in) const { return left(in) * right(in); }
};

MulProxy operator*(identity left, identity right) {
    return MulProxy(left, right);
}

#endif

#ifdef _2nd_attempt_

template <typename LType, typename RType>
struct MulProxy {
    LType left;
    RType right;
    MulProxy(LType _l, RType _r) : left(_l), right(_r) {}
    double operator()(double in) const { return left(in) * right(in); }
};


MulProxy<identity, identity> operator*(identity left, identity right) {
    return MulProxy<identity, identity>(left, right);
}

template <typename L1, typename L2>
MulProxy<MulProxy<L1, L2>, identity>
operator*(MulProxy<L1, L2> left, identity right) {
    return MulProxy<MulProxy<L1, L2>, identity>(left, right);
}

template <typename L1, typename L2, typename R1, typename R2>
MulProxy<MulProxy<L1, L2>, MulProxy<R1, R2>>
operator*(MulProxy<L1, L2> left, MulProxy<R1, R2> right) {
    return MulProxy<MulProxy<L1, L2>, MulProxy<R1, R2>>(left, right);
}

// Can create all kinds of such trees
// Cannot yet declare victory as code is a mess
// For the project, do everything for addition

#endif

#ifdef _okay_place_to_start_

template <typename LType, typename RType>
struct MulProxy {
    LType left;
    RType right;
    MulProxy(LType _l, RType _r) : left(_l), right(_r) {}
    double operator()(double in) const { return left(in) * right(in); }
};

// This would solve the problem for all cases, but it overloads the operator*
// and therefore anyone else using the normal * would end up calling this function
// This is not Chase's favorite so he won't like this. One could use enable_if 
// type constructs to get around that

template <typename L, typename R>
MulProxy<L, R> operator*(L left, R right) {
    return MulProxy<L, R>(left, right);
}

#endif

#ifdef _trick_
// The trick is to write a template with 2 arguments, and as long as the second
// argument is hidden and has a default argument, it should work. The ValArray is going to
// be a wrapper around the type R (represenation). The trick goes like this: we are going 
// to instantiate R as a vector with the leaves of the expression tree and R is going to be
// a Proxy for internal nodes. The purpose of the valarray is to provide an object that enables
// the real R objects to get added to each other

// You guys have to calculate your return types. Here everything is double, 
// but in the project you need to calculate the return types.
//
// Valarray has to be a derived type of R
template <typename T, typename R = std::vector<T>>
struct Valarray : public R {
    explicit Valarray(uint64_t sz) : R(sz) {}
};

// Have to do choose_type thing to return the correct return type
template <typename T1, typename R1, typename T2, typename R2>
Valarray<T1, AddProxy<R1, R2>>
operator +(const Valarray<T1, R1>& x, const Valarray<T2, R2>& y) {
    return ValArray<T1, AddProxy<R1, R2>>(x, y);
}

#endif

// We have a problem in Project 2 with the lifetime of the template objects
// that we are creating. What we want is to postpone the evaluation of the
// entire expression tree.
// use constant references to avoid deep copies.
// If you are building a proxy that has a left type as a vector, you make the left 
// type as a reference. If you have a left type as a scalar, then you make the left 
// type as a scalar. How do we do that? Template metafunction.
// The reason it is safe to store a reference to a vector is that we can expect the vector
// destructor to not run during the lifetime of this expression. However, the destructor for
// the first proxy will run before the expression finishes. 
// This would break if one of the objects was a movable object (std::cout << x + y + x + getVector(y))
// If we had a 2 instead of getvector(), eventually the 2 would be be promoted to a scalar proxy

int main(void) {
    identity i;
    auto fun = i * i;
    double x = integrate(fun, 0, 5, 0.001);
    std::cout << " The integral is: " << x << std::endl;
    std::cout << " The integral is: " << integrate(i * i * i, 0, 5, 0.001) << std::endl;
    std::cout << " The integral is: " << integrate(i * i * i * i, 0, 5, 0.001) << std::endl;

    Valarray <double> x(10);
    Valarray <double> y(10);
    std::cout << x + y;
    std::cout << x + y + x;
    std::cin.get();
    return 0;
}
