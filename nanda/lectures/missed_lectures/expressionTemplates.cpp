// So, this lecture is about learing the expression templates.
// Sir, were saying something that we may not need to use expression templates
// for the second project, but who knows. 
//  - He says expression templates are entirely an optimization.
//  - Basically for vectors, we are only going to construct types on the fly
//  and dont actually create vectors and populate them.

#include <iostream>
#include <cstdint>
#include <vector>

using namespace std;

struct Identity {
    double operator() (double in) const {
        return in;
    }
};

template<typename LType, typename RType>
struct MulProxy {
    typename RefType<LType>::type left;
    typename RefType<RType>::type right;
    MulProxy(LType _l, RType _r) : left(_l), right(_r) {}
    
    double operator()(double in) const {
        return left(in) * right(in);    
    }
};

MulProxy<Identity, Identity> operator*(Identity left, Identity right) {
    return MulProxy<Identity, Identity>(left, right);
}

double integrate(Fun f, double start, double end, double incr) {
    double result = 0.0;
    for (double x=start; x < end; x+=incr;) {
        result += f(x) * incr;
    }
    return result;
}

int main(void) {
    Identity i;
    auto fun = i * i;
    double x = integrate(fun, 0, 5, .001);
    cout << "The integral is: " << x << endl;
    cout << "The integral is: " << integrate(i * i * i, 0, 5, .001) << endl;
    cout << "The integral is: " << integrate(i * i * i * i, 0, 5, .001) << endl;
}

// Tips fi tge vector class.
// - It will have size.
// - Proxies are read only.
// - Vector type. (nested type??)
// - begin and end. (return const pointers)
// - It will be a const vector).
// - Try to make everything work for addition first and then we will see.
// Template Metafunction. Expression Templates.
// We have to take care of the case where our temporary references would
// go out of scope and just vanish then.
//
// Simply put, create expression tree, create copies of your proxies
// and store reference to vector (it is safe, because we don't expect destructor
// to be invoked until the scope of the expression).
