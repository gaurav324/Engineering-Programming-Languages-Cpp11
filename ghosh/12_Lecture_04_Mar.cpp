#include <iostream>
#include <cstdint>

using std::cout;
using std::end;

#define constexpr const

// Project 2 is going to involve val_array, and will be more mathematical.

#ifdef ANYTHING_ELSE
// Params should probably be const & but its not worth the trouble here
template <typename T>
T max(T x, T y) {
    if (y > x) { return y; }
    else { return x; }
}

// In case of character strings, you probably don't wanna compare them with 
// > or <, then you probably want to write a separate specialized function.
char* max(char *x, char* y) {
    // ...
}

// This is a disaster as the type it returns depends on the one with higher value
// and therefore it cannot always be T1 as per the signature
// Going to interrogate the types T1 and T2 about which one is more general/better
// than the other and then going to use that type. Therefore we need a definition
// for a better type. Currently we are using int, float, double as examples.
template <typename T1, typename T2>
T1 max(T1 x, T2 y) {
    if (x < y) { return y; }
    else { return x; }
}


// The rules for creating specializations of a template itself is that the
// template needs to be declared before you go ahead and create a specialization
template <typename T1, typename T2>
struct choose;

// The <> syntax is used for a specialization of the template with the types
// mentioned, we are doing full specialization below..since the specialization
// is not a template itself, but we still need to mention to the compiler that
// it is a template specialization we use template <>
// In case of the iterator_traits we were doing partial specialization.
template <> struct choose<int, int> { using type = int; };
template <> struct choose<float, int> { using type = float; };
template <> struct choose<double, int> { using type = double; };
template <> struct choose<int, int> { using type = int; };

template <typename T1, typename T2>
type_traits_fun<T1, T2>::type max(T1 x, T2 y) {
    if (x < y) { return y; }
    else { return x; }
}

#endif

#ifdef WIN32
// Above is a horrible solution to the problem of choosing the output type
// This is because of the exponential number of choose specializations being
// mentioned. Solution? Level of indirection during compile time.
// Take my types T1 and T2, and encode their power as an integer, and 
// take the max of the number and convert that back as the template.
template <typename T>
struct SRank;

// Functions compute and return values, we are computing types here
// This one is straightforward and it a little but traits like
template <> struct SRank<int> { static constexpr int value = 1; };
template <> struct SRank<float> { static constexpr int value = 2; };
template <> struct SRank<double> { static constexpr int value = 3; };

template <int rank>
struct SType;

template <> struct SType<1> { using type = int; };
template <> struct SType<2> { using type = float; };
template <> struct SType<3> { using type = double; };

// A const may be created at load time or runtime even and cannot be changed
// A constexpr is a compile time constant, and so it is known by the compiler
template<typename T1, typename T2>
struct choose {
    static constexpr int t1_rank = SRank<T1>::value;
    static constexpr int t2_rank = SRank<T2>::value;
    static constexpr int max_rank = max(t1_rank, t2_rank);
    using type = typename SType<max_rank>::type;
};

template <typename T1, typename T2>
typename choose<T1, T2>::type max(T1 x, T2 y) {
    if (x < y) { return y; }
    else { return x; }
}

#endif 

int main(void) {
    int a = 3; double b = 3.14;
    cout << max<int, double>(a, b);
}


#ifdef ANYTHING_ELSE
int main(void) {
    // could be arrays of 100, 100, 100
    valarray<double> x, y, z;
    // operator + which would take the valarrays in a loop and perform 
    // element by element addition, and returns a temporary result 
    // to which z is added in a similar way, which is passed to the ostream operator
    // which then needs to do a third loop to print it. That is crazy (3 loops) and 
    // if it was C, one would have written a single for-loop which could do this 
    // in a single line: 
    // for (...) {
    //   cout << x[k] + y[k] + z[k];
    // }
    // This ends up becoming very inefficient and resutls are slower than hand written code.
    // Using expression template mechanism, we will make this C++ code result in an equal 
    // performance as handwritten code - efficiency and great syntax.
    // VSIPL - C++ signal processing library specifically designed to do this work
    cout << x + y + z << endl;
}

#endif
