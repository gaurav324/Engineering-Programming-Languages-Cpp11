//if (p == x.end()) {
    // This should not throw an exception.
//}
#include<iostream>
#include<cstdint>

using namespace std;

int main(void) {
    valarray<double> x, y, z;

    cout << x + y + z << endl;
    for (...) {
        // Expression templates would be used
        // to convert above cout to this cout.
        //
        // Proxy would be returned from above expression
        // and that would not have any data (but would have
        // vector stored in that).
        cout << x[k] + y[k] + z[k];
    }
}

template<typename T>
T max(T x, T y) {
    if (y > x) { return y;}
    else { return x; }
}


// What type should it return?
// Syaapa hai !!
// In case one, we are returning one type
// and in another case, we return another type.
template <typename T1, typename T2>
T1 max(T1 x, T2 y) {
    if (x < y) { return y;}
    else {return x;}
}


//Declaration of template.
template <typename T1, typename T2>
struct choose;

// Specializations.
template <> struct choose<int, int> { using type = int;}
template <> struct choose<float, int> { using type = float;}
template <> struct choose<double, int> { using type = double;}
template <> struct choose<int, int> { using type = int;}

//template <typename t>
//struct iterator_traits<t*> {
//
//};

template <typename T1, typename T2>
choose<T1, T2>::type_max(T1 x, T2) {
    if (x < y) { return y;}
    else {return x;}
}

//////// Let us do another trakee de naakl. /////////////
template<typename T>
struct SRank;

template <> struct SRank<int> { static constexpr uint32_t value = 1};
template <> struct SRank<float> { static constexpr uint32_t value = 2};
template <> struct SRank<double> { static constexpr uint32_t value = 3};

template<uint32_t rank>
struct Stype;

template <> struct Stype<1> { using type = int; };
template <> struct Stype<2> { using type = float; };
template <> struct Stype<3> { using type = double; };


template<typename T>
constexpr T max(const T& x, const T& y) {
    if (x < y) { return y; }
    else { return x; }
}

// Why use structs, but not functions?
// Because, functions return values and not types.
template <typename T1, typename T2>
struct choose {
    // constexpr is a compile time constant.
    // compiler has to know this value; However const is something
    // that can be at run time.
    static constexpr uint32_t t1_rank = SRank<T1>::value;
    static constexpr uint32_t t2_rank = SRank<T2>::value;
    static constexpr uint32_t max_rank = max(t1_rank, t2_rank);
    //using type = Stype<max_rank>::type;
    using type = typename Stype<max_rank>::type; // (??)
}
