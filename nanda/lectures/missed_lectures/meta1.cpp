#include <iostream>
#include <cstdint>

using std::cout;
using std::endl;

// Looks like that we have to make a max function.
// max(t1, t2).

// We need a way to rank two types.
template <typename T>
struct SRank;

template<> struct SRank<int> { static constexpr uint32_t value = 1; };
template<> struct SRank<float> { static constexpr uint32_t value = 2; };
template<> struct SRank<double> { static constexpr uint32_t value = 3; };

// We need a way to Struct Type, given the rank.
template <uint32_t rank>
struct SType;

template <> struct SType<1> { using type = int; };
template <> struct SType<2> { using type = float; };
template <> struct SType<3> { using type = double; };

// Finding max, when both arguments are of same type.
template <typename T>
constexpr T max(const T& x, const T& y) {
    return (x < y) ? y : x;
}

// Choose max between two types and set type inside struct.
template <typename T1, typename T2>
struct choose {
    static constexpr uint32_t t1_rank = SRank<T1>::value;
    static constexpr uint32_t t2_rank = SRank<T2>::value;

    static constexpr uint32_t max_rank = max(t1_rank, t2_rank);

    using type = typename SType<max_rank>::type;
};


// This is the actual max for two different types.
template <typename T1, typename T2>
typename choose<T1, T2>::type max(T1 x, T2 y) {
    if (x < y) { return y; }
    else { return x; }
}

int main(void) {
    cout << max(10.2, 1) << endl;
}
