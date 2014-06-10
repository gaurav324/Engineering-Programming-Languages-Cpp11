template<typename T>
struct SRank;

template <> struct SRank<int> { static constexpr uint32_t value = 1};
template <> struct SRank<float> { static constexpr uint32_t value = 2};
template <> struct SRank<double> { static constexpr uint32_t value = 3};

template <typename T> struct SRank<std::complex<T>> {
    static const uint32_t value = SRank<T>::value;
}

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

// Till lecture12, we will get error*chcek this for SRankcomplex<float>.
// kuch samajh nahi lag reha c.
//

// study about template meta functions.
// static_assert(error on compile time)
// This is not that a good approach, because that will make all other vector implementations
// would not be able to support addition (as we have it at global level).
