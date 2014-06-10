#include <iostream>
#include <cstdint>
#include <complex>

#include <type_traits>
#include <utility>

using namespace std;
#define approach5 true

//template <typename T>
//struct has_less(void) {
//    return ? ? ?;
//}

//Approach-1
//template <typename T>
//bool has_less(void) {
    //if (T{} > T{}) {
    // This substitution failure should ideally be happening in the 
    // declaration of the function, but not in the body.
    //
    // We cannot call if in the body for substitution error.
    // So lets our next approach.
    //}
    
    // Use the type of the < operator.
    //decltype(T{} < T{}) variable;
//}

#ifdef approach2
// This is bammb, technique. There would be a substitution failure
// in the template.
// Approach-2
template <typename T, typename unused = decltype(T{} < T{})>
bool has_less(void) {
    return true;
}

template <typename T>
bool has_less(void) {
    return false;
}

int main(void) {
    cout << boolalpha;
    // Approach-2 would fail for has_less<int>, because both of the has_less
    // are valid for int. So, compiler gets confused.
    cout << has_less<int>() << endl;
    cout << has_less<std::complex<double>>() << endl;

    // We want to put these has_less functions in the enable_if.
}
#endif

#ifdef approach3
template <typename T, typename unused = decltype(T{} < T{})>
bool has_less(const T&) {
    return true;
}

template <typename T>
bool has_less(...) {
    return false;
}

int main(void) {
    cout << boolalpha;
    // We have made it work, by using property of C++ that if
    // a function has two versions, variadic and non-variadic, then
    // compiler would choose the non-variadic function.
    cout << has_less<int>(0) << endl;
    cout << has_less<std::complex<double>>(std::complex <double> {0}) << endl;

    // We want to put these has_less functions in the enable_if.
}

#endif

#ifdef approach4
template <typename T, typename unused = decltype(std::declval<T>() < std::declval<T>() )>
std::true_type has_less_helper(const T&);

// XXX A girl suggested that, have void as an argument and it should do balle balle.
//std::true_type has_less_helper(const T&) {
//    return std::true_type{};
//}

//template <typename T>
//std::false_type has_less_helper(...) {
//    return std::false_type{};
//}
// Even better.
template <typename T> std::false_type has_less_helper(...);

template <typename T>
bool has_less(void) {
    using my_type = decltype(has_less_helper<T>(std::declval<T>()));
    return my_type::value;
}

int main(void) {
    cout << boolalpha;
    // We have made it work, by using property of C++ that if
    // a function has two versions, variadic and non-variadic, then
    // compiler would choose the non-variadic function.
    cout << has_less<int>() << endl;
    cout << has_less<std::complex<double>>() << endl;

    // We want to put these has_less functions in the enable_if.
}

#endif


#ifdef approach5
template <typename T, typename unused = decltype(std::declval<T>() < std::declval<T>() )>
std::true_type has_less_helper(const T&);

template <typename T> std::false_type has_less_helper(...);

template <typename T>
constexpr bool has_less(void) {
    using my_type = decltype(has_less_helper<T>(std::declval<T>()));
    return my_type::value;
}

int main(void) {
    cout << boolalpha;
    // We have made it work, by using property of C++ that if
    // a function has two versions, variadic and non-variadic, then
    // compiler would choose the non-variadic function.
    cout << has_less<int>() << endl;
    cout << has_less<std::complex<double>>() << endl;

    static_assert(has_less<int>(), "Of course it is ok.");
    // You can change, has_less to be a struct... so that visual studio can make it work.
    // We want to put these has_less functions in the enable_if.
}

#endif

// Write a function that tells us that, if T1 derives from T2.

