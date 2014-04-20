/*
Notes - 03/20/2014
------------------

auto t = (x + y).begin() -> iterator should be invalid.
auto t = (x + y);
auto p = t.begin() -> this should be a valid iterator.
*p -> this would cause evaluation. 

Below case fails as the destructor is run before the code enters the loop.
for (v : x + y) {
	cout << v;
}

Need to use this-> to call base class functionalities. 
Since you use this-> it realizes that it doesn't know what T is, so the this-> needs to be disambiguated, which is why the compiler does not crib.

Concept checking - making sure the template you are instantiating is credible. We wanna put a restriction in some way [eg. require T < T exist]
----------------

Made a constexpr function that is a compile time check. Concept checking has been in boot library since a long time. 

*/

#include <cstdint>
#include <iostream>
#include <complex>
#include <utility>
#include <type_traits>

using namespace std;

template <typename T>
class BinarySearchTree {

};

#ifdef _1st_TRICK_
// Substitution failure is a tool we can use
// I have a template function that is overloaded, one that results in a substitution failure
// and one that doesn't 
template <typename T, typename unused = decltype(T{} < T{})>
bool has_less(void) {
	return true;
}

template <typename T>
bool has_less(void) {
	return false;
}
#endif 

#ifdef _2nd_TRICK
// Have inadvertently put a default constructor dependancy
// The const T& is needed as gcc & VS intellisense think that (void) and 
// ... both are still ambiguous, but VS compiler goes through. 
template <typename T, typename unused = decltype(T{} < T{})>
bool has_less(const T&) {
	return true;
}

template <typename T>
bool has_less(...) {
	return false;
}
#endif

// THE DEFINITION OF THE FUNCTION IS NOT BEING EXECUTED!! EVERYTHING HERE
// WORKS WITH THE DECLARATIONS! MIND-F*ING-BLOWN!
// When a function is declared, compiler can perform overload resolution, 
// when it performs overload resolution it can deduce types 
// and that is what we need.
template <typename T, typename unused = decltype(std::declval<T>() < std::declval<T>())>
std::true_type has_less_helper(const T&); /* {
	return std::true_type{};
} */

template <typename T>
std::false_type has_less_helper(...); /* {
	return std::false_type{};
}
*/
template <typename T>
constexpr bool has_less(void) {
	using my_type = decltype(has_less_helper<T>(std::declval<T>()));
	return my_type::value;
}

// declval is in the std library, its a function you can't run
// but if you need to get at the type T, and you can use declval

int main(void) {
	cout << boolalpha;
#ifdef _1st_TRICK_
	// Below invocation is ambiguous, so there is no substitution failure in both cases
	// Compiler has no idea which one it is supposed to call
	cout << has_less<int>() << endl;
	cout << has_less<complex<int>>() << endl;
#endif 
	
#ifdef _2nd_TRICK_
	// If the compiler has a variadic function, and it has a non variadic function for 
	// some invocation, the compiler prefers the non-variadic function. Buried deep
	// in C++ trivia!
	cout << has_less<int>(0) << endl;
	cout << has_less<complex<int>>(std::complex<double>{0}) << endl;
#endif
	cout << has_less<int>() << endl;
	cout << has_less<complex<int>>() << endl;
	// Below works with constexpr only, so has_less needs to be constexpr.
	static_assert(has_less<int>(), "of course INT is ok.");
}