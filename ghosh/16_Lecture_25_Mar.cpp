// Syllabus for Test
// Knowing what the STL style is - generic algorithm (such as sort) which has a container and a function, 
// the container is passed in as a pair of same type iterators. That's the convention of the C++ STL (always pass iterators, not containers).
// When you have a function, it could be a pointer to a function but that is atypical. The function is passed in as a class that has a 
// operator(). std::less is a good example of a C++ function. 

// Operator syntax of new/delete vs function syntax - probably wont test on this 
// Copy on write - design technique is not terribly useful anymore due to move semantics
// If COW is explained and asked to analyze the approach wrt move semantics for some context.

// Definitely reference counting and smart pointers
// Auto and decltype - what would you do if iterators did not have value_type
// iterator_traits remember the two main components value_type and iterator_category
// Expect to be familiar with the algorithm selection mechanism (old type with sneaky parameter in the end)
// new way: enable_if with iterator tags

// if statement inside gcc inside a constexpr is probably not allowed - CHECK!

// Selecting a type, deciding between a reference or a copy of the subtree
// enable_if specifically has a technique
// Go easy on SFINAE - remember that what we do with enable_if is removing a function from overload resolution
// Can't enable_if a class, you can use it on a member function, to remove it from overload resolution
// could be asked to write std::is_same, template metafunction to use iteration instead of recursion

// variadic template (class and expression functions) are absolutely inbounds
// expression templates is a general technique

// for legacy reasons the keyword class is accepted instead of typename in a template 
// class does not mean class in this case

#ifdef _EXAMPLE_
template <typename... Args>
struct tuple {};

// Cannot have variable number of fields in tuple to match the number of types
// The trick is to inherit from tuple<Args> so that you only need to have 
// a field of type Head and rest are inherited from tuple<Args>
// Curiously Recursive Template Patterns
template <typename Head, typename... Args>
struct tuple : public tuple<Args> {
	Head field;
};

tuple<int, int, string> x;
x.get<2>();
#endif

// auto x = make_pair(a, b) is a much much nicer way to specify a pair than the old 
// method pair<typeA, typeB> x = pair<typeA, typeB>(a, b);

// Template argument deduction happens all the time, and it helps make code easier to read

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// BaseExample

#include <iostream>
#include <cstdint>

class BaseExample {};

class DerivedExample : public BaseExample {};

// The type has to be of the same type as static casting D to B, problem is if they are not 
// related via inheritance hierarchy, you cannot static_cast them but can C-style cast them
template <typename B, typename D>
std::true_type is_derived_helper(const B&, decltype(static_cast<const B&>(std::declval<D>())));

template <typename B, typename D>
std::false_type is_derived_helper(...);

template <typename B, typename D>
struct isBaseClass {
	using type 
		= decltype(is_derived_helper<B, D>(std::declval<B>(), std::declval<D>()));
	static const bool value = std::is_class<D>::value && type::value;
};

// There are 4 C++ casts
// static & dynamic casts: exactly the same syntax. These are intended for use within related types with inherited hierarchy.
// static is a compile time assertion that the objects are related. dynamic cast relies on the VFn table to do a runtime type
// check to assure that the the types are actually the same. dynamic_cast throws an exception in case the object being cast is 
// a reference and it returns a null pointer in case the object is a pointer.
// reinterpret_cast: I know what the hell I am doing, please allow me to do this. Its like a C style case. There are very few things
// that are not allowed, casting a function to a member function is one example.
// const_cast: adding or removing constness of a variable

int main(void) {
	std::cout << "Hello World!\n";
	std::cout << std::boolalpha;
	std::cout << isBaseClass<BaseExample, DerivedExample>::value << std::endl;
	std::cout << isBaseClass<DerivedExample, BaseExample>::value << std::endl;
	std::cout << isBaseClass<int, double>::value << std::endl;
}

// Can't put the count in the pointer, have to put the count in the object
// Chase wnated to make a class that would implement the reference counting itself (CtrlBlk)
// An object that would have the smart pointers, could probably derive from CtrlBlk or point to CtrlBlk
// More efficient if the object and the CtrlBlk are in the same piece of memory - easiest way 
// would be to have CtrlBlk as a base class. The context where this is coming from is to decide 
// whether the CtrlBlk is a base class of the object so as to decide which design to use. 