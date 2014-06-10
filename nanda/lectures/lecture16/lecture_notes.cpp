// Topics for the exam:
// 
// Project - 1:
//
// Copy and move.
// rvalue references, move semantics, copy semantics, copy constructors. (copy on write may be there when comparing against rvalue references <less chance though>).
// iterators and generic programming.
//
// STL generic programming. Sort generic algorithm. It has container and function, container is passed with iterators. Always pass iterators and never containers. Always implement a function object, but never pointer to function, because that cannot be inline.
//
// std::less, check this.
// Initializer lists.
// new and delete. Placement new. Operator new and placement new. (Won't be there on exam.)
// Reference counting. Smart pointers.
//
// Project - 2
//
// auto and decltype.
// old stl conventions. What if new conventions are not there. What is value_type is not there. use decltype or something like that.
// iterator_Traits value_type, iterator_category (various tags in iterator category).
// (algorithm selection) how choose algorithm is choosen, given the iterator category. (how it was done earlier). enable_if is new way, old way is to overload.
// How can you compare two types? (is_same)?
// enable_if technique. check for compile constants.
// substituion failures not an error.
// enable_if only a function/member function, but never a class.
// template meta functions with recursion.
// variadic templates. (structs and functions)
// expression templates as a general technique.
//
// May be not on exam: 
// template <typename Head, typename... Args
// struct tuple : public tuple<Args...> {
//     Head field;
// }
//
// Template argument detection happens only for functions, but not for classes.
//
// There are four cast. static and dyanamic cast, exact same syntax for related types. static cast tells that on compile time, I know this is correct. Compiler permits if they are related. Dynamic cast relies on virtual function table to do a runtime type check. re_interpret cast works as C style.
// 
