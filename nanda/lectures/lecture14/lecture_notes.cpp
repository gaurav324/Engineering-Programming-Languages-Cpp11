// Valarray<int> x(10);
// x = 5.5; // Populates all the elements in x to be 5.5
//
// Valarray<int> y {x + x}; // This should also be there.
//
// apply(f);
// F f;
// F::result_type is well defined.
// Implement sqrt by calling apply function. and you should be able to build
// function object of correct characteristics.
//
// (X + X).push_back(42); should not work, it must be an error.
//
// MKS //

// SFINAE
// has to do function argument overloading. (function overload resolution)
// template specialization is different problem.
//
// two or more functions with same name, compiler has to select one of those,
// looks at type of argument, compares with declared types and select the best type.
//
// template <typename T>
// T::value_type doit (T x) {
//     
// }
//
// template <typename T>
// T doit(T* p) {
//     return *p;
// }
//
// INVOCATION:
// vector v;
// doit(v.begin());
//
// int array[10];
// doit(array); // This one matches second, because there is error with a 
//              // declaration of first one.
//
// Idea is to have a substituion error, you have it in template argument names
// return type and argument type.
