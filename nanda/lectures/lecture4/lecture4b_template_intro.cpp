// Second part of the lecture was more of an introduction to the
// templates.

/* There is a big difference between them. In C++ you don't have to specify a class or an interface for the generic type. That's why you can create truly generic functions and classes, with the caveat of a looser typing.
 * <typename T> T sum(T a, T b) { return a + b; }
The method above adds two objects of the same type, and can be used for any type T that has the "+" operator available.
 * In Java you have to specify a type if you want to call methods on the objects passed, something like:
 * <T extends Something> T sum(T a, T b) { return a.add ( b ); }
 */
#include <iostream>

using namespace std;

class Foo {
public:
    Foo(void) { cout << "Foo Constructor." << endl;}
    ~Foo(void) {cout << "Foo Destructor." << endl;}
};

// Template replacement in CPP, works like expanding macros.
// We would have different versions of the template class for
// different parameters passed.
template <typename T1, typename T2>
class TemplateFoo {
    T1 x;
    T2 y;
};

int main(void) {
    TemplateFoo<Foo, Foo> f;
}

