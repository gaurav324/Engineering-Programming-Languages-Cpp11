#include <iostream>
#include <cstdint>

class Foo {
public:
    Foo(void) { std::cout << "default constructor\n"; }
    ~Foo(void) { std::cout << "destructor\n"; }
};

// Malloc in C provides uninitialized memory, but in C++ new provides initialized memory
// Explicit destruction p->~Foo();
// For assignment, separate out allocation and initialization, deallocation and deinitialization
void doIt(void) {
    // Operator syntax for new does allocation and initialization both
    // However method syntax for new does not do initialization. It expects the size in bytes just like 
    // malloc and returns a void * pointer. Also: ::operator new
    // New with [] allocated more space than array of 10, saves number of items before the space
    // Using method syntax of delete with the new[] syntax crashes as free() needs a pointer to the 
    // start of the allocated block of memory
    Foo* p = (Foo *) operator new(10 * sizeof(Foo));
    // p->Foo() does not work. Any context that is calling a constructor is supposed to be creating 
    // a brand new object
    for (uint32_t k = 0; k < 10; k++) {
        // Placement new - operator syntax. This informs the compiler that the space has already been
        // allocated, just the constructor needs to be invoked. 
        new (p + k) Foo{};
    }
    for (uint32_t k = 0; k < 10; k++) {
        p[k].~Foo();
    }
    // You cannot write an operator to implement '.' and '?'
    // An operator can be invoked using the operator syntax X + Y or the
    // method syntax: operator +(X, Y).
    // For all operators, other than new and delete there is no difference between the operator syntax
    // and method syntax. Operator syntax for delete calls the destructor and then calls free. However
    // the method syntax only calls free. Also: ::operator delete
    operator delete(p);
    // BTW deleting stuff right before main() exits is fundamentally stupid :D
}


class Bar {
public:
    Bar(int) {

    }

    Bar(const Bar& that) {

    }
};

// ?? In Java machine code is produced compiled in a library and made available to everyone
// Advantage in Java and C# world is that there is only one implementation of the generic type
// C++ works in a more simplistic way - the template can be parsed, but not compiled. It is a glorified macro. 
// It does not result in any machine code. After one requests a template implementation, C++ does a text substitution 
// with Foo for T and then emits code. 
// TEMPLATE CLASSES CAN ONLY BE WRITTEN IN HEADER FILES!! CANNOT WRITE WITH IMPL AND H SEPARATED! :D
template <typename T>
class Vector {
public:
    Vector(void) {

    }

    // Below constructor assumes that T can be default constructed
    explicit Vector(int initial_size) {
        buffer = new T[initial_size];
    }
    
    T* buffer;
    T& operator[](int k) {
        return buffer[k];
    }

    // In C++ when a template class is initialized, it creates the layout for the template class
    // and only those parts of the template class that is used (invoked explicitly). Here the oops
    // method is not invoked, so it will not crib about the function call below that does not exist.
    T oops(void) {
        T::var_name = 42;
        T::type_name my_var;
        T k = *buffer;
        *k = k.does_not_exist();
    }
};


// Templates are expanded in compile time, and it can be expanded recursively. 
// This makes this feature ridiculously powerful.
// Initially templates used to have 'class' instead of 'typename'. 
// Could have used 'type' instead, but would have broken a lot of C programs but had to maintain backwards compat.
// Methods inside a template are not created/syntax-checked/error-checked until they are used: people who 
// created template libraries, left ugly errors in the code. That is why, now templates need to be atleast
// syntactically sound. 
template <typename T1, typename T2>
class Fred {
    T1 x;
    T2 y;
};

template <typename T>
void myFun(void) {
    T::var_name = 42;
    typename T::type_name my_var;
}


int main(void) {
    Vector<Foo> f;
    Vector<int> g;
    f[10] = Foo{};
    g[3] = 42;
    // As far as C++ is concerned, f and g are fundamentally unrelated types

    Vector<Bar> b;
    // Wont work, as Bar does not have a default constructor
    //Vector<Bar> b2(10);
}