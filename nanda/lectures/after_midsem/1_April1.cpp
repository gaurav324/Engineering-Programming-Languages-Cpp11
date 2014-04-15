// Inheritance has two subclasses Code-Reuse, Subtyping,polymorphism and O-O-P. Sir drew a tree for this.
//
// We are going to talk about the left side of the tree.
//
// class Foo {...};
//
// class Bar : public Foo {
//  void doit(void) {...}
// };
//
// Some gotchas:
//
// 1. Private interitance is default behavior. It is rarely useful. Implications are, public components of base class are private to the derived class. If we are doing private inheritance, we could have instead simply created a reference to Foo f in the derived class. That is how I would be apply to access the public members.
// 2. Copy constructors are inherited by default (check). Truly its not, but in practice it is. If you have specified a copy constructor, one is created by default.
// 3. Everything is inherited by default, except constructors (copy is sorta inherited, can explictly inherit constructors).
// 4. Overriden functions are hidden.
//
// class Base {
//  void doit(void) {...}
// };
//
// class Derived : public Base {
//  void doit(MyType) {...} // Overrides Base::DoIt.
//  // Overrides by name, not by signature.
//
//  // People coming from java should watch that, once you override a function by name, you hide all the base functions 
//  // with the same name.
//  //
//  // If you want to add a function back, then you explictly add that function back.
// }
//
// 5. Assignment operator has wrong return type.
// Derived a, b, c;
// a = (b = c)

// Ok, so how does inheritance work?
//
#include <iostream>
using namespace std;

#ifdef one
class Base {
public:
    int x;

    void doit(void) { x = 42; }
};

class Derived : public Base {
public:
    int x;
    int y;

    void der_doit(void) {
        y = 10;
        doit();
    }
};

int main() {
    cout << "Hello, world" << endl;
    Derived d;
    d.der_doit();
}
#endif
// Always keep in mind that derived class is just that an extension of the dervied class.
// If Base class source code is not available to me, we can still inherit from that class. It is trivial for the
// function to use the base class.

// Derived array[10];
// array[k].doit() // Works.
// Base* p = array; // This is wrong, as pointer arithmetic should not work correctly.
//

class Base {
public:
    int x;

    void doit(void) { x = 42; cout << "x is: " << x << endl; }
};

class Base2 {
public:
    int y;

    void doit2(void) { y = 142; cout << "y is: " << y << endl; }
};


class Derived : public Base, public Base2 {
public:
    int z;
};

int main() {
    cout << "Hello, world" << endl;
    Derived d;
    d.doit();
    d.doit2(); // Here compiler adds the offset to move it to the Y for Base2. and this now points to Base2.
    // Internally, its all about the offset the compiler has to use.

    Base* p = &d; // This always work.
    Base2* q = &d; // But this pointer conversion, requires an offset. Static cast, dynamic cast does calculates/uses offset.
}

// Another interesting thing.
//func (Base b) {
//}
//
//main() {
//    Derived d;
//    fun(d); // d would be truncated in the func Base class.
//}

// If you are using reference, then it is simply pointers that which are copied.
//func(Base& b) {
//    Derived& d = () b;
//}
// reinterpret_cast does not do the offset.

// protected in base class
// is available to derived class. But your clients cannot access.
//
// T::doit() {
//  R::X; // R must be a base class for T.  
// }

func(Base* p) {
    p->doit(); // select method at runtime. Type check at compile tume.
}

class Base {
    virtual void doit(void) {...} // This is do dynamic type checking.
    // A virtual method add an implicit data component to the Base class.
};

class Derived : public Base {
    void derived(void) {...}
};
