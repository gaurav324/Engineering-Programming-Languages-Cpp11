#include <iostream>

using namespace std;

#define One

#ifdef One
class Base {
public:
    int x;
    void doit(void) { cout << "Base" << endl; }

    // Only when you have virtual compiler adds RTTI entry in the table.
    virtual ~Base(void) {}
};

int main() {
    // Its not virtual, not pointer or reference. Compiler direectly points to b and make appropriate call.
    // Nothing dynamic is done. At this call, doit was not virtual.
    Base b;
    b.doit();

    cout << sizeof(b) << endl;
}
#endif

// b's structure would have another pointer because of RTII. This pointer would point to a block
// which would have two sections. One is for doit and another for destructor. Which is up or which is done, implementation dependent.

//|||||||||||||||
//|      x      |
//|||||||||||||||
//| Base:vftptr |
//|||||||||||||||
//
//
// Base:vft (This is just a label)
//|||||||||||||||
//|     doit    |
//|||||||||||||||
//|    ~Base    |
//|||||||||||||||

// Every time you create a new class, you create a new pointer table. 
// If b->doit(), pointer has to change it to something like *(b->vftptr[0])(). Something like this internally shuold happen.
// It is easy for the compiler to offset from the base, to vftptr and then to doit function in the second virtual table.

// This notes making thing is awesome. :) If you are reading this, I like dark choclates.
// Even if there is not hierarchy, compiler needs to produce all this virtual thing. Because someone might inherit 
// from this class someother day.
// 
// Summary:
// 1) If you write virtual, vftptr is created. Irrespective of the fact, there is a pointer call or not.
// 2) If b is a pointer/reference, b->doit(), would change it to *(b->vftptr[0])(). Otherwise, compiler would directly call the correct 
// object.

// He never understood, how the pointer to functions are written. Something like that he said.

void doit(void) {
    cout << "Bahar wala doit" << endl;
}

int main() {
    //using Ptr_fun = void (*)(void);

    // Language considers, address of doit and Base::doit as ir-reconcilably different.
    void (*p)(void) = &doit;
    p = &Base::doit;

    void(Base::*p)(void);
    (b.*p)();
}


//this->
//|||||||||||||||||
//
//Base1
//
//|||||||||||||||||
//
//Base2
//
//|||||||||||||||||
//
//Derived
//
//|||||||||||||||||
//
// If p is a pointer to a function from Base2, how we access that. Therefore, p has to be a pair of offset and machine address.
// Offset is added to the this(address of the actual object), and reach to the actual Base2 class. This is not specific to the 
// virtual function.


// Trampoline is called thunk. What is all this? I was not paying attention here. Sorry.
void Derived::doit() {
    Base1::doit();
}

void Derived::doit2() {
    Base1::doit2();
}

p = &Base1::doit; // Can you call this function on derived object.

//////////
Base* b;
b->doit();

///////////
How many virtual function tables?
Only 1.
They have functions for Base1, Base2 and Derived.

We are going to do some experiments on this.
pointer declared to base and use it on derived object.?
