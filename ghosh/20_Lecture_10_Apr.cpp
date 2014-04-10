// WARNING: This file has not been checked for compilation!
//
// 04/10/2014 Lecture 20
// =====================

// C++ has a couple of pointer classes - shared_ptr and unique_ptr. Earlier in C++03 there was an auto_ptr. 
// Shared_ptr does reference counting and destructs on refcount reaching 0. Unique pointer moves around and the refcount is always 1.

// In the STL, there are a couple of ways you can get a shared_ptr. 
// If you already have an object and you want to get a shared_ptr with a reference count, the implementation has little choice apart 
// from allocating a control block and putting a pointer to the actual object in the control block. 
// Therefore there are 2 allocated areas. If you use make_shared, however then what it does is it creates the object 
// using a constructor invoked using variadic template, and adds the control block before the object data. 
// Chase wants this behavior in project 3 in the makemanaged<>() method.

// shared_ptr<T> ptr = make_managed<T>(args);

#include <iostream>
#include <stdint>

using namespace std;

struct ControlBlock {
    uint32_t ref_count;
    virtual ~ControlBlock(void) {}
};

template <typename T>
// Could we compose Managed from ControlBlock and T?
// Chase: I don't know! CHECK.
// In case of composition, still we need a single contiguous memory allocation
// make it like this: 
// struct Managed {
//     T obj;
//     uint32_t ref_count;
//     template<typename... Args>
//     Managed(Args... args) : obj(forward<Args...>(args)...) {}
// };
// Composition would probably not incur any of the hacks that we use for the inheritance case
struct Managed : public ControlBlock, public T {
    // Its possible that some of the arguments being passed in are references
    // or rvalue references. Therefore use std::forward to forward the arguments 
    // in correct format
    // What forward does is that it has template metafunctions to check whether something is a 
    // rvalue reference or not
    // CHECK: The below syntax for forward doesn't make sense but it works!
    template <typename... Args>
    Managed(Args... args) : T (forward<Args...>(args)...) {}
};

template <typename T>
class SmartPointer {
public: 
    // Logically static_cast is not required as we are going up the inheritance hierarchy
    // But since we know that there is an offset involved, static_casting for safety.
    T& operator*(void) const { return static_cast<T&>(*ptr); }
    T* operator->(void) const { return static_cast<T*>(ptr); }
    ~SmartPointer(void) {
        if (ptr) {
            ptr->ref_count -= 1;
            if (ptr->ref_count == 0) {
                delete ptr;
            }
        }
    }

    SmartPointer(T* rhs) {
        ptr = static_cast<Managed<T>*>(rhs);
        ptr->ref_count += 1;
    }

    SmartPointer(void) { ptr = nullptr; }

    SmartPointer(Managed<T>* ptr) {
        this->ptr = ptr;
        ptr->refs += 1;
    }

    // So the question about can you do this with composition, this is where it should be 
    // asked. It can be done with a hack. 
    // Managed: has T at the top, and control block at the bottom. 
    // 
    //
    //         their_ptr ---> -------------
    //                       |      X      |
    //                        -------------
    //                       |      Y      |
    //         their_cb-----> -------------
    //                       |     CB      |
    //                        -------------
    //
    // Visual studio prefers to align the polymorphic type at the beginning (if base class of
    // Managed is polymorphic), and then control block is pushed to the bottom. That results in
    // a crash when the main is invoked. In project 3, we will be using event handlers in the 
    // form of raw pointers -> have to go from raw pointers to the managed object. 
    // The solution to this is to make ControlBlock as a polymorphic base class -> that makes 
    // Visual Studio and gcc both behave the same way and align the objects as provided in the
    // order of inheritance. 
    template <typename U>
    SmartPointer(const SmartPointer<U>& rhs) {
        static_assert(std::is_base_of<T, U>::value, "dude! you can't convert that way!");
        Managed<U>* their_ptr = rhs.ptr;
        ControlBlock* their_cb = static_cast<ControlBlock*>(their_ptr);
        Managed<T>* my_ptr = static_cast<Managed<T>*>(their_cb);
        ptr = my_ptr;
        ptr->ref_count += 1;
    }

    // This is for the if (ptr) check for SmartPointer - this enables the C/C++ style
    // check for a pointer being null or not. This is provided by the underlying Managed*
    // object which is itself a pointer.
    operator bool(void) const { return ptr; }

private:
    Managed<T>* ptr;
};

template <typename T, typename... Args>
SmartPointer<T> makeManaged(Args... args) {
    Managed<T>* ptr = new Managed<T>{std::forward<Args...>(args)...};
    ptr->ref_count = 1;
    return SmartPointer<T>(ptr);
}

class Base {
protected: 
    int x;

public:
    Base(void) { x = 42; }
    virtual void doit(void) { cout << " Base doit: " << x << endl; }
    virtual ~Base(void) { cout << " Base destructor: " << x << endl; }
};

class Derived : public Base {
private:
    int y;

public: 
    Derived(void) { y = 17; }
    virtual void doit(void) { cout << "Derived doit: " << x << " " << y << endl; }
    virtual ~Derived(void) { cout << "Derived destructor: " << x << " " << y << endl; }
};

#ifdef _NO_ABSTRACT_
// Probably the best way for Project 3 is to have a ref_count in the Base object itself.
// Stop pretending that the stuff works for all T and just assume that the Base class contains a ref_count
int main(void) {
    SmartPointer<Base> p = makeManaged<Base>();
    p->doit();

    SmartPointer<Base> q = makeManaged<Derived>();
    q->doit();

    SmartPointer<Derived> r = q;

    return 0;
}
#endif

// The syntax that C++ chooses for abstract methods is horrible.
// It is very common for developers to create an interface for the base class.
// Java has the interface keyword to define a class without any implementation. 
// If you think about it, there is no such thing as a cake. Any cake has some kind of 
// flavor. The concept of a cake provides a nice reasonable interface. In OOP we have a 
// mechanic to provide an interface using abstract classes. 

class Cake {
public:
    virtual void eat(void) = 0;

protected:
    int layers;
    // Does it make sense to have a constructor for an abstract class?
    // Yes it does, because if the base class is providing any functionality 
    // it needs to be initialized.
    // When ChocolateCake is created it calls the default constructor of Cake and inits layers = 0
    Cake(void) {
    
    }
};

class ChocolateCake : public Cake {
public:
    virtual void eat(void) { cout << " yum!" << endl; }
};

int main(void) {
    // Once I create a pure virtual method in my abstract class Base
    // you cannot create a variable of that type. 
    // Cake c;
    ChocolateCake c;
    Cake* p = &c;
    Cake&q = c;
}

