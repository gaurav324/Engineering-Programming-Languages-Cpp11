// Lecture 21 - 04/15/2014
// -----------------------

// Polymorphic constructors
// ------------------------

// Number <----- Integer
//        <----- Double

// Number y {x};   // This must fail since y is declared as the base class

// If you want to create a polymorphic duplicate, how do you do it?

// Cake* a = ...;
// Cake* b = a->clone();   // returns a constructed pointer via a virtual method
// Clone is a duplicated object, and any references to b do not refer to a and vice versa.
// When the code is invoking that method, clone will select the method appropriate for the real object.

// To reconcile the above failed scenario with the clone here, we will devise a way using proxies.

// Base_Number    <---- Integer
//                <---- Double 

// In the design, x and y will be of a certain type on the stack which will have a pointer to the real object on the heap. 
// When we did this type of stuff before, we would have made x and y SmartPointer. 
// Syntax of working with the object is pointer, which is a disadvantage. 
// But thats not what we are looking for. We want y to look like a number, not a pointer. 

#include <iostream>
#include <cstdint>

#ifdef _BASIC_

// The root of the inheritance hierarchy is abstract
class Base_Number {
    void add(Base_Number* x) = 0;
    virtual Base_Number* clone(void) const = 0;
};

// Integer and Double look very similar as far as the clone method go
// const methods can be called in pretty much any context, other methods cannot
// be called from a const object. In C++ you put const in pretty much any where you can.
class Integer : public Base_Number {
    int value;
    // Q: Could I have declared this to return Integer* and still be considered overriding?
    // Answer: Yes. As long as what I am returning is a derived class of Base_Number, it will work.
    // There is probably no advantage to declaring it either way. 
    // Q: If I was inside the Integer class and use this then will have to downcast it to Integer* again.
    // Answer: Why not then use the copy constructor in those cases?
    // Bottom line - you are allowed to declare the return type as a sub type of the base class
    virtual Base_Number* clone(void) const {
        return new Integer{*this};
    }
};

// Invoking the constructor in C++ is a compile time choice - static.
// Cannot do a runtime selection of the constructor based on the type of the object.

class Double : public Base_Number {
    double value;
    // virtual is very important in C++ - you shouldn't be allowed to have two different kinds of syntaxes
    // In the derived class you could either use virtual or not but both have the same effect
    virtual Base_Number* clone(void) const {
        return new Double{*this};
    }
};

// If there is something that is available via the type interface Base_Number 
// must be duplicated via the Number wrapper interface.
// In case of numbers defined only in Integer - they might be interesting in some context
// but in today's example he is concerned only with common functions
class Number {
    Base_Number* ptr;
public:
    Number(const Number& rhs) { 
        ptr = rhs.ptr->clone();
    }

    Number add(const Number& rhs) const;
};

// This defeats the entire idea of object oriented programming as you're dynamic casting
// down the inheritance hierarchy - which you should never have to do. Here you have one 
// method in your code which is enumerating all the sub types and downcasting. This is ugly.
Number Number::add(const Number& rhs) const {
    if (dynamic_cast<Integer*>(ptr) {
        if (dynamic_cast<Double*>(rhs.ptr) {
            return Number(new Integer(...));
        }
    }
    else if (dynamic_cast<Double*>(....
}

#endif

// Virtual Member functions can never be templates
// Templates require instantiation at compile time and virtual functions are deciphered at runtime.
// The implementation of C++ defines that the number of entries in the VFn table be static and decided at compile time.
// If Base_Number had two overloads: virtual Base_Number* add(Integer *) and virtual Base_Number* add(Double *) and the class 
// Number invokes Base_Number* t = ptr->add(rhs.ptr) the compilation fails as it fails to find an overload which
// takes a Base_Number* pointer. 

#ifdef _DDD_

// Solution is double dynamic dispatch!
// C++ gives us the semantics and Java gives us the same. 
// Method semantics depends on the LHS but not on the RHS

class Base_Number {
public:
    virtual Base_Number* add(Base_Number* x) = 0;
    virtual Base_Number* clone(void) const = 0;
    virtual Base_Number* add2(Integer *) = 0;
    virtual Base_Number* add2(Double *) = 0;
};

class Integer : public Base_Number {
    int value;
public:
    virtual Base_Number* add(Base_Number* rhs) {
        return rhs->add2(this):
    }

    virtual Base_Number* add2(Integer* rhs) {
        return new Integer(value + rhs->value);
    }

    virtual Base_Number* add2(Double* rhs) {
        return new Double((double)value + rhs->value);
    }

    virtual Base_Number* clone(void) const {
        return new Integer{*this};
    }
};

class Double : public Base_Number {
    double value;
public:
    virtual Base_Number* add(Base_Number* rhs) {
        return rhs->add2(this):
    }

    virtual Base_Number* add2(Integer* rhs) {
        return new Double(value + (double)rhs->value);
    }

    virtual Base_Number* add2(Double* rhs) {
        return new Double(value + rhs->value);
    }

    virtual Base_Number* clone(void) const {
        return new Double{*this};
    }
};

class Number {
    Base_Number* ptr;
public:
    Number(const Number& rhs) { 
        ptr = rhs.ptr->clone();
    }

    Number add(const Number& rhs) const {
        Base_Number* t = ptr->add(rhs.ptr);
        return Number(t);
    }
};

#endif

// This is something similar to Project 3
// This is the naive approach where you do a case split 
Base* factoryMethod(String type) {
    if (type == "Craig") { return new Craig; }
    else if (type == "Algae") { return new Algae; }
}

// Another way is to create a map of string and an exemplar object
// and lookup the string and get the exemplar object on which you can call the
// copy constructor
// Chase's method assumes that all objects are default constructible
// Below is a map of a string to a function that takes no parameters and returns a Base* object
std::map<string, function<Base*(void)>> creators;
Base* factoryMethod(string type) {
    // This returns an object on the heap
    return creators[type]();
}
