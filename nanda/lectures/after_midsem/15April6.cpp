// Today I would be listening in the lecture.
//

Number x{5};
Number x{3.5};

x = y + 1.0; // This is a problem ?

I want to have polymorphic numbers, (rational/or you can have big integers/complex.. all kind of subtype are numbers). Integer and
floating point have enough flexibility.

I am picking numbers, due to operator overloading wali thing.

Number (Parent) --> Integer and Double.

== 
Number Y {X};
// But this must fail. Since Y is a declared as a base class.
// This is a fundamental issue.
// Y is going to be the base class.
//
// If you want to create polymorphic duplicate, you simply ask the object to duplicate itself (Type and state).
//
// Cake *a = ... (some choclate cake);
// Cake *b = a->clone();
//
// Make clone in virtual method in the base class.
//
// Sir, want to reconcile this solution with the obvious failure. What do we do in general ??? we bring indirection. yeayy !!!
//
// We created SmartPointer earlier for this kind of thing. But here we would not want to do that kind of thing.
// 
// SmartPointer<Number> y = ...;
// Number y{x};
//

// Here starts the real solution, after all the background.
//
class BaseNumber {
    void add(BaseNumber* x) = 0;

    virtual BaaseNumber* clone(void) const = 0;
}

class Integer: public BaseNumber {
    int value;

    // Should we replace BaseNumber with a Integer here?
    //  - We can first of all, because it is a child.
    //  - We should not ideally, because why would someone want to return Integer from clone.
    //    If you want to do that, you can simply use "new Integer{*this}".
    virtual BaseNumber* clone(void) const {
        return new Integer {*this};
    }
}

class Number {
    Base_Number *ptr;
public:
    Number(const Number& rhs) {
        ptr = rhs.ptr->clone();
    }

    void add(Number rhs) {
    
    }
}

// All he cares about is, add two integers, we want integers.
// If we want to ad two floating points, we want floating points.
//
// There is some classic EPL final exam question, sir is talking about. which we have to solve 
// without if statements.
//
// Instead of doing if/else cases in the add function of the wrapper. We would add overloaded operators in the BaseNumber class.
// These are going to be abstract in the base class.
//
// Why cant we make our virtual functions to be templatized?
//  - There is something class virtual function table. Implementation insists that entries in the Vtable are 
//  statically determined.
//
//  For a function like, virtual void doit(T *) { ... };
//  We don't know what all type of T can be there, so CPP is not sure that how many entries are there in the V-Table.
//  
//  Sir, wanted to do some run time selection. That does not seem to work.
//  BaseNumber *t = ptr->add(rhs.ptr); // This is panga. Because rhs.ptr is BasePtr type, and functions in the BaseNumber
//  are like "virtual BaseNumber* add2(Integer*); virtual BaseNumber* add2(Double*); ".. so this is compile error.
//
// Ok, now this gets interesting.
// We have two verisons of the add. Add and Add2.
//
// BaseNumber* add(BaseNumber* rhs) { return rhs->add2(this); }
//
// Looking something like:
class BaseNumber {
    void add(BaseNumber* x) = 0;

    virtual BaaseNumber* clone(void) const = 0;

    virtual BaseNumber* add(BaseNumber* rhs) = 0;

    virtual BaseNumber* add2(Integer* rhs) = 0;
    virtual BaseNumber* add2(Double* rhs) = 0;
}

// Now, this kind of code would run:
//
// BaseNumnber* t = ptr->add(rhs.ptr);

/////////// Now Sir are going to tell us some other problem, which we would solve in another awesome way ////////////
// This would be the topic of the project3. 
//
// Base* factoryMethod (String type) {
//  if (type == "Craig") { return new Craig; }
//  else..
//  else..
// }
//
// One approach is to have the Map<String, <exemplar>>. Only problem is to have the exemplars (objects) in the map.
//
// Sir, is going to put functions.
// Map<String, function<Base* (void)>>
//
// All objects are having the default constructors.
// void buildMap (void) {
//  creators["craig"] = craig_func;
//  creators["algae"] = algae_func;
//
//  // Good thing is that all the sub-classes can add their entry to the map. Use global static map.
// }
