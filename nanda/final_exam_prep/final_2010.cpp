#include<iostream>
#include<functional>

using namespace std;
#define prob6

#ifdef prob1

class A {
public:
    A() {}
    virtual ~A() { cout << "A" << endl;}
};

class B : public A {
public:
    char* st;
    B() {st=nullptr;}
    B(int n) {st=new char(n);}
    
    ~B() {
        cout << "B" << endl;
        if (st) {
            delete st;
        }
    }
};

int main() {
    A *a = new B[10];
    delete[] a;
}

#endif

#ifdef prob1e

class A {
public:
    int v;
}; 

class B : public A {

};

class X {
};

class C { 
public: 
    virtual A doit() { 
        A *a = new A(); 
        return *a;
    }
};

class D : public C { 
public: 
    B doit() { 
        B *x = new B(); 
        return *x; 
    } 
};

int main() {
    cout << "hi" << endl;
    C *c = new D();
    A a = c->doit();
}

#endif

#ifdef prob2

class A {
    static void f1();
    void f2();

    static int a;
    int b;

    virtual void funA() {};
    virtual void funB() {};
};

int main() {
    A a;
    cout << sizeof(a) << endl;
}
#endif

#ifdef prob2b

template<typename T>
class Initializer {
public:
    int& count(void) { static int x = 0; return x; }
    
    Initializer(void) {
        if ((count())++ == 0) {
            T::initialize();
        }
    }
};

class Shape {
public:
    std::function<Shape*(void)>
    static updateAllocator(int size, std::function<Shape*(void)> x) {
        static int max = -1;
        static std::function<Shape*(void)> p;
        if (size > max) {
            p = x;
            max = size;
            cout << "Update size to " << max << endl;
        }
        return p;
    }

    static Shape* alloc() {
        return updateAllocator(-1, nullptr)();
    }

    virtual void doit() = 0;
};

class Triangle : public Shape {
private:
    int x;
    double y;
public:

    static void initialize() {
        Shape::updateAllocator(sizeof(declval<Triangle>()), Triangle::create);
    }

    static Shape* create(void) {
       return new Triangle;
    }

    virtual void doit() {
        cout << "Triangle" << endl;
    }
};

Initializer<Triangle> __my_init;

class Rectangle : public Shape {
private:
    int x;
    double y;
    double z;
public:

    static void initialize() {
        Shape::updateAllocator(sizeof(declval<Rectangle>()), Rectangle::create);
    }

    static Shape* create(void) {
       return new Rectangle;
    }

    void doit() {
        cout << "Rectangle" << endl;
    }
};

Initializer<Rectangle> __my_rect_init;

int main() {
    Shape* p = Shape::alloc();
    p->doit();
}

#endif

#ifdef prob6

class IInteger;
class DDouble;

class BaseNumber {
public:
    virtual BaseNumber* divide(BaseNumber*) = 0;
    virtual BaseNumber* divide(IInteger*) = 0;
    virtual BaseNumber* divide(DDouble*) = 0;
    virtual void print() = 0;
    virtual BaseNumber* clone(void) const = 0;
};

class IInteger : public BaseNumber {
public:
    int val;
    IInteger(int v) {
        this->val = v;
    }
    BaseNumber* divide(BaseNumber *other);

    BaseNumber* divide(IInteger *other);

    BaseNumber* divide(DDouble *other);
    
    void print();

    BaseNumber* clone(void) const;

    int getVal();
};


class DDouble : public BaseNumber {
public:
    double val;
    DDouble(double v) {
        this->val = v;
    }

    BaseNumber* divide(BaseNumber *other) {
        other->divide(this);
    }

    BaseNumber* divide(IInteger *other) {
        return new DDouble(other->getVal() / this->val);
    }

    BaseNumber* divide(DDouble *other) {
        return new DDouble(other->getVal() / this->val);
    }

    void print() {
        cout << val << endl;
    }

    BaseNumber* clone(void) const {
        return new DDouble(*this);
    }

    double getVal() {
        return val;
    }
};

    BaseNumber* IInteger::divide(BaseNumber *other) {
        cout << "I-1" << endl;
        other->divide(this);
    }

    BaseNumber* IInteger::divide(IInteger *other) {
        cout << "I-2" << endl;
        if (other->val % this->val == 0){
            return new IInteger(other->getVal() / this->getVal());
        } else {
            return new DDouble(1.0 * other->getVal() / this->getVal());
        }
    }

    BaseNumber* IInteger::divide(DDouble *other) {
        return new DDouble(other->getVal() / this->getVal());
    }

    void IInteger::print() {
        cout << val << endl;
    }

    BaseNumber* IInteger::clone(void) const {
        return new IInteger(*this);
    }

    int IInteger::getVal() {
        return val;
    }

class Number {
private:
    BaseNumber *base;
public:
    Number() { base = nullptr; }

//    Number(long v) {
//        base = new IInteger(v);
//    }
    
    Number(const Number& rhs) {
        base = rhs.base->clone();
    }
//    Number(double v) {
//        base = new DDouble(v);
//    }
    
    Number operator/(Number other) {
        Number* result = new Number();
        result->base = base->divide(other.base);
        return *result;
    }

    void print() {
        base->print();
    }
};

int main() {
    Number x, y, z;
    x = 30;
    x.print();
    y = 6;
    y.print();
    z = x/y;
    z.print();
    z = z/y;
    z.print();
}
#endif
