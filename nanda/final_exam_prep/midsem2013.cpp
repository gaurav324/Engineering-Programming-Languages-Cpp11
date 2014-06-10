#include<iostream>
#include <typeinfo>

using namespace std;

#define prob6

#ifdef prob3

class Base {
public:
    virtual void doit(void) { cout << "TRUE" << endl; }
    void foo(void) { doit(); }
};

class Derived : public Base {
    int x;
public:
    virtual void doit(void) { cout << "FALSE" << endl; }
};

template <typename T>
void tfun1(T x) { 
    x.doit();
}

template <typename T>
void tfun2(T x) {
    x.foo();
}

int main() {
    Base* array = new Derived[10];
    
    array->doit();
    array->foo();
    //(array+1)->doit();
    array[0].doit();
    //array[1].doit();
    tfun1(*array);
    tfun2(*array);
    cout << typeid(array[0]).name() << endl;
}
#endif

#ifdef prob4

#include <vector>
#include <string>

template<typename T, typename OP>
bool less_than(T a, T b, OP op) {
    return op(a, b);
}

template<typename T, typename op=std::less<typename T::value_type>>
typename T::value_type smallest(const T& container)
{
    typename T::value_type smallest = typename T::value_type{};
    for (auto p : container) {
        if (less_than<typename T::value_type, op>(p, smallest, op())) {
            smallest = p;
        }
    }
    return smallest;
}

template <typename T>
struct mera_less {
    bool operator() (const T& x, const T& y) const {return y > x;}
};

int main() {
    vector<int> v1 {10, 52, 6, 2, 5, 36, 7, 34, 46};
    cout << "Smallest of v1 is: " << smallest<vector<int>, mera_less<int>>(v1) << endl;
}

#endif

#ifdef prob6

class B {int a;};
class A : public B {double a;};

class B1 {double b;};

class D : public B, public B1 {float c;};

int main()
{
    //B *p; 
    //D *q;

    int *p = new int(10);
    double *q = new double(34);

    *p = *q;
    cout << sizeof(*p) << " " << sizeof(*q) << endl;
    //q = p;
    
    B *b = new B();
    A *a = new A();

    *b = *a;
    cout << sizeof(*b) << " " << sizeof(*a) << endl;
}

#endif

#ifdef prob7

class B;
class A {
public:
    virtual A* fun(int) = 0;
};

class B : public A {
public:
    virtual B* fun(int) = 0;
};

int main() {

}

#endif
