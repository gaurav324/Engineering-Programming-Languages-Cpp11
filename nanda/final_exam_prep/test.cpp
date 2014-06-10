#include <iostream>
#include <string>

using namespace std;

#define prob_test

#ifdef prob1
class A {
public:
    string x;

    A(string x) {
        this->x = x;
    }
};
   

int func(int b) {
    static int a;

    if (b>a)
        a = b;
    return a;
}

int main() {
    cout << func(5) << endl;
    cout << func(10) << endl;
    cout << func(1) << endl;
}
#endif

#ifdef prob2

class A {
public:
    int v;
    virtual ~A(){}
};

class B : public A {
};


class C {};

template<class T>
class IsPolymorphic
{
private:
    class Derived: T { virtual ~Derived() {} };
public:
    enum{ yes = (sizeof(Derived) == sizeof(T)), no = !yes };
};


int main() {
    A a;
    B b;

    cout << IsPolymorphic<C>::yes << endl;
}

#endif

#ifdef prob_test

template<typename T>
void ok(T& x) {}

template<typename T>
void notok(T x) {}

class T
{
public:
    virtual void doit() = 0;
};

class S : public T {
    virtual void doit() {}
};

int main() {
    S x;
    
    ok<T>(x);
    //notok<T>(x);
}

#endif
