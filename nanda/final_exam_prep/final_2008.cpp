#include <iostream>

using namespace std;
#define prob2

#ifdef prob1

template<bool val, typename T, int size>
class ArrayFixed;

template<typename T, int size>
class ArrayFixed<true, T, size>
{
public:
    T obj[size];

    T& operator[] (int index) {
        return obj[index];
    }
};

template<typename T, int size>
class ArrayFixed<false, T, size>
{
public:
    T *obj;

    ArrayFixed() {
        obj = new T[size];
    }

    T& operator[] (int index) {
        return obj[index];
    }
};

template<typename T, int size>
class Array : public ArrayFixed<(size <= 128), T, size> {
};

int main() {
    Array<int, 100> a; 
    Array<int, 150> b; 

    cout << sizeof(a) << " " << sizeof(b) << endl;
}

#endif

#ifdef prob2

class Base {
public:
    virtual ~Base() {}
};

class Derived : public Base {
    int* p;
public:
    Derived() {p = new int;} 
    ~Derived() { delete p; }
};

int main() {
    Base *b = new Base[5];
    Derived *d = new Derived[5];
    delete[] b;
    delete[] d;
}

#endif
