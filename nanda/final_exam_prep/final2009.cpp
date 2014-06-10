#include <iostream>

using namespace std;

#define last

#ifdef prob1

template <typename T>
struct object_traits
{
    static constexpr bool is_basetype = false;
    static constexpr bool is_pointer = false;
    static constexpr bool is_classtype = std::is_class<T>::value;
    using deref_type = T;
};

template <typename T>
struct object_traits<T*>
{
    static constexpr bool is_basetype = false;
    static constexpr bool is_pointer  = true;
    static constexpr bool is_classtype = false;
    using deref_type = T;
};

template <>
struct object_traits<int>
{
    static constexpr bool is_basetype = true;
    static constexpr bool is_pointer  = false;
    static constexpr bool is_classtype = false;
    using deref_type = int;
};

template <>
struct object_traits<float>
{
    static constexpr bool is_basetype = true;
    static constexpr bool is_pointer  = false;
    static constexpr bool is_classtype = false;
    using deref_type = float;
};

template <>
struct object_traits<double>
{
    static constexpr bool is_basetype = true;
    static constexpr bool is_pointer  = false;
    static constexpr bool is_classtype = false;
    using deref_type = double;
};

class A {
public:
    int val;
    A() {
    }
    A(int v) {
        val = v;
    }
};

class B : public A {
public:
    B(int v) : A(v) {}
};

class UCF {
public:
    template <typename T>
    static T copy(T x) {
        return T(x);
    }

    template <typename T>
    static typename std::iterator_traits<T>::pointer_type 
        copy(T* x) {
            T ans = new T(*x);
            return ans;
    }
};

int main() {
    const B b(15);
    const int i = 100;
    const double d = 34.35;
    const A *a = new A(1234);
    cout << object_traits<B>::is_classtype << endl;
    cout << object_traits<int>::is_basetype << endl;
    cout << object_traits<double>::is_classtype << endl;
    cout << object_traits<A*>::is_pointer << endl;

    cout << UCF::copy(i) << endl;
    cout << UCF::copy(b).val << endl;
    cout << UCF::copy(a)->val << endl;
}

#endif

#ifdef last

class A {
public:
    int x;
    virtual ~A() {}
};

class B {
public:
    double y;
    virtual ~B() {}
};

class C : public A, public B {
public:
    float x;
};

int main() {
B *p;
A *q;

C x;

p = &x;
q = &x;

if ((void*)p == (void*)q){
    cout << "False" << endl;
} else {
    cout << "True" << endl;
}

}

#endif
