#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;
#define prob5

#ifdef prob1
class Base {
public:
    virtual void doit(void) const = 0;
    virtual Base* clone() const = 0;

    virtual ~Base() {};
};

class Derived1 : public Base {
private:
    string x;
public:
    Derived1(string x) { this->x = x; }
    virtual void doit(void) const { cout << x << endl; }

    virtual Base* clone(void) const {
        return new Derived1(x);
    }

    ~Derived1() {}
};

class Derived2 : public Base {
private:
    int x;
public:
    Derived2(int x) { this->x = x; }
    virtual void doit(void) const { cout << x << endl; }

    virtual Base* clone(void) const {
        return new Derived2(x);
    }

    ~Derived2() {}
};

class Wrap {
private:
    Base* ptr;
public:
    Wrap(void) {ptr = nullptr;}
    
    Wrap(const Wrap& other) {
        this->ptr = other.ptr->clone();
    }

    Wrap& operator=(const Base& other) {
        if (this->ptr) { delete ptr; }
        this->ptr = other.clone();
    }
    
    ~Wrap() {
        delete ptr;
    }

    void doit() {
        if(ptr) {
            ptr->doit();
        }
    }
};

void fun(Wrap x) {
    x.doit();
}

class PolyBase {
public:
    union {
        Derived1 d1;
        Derived2 d2;
    };

    PolyBase(const string& s) : d1(s) {}
    PolyBase(int x) : d2(x) {}

    void doit() const {
        ((Base*) &d1)->doit();
    }

    PolyBase& operator=(const Derived1& d) {
        //cout << &d1 << "  " << &d2 << endl;
        //delete &d1;
        new (&d1) Derived1(d);
        //this->d1 = d1;
        return *this;
    }

   PolyBase& operator=(const Derived2& d1) {
        this->d2 = d2;
        return *this;
    }

    ~PolyBase() {}
};

int main(void) {
//    Wrap w;
//    w = Derived1("The answer is");
//    fun(w);
//    w = Derived2(42);
//    fun(w);
    PolyBase p(100);
    p.doit();

    //Derived2 d2(100);
    //cout << sizeof(d1) << " " << sizeof(d2) << endl;

    Derived1 d1("nanda saab");
    p = d1;
    p.doit();
}
#endif

#ifdef prob3

class Boy;
class Girl;

class Child {
public:
    virtual void play() = 0;
    virtual void play(Child*) = 0;
    virtual void play(Boy*) = 0;
    virtual void play(Girl*) = 0;
};

class Boy : public Child {
public:
    void play() {
        cout << "Action" << endl;
    }

    void play(Child* ch) {
        if (ch) {
            ch->play(this);
        }
    }

    void play(Boy* boy) {
        cout << "Roughhouse" << endl;
    }

    void play(Girl* girl) {
        cout << "Cooties" << endl;
    }
};

class Girl : public Child {
public:
    void play() {
        cout << "Dolls" << endl;
    }

    void play(Child* ch) {
        if (ch) {
            ch->play(this);
        }
    }

    void play(Boy* boy) {
        cout << "Tea Party" << endl;
    }

    void play(Girl* girl) {
        cout << "Cooties" << endl;
    }
};

Child* getChild() {
    if (drand48() < .5) {
        return new Boy();
    } else {
        return new Girl();
    }
}

int main() {
    Child* alex = getChild();
    Child* ryan = getChild();

    alex->play();
    ryan->play();
    alex->play(ryan);
}

#endif 

#ifdef prob4

class Die {
public:
    int max;

    Die(int max) {
        this->max = max;
    }

    uint32_t operator()(void) {
        return rand() % max;
    }
    
    Die& operator = (Die& d) {
        this->max = d.max;
        return *this;
    }
};

Die& operator+(Die& die, int extra) {
    Die *d = new Die(die.max + extra);
    return *d;
}

Die& operator+(Die& die1, Die& die2) {
    Die *d = new Die(die1.max + die2.max);
    return *d;
}

int main() {
    srand (time(NULL));
    
    uint32_t roll;
    Die basic_die = Die(6);
    cout << basic_die() << endl;
    
    auto die6 = basic_die + 1;
    cout << die6() << endl;

    auto dice = die6 + die6;
    cout << dice() << endl;
}

#endif

#ifdef prob5

template<typename T1, typename T2>
struct Pair {
    T1 first;
    T2 second;

    Pair(const T1& f, const T2& s) : first(f), second(s) {}
};

template<int index, typename T1, typename T2>
class Getter;

template<typename T1, typename T2>
class Getter<0, T1, T2> {
public:
    using Type = T1;
    T1 get(const Pair<T1, T2>& p) {
        return p.first;
    }
};

template<typename T1, typename T2, typename T3>
class Getter<0, T1, Pair<T2, T3>> {
public:
    using Type = T1;
    T1 get(const Pair<T1, Pair<T2, T3>>& p) {
        return p.first;
    }
};

template<typename T1, typename T2>
class Getter<1, T1, T2> {
public:
    using Type = T2;
    T2 get(const Pair<T1, T2>& p) {
        return p.second;
    }
};

template<typename T1, typename T2, typename T3>
class Getter<1, T1, Pair<T2, T3>> {
public:
    using Type = T2;
    T2 get(const Pair<T1, Pair<T2, T3>>& p) {
        Getter<0, T2, T3> getter;
        return getter.get(p.second);
    }
};

template<int index, typename T1, typename T2, typename T3>
class Getter<index, T1, Pair<T2, T3>> {
public:
    using Type = typename Getter<index - 1, T2, T3>::Type;
    Type get(const Pair<T1, Pair<T2, T3>>& p) {
        Getter<index-1, T2, T3> getter;
        return getter.get(p.second);
    }
};

template<uint32_t idx, typename T1, typename T2>
typename Getter<idx, T1, T2>::Type get(const Pair<T1, T2>& p) {
    Getter<idx, T1, T2> getter;
    return getter.get(p);
}

int main(void) {
    Pair<int, double> p(42, 3.14);
    Pair<const char*, Pair<int, double>> p1("Gaurav Nanda", p);
    cout << get<2>(p1) << endl;
    cout << get<1>(p1) << endl;
    cout << get<0>(p1) << endl;
}

#endif

#ifdef prob6

template <bool, typename T1, typename T2>
struct find_bigger;

template <typename T1, typename T2>
struct find_bigger<false, T1, T2> {
    using Type = T2;
};

template <typename T1, typename T2>
struct find_bigger<true, T1, T2> {
    using Type = T1;
};

template <typename T1, typename T2>
struct Bigger {
    using Type = typename find_bigger<(sizeof(declval<T1>()) > sizeof(declval<T2>())), T1, T2>::Type;
};

int main() {
    using BiggerType = Bigger<int, double>::Type;
    BiggerType val = 10.34;
    cout << val << endl;
}

#endif
