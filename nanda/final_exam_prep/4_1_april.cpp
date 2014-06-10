#include <iostream>

using namespace std;

#define example3 1

#ifdef example1

class Base1 {
    int x;
public:
    void doit(void) { 
        x = 20;
    }
};

class Base2 {
    int y;
public:
    void doit2(void) { 
        y = 20;
    }

};

class Derived : public Base1, public Base2 {
    int z;
};

void hoho(Base1 b) {
    // This would simply truncate the object.
    // All the upcast, downcast stuff makes sense only
    // for the pointers.
}

#endif

#ifdef example2

class A  {
private:
    int a;
public:
    int b;
};

class A_pr : protected A {
public:
    // Cannot do this if private or even public inheritance.
    // void setPrivate(int a) { this->a = a; }
    void setPublic(int b) { this->b = b; }
};

class A_A_ar : public A_pr {
public:
    void setB(int b) {
        this->b = b;
    }
};

class LessPrivate : public A_A_ar {
protected:
    void setB(int b) {
        cout << "Less private" << endl;
    }
};

class MorePrivate : public LessPrivate {
public:
    void setB(int b) {
        cout << "More private" << endl;
    }
};
#endif


#ifdef example3

class Base {
public:
    void print_non() {
        cout << "Base:: print_non" << endl;
    }

    virtual void print_virtual() {
        cout << "Base:: print_virtual" << endl;
    }
};

class Derived : public Base {
public:
    void print_non() {
        cout << "Child:: print_non" << endl;
    }

    void print_virtual() {
        cout << "Child:: print_virtual" << endl;
    }
};

#endif

int main() {
#ifdef example3
    Base* b = new Base();
    b->print_non();
    b->print_virtual();

    Derived* d = new Derived();
    d->print_non();
    d->print_virtual();

    Base* mix = new Derived();
    mix->print_non();
    mix->print_virtual();

    /*
    /Users/gnanda/epl/shared-repo/nanda/final_exam_prep➜ git:(master) ✗./4_file   
    Base:: print_non
    Base:: print_virtual
    Child:: print_non
    Child:: print_virtual
    Base:: print_non
    Child:: print_virtual
    */
#endif

#ifdef example1

    Derived arr[10];
    arr[0].doit();

    Base1* p = arr;
    p->doit();
    Base2* p1 = arr;
    p1->doit2();

    //hoho();
    
#endif

#ifdef example2
    A_pr *apr = new A_pr();
    //apr->setPrivate(10);
    apr->setPublic(10);

    // Cannot do if privately or protectedly inherit.
    // apr->b = 10;

    A_A_ar *aapr = new A_A_ar();
    aapr->setB(10);

    LessPrivate *p = new LessPrivate();
    //p->setB(10);

    MorePrivate *p1 = new MorePrivate();
    p1->setB(10);
#endif
}
