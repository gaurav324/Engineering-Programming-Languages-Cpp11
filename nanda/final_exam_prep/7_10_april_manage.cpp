#include <iostream>

using namespace std;

#define example2 1

#ifdef example2

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

#endif


#ifdef example1 
class Base {
public:
    int val;

    Base() {val = 0;}

    Base(int v) { this->val = v; }
    
    void my_value() {
        cout << "My value is: " << val << endl;
    }
};

class Derived : public Base {
public:
    double v1;
    double v2;

    Derived(double v1, double v2) {
        this->v1 = v1;
        this->v2 = v2;
    }

   void my_value() {
        cout << "My Double values is: " << v1 << " " << v2 << endl;
    }
};
#endif

#ifdef example2

// Actual control block.
struct ControlBlock {
    uint32_t ref_count;
    virtual ~ControlBlock(void) {}
};

// Managed block, would be T and ref_block together.
template<typename T>
struct Managed : public ControlBlock, public T {
    template<typename... Args>
    Managed(Args... args) : T(std::forward<Args...>(args)...) {}
};

// Smart pointer would require Managed block and all.
template <typename T>
class SmartPointer {
public:
    Managed<T>* ptr;
    SmartPointer(T* rhs) {
        ptr = static_cast<Managed<T>*>(rhs);
        ptr->ref_count += 1;
    }

    T& operator*(void) const { return (*ptr);}
    T* operator->(void) const { return (ptr);}

    SmartPointer(Managed<T> *ptr) {
        this->ptr = ptr;
        this->ptr->ref_count += 1;
    }

    template<typename U>
    SmartPointer(const SmartPointer<U>& rhs) {
        static_assert(std::is_base_of<T, U>::value, "Can convert.");
        Managed<U>* other_ptr = rhs.ptr;

        ControlBlock *cb = static_cast<ControlBlock*>(other_ptr);
        Managed<T> *my_ptr= static_cast<Managed<T>*>(cb);
        this->ptr = my_ptr;
        this->ptr->ref_count += 1;
    }

    ~SmartPointer() {
        if (ptr) {
            this->ptr->ref_count -= 1;
            if (this->ptr->ref_count == 0) {
                delete this->ptr;
            }
        }
    }

    operator bool(void) const { return ptr; }
};

template <typename T, typename... Args>
SmartPointer<T> makeManaged(Args... args) {
    Managed<T> *ptr = new Managed<T>(std::forward<Args...>(args)...);
    return SmartPointer<T>(ptr);
}

int main() {
    #ifdef example1
    Base *b = new Base(10);
    SmartPointer<Base> p(b);
    p->my_value();

    SmartPointer<Base> p1(p);
    p1->my_value();

    Derived *d = new Derived(51, 101);
    SmartPointer<Derived> p2(d);
    p2->my_value();

    SmartPointer<Base> p3(p2);
    p3->my_value();

    SmartPointer<Base> mm = makeManaged<Base>(75);
    mm->my_value();
    #endif
    
    {
        SmartPointer<Base> p = makeManaged<Base>();
        p->doit();
    }
    SmartPointer<Base> q = makeManaged<Derived>();
    q->doit();

    //SmartPointer<Derived> r = q;
    //r->doit();

    return 0;
}
#endif
