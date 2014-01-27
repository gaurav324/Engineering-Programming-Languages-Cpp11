#include <iostream>
#include <cstdint>

using std::cout;
using std::endl;

class Foo {
private:    
    int32_t x;
    int64_t y;

public:    
    Foo(void) = default;

    Foo(int32_t x, int64_t y) {
        this->x = x;
        this->y = y;
    }

    // ISO C++ forbids in-class initialization of non-const static member 'Foo::z'
    // static int32_t z = 50;
    static int32_t z;

    static void doit(void) {
        cout << "Inside Foo's static doit function." << endl;
    }

    void doit2() {
        Foo g{};
        g.x = this->x;

        cout << "Assigned this->x to new G object, a value of: " << g.x << endl;
    }
};

