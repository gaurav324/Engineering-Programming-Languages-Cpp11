#include <iostream>

#define example1 1

using namespace std;
#ifdef example1
class Base {
public:
    int v;

    Base(int v) {
        this->v = v;
    }

    void doIt() {
        cout << "Base::doIt" << endl;
    }
};

void simple() {
    cout << "Simple" << endl;
}

int main() {
    using ptr_fun = void(Base::*)(void);
    
    Base b(10);
    ptr_fun p = &Base::doIt;
    (b.*p)();
}
#endif
