#include "lecture1.h"

// Define the static variable defined in the Foo class.
int32_t Foo::z = 50;

// Declaration.
void doit(void);

int main() {
    
    // Constructor style.
    Foo f{10, 20};

    // Compiler would pad extra bits so that it is easy for the 
    // compiler to read stuff.
    cout << "Size of object of F is: " << sizeof(f) << endl;

    // Read out the static variable's value.
    cout << "Static variable z's value is: " << f.z << endl;

    // Checking syntax to call doit function as a static function
    // and calling doit function normally.
    Foo::doit();
    doit();

    // Invoke doit2() function on the Foo's f.
    f.doit2();
}

void doit(void) {
    cout << "Inside main's doit function." << endl;
}
