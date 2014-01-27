// Let us try out a few things in discussed in Lecture-4.
//
// Important topic was to sepaarate deconstruction and deallocation.
// Similarly, construction and allocation go by side by side.
//
#include <iostream>
#include <cstdint>
#include <cString>

using std::cout;
using std::endl;
using std::ostream;

class String {
public:
    char* buffer{nullptr};
    uint32_t len{0};

    // Default constructor.
    String(void) = default;

    // Constructor.
    String(const char* value) {
        len = strlen(value);
        buffer = new char[len];
        strcpy(buffer, value);
    }

    // Destructor.
    ~String() {
        delete buffer;
    }

    // Function to print the buffer.
    void print(ostream& out) {
        if (buffer) {
            for (uint32_t k=0; k < len; ++k) {
                out << buffer[k];
            }
        }
    }
};

ostream& operator<<(ostream& os, String& str) {
    str.print(os);
    return os;
}

int main(void) {
    String* p = new String("Nanda Saab");
    cout << "Value of P: " << p->buffer << endl;
    delete p;

    String* arrStrings = new String[10];
    for (uint32_t i=0; i < 10; ++i) {
       cout << "Array of strings[" << i << "]: " << arrStrings[i] << endl; 
    }

    // If memory is allocated using new, it has to be associated with
    // a corresponding delete[] operator.
    //delete[] arrStrings;

    // Explicitly invoke destuctor for String objects.
    for (uint32_t i=0; i < 10; ++i) {
        arrStrings[i].~String();
    }

    //delete []arrStrings;
    operator delete(arrStrings);
}
