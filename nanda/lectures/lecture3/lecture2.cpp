#include <iostream>
#include <cstdint>
#include <cString>

using std::cout;
using std::endl;
using std::ostream;

class String {
private:
    // In C++11, we can now initialize default data members.
    char* data = {nullptr};
    uint32_t len;
public:
    String(const char* c) {
        len = strlen(c);
        data = new char[len];
        strcpy(data, c);
    }

    // Create a copy constructor for our class.
    explicit String(const String& s) {
        this->len = s.len;
        
        if (s.data) {
            this->data = new char[this->len];
            strcpy(data, s.data);
        } else {
            this->data = nullptr;
        }
    }

    ~String() {
        cout << "Destructor invoked." << endl;
        delete[] data;
    }

    void print() {
        cout << data << endl;
    }
};

//ostream& operator << (ostream& os, String& s) {
//    os << endl;
//}

void checkDoubleDelete(String s) {
    cout << "Inside the checkDoubleDelete." << endl;
}


void checkDoubleDeleteCorrected(String& s) {
    cout << "Inside checkDoubleDeleteCorrected." << endl;
}

int main(void) {
    String s{"Gaurav Nanda."};

    // I had a lot of issues, in overriding << operator.
    // I can certainly pass String, but then I would have to 
    // create some getString() function in String class,
    // so that I can print it customly.
    s.print();

    // This is to see what happens, when we dont pass a reference 
    // of the String object.
    //
    // The default copy constructor and default assignment operators 
    // it provides use a copying method known as a shallow copy 
    // (also known as a memberwise copy).
    //
    // That is why, actual memory is not allocated again for the String,
    // but only pointer is copied.
    //
    //checkDoubleDelete(s);

    // If we pass reference, we are good.
    //checkDoubleDeleteCorrected(s);

    // How to get rid of this problem??
    // 
    // Basically, we have to create our copy constructor in such a 
    // way that we end up doing deepcopy.
    //checkDoubleDelete(s);
    
    // When "String(String& s) {..}" constructor is made explicit,
    // then the following line does not work. XXX I don't get why??
    String s1 = s;
    s1.print();

    // If the copy constructor's argument is non-const, then this would
    // fail and just not work.
    // 
    // Why?
    //
    // Because, only lvalues can be bound to references to non-const. Here 
    // temporary object is constructed on the right, which is not allowed to 
    // be assigned to a non-const variable.
    //String s2 = "Chakk de phatte";
    //s2.print();
}

