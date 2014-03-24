#include <iostream>
#include <cstdint>

using std::cout;
using std::cin;
using std::endl;

class String {
private:
    char* buffer{nullptr};  // 0 can be ambiguously interpreted as an address, use nullptr
    uint32_t len;// {0};

public:
    String(void) {} // = default; Invokes the default constructor for each of the data members
                    // Stroutstrup's recommendation: Don't write a default and copy constructor. If you don't write one and someone
                    // adds more data members, then its perfect. However if you did add one and someone added more members and does not change the 
                    // constructor, then it does not initialize the members
    
    String(const char* input) {
        len = strlen(input);
        buffer = new char[len];
        for (uint32_t k = 0; k < len; k += 1) {
            buffer[k] = input[k];
        }
    }
    
	// Some objects are not copyable, such as file handles. 
    // In that case, hide the methods by making them private.
    String(const String& that) {    // Copy constructor: compiler calls this constructor everytime a copy is made
        copy(that);                 // Doesn't have to be a constant reference, but has to be a reference otherwise it will 
    }                               // be a recursive call. 
                                    
    
    // Separating initialization and reinitialization (assignment). 
    // Reinit requires destroy followed by copy. 
    // Default implementation of reassignment if reassignment of all the members.
    // a = (b = c):    a becomes return value of a = a.operator=(b.operator=(c));
    // BUGBUG: 'a = a;' fails! First step destroys 'a' and then can't copy from it!
    String& operator=(const String& that) {
        if (this != &that) {
            destroy();
            copy(that);
        }
        return *this;
    }

    ~String() {
        delete[] buffer;    // character is being deallocated. If you have a pointer to an object, the compiler invokes the correct destructor. 
                            // If you have an array, the compiler needs to run the destructor in a loop if the object has a destructor. When new[]
                            // allocates an array, the compiler sneaks in an extra 8 bytes which holds the size of the array. While deallocating, 
                            // it moves the size to a register, uses the register to run a loop and cleans up and ends up back in the first cell. 
                            // Using delete[] instead of delete would using a bogus value as the size.
                            // In this case there is absolutely no difference between delete[] and delete, as initially compiler does not annotate
                            // with size as it knows it does not need to run the destructor in a loop.
    }

    void print(std::ostream& out) const {
        if (buffer) {
            cout << "Length is: " << len << endl;
            for (uint32_t k = 0; k < len; k += 1) {
                out << buffer[k];
            }
        }
    }

private:
    void copy(const String& that) {
        this->len = that.len;
        if (len == 0) {
            buffer = nullptr;
        }
        else {
            this->buffer = new char[len];
            for (uint32_t k = 0; k < len; k += 1) {
                this->buffer[k] = that.buffer[k];
            }
        }
    }

    void destroy() {
        if (len != 0) {
            delete[] buffer;
            len = 0;
        }
    }
};

// If you push an object on the stack, there are two objects one the original one and the other the copy on the stack. You can avoid pushing an object
// on the stack by using a reference which pushes a pointer or reference of the object onto the stack. Also, the reference should be constant.
std::ostream& operator<<(std::ostream& out, const String& myString) {
    myString.print(out);
    return out;
}

// Since there is no copy constructor, when 't' goes out of scope the function doIt calls the destructor for String on t, which cleans the 
// data block for the String 's' which was created in main().
// 
// Is it better to copy the 't' (use a copy constructor) or should you use references? 
// Ans: Using a copy constructor is better as keeps design choices open for clients on the other side of your code. 
// If you have a type that cannot be copied properly, you must remove the copy constructor. 
void doIt(String t) {
    cout << t << endl;
    // while (true);
}

// For the most part Java does not allow automatic type conversion, except for the basic types
// Eg: (const char* input) converts from a type to const char*. If we do not want the constructor to act as a type converter, 
// we need to mark it explicit. Without this, we can use the following: s = "Oh My!"; since s has a constructor with const char* in it.
// 1. There is an assignment for all types. Whether you define assignment semantics or not. 
// 2. The RHS is a const char* which can be promoted to String object using the constructor, and then does the default string assignment to the LHS. 
// Then it calls the destructor for the RHS. 
//

int main() {
    char c[10];
    String s { "Hello World!" };
    cout << s << endl;
    doIt(s);
    cout << s << endl;  // Chase: Hello World should still be present. Seen: Garbage is printed. Need to clarify.
    while (true);
    cout << "Press Enter to continue.." << endl;
    cin.getline(c, 1);

    // String *strArray = new String[100];
    // delete[] strArray;
    // delete strArray;      // Runs the destructor once, beginning of the block was the number 100 but it is using String destructor for
                            // freeing a cell that contains a number. This will crash.

    // String *strArray = new String[1];        // Single element array, should not be used this way. Initialize as non array element. 
    // delete[] strArray;                       // Delete runs in a loop, this is an overhead.
    return 0;
}