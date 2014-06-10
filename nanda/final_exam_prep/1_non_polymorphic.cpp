#include <iostream>

using namespace std;

class Storable {
public:
    const char* name = "Storable";
    Storable() {}

    const char* my_name() {
        return name;
    }
};

class Component : public Storable {
public:
    const char* name = "Component";
    Component() {}
    
    const char* my_name() {
        return name;
    }
};

class Receiver : public Component {
public: 
    const char* name = "Receiver";
    Receiver() {}
    
    const char* my_name() {
        return name;
    }
};

class Transmitter : public Component {
public:
    const char* name = "Transmitter";
    Transmitter() {}
    
    const char* my_name() {
        return name;
    }
};

class Radio : public Receiver, public Transmitter {
public:
    const char* name = "Radio";
    Radio() {}
    
    const char* my_name() {
        return name;
    }
};

int main() {
    cout << "Hello, World !!" << endl;

    Storable  *s1 = new Storable();
    Component *c1 = new Component();
    Radio     *r1 = new Radio();

    cout << "BASIC SCENARIION" << endl;
    cout << "S1 name is: " << s1->my_name() << endl;
    cout << "C1 name is: " << c1->my_name() << endl;
    cout << "R1 name is: " << r1->my_name() << endl;

    cout << "UP-CAST" << endl;
    // This is not allowed. It complains that there is an ambiguity in the hierarchy.    
    //Storable  *s2 = r1;
    //Component *c2 = r1;
    //cout << "S2 name is: " << s2->my_name() << endl;
    //cout << "C2 name is: " << c1->my_name() << endl;

    // This simply prints the name of the storable and does not do any virtual stuff.
    Storable  *s3 = c1;
    cout << "S3 name is: " << s3->my_name() << endl;

    cout << "Down-Cast" << endl;
    // This is not allowed, as base type is not polymorphic.
    //Radio *r2 = dynamic_cast<Radio*>(s1);
    //cout << "R2 name is: " << r2->my_name() << endl;

    // This is still giving that ambigous related problem.
    //Radio *r3 = (Radio*)s1;
    //cout << "R3 name is: " << r3->my_name() << endl;
}
