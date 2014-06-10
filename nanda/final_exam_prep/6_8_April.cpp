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
    Component *c = new Component();
    Storable *s = c;
    
    // No virtualization.
    cout << s->my_name() << endl;

    // Static cast works even without any virtual table and all.
    Component *c1 = static_cast<Component*>(s);
    cout << c1->my_name() << endl;

    // Check static cast, when cast is actually wrong.
    Receiver *r = new Receiver();
    Component *cr = r;

    Transmitter *t = static_cast<Transmitter*> (cr);
    cout << t->my_name() << endl;
}   
