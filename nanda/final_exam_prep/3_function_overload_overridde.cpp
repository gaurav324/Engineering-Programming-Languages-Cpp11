#include<iostream>

using namespace std;
class Parent {
private:
    int value;
public:    
    Parent(int v) {
        cout << "Parent Int constructor invoked" << endl;
        this->value = v;
    }

    void setValue(int v) {
        cout << "Parent setting int value to: " << v << endl;
        this->value = v;
    }

};

class Child : public Parent {
private:
    int child_value;
    char ch;
public:
    Child(int v) : Parent(v){
        cout << "Child setting int child_value to: " << v << endl;
        this->child_value = v;
    }

    // This is functin override, based upon the name not on signature.
    void setValue(double v) {
        cout << "Child setting double value to: " << v << endl;
        Parent::setValue(v);
    }

    // This is overloading wrt to the double function defined here.
    void setValue(char v) {
        cout << "Child setting char value to: " << v << endl;
        this->ch = v;
    }
};

int main() {
    Parent p1(100);

    Child c1(10);
    c1.setValue(20.5);
    c1.setValue('g');
}
