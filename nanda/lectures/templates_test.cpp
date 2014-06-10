#include<iostream>
#include<cstdint>

template <typename BinaryOperation, typename Left, typename Right=Left>
class A {
public:    
    BinaryOperation op;
    Left op1;
    Right op2;
    A(BinaryOperation _op, Left _op1, Right _op2) : op(_op), op1(_op1), op2(_op2) {}
    Left operator()() {
        return op(op1, op2);
    }
};

template <typename Left>
class A<std::negate<Left>, Left> {
public:
    using UnaryOperation = std::negate<Left>;
    UnaryOperation op;
    Left op1;
    A(UnaryOperation _op, Left _op1) : op(_op), op1(_op1) {}
    Left operator()() {
        return op(op1);
    }
};

template <typename Operation, typename Left, typename Right=Left, typename Parent=A<Operation, Left, Right>>
class Child : public Parent
{
public:
    Child(Operation op, Left arg1) : Parent(op, arg1) {}
    Child(Operation op, Left arg1, Right arg2) : Parent(op, arg1, arg2) {}
};

using namespace std;
int main() {
    A<std::negate<int>, int> a(std::negate<int>(), 10);
    A<std::plus<int>, int, int> b(std::plus<int>(), 10, 20);

    Child<std::negate<int>, int> c(std::negate<int>(), 50);
    cout << "A is a: " << a() << endl;
    cout << "B is a: " << b() << endl;
    cout << "B is a: " << c() << endl;
}
