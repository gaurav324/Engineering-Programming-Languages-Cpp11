// This is to revise the lecture-2 of meta programming. We would try to go over the example discussed in the class once.
#include <cstdint>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

template <typename T>
vector<T> operator+(vector<T>& x, vector<T>& y) {
    vector<T> result;
    for (auto p = x.begin(), q = y.begin(); p != x.end(), q != y.end(); ++p, ++q) {
        result.push_back(*p + *q);    
    }
    return result;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const vector<T>& x) {
    const char* prefix = "";
    for (auto p = x.begin(); p != x.end(); ++p) {
        out << prefix << *p;
        prefix = ", ";
    }
    return out;
}

int main(void) {
    vector<int> x = {1, 2, 3, 4, 5};
    vector<int> y = {11, 12, 13, 14, 15};

    cout << x + y << endl;

    vector<string> a = {"Hello", "World"};
    vector<string> b = {"Gaurav", "Nanda"};

    cout << a + b << endl;
}
