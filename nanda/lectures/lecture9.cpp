#include <cstdint>
#include <iostream>
#include <vector>
#include <list>

using namespace std;

template <typename Ptr>
void printSequence(Ptr b, Ptr e) {
    const char* prefix = "";
    while(b != e) {
        cout << prefix << *b;
        ++b;
        prefix = ", ";
    }
    cout << endl;
}

namespace epl {

template <typename Ptr, typename CompType>
Ptr partition(Ptr b, Ptr e, CompType cmp) {
    Ptr p = b;
    Ptr k = b;
    ++k;

    while(k != e) {
        if (cmp(*p, *k)) {
            std::swap(*k, *p);
            ++p;
            std::swap(*p, *k);
        }
        ++k;
    }
    return p;
}

template <typename Ptr>
uint64_t difference(Ptr b, Ptr e, std::forward_iterator_tag) {
    //std::cout << "Forward iterator tagger" << endl;
    uint64_t count = 0;
    while(b != e) {
        ++count;
        ++b;
    }
    return count;
}

template <typename Ptr>
uint64_t difference(Ptr b, Ptr e, std::random_access_iterator_tag) {
    //std::cout << "Random access iterator tagger" << endl;
    return e - b;
}

template <typename Ptr>
uint64_t difference(Ptr b, Ptr e) {
    difference(b, e, typename iterator_traits<Ptr>::iterator_category{});
} 

template <typename Ptr, typename CompType>
void sort(Ptr b, Ptr e, const CompType& cmp) {
    // Probably - would not work in every case.?
    // No, would work. What will not work is b - e.
    if (difference(b, e) < 2) {
        return;
    }

    Ptr p = partition(b, e, cmp);
    sort(b, p, cmp);
    ++p;
    sort(p, e, cmp);
}


template <typename T>
struct Default_LT {
    bool operator() (const T& x, const T& y) const {
        if (x > y) { return true; } 
        else { return false; }
    }
};

template <typename Ptr>
void sort(Ptr b, Ptr e) {
    using T = typename iterator_traits<Ptr>::value_type;
    Default_LT<T> comp;
    sort(b, e, comp);
}

} // End of the namespace epl.


// Making it struct so that we can inline the function calls.
struct IntCmpFunc {
    bool operator()(int x, int y) {
        if (x > y) { return true; } else { return false; }
    }
};

int main(void) {
    int x[10] = {5, 2, 3, 9, 1, 6, 4, 8, 0, 7};
    
    printSequence(&x[0], &x[10]);

    epl::sort(&x[0], &x[10], IntCmpFunc{});

    printSequence(&x[0], &x[10]);
    
    // Till now, we have sorted basic integer pointers.
    // Lets try sort Vector of Integers now.
    //std::vector<int> y = {5, 2, 3, 9, 1, 6, 4, 8, 0, 7};
    std::vector<int> y = std::initializer_list<int> {5, 2, 3, 9, 1, 6, 4, 8, 0, 7};

    cout << "Vector before sorting." << endl;
    printSequence(y.begin(), y.end());

    epl::sort(y.begin(), y.end());
    cout << "Vector after sorting." << endl;
    printSequence(y.begin(), y.end());
    // Vector of integers also worked fine with the basic model.

    // Do, we satisfy all the goals of STL.
    // 1. Generic wrt element type?
    // 
    // Where do we compare elements?
    // if (*k < *p) {
    //
    // This is certainly not the generic way elements should be 
    // compared. Is comparing objects the only reason?? - YES
    //
    // Anyhow, lets add support for comparator functions.
    // So one approach is simply send a reference to the function.
    // However, that would incur another problem that we would
    // add extra function calls for comparison. Therefore, we should 
    // try to inline these functions.
    //
    // -So making it a struct for comparator function.
    // But, what if someone does not want to pass a comparator
    // functiom??
    //  - Add a default comparator for them.
    //
    // 2. Is it generic wrt to a list?
    std::list<int> l = {5, 2, 3, 9, 1, 6, 4, 8, 0, 7};
    cout << "List before sorting." << endl;
    printSequence(y.begin(), y.end());
    epl::sort(l.begin(), l.end());
    cout << "List after sorting." << endl;
    printSequence(y.begin(), y.end());
    // Well this would not work, because iterator used in list is 
    // a bidirectional iterator and we are using e - b, which
    // can only be operated on the random access iterators.
    // So, what can we do, we have to implement difference function 
    // which would take iterators as input and depending upon 
    // their type, it would try to find difference.
    //
    // Ok, we have added difference function, which iterates incrementally.
    // But, this is not optimal for containers which support random
    // access iterators.
}
