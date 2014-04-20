#include <cstdint>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

//
// Goals for writing the STL:
// i. Generic wrt element type
// ii. Generic wrt container type
// iii. Predictable time complexity - if you cannot provide random access (p + k) to your iterators in constant time 
// or amortized constant time, then don't do it.
// iv. Efficieny comparable to handwritten code - users should not be tempted to write code that gets them extra
// 20% performance over the STL code
// Diff between STL and Java: STL considers array to be a container, but Java does not consider Array to be 
// a container - that's why it is not a part of its java.util.collections collection. 
//

// Very careful only using forward iterator concepts 
template <typename Ptr>
void printSequence(Ptr b, Ptr e) {
    const char* prefix = "";
    while (b != e) {
        cout << prefix << *b;
        // very significant difference in the eyes of the STL, even if for this 
        // type (int) it does not matter
        // In STL -> below line is guaranteed to have linear time complexity (either simply or amortized)
        ++b;                  
        prefix = ", ";
    }
    cout << endl;
}

// A reasonable library designer would write two sort functions where one has params (Ptr, Ptr)
// and the other has params (Ptr, Ptr, Func). A good designer would make the first function 
// call the second one with the appropriate operator function
namespace epl {
    template <typename Ptr>
    uint64_t distance(Ptr b, Ptr e) {
        uint64_t result = 0;
        while (b != e) {
            ++result;
            ++b;
        }
        return result;
    }

    // A faster partition would come from left and find the first one thats larger
    // and come from right and check the first that is smaller
    // and then swap them - this would use half the number of swaps
    template <typename Ptr, typename CompType>
    Ptr partition(Ptr b, Ptr e, CompType comp) {
        Ptr p = b;
        Ptr k = b;
        ++k;
        while (k != e) {
            // some types might not have the less than operator defined
            // suppose T is a student type, it does not make sense to use the '<' operator
            // need to make it more generic
            // With comp.comp_fun, and optimizations on this below code will result in a 
            // code exactly equal to the '<' comparison 
            // STL adopted this early: the way to do this was to parameterize stuff and embed the 
            // operation in a function. They named the function operator() - why was the name chosen?
            // This is because the method can be invoked using comp(*k, *p)
//            if(comp.comp_fun(*k, *p)) {
            if (comp(*k, *p)) {
            // swap *k into pivot spot
                std::swap(*p, *k);
                // increase the number of smalls
                ++p;
                // and swap the pivot value back where it belongs
                std::swap(*p, *k);
            }
            ++k;
        }
        return p;
    }

    // Better handwritten code would check corner cases first and return
    // But partition is not very efficient, won't do the double swap trick
    // comp is not passed as a reference as it has so little state, that Chase just doesn't care!
    // If it should be const CompFun& then he concedes the point!
    template <typename Ptr, typename CompFun>
    void sort(Ptr b, Ptr e, CompFun comp) {
        // Here the pivot is not being inteligently chosen (median of first middle and last)
        // Following is a lazy implementation just for demonstration :)
        if (epl::distance(b, e) < 2)  { 
            // This is a random access iterator concept, won't compile if
            // Ptr was a std::liked_list iterator
            return; 
        }
        // decltype is awesome, recent addition to the STL
        // auto would look better here, clearer
        // difference between decltype and auto is that decltype does not require a RHS
        // decltype is strictly more powerful than auto, but auto is much more clearer and precise
        decltype(*b) pivot = *b;
        Ptr p = partition(b, e, comp);
        sort(b, p, comp);
        ++p;
        sort(p, e, comp);
    }

    template <typename Ptr>
    void sort(Ptr b, Ptr e) {
        // If Ptr were an iterator, then we could call Ptr::value_type
        // using T = decltype(*b);
        // There is a way to read the element type of a data structure 
        // The problem is if we rely on that, we don't have backward compat with arrays. 
        // Rather than asking Ptr what is your element type, he will ask what Ptr's element type is
        using T = Ptr::value_type;
        Default_LT<T> comp;
        sort(b, e, comp);
    }

    template <typename It>
    struct iterator_traits {
        using value_type = typename It::value_type;
    };

    // Template specialization
    template <typename T>
    struct iterator_traits<T*> {
        using value_type = T;
    };

} // END namespace epl


// T::operator< is written for most classes, but int does not have it
// This is why we cannot do this generically
template <typename T>
struct Default_LT {
    // This is an implementation of std::less
    bool operator()(int& x, int& y) const {
        if (y < x) { return true; }
        else return false;
    }
};


// This is being done for inlining the comparision operation
// and for removing the decltype usage
// This could be done with an object that encapsulates the function, but not with
// a function pointer
class IntCompFun {
    // if someone wants to compare in a special way they will
    // write their own comparison fucntion
public:
    bool operator()(int& x, int& y) const {
        if (y < x) { return true; }
        else return false;
    }
};

int main(void) {
    int x[10] = { 5, 2, 3, 9, 1, 6, 4, 8, 0, 7 };

    // Convention in STL is to pass array values in the form of iterators
    printSequence(&x[0], &x[10]);
    epl::sort(&x[0], &x[10], IntCompFun{});
    printSequence(&x[0], &x[10]);
    // below line, STL routes the rhs to the constructor with std::initializer list (C++ 11)
    // the '=' is a matter of choice, some people like the equals some don't
    std::vector<int> y = { 5, 2, 3, 9, 1, 6, 4, 8, 0, 7 };
    epl::sort(y.begin(), y.end());
    printSequence(y.begin(), y.end());

    getchar();
    return 0;
}