//They have to give us the invalid iterator wali class.

// Something related to the Project 1c.
p = x.begin();
x.pop_front();
p = p - 2; // Raise severe exception here itself.
p = p + 2;
*p = 42;

// Something related to the Projcet 1c.
p = x.begin();
p = p - 2; 
p = p + 2; // This is total valid. Unless we don't change vector using one of the vector operations, we don't invalidate it.
*p = 42;

// Something related to the Projcet 1c.
p = x.begin();
p = p - 2; 
*p = 42; // So this is invalid.

// If vector is out of scope, then we don't worry about those iterators.

p = x.begin();
x = y;
p = p - 2; // Moderate Invalidation.

// opertors.
q - p (int64_t)
p + k (iterator)
++p
--p
*p
p == q
p->field
p != q
(*p).field

// If p and q are pointing to different vectors, then undefined behavior.

// No cbegin(), that is stupid.

// emplace_back example.
template <typename T>
class Foo {
    T data;
public :
    Foo (const T&) {cout << "constructor\n"; }
    Foo(T, double) {
        cout << "T amd double constructor\n";
    }

    template <typename AltType>
    Foo<T> operator=(const AltType& rhs) {
        T var = (T) rhs;
        data = var;

        return *this;
    }

    // Call it like
    // x.operator=<double>(3.14519);
    // But compiler can deduce on its own/
    // x.operator=(3.14519);
};


template<typename U>
Foo<U>::Foo(const U& val) {
    cout << "constructor\n";
}


// If we want to move the Alttype thing out.
template <typename T>
template <typename T2>
Foo<T>& Foo<T>::operator=(const T2& rhs) {
        T var = (T) rhs;
        data = var;

        return *this;
    }
}


// Variable number of arguments. 
template <typename... Args>
void doit(Args...) {
    // This was not completed in class.
}

template <typename T, typename... Args>
T* make_thing(Args... args) {
    return new T(args...);
}

int main(void) {
    Foo<int> x(42);

    Foo<int*> p = make_thing<Foo<int>>(42, 3.14159);
}
