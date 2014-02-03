// Vector.h -- header file for Vector data structure project

#include <iostream>
#include <stdexcept>
#include <assert.h>
#include <cstring>

//#pragma once
#ifndef _Vector_h
#define _Vector_h

#define MIN_CAPACITY 8
using namespace std;

namespace epl {

template <typename T>
class Vector {
private:
    T* buffer{nullptr};
    int64_t front{-1};
    int64_t back{-1};
    uint64_t capacity{0};
public:
    // Creates an array with some minimum capacity (8 is fine)
    // and length equal to zero. Must not use T::T(void). In fact,
    // as long as Vector::Vector(int) is not called, you should 
    // never use T::T(void).
    Vector(void) {
        buffer = (T*) operator new(MIN_CAPACITY * sizeof(T)); 
        front = -1;
        back = -1;
        capacity = MIN_CAPACITY;

        cout << "Constructed vector: " << this << endl;
    }

    // explicit Vector(uint64_t n) – create an array with capacity
    // and length exactly equal to n. Initialize the n objects using
    // T::T(void). As a special case, if n is equal to zero, you can
    // duplicate the behavior of vector(void).
    explicit Vector(uint64_t size) {
        assert(size >= 0);
        if (size == 0) {
            buffer = (T*) operator new(MIN_CAPACITY * sizeof(T)); 
            front = -1;
            back = -1;
            capacity = MIN_CAPACITY;
        } else {
            front = 0;
            back = size - 1;
            capacity = size;
            buffer = (T*) operator new(size * sizeof(T)); 
            for (int64_t i=0; i < size; i += 1) {
                new (buffer + i) T{}; 
            }
        }
        cout << "Constructed vector: " << this << endl;
    }

    // Copy constructor or vector<T> arguments (same type).
    Vector(const Vector<T>& that) {
        copy(that);
        cout << "Copy Constructed vector: " << this << endl;
    }

    // Assignment operator.
    Vector<T>& operator=(const Vector<T>& that) {
        if (this != that) {
            destroy();
            copy(that);
        }
        return *this;
    }

    // Destructor.
    ~Vector<T>(void) {
        destroy(); 
    }

    // T& operator[](uint64_t k) – if k is out of bounds (equal to 
    // or larger than length), then you must throw std::out_of_range
    // (“subscript out of range”); or a similar
    // error message (the string is arbitrary). If k is in bounds,
    // then you must return a reference to the element at position k.
    T& operator[](uint64_t index) {
        if (index >= size()) {
            throw std::out_of_range("subscript out of range");
        }
        return buffer[(front + index) % capacity];
    }

    // XXX Why have both the version here?
    const T& operator[](uint64_t index) const {
        if (index >= size()) {
            throw std::out_of_range("subscript out of range");
        }
        return buffer[(front + index) % capacity];
    }   

    // uint64_t size(void) const – return the number of constructed
    // objects in the vector.
    uint64_t size(void) {
        if (front == -1) {
            return 0;
        }
        if (back >= front) {
            return back - front + 1;
        } else {
            return back + (capacity - front) + 1;
        }
    }

    // (A) void push_back(const T&) – add a new value to the end of the array, 
    // using amortized doubling if the array has to be resized.
    // Copy construct the argument.
    void push_back(const T& arg) {
        cout << "Going to push back: " << arg << endl;
        if (front == (back + 1) % capacity) {
            // It means now we have allocate new memory. 
            T* new_loc = (T*) operator new(capacity * 2 * sizeof(T));

            for (int i=0; i < size(); ++i) {
                new (new_loc + i) T{std::move(this->operator [](i))};
            }
            
            // Add new element.
            new (new_loc + capacity) T(arg);

            // Once we have added new element, we can delete the old buffer.
            // arg could be from the old buffer itself, therefore we are 
            // deleting it late.
            for (int i=0; i < size(); ++i) {
                // Is this reallocation compulsory when we are using move semantics??
                (buffer + i)->~T();
            }
            operator delete(buffer);
            
            front = 0;
            back = capacity;
            capacity = capacity * 2;
           
            // Reset buffer to the new location.
            buffer = new_loc;
        } else {
            back = (back + 1) % capacity;
            new (buffer + back) T(arg);
            if (front == -1) {
                front = 0;
            }
        }
        
    }

    // (A) void push_front(const T&) and (B) void push_front(T&&) -- 
    // similar to push_back, but add the element to the front of the Vector
    void push_front(const T& arg) {
        cout << "Going to push in front: " << arg << endl; 
        if (front == (back + 1) % capacity) {
            // It means now we have allocate new memory. 
            T* new_loc = (T*) operator new(capacity * 2 * sizeof(T));

            for (int i=0; i < size(); ++i) {
                new (new_loc + i) T{std::move(this->operator [](i))};
            }

            // Add new element.
            new (new_loc + (capacity * 2) - 1) T(arg);
            
            // Once we have added new element, we can delete the old buffer.
            // arg could be from the old buffer itself, therefore we are 
            // deleting it late.
            for (int i=0; i < size(); ++i) {
                // Is this reallocation compulsory when we are using move semantics??
                (buffer + i)->~T();
            }
            operator delete(buffer);
            
            back = capacity - 1;
            capacity = capacity * 2;
            front = capacity - 1;

            buffer = new_loc;
        } else {
            if (front == -1) {
                front = 0;
                back = 0;
            } else {
                if (front == 0) {
                    front = capacity;
                }
                front = (front - 1) % capacity;
            }
            new (buffer + front) T(arg);
        }
    }

    // (A) void pop_back(void) and void pop_front(void) – if the array is empty, 
    // throw a std::out_of_range exception (with any reasonable string error 
    // message you want). Otherwise, destroy the object at the end (or beginning) 
    // of the array and update the length (and any pointers you need).
    // Do not reallocate storage, even if the vector becomes empty. Obviously, 
    // capacity increases by one. It is possible that a vector can have capacity
    // at both the front and back simultaneously.
    void pop_back(void) {
        // This represents that there is no element in the array.
        if (front == -1) {
            throw std::out_of_range("Vector is already empty.");
        }
        cout << "Going to pop one element from back." << endl;
        
        // Call the destuctor on that particular object.
        (buffer + back)->~T();

        // Reduce the back counter by 1.
        back = (back - 1) % capacity;

        // If all the elements in the array have been removed, 
        // then we would reset our front and back again.
        if (back == -1 || front == (back + 1) % capacity) {
            back = -1;
            front = -1;
        }
    }

    // void pop_front().
    void pop_front(void) {
        // This represents that there is no element in the array.
        if (front == -1) {
            throw std::out_of_range("Vector is already empty.");
        }
        cout << "Going to pop one element from front." << endl;
        
        // Call the destuctor on that particular object.
        (buffer + front)->~T();

        // Reduce the back counter by 1.
        front = (front + 1) % capacity;

        // If all the elements in the array have been removed, 
        // then we would reset our front and back again.
        if (back == -1 || front == (back + 1) % capacity) {
            back = -1;
            front = -1;
        }

    }

    uint64_t getCapacity() {
        return capacity;
    }

    void print() {
        cout << "Front: " << front << "; Back: " << back << "; Size: " << size() << "; Capacity: " << capacity << endl;
        for (int i=0; i < size(); ++i) {
            if (buffer[(front + i) % capacity] == nullptr) {
                cout << "NO !!" << endl;
            } else {
                //cout << "Index[" << i << "]: " << buffer[(front + i) % capacity] << endl; 
                cout << "Index[" << i << "]: " << this->operator [](i) << endl; 
            }
        }
    }

private:
    void destroy(void) {
        for (int i=0; i < size(); ++i) {
            (this->operator [](i)).~T();
        }
        operator delete(buffer);
        cout << "Going to destroy Vector object: " << this << endl;
    }

    void copy(const Vector<T>& that) {
        this->front = that.front;
        this->back = that.back;
        this->capacity = that.capacity;

        if (capacity == 0) {
            this->buffer = nullptr;
        } else {
            this->buffer = (T*) operator new(capacity * sizeof(T));
            for (uint64_t k=0; k < size(); k += 1) {
                new (this->buffer + k) T{that.buffer[(front + k) % capacity]};
            }
        }
    }
};

} //namespace epl

#endif /* _Vector_h */
