// Vector.h -- header file for Vector data structure project

#include <iostream>
#include <stdexcept>
#include <assert.h>
#include <cstring>
#include <stdexcept>
#include <utility>
#include <initializer_list>

//#pragma once
#ifndef _Vector_h
#define _Vector_h

#define MIN_CAPACITY 8

//Utility gives sdt::rel_ops which will fill in relational
//iterator operations so long as you provide the
//operators discussed in class.  In any case, ensure that
//all operations listed in this website are legal for your
//iterators:
//http://www.cplusplus.com/reference/iterator/RandomAccessIterator/
using namespace std::rel_ops;
using namespace std;

namespace epl {

class invalid_iterator {
	public:
	typedef enum {SEVERE,MODERATE,MILD,WARNING} Severity_level;
	Severity_level level;	

	invalid_iterator(){ level = SEVERE; }
	invalid_iterator(Severity_level _level){ level = _level; }
	virtual const char* what() const throw(){
    switch(level){
      case WARNING:   return "Warning";
      case MILD:      return "Mild";
      case MODERATE:  return "Moderate";
      case SEVERE:    return "Severe";
      default:        return "ERROR";
    }
	}
};

template <typename T>
class Vector {
private:
    T* buffer{nullptr};
    int64_t front{-1};
    int64_t back{-1};
    uint64_t capacity{0};

    uint64_t version{0};
    uint64_t assignment_version{0};
    uint64_t push_fronts{0};
    uint64_t pop_fronts{0};
public:
    // Forward declaration.
    class const_iterator;

    /*
     * Iterator for this vector.
     */
    class iterator {
    private:
        Vector* container;
        int64_t index;

        uint64_t version;
        
        // Older state.
        T* buffer;
        int64_t front;
        int64_t back;
        uint64_t capacity;
        uint64_t assignment_version;
        uint64_t push_fronts;
        uint64_t pop_fronts;
    public:
        using value_type = T;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = int;
        using pointer = int;
        using reference = int;

        // Constructors.
        iterator(Vector* v, int64_t index) {
            //cout << "Inside iterator constructor." << endl;
            this->container = v;
            this->index = index;

            this->version = v->version;
            this->buffer = v->buffer;
            this->front = v->front;
            this->back = v->back;
            this->capacity = v->capacity;
            this->assignment_version = v->assignment_version;
            this->push_fronts = v->push_fronts;
            this->pop_fronts = v->pop_fronts;
        };

        // Cast operator.
        operator const_iterator() {
            return const_iterator(container, index);
        }

        // Copy constructor amd assignment.
        iterator(const iterator& rhs) {
            //cout << "Inside iterator copy constructor." << endl;
            
            this->container = rhs.container;
            this->index = rhs.index;

            this->version = rhs.version;
            this->buffer = rhs.buffer;
            this->front = rhs.front;
            this->back = rhs.back;
            this->capacity = rhs.capacity;
            this->assignment_version = rhs.assignment_version;
            this->push_fronts = rhs.push_fronts;
            this->pop_fronts = rhs.pop_fronts;
        }

        iterator& operator=(const iterator& rhs) {
            //cout << "Inside iterator= assignment." << endl;
            rhs.validate_iterator();

            if(this != &rhs) {
                this->container = rhs.container;
                this->index = rhs.index;

                this->version = rhs.version; 
                this->buffer = rhs.buffer;
                this->front = rhs.front;
                this->back = rhs.back;
                this->capacity = rhs.capacity;
                this->assignment_version = rhs.assignment_version;
                this->push_fronts = rhs.push_fronts;
                this->pop_fronts = rhs.pop_fronts;
            }

            return *this;
        }
    
        // Relational operators.
        // == operator.
        bool operator==(const iterator& rhs) const {
            // I think we should validate both of the 
            // iterators to ensure that both are pointing
            // to the current version.
            //cout << "Inside iterator comparison." << endl; 
            validate_iterator();
            rhs.validate_iterator();

            if ( this->index == rhs.index) { 
                return true;
            } 
            else { 
                return false; 
            }
        }

        // < operator.
        bool operator<(const iterator& rhs) const {
            // I think we should validate both of the 
            // iterators to ensure that each one is valid
            // and pointing to the current version.
            //cout << "Inside iterator comparison." << endl; 
            validate_iterator();
            rhs.validate_iterator();

            if (this->index < rhs.index) { 
                return true;
            } 
            else { 
                return false; 
            }
        }

        // Dereference operator.
        T& operator*() {
            //cout << "Inside iterater dereference. Index is: " << index << endl;
            validate_iterator(true);
            
            return container->operator[](index);
        }

        // Return address of the element.
        T* operator->() {
            //cout << "Inside iterator pointer de-reference." << endl;
            validate_iterator(true);

            return &(container->operator[](index));
        }

        // Pre-increment operator.
        iterator& operator++() {
            //cout << "Inside iterator ++ operator." << endl;
            validate_iterator();

            ++index;
            return *this;
        }

        // Post increment operator.
        iterator operator++(int) {
            //cout << "Inside iterator post++ operator." << endl;
            validate_iterator();

            iterator t{*this};
            ++index;
            return t;
        }

        // Pre-decrement operator.
        iterator& operator--() {
            //cout << "Inside iterator -- operator." << endl;
            validate_iterator();

            --index;
            return *this;
        }

        // Post increment operator.
        iterator operator--(int) {
            //cout << "Inside iterator post-- operator." << endl;
            validate_iterator();

            iterator t{*this};
            --index;
            return t;
        }

        // Arithmetic operaters.
        // + operator.
        iterator operator+(int64_t n) {
            //cout << "Inside iterator + n operator." << endl;
            validate_iterator();

            return iterator{container, index + n};
        }
    
        friend const iterator operator+(int64_t n, iterator& it) {
            //cout << "Inside friend iterator + n operator." << endl;
            return it + n;
        }

        // - operator.
        iterator operator-(int64_t n) {
            //cout << "Inside iterator - n operator." << endl;
            validate_iterator();

            return iterator{container, index - n};
        }

        // diff operator.
        int64_t operator-(const iterator& that) {
            //cout << "Inside iterator a - b operator." << endl;
            validate_iterator();
            that.validate_iterator();

            return this->index - that.index;
        }

        // Compound operators.
        // +=
        iterator& operator+=(int64_t n) {
            //cout << "Inside iterator += operator." << endl;
            validate_iterator();

            index += n;
            return *this;
        }

        // -=
        iterator& operator-=(int64_t n) {
            //cout << "Inside iterator -= n operator." << endl;
            validate_iterator();

            index -= n;
            return *this;
        }

        // [] operator.
        T& operator[](int64_t n) {
            //cout << "Inside iterator [] operator." << endl;
            validate_iterator(true);

            return container->operator[](index + n);
        }
            
        // Basic version of iterator validation.
        void validate_iterator(bool is_dereference=false) {
            if (this->version != this->container->version) {
                
                if (this->push_fronts != this->container->push_fronts) {
                    index += (this->container->push_fronts - this->push_fronts);
                }
 
                if (this->pop_fronts != this->container->pop_fronts) {
                    index -= (this->container->pop_fronts - this->pop_fronts);
                }

                this->push_fronts = this->container->push_fronts;

                // If index is out of bounds, then we are certainly
                // having a severe exception.
                if (is_dereference && (index < 0 || index >= container->size())) {
                    throw invalid_iterator{invalid_iterator::SEVERE};
                }
                // Though, we are in-bounds, but the underlying buffer 
                // has changed. This could have happened either because of
                // push_back reallocation or assignment to the vector.
                else if (assignment_version != container->assignment_version ||
                    buffer != container->buffer) {
                    throw invalid_iterator{invalid_iterator::MODERATE};
                }
                // Now we know that version has changed, but we are still
                // not out of bounds, neither underlying memory locaiton 
                // has changed.
                // Then two things could have happend, eiher changes to back
                // or some changes to back.
                // Spec says if changes are made to back, then throw MILD
                // and changes to front are WARNING.
                // 
                // We will check for the front case earlier, because if index
                // is changed, we will certainly throw warning and for the 
                // rest of the cases, we can live with MILD exception case.
                else if (front != container->front) {
                    throw invalid_iterator{invalid_iterator::WARNING};
                }
                // For everything else, we are just throwing the MILD exception.
                // That is if some 
                else {
                    throw invalid_iterator{invalid_iterator::MILD};
                }
            }
        }

        // Const version of the iterator validation.
        // Added this, so that I can vaildate the iterators
        // which are compared against.
        void validate_iterator(bool is_dereference=false) const {
            int index = this->index;
            if (this->version != this->container->version) {

                if (this->push_fronts != this->container->push_fronts) {
                    index += (this->container->push_fronts - this->push_fronts);
                }
 
                if (this->pop_fronts != this->container->pop_fronts) {
                    index -= (this->container->pop_fronts - this->pop_fronts);
                }

                // If index is out of bounds, then we are certainly
                // having a severe exception.
                if (is_dereference && (index < 0 || index >= container->size())) {
                    throw invalid_iterator{invalid_iterator::SEVERE};
                }
                // Though, we are in-bounds, but the underlying buffer 
                // has changed. This could have happened either because of
                // push_back reallocation or assignment to the vector.
                else if (assignment_version != container->assignment_version ||
                    buffer != container->buffer) {
                    throw invalid_iterator{invalid_iterator::MODERATE};
                }
                // Now we know that version has changed, but we are still
                // not out of bounds, neither underlying memory locaiton 
                // has changed.
                // Then two things could have happend, eiher changes to back
                // or some changes to back.
                // Spec says if changes are made to back, then throw MILD
                // and changes to front are WARNING.
                // 
                // We will check for the front case earlier, because if index
                // is changed, we will certainly throw warning and for the 
                // rest of the cases, we can live with MILD exception case.
                else if (front != container->front) {
                    throw invalid_iterator{invalid_iterator::WARNING};
                }
                // For everything else, we are just throwing the MILD exception.
                // That is if some 
                else {
                    throw invalid_iterator{invalid_iterator::MILD};
                }
            }
        }        
    }; // End of class iterator.


    /*
     * Const Iterator for this vector.
     */
    class const_iterator {
        // XXX Need to make this const?
        const Vector* container;
        int64_t index;
        
        uint64_t version;
        
        // Older state.
        T* buffer;
        int64_t front;
        int64_t back;
        uint64_t capacity;
        uint64_t assignment_version;
        uint64_t push_fronts;
        uint64_t pop_fronts;
    public:
        using value_type = T;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = int;
        using pointer = int;
        using reference = int;

        // Constructors.
        const_iterator(const Vector* v, int64_t index) {
            //cout << "Inside const_iterator constructor." << endl;
            this->container = v;
            this->index = index;

            this->version = v->version;
            this->buffer = v->buffer;
            this->front = v->front;
            this->back = v->back;
            this->capacity = v->capacity;
            this->assignment_version = v->assignment_version;
            this->push_fronts = v->push_fronts;
            this->pop_fronts = v->pop_fronts;
        };

        // Copy constructor amd assignment.
        const_iterator(const const_iterator& rhs) {
            //cout << "Inside const_iterator copy constructor." << endl;
            
            this->container = rhs.container;
            this->index = rhs.index;

            this->version = rhs.version;
            this->buffer = rhs.buffer;
            this->front = rhs.front;
            this->back = rhs.back;
            this->capacity = rhs.capacity;           
            this->assignment_version = rhs.assignment_version;
            this->push_fronts = rhs.push_fronts;
            this->pop_fronts = rhs.pop_fronts;
        }

        const_iterator& operator=(const const_iterator& rhs) {
            //cout << "Inside const_iterator= assignment." << endl;
            rhs.validate_iterator();

            if(this != &rhs) {
                this->container = rhs.container;
                this->index = rhs.index;

                this->version = rhs.version; 
                this->buffer = rhs.buffer;
                this->front = rhs.front;
                this->back = rhs.back;
                this->capacity = rhs.capacity;
                this->assignment_version = rhs.assignment_version;
                this->push_fronts = rhs.push_fronts;
                this->pop_fronts = rhs.pop_fronts;                
            }

            return *this;
        }

        // Relational operators.
        // == operator.
        bool operator == (const const_iterator& rhs) const {
            // I think we should validate both of the 
            // iterators to ensure that both are pointing
            // to the current version.
            //cout << "Inside const_iterator comparison." << endl; 
            validate_iterator();
            rhs.validate_iterator();

            if ( this->index == rhs.index) { 
                return true;
            } 
            else { 
                return false; 
            }
        }

        // < operator.
        bool operator < (const const_iterator& rhs) const {
            // I think we should validate both of the 
            // iterators to ensure that each one is valid
            // and pointing to the current version.
            //cout << "Inside const_iterator comparison." << endl; 
            validate_iterator();
            rhs.validate_iterator();

            if ( this->index < rhs.index) { 
                return true;
            } 
            else { 
                return false; 
            }
        }
 
        // Dereference operator.
        const T& operator*() const {
            //cout << "Inside const_iterater dereference. Index is: " << index << endl;
            validate_iterator(true);
            
            return container->operator[](index);
        }

        // Return address of the element.
        const T* operator->() const {
            //cout << "Inside const_iterator pointer de-reference." << endl;
            validate_iterator(true);

            return &(container->operator[](index));
        }
        
        // Pre-increment operator.
        const_iterator& operator++() {
            //cout << "Inside const_iterator ++ operator." << endl;
            validate_iterator();

            ++index;
            return *this;
        }        

        // Post increment operator.
        const_iterator operator++(int) {
            //cout << "Inside const_iterator post++ operator." << endl;
            validate_iterator();

            const_iterator t{*this};
            ++index;
            return t;
        }

        // Pre-decrement operator.
        const_iterator& operator--() {
            //cout << "Inside const_iterator -- operator." << endl;
            validate_iterator();

            --index;
            return *this;
        }        

        // Post decrement operator.
        const_iterator operator--(int) {
            //cout << "Inside const_iterator post-- operator." << endl;
            validate_iterator();

            const_iterator t{*this};
            --index;
            return t;
        }

        // Arithmetic operaters.
        // + operator.
        const_iterator operator+(int64_t n) {
            //cout << "Inside const_iterator + n operator." << endl;
            validate_iterator();

            return const_iterator(container, index + n);
        }

        // Friend + opertor.
        friend const const_iterator operator+(int64_t n, const const_iterator& it) {
            //cout << "Inside friend const_iterator + n operator." << endl;
            return it + n;
        }

        
        // - operator.
        const_iterator operator-(int64_t n) {
            //cout << "Inside const_iterator - n operator." << endl;
            validate_iterator();

            return const_iterator(container, index - n);
        }

        // diff operator.
        int64_t operator-(const const_iterator& that) {
            //cout << "Inside const_iterator a - b operator." << endl;
            validate_iterator();
            that.validate_iterator();

            return this->index - that.index;
        }

        // Compound operators.
        // +=
        const_iterator& operator+=(int64_t n) {
            //cout << "Inside const_iterator += operator." << endl;
            validate_iterator();

            index += n;
            return *this;
        }

        // -=
        const_iterator& operator-=(int64_t n) {
            //cout << "Inside const_iterator -= n operator." << endl;
            validate_iterator();

            index -= n;
            return *this;
        }

        // [] operator.
        const T& operator[](int64_t n) {
            //cout << "Inside const_iterator [] operator." << endl;
            validate_iterator(true);

            return container->operator[](index + n);
        }

        // Basic version of iterator validation.
        void validate_iterator(bool is_dereference=false) {
            if (this->version != this->container->version) {
                
                if (this->push_fronts != this->container->push_fronts) {
                    index += (this->container->push_fronts - this->push_fronts);
                }
 
                if (this->pop_fronts != this->container->pop_fronts) {
                    index -= (this->container->pop_fronts - this->pop_fronts);
                }

                this->push_fronts = this->container->push_fronts;

                // If index is out of bounds, then we are certainly
                // having a severe exception.
                if (is_dereference && (index < 0 || index >= container->size())) {
                    throw invalid_iterator{invalid_iterator::SEVERE};
                }
                // Though, we are in-bounds, but the underlying buffer 
                // has changed. This could have happened either because of
                // push_back reallocation or assignment to the vector.
                else if (assignment_version != container->assignment_version ||
                    buffer != container->buffer) {
                    throw invalid_iterator{invalid_iterator::MODERATE};
                }
                // Now we know that version has changed, but we are still
                // not out of bounds, neither underlying memory locaiton 
                // has changed.
                // Then two things could have happend, eiher changes to back
                // or some changes to back.
                // Spec says if changes are made to back, then throw MILD
                // and changes to front are WARNING.
                // 
                // We will check for the front case earlier, because if index
                // is changed, we will certainly throw warning and for the 
                // rest of the cases, we can live with MILD exception case.
                else if (front != container->front) {
                    throw invalid_iterator{invalid_iterator::WARNING};
                }
                // For everything else, we are just throwing the MILD exception.
                // That is if some 
                else {
                    throw invalid_iterator{invalid_iterator::MILD};
                }
            }
        }

        // Const version of the iterator validation.
        // Added this, so that I can vaildate the iterators
        // which are compared against.
        void validate_iterator(bool is_dereference=false) const {
            int index  = this->index;
            if (this->version != this->container->version) {

                if (this->push_fronts != this->container->push_fronts) {
                    index += (this->container->push_fronts - this->push_fronts);
                }
 
                if (this->pop_fronts != this->container->pop_fronts) {
                    index -= (this->container->pop_fronts - this->pop_fronts);
                }

                // If index is out of bounds, then we are certainly
                // having a severe exception.
                if (is_dereference && (index < 0 || index >= container->size())) {
                    throw invalid_iterator{invalid_iterator::SEVERE};
                }
                // Though, we are in-bounds, but the underlying buffer 
                // has changed. This could have happened either because of
                // push_back reallocation or assignment to the vector.
                else if (assignment_version != container->assignment_version ||
                    buffer != container->buffer) {
                    throw invalid_iterator{invalid_iterator::MODERATE};
                }
                // Now we know that version has changed, but we are still
                // not out of bounds, neither underlying memory locaiton 
                // has changed.
                // Then two things could have happend, eiher changes to back
                // or some changes to back.
                // Spec says if changes are made to back, then throw MILD
                // and changes to front are WARNING.
                // 
                // We will check for the front case earlier, because if index
                // is changed, we will certainly throw warning and for the 
                // rest of the cases, we can live with MILD exception case.
                else if (front != container->front) {
                    throw invalid_iterator{invalid_iterator::WARNING};
                }
                // For everything else, we are just throwing the MILD exception.
                // That is if some 
                else {
                    throw invalid_iterator{invalid_iterator::MILD};
                }
            }
        }        
    }; // End of class const_iterator.

    // Provide begin and end functions for your iterator.
    iterator begin() {
        // If no element is there, then iterator is pointing 
        // to nullptr. Otherwise, return a pointer to front 
        // element.
        //cout << "Inside begin()." << endl;
        if (front == -1) {
            return iterator(this, -1);
        } else {
            return iterator{this, 0};
        }
    }

    // Provide begin and end functions for your const iterator.
    const_iterator begin() const {
        // If no element is there, then iterator is pointing 
        // to nullptr. Otherwise, return a pointer to front 
        // element.
        //cout << "Inside const_begin()." << endl;
        if (front == -1) {
            return const_iterator(this, -1);
        } else {
            return const_iterator{this, 0};
        }
    }

    // End function.
    iterator end() {
        //cout << "Inside end()." << endl;
        // If front == -1, means that there is no
        // element in the vector. 
        if (front == -1) {
            return iterator(this, -1);
        } else {
            return iterator(this, size());            
        }
    }

    // End function.
    const_iterator end() const {
        //cout << "Inside const_end()." << endl;
        // If front == -1, means that there is no
        // element in the vector. Therfore end should
        // also return the nullptr address.
        if (front == -1) {
            return const_iterator(this, -1);
        } else {
            return const_iterator(this, size());
        }
    }    

    // Creates an array with some minimum capacity (8 is fine)
    // and length equal to zero. Must not use T::T(void). In fact,
    // as long as Vector::Vector(int) is not called, you should 
    // never use T::T(void).
    Vector(void) {
        buffer = (T*) operator new(MIN_CAPACITY * sizeof(T)); 
        front = -1;
        back = -1;
        capacity = MIN_CAPACITY;

        //cout << "Constructed vector: " << this << endl;
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
        //cout << "Constructed vector: " << this << endl;
    }

    // Copy constructor or vector<T> arguments (same type).
    Vector(const Vector<T>& that) {
        copy(that);
        //cout << "Copy Constructed vector: " << this << endl;
    }

    // Move Constructor.
    Vector(Vector&& rhs) {
        this->front = rhs.front;
        this->back = rhs.back;
        this->capacity = rhs.capacity;
        this->buffer = rhs.buffer;

        // Make buffer of the moving object to be null. This would
        // ensure that if rhs gets destructed nothing gets
        rhs.buffer = nullptr;
        rhs.front = -1;
        rhs.back = -1;
        rhs.capacity = 0;
        
        // If we are moving over some other vector, then that
        // vector is not the same anymore.
        rhs.version += 1;

        //cout << "Move Constructed vector: " << this << endl;
    }

    // Vector initialized using static intializer block.
    Vector(std::initializer_list<T> il) {
        uint64_t size = il.size();
        if (size == 0) {
            buffer = (T*) operator new(MIN_CAPACITY * sizeof(T)); 
            front = -1;
            back = -1;
            capacity = MIN_CAPACITY;
        } else {
            buffer = (T*) operator new(size * sizeof(T));
            capacity = size;
            front = 0;
            back = size - 1;

            auto p = il.begin();
            for(uint64_t i=0; i < size; i+=1) {
                // I am moving elements from the static initializer list,
                // because what would that do with those elements.
                // I think this is correct.
                new (this->buffer + i) T{std::move(p[i])};
            }
        }
    }

    // Vector initializing using iterator pair b and e.
    template <typename InputIterator>
    Vector(InputIterator b, InputIterator e) {
        iterate_initialize(b, e, typename iterator_traits<InputIterator>::iterator_category{});
    }

    template <typename InputIterator>
    void iterate_initialize(InputIterator b, InputIterator e, std::random_access_iterator_tag) {
        //cout << "Random access iterator initialization invoked." << endl;
        uint64_t sz = e - b;
        if (sz != 0) {
            buffer = (T*) operator new(sz * sizeof(T));
            capacity = sz;
            front = 0;
            back = sz - 1;

            for(uint64_t i=0; i < sz; i+=1) {
                new (this->buffer + i) T{b[i]};
            }
        } else {
            buffer = (T*) operator new(MIN_CAPACITY * sizeof(T)); 
            front = -1;
            back = -1;
            capacity = MIN_CAPACITY;
        }
    }

    template <typename InputIterator>
    void iterate_initialize(InputIterator b, InputIterator e, std::input_iterator_tag) {
        //cout << "Input iterator initialization invoked." << endl;
        int i = 0;

        buffer = (T*) operator new(MIN_CAPACITY * sizeof(T)); 
        front = -1;
        back = -1;
        capacity = MIN_CAPACITY;

        while(b != e) {
            push_back(*b);
            ++b;
        }
    }

    // Assignment operator.
    Vector<T>& operator=(const Vector<T>& that) {
        if (this != &that) {
            // Destruct the older objects.
            for (int i=0; i < size(); ++i) {
                (this->operator [](i)).~T();
            }
            // If the object to be copied has more capacity than this object,
            // then we need to deallocate older memory and allocate new and 
            // larger memory block for this.
            if (that.capacity > this->capacity) {
                // Is it safe to delete here?
                //  I guess yes !! Because we are in assignment operator,
                //  and we cannot assign something from within our buffer.
                operator delete(buffer);
                if (that.capacity == 0) {
                    this->buffer = nullptr;
                } else {
                    this->buffer = (T*) operator new(that.capacity * sizeof(T));
                }
                this->capacity = that.capacity;
            }
            
            uint64_t that_size = that.size();
            
            // If the other vector is empty, then reset front and back;
            if (that_size == 0) {
                this->front = -1;
                this->back = -1;
            } else {
                this->front = 0;
                this->back = that_size - 1;

                for(uint64_t i=0; i < that_size; i+=1) {
                    new (this->buffer + i) T{that[i]};
                }
            }

            // If we have copied over some other vector, then we could 
            // have changed the vector, therefore version++/
            version += 1;
            assignment_version += 1;
        }
        return *this;
    }

    // Move Assignment operator.
    Vector<T>& operator=(Vector<T>&& rhs) {
        std::swap(this->buffer, rhs.buffer);
        std::swap(this->front, rhs.front);
        std::swap(this->back, rhs.back);
        std::swap(this->capacity, rhs.capacity);

        // Both the vectors have changed their state.
        this->version += 1;
        rhs.version += 1;
        assignment_version += 1;

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
    // Why make it const???
    // A: Because making it const would ensure that I can invoke
    // size() function on const objects also.
    uint64_t size(void) const {
        if (front == -1) {
            return 0;
        }
        if (back >= front) {
            return back - front + 1;
        } else {
            return back + (capacity - front) + 1;
        }
    }

    // Emplace back constructor.
    // This would be almost similar to what we already have for
    // push_back. Just replace the way, we construct T.
    template <typename... Args>
    void emplace_back(Args&&... args) {
        //cout << "Going to emplace back: " << args << endl;
        if (front == (back + 1) % capacity) {
            // It means now we have allocate new memory. 
            T* new_loc = (T*) operator new(capacity * 2 * sizeof(T));

            // Add new element.
            // This addition of the new element is done in advance 
            // so that if the arg is somehow going to be moved in the 
            // next statement, we would not be able to make our copy.
            new (new_loc + capacity) T(std::forward<Args>(args)...);

            for (int i=0; i < size(); ++i) {
                new (new_loc + i) T{std::move(this->operator [](i))};
            }

            // Once we have added new element, we can delete the old buffer.
            // 'arg' could be from the old buffer itself, therefore we are 
            // deleting it late.
            for (int i=0; i < size(); ++i) {
                // Is this reallocation compulsory when we are using move semantics??
                // Ans:- Yes. It is responsibility of move designer, to take
                //      care of the possible destruction.
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
            new (buffer + back) T(std::forward<Args>(args)...);

            // If it was the first element, reset front also.
            if (front == -1) {
                front = 0;
            }
        }
    }

    // (A) void push_back(const T&) – add a new value to the end of the array, 
    // using amortized doubling if the array has to be resized.
    // Copy construct the argument.
    void push_back(const T& arg) {
        //cout << "Going to push back: " << arg << endl;
        if (front == (back + 1) % capacity) {
            // It means now we have allocate new memory. 
            T* new_loc = (T*) operator new(capacity * 2 * sizeof(T));

            // Add new element.
            // This addition of the new element is done in advance 
            // so that if the arg is somehow going to be moved in the 
            // next statement, we would not be able to make our copy.
            new (new_loc + capacity) T(arg);

            for (int i=0; i < size(); ++i) {
                new (new_loc + i) T{std::move(this->operator [](i))};
            }

            // Once we have added new element, we can delete the old buffer.
            // 'arg' could be from the old buffer itself, therefore we are 
            // deleting it late.
            for (int i=0; i < size(); ++i) {
                // Is this reallocation compulsory when we are using move semantics??
                // Ans:- Yes. It is responsibility of move designer, to take
                //      care of the possible destruction.
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

            // If it was the first element, reset front also.
            if (front == -1) {
                front = 0;
            }
        }
        
        // Increment the version on pushing back.
        version += 1;
    }

    // Push back using move construct.
    void push_back(T&& arg) {
        //cout << "Going to push back using move construct: " << arg << endl;
        if (front == (back + 1) % capacity) {
            // It means now we have allocate new memory. 
            T* new_loc = (T*) operator new(capacity * 2 * sizeof(T));

            for (int i=0; i < size(); ++i) {
                new (new_loc + i) T{std::move(this->operator [](i))};
            }
            
            // Add new element.
            // DIFF from copy construction.
            new (new_loc + capacity) T(std::move(arg));

            // Once we have added new element, we can delete the old buffer.
            // 'arg' could be from the old buffer itself, therefore we are 
            // deleting it late.
            for (int i=0; i < size(); ++i) {
                // Is this reallocation compulsory when we are using move semantics??
                // Ans:- Yes. It is responsibility of move designer, to take
                //      care of the possible destruction.
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

            // DIFF from the copy construct.
            new (buffer + back) T(std::move(arg));

            // If it was the first element, reset front also.
            if (front == -1) {
                front = 0;
            }
        }

        // Increment version after adding one more element.
        version += 1;
    }

    // (A) void push_front(const T&) and (B) void push_front(T&&) -- 
    // similar to push_back, but add the element to the front of the Vector
    void push_front(const T& arg) {
        //cout << "Going to push in front: " << arg << endl; 
        if (front == (back + 1) % capacity) {
            // It means now we have allocate new memory. 
            T* new_loc = (T*) operator new(capacity * 2 * sizeof(T));

            // First add the new element, then move the older objects
            // to the new location.
            new (new_loc + (capacity * 2) - 1) T(arg);

            for (int i=0; i < size(); ++i) {
                new (new_loc + i) T{std::move(this->operator [](i))};
            }
            
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
                front = front - 1;
            }
            new (buffer + front) T(arg);
        }
        
        // Increment version after adding an element to the front.
        version += 1;
        push_fronts += 1;
    }

    void push_front(T&& arg) {
        //cout << "Going to push in front using move construct: " << arg << endl; 
        if (front == (back + 1) % capacity) {
            // It means now we have allocate new memory. 
            T* new_loc = (T*) operator new(capacity * 2 * sizeof(T));

            for (int i=0; i < size(); ++i) {
                new (new_loc + i) T{std::move(this->operator [](i))};
            }

            // Add new element.
            // DIFF from the copy construct.
            new (new_loc + (capacity * 2) - 1) T(std::move(arg));
            
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
                front = front - 1;
            }
            // DIFF from the copy construct.
            new (buffer + front) T(std::move(arg));
        }

        // Increment the version after adding one element to the front.
        version += 1;
        push_fronts += 1;
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
        //cout << "Going to pop one element from back." << endl;
        
        // Call the destuctor on that particular object.
        (buffer + back)->~T();

        // Reduce the back counter by 1.
        if (back == 0) {
            back = capacity;
        } 
        back -= 1;

        // If all the elements in the array have been removed, 
        // then we would reset our front and back again.
        if (front == (back + 1) % capacity) {
            back = -1;
            front = -1;
        }

        // Removing element from the back would lead to increase in version.
        version += 1;
    }

    // void pop_front().
    void pop_front(void) {
        // This represents that there is no element in the array.
        if (front == -1) {
            throw std::out_of_range("Vector is already empty.");
        }
        //cout << "Going to pop one element from front." << endl;
        
        // Call the destuctor on that particular object.
        (buffer + front)->~T();

        // Move front counter by 1.
        front = (front + 1) % capacity;

        // If all the elements in the array have been removed, 
        // then we would reset our front and back again.
        // back == -1 check is redundant and stupid.
        if (front == (back + 1) % capacity) {
            back = -1;
            front = -1;
        }

        // Remove an element from the front.
        version += 1;
        pop_fronts += 1;
    }

    uint64_t getCapacity() {
        return capacity;
    }

    void print() const {
        //cout << "Front: " << front << "; Back: " << back << "; Size: " << size() << "; Capacity: " << capacity << endl;
        for (int i=0; i < size(); ++i) {
            //if (buffer[(front + i) % capacity] == nullptr) {
                //cout << "NO !!" << endl;
            //} else {
                ////cout << "Index[" << i << "]: " << buffer[(front + i) % capacity] << endl; 
                //cout << "Index[" << i << "]: " << this->operator [](i) << endl; 
            //}
        }
    }
private:
    void destroy(void) {
        for (int i=0; i < size(); ++i) {
            (this->operator [](i)).~T();
        }
        operator delete(buffer);
        //cout << "Going to destroy Vector object: " << this << endl;
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
                new (this->buffer + ((front + k) % capacity)) T{that[k]};
            }
        }
    }
};

} //namespace epl

#endif /* _Vector_h */
