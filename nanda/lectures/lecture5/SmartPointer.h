#include <iostream>
#include <cstdint>
#include <cassert>
#include <memory>
#include <vector>

using std::cout;
using std::endl;

/* your goal is to design this class */
template <typename T>
class SmartPtr {
private:
    struct ControlBlock {
        T* ptr;
        uint64_t count;
        ControlBlock(T* _ptr, uint64_t _count) : ptr(_ptr), count(_count) {};
        ~ControlBlock() { delete ptr; }
    };

    ControlBlock* cntl;
public:
    // Normal Constructor.
    SmartPtr(T* ptr) {
        cntl = new ControlBlock{ptr, 1};
    }

    // Copy Constructor.
    SmartPtr(const SmartPtr& that) {
        copy(that);
    }

    // Move Constructor.
    SmartPtr(SmartPtr&& that) {
        this->cntl = that.cntl;
        if(cntl) {
            cntl->count += 1;
        }
        //that.cntl = nullptr;
    }

    // Destructor.
    ~SmartPtr() {
        destroy();
    }

    // Assignment operator.
    SmartPtr& operator=(const SmartPtr& that) {
        if (this != &that) {
           destroy();
           copy(that);
        }
        return *this;
    }

    // InDirection operator.
    T* operator->() {
        return cntl->ptr;
    }
    
    // De-reference operator.
    T& operator*() {
        return *(cntl->ptr);    
    }

    // Copy contents of one SmartPointer to another.
    void copy(const SmartPtr& that) {
        this->cntl = that.cntl;
        this->cntl->count += 1;
    }

    // Destroy the Smart pointer object.
    void destroy() {
        if (cntl) {
            cntl->count -= 1;
            if (cntl->count == 0) {
                delete cntl;
            }
        }
    }
};

