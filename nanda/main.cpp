// SmartPointer/Source.cpp -- case study and exercise on reference counting and smart pointers

/*
 * The purpose of this exercise is to design a pointer that performs
 * reference counting and automatically deallocates objects once the
 * number of references to that object reaches zero. The C++ standard
 * library includes a type std::shared_ptr<T> which implements the
 * functionality in this exercise. It is not necessary to look at 
 * the library specification for shared_ptr for this project. 
 * In fact, shared_ptr has some interesting functionality that is
 * is beyond the scope of what we can do (given what we know so far).
 * If you are tempted to look up shared_ptr, that's fine. I ask that
 * you limit your review to the online documentation for shared_ptr
 * and that you refrain from looking at the actual implementation.
 * Looking at the implementation will defeat the purpose of this exercise.
 *
 *
 * We will create a template class SmartPtr<T>. The purpose of this
 * class is to create an object reference that acts (syntactically) 
 * like a pointer of type T*
 * For example, you can initialize a SmartPtr using:
 *	SmartPtr<std::string> p { new std::string { "Hello World" } };
 *
 * The variable p becomes a reference to the string object created
 * on the heap. You can dereference a SmartPtr using operator* (just 
 * like a pointer) and operator-> (just like a pointer). So, for example
 *   if (p->size() > 0) { 
 *      cout << *p << endl;
 *   }
 *
 * The object p is a "smart" pointer because it tracks the number of 
 * references to the heap object. In the example above, p is the only
 * reference to the object on the heap, so the reference count is
 * equal to 1. If we were to create another reference, for example
 *   SharedPtr<std::string> q {p};
 * then both q and p reference the same object, and thus the reference
 * count must be incremented to two.
 *
 * A smart point will provide overloads for copy construction and
 * assignment that increment the reference count, and a destructor
 * that decrements the reference count. With our example, p and q
 * both reference the same object on the heap. When either variable
 * (p or q) goes out of scope, the destructor decrements the reference
 * count. When both variables have gone out of scope, the reference
 * count will be zero, and the desructor for SmartPtr will 
 * invoke delete (deallocating the object from the heap). 
 *
 * Using smart pointers creates an experience somewhat like using
 * a garbage collector. You allocate memory explicitly (by invoking new),
 * but you never deallocate memory explicitly -- the reference counting
 * takes care of deallocation. Smart pointers are limited to working
 * in applications where all object references are acyclic. For example
 * a classic doubly linked list is a poor candidate for smart pointers.
 * Even when the list itself is no longer being used, the elements that
 * make up the list are all referenced by (i.e., linked from) at 
 * least one other element in the list. Hence, none of the reference
 * counts ever goes to zero, even though the linked list itself is
 * garbage and should be deallocated.
 *
 * To complete this exercise, you must develop a SmartPointer<T> 
 * template that correctly works with the following simple example.
 * You will get the maximum benefit from the example if you try 
 * to desgin your solution to work generally (and not work only
 * for the very specific code below).
 */

#include <iostream>
#include <cstdint>
#include <cassert>
#include <memory>
#include <vector>
#include "SmartPointer.h"

using std::cout;
using std::endl;

/* your goal is to design this class */
template <typename T> class SmartPtr;

class Foo {
public:
	static int32_t constructions;
	static int32_t destructions;

	int id;
	Foo(void) {
		id = constructions;
		constructions += 1;
	}

	~Foo(void) {
		destructions += 1;
	}
};

int32_t Foo::constructions = 0;
int32_t Foo::destructions = 0;

/* the test program works when executed with shared_ptr<Foo>,
 * your goal is to make it work when using your SmartPtr<Foo> */
//using Pointer = std::shared_ptr<Foo>;
using Pointer = SmartPtr<Foo>;

int main(void) {
	{ // first test
		Pointer p{new Foo};
		assert(p->id == 0);
	}
//	assert(Foo::constructions == 1 && Foo::destructions == 1);
//	cout << "first test passed\n";
//
//	{ // second test
//		std::vector<Pointer> v;
//		v.push_back(Pointer{new Foo});
//		v.push_back(v[0]);
//		v.push_back(v[0]);
//		assert(v[0]->id == 1 && v[1]->id == 1);
//
//		v.push_back(Pointer{nullptr}); 
//		assert(Foo::constructions == 2 && Foo::destructions == 1);
//
//		Pointer p{new Foo};
//		v[1] = p;
//		assert((*p).id == 2 && v[1]->id == 2);
//		assert(Foo::constructions == 3 && Foo::destructions == 1);
//
//		v.pop_back(); // remove nullptr
//		v.pop_back(); // remove v[2] (references same object as v[0], object id 1)
//		v.pop_back(); // remove v[1] (references same object as p, object id 2)
//		assert(Foo::constructions == 3 && Foo::destructions == 1);
//
//		v.pop_back(); // remove v[0], forces destruction of object id 1
//		assert(Foo::constructions == 3 && Foo::destructions == 2);
//	} // p goes out of scope, forces destruction of object id 2
//	assert(Foo::constructions == 3 && Foo::destructions == 3);
//	cout << "second test passed\n";
}
