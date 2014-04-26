// Lecture 22 - April 17th 2014
// ----------------------------
//
// So you have an Event Queue which is a priority queue, where the events are ordered by time.
// We seed the queue with a bunch of initial events. 
// In our case we are simulating a case of bugs roaming around. If bugs collide they handle the 
// events - change direction, or create a new bug. Assuming our events only go forward in time 
// you add new events to the event queue. 
//
// So the event handling loop goes something like this:
// int main() {
//	initialize_world();
//	while (!done) {
//		Event e = eventQueue.next();
//		e.handle();
//	}
//}
//
// We want to design a software that is reasonably clean and extensible. If I ever wanted an event
// driven simulator then all this code should be able to be hauled out and made into a simulator with 
// minimal changes.
//

#include <iostream>
#include <cstdint>
#include <functional>

// For time, 0.001s is the lowest resolution
#define TIME_MIN_RES 0.001

class Event {
	// Time when the event occurs
	// There is no need to discretize time, the closest to Real is double
	double time;

	// The function that handles the event
	std::function<void(void)> handle;
};

// A template class that takes an argument for the return type and the parameter type
std::function<void (void)> f = &doit;

f = LifeForm::eat

// Implementation in case we did not use std::function but created 
// an epl::function
template <typename T>
struct function {
	T* obj;
	void (T::*p)(void);
	void operator()(void) {
		// Invoke the member function 'p' on the object
		(obj->*p)();
	}
};

// If we had std::function<Lifeform> handle, then it would mean it was an event for only the Lifeform
// class! What if we wanted to use it later to simulate network packets? What about templatizing Event?
// I want to have a runtime polymorphic event queue. Some of the events apply to all Lifeforms, some of the 
// events apply to different subclass of life forms.

// virtual void op()(void) = 0;
// we have a declaration of  avariable, we have the assignment and then we have the invocation.

// std::bind was part of the STL library since long ago. It takes a function that normally expects an argument and it takes
// a value for the argument, and provides you a function that no longer needs an argument. 

// C++ Lambdas
// -----------
//
auto f = [&x](void) { x.doit; };

// if you wanted to sort the the following could be done
// The third argument would have been matched with a template parameter as follows
// template <typename It, template Comp>
// std::sort(It b, It e, Comp c);
std::sort(x.begin(), x.end(), 
	[](String a, String b){ return b < a; });

// std::function has a member template assignment operator that makes it accept lambdas
// quad trees - ask question if any other bug is close by.



