//
// Lecture 23 - 04/22/2014
//
// The time will be as fine grained as double will allow.
// If the lifeform is going in a straight line, and we know which direction he was 
// going and how fast he was going we can just calculate his position. Every life form 
// object has a position. All you need to know is the delta-time. The update is a data member
// in the life form base class. Almost all the objects will have stale information at any time.
// By definition all objects are stale except the one you are working on. 
//
// If an object is about to change its course or change its speed, I have to update its state.
// All of the data members in the lifeform class are private and all of the updater functions are
// protected. How does the simulation recognize that Craig object is trying to turn? The Craig object
// will create an event for the future - because the Craig object is running a behavioural model 
// internally which says that the Craig object moves for 5 seconds and turns right. But what if the 
// object collides before the event can be handled?
//
// When we update the object's state, we should change all the information contained in the object.
// Also need to check if the object is dead. Objects are continuosly expending energy, the energy penalty
// is defined in params.h - if the object is not still alive then you mark it as dead using a flag and take 
// it out of the quad tree. If an object reaches the end of the world, it has to be marked as dead.
// Schedule an event to update the object state when it is transitioning out of a region into another. 
// Transitioning out of a region out of the world is a special case. 
//
// Collision cannot happen if distance between two objects is more than 1.0
// Before you simulate the collision between X and Y, you need to update the information of both X and Y. 
// As soon as you update the object's position you update the objects position in the quad-tree. 
// The problem is when I moved X, I have invalidated the event created by Y because I have modified the regions.
// Whenever anything like that happens, we have cancel the events for Y and reschedule the event for Y so that 
// it happens at the right time. The mechanic that we use in Project 3 is that every object that is inserted into
// the quad tree, is inserted with a callback (region_resize_callback) - whenever the events need to be updated, 
// that callback is invoked. What could happen is that X causes update for Y, Y causes update for X and so on and 
// you need to be careful to make sure that you do not code that badly. 
//
// Find the nearest object always - and that will be only one object. 
// Add enough fudge factor to the time, small enough that it won't impact the simulation but 
// bug enoguh that it does what you want (make something cross the border).
// set_course, set_speed can cancel a scheduled event - border_cross cannot as the event has already happened. 
// Quad tree knows where the borders are, so you have to ask the quad tree where is the border by giving it a course. 
// All the functions which you will write will be written for the Lifeform base class. Please create events on the heap.
// Time given to events is going to be relative time. 
//
// border_cross_event = new Event(
//							delta_t, [me] (void) { 
//													me->border_cross(); 
//												 });
