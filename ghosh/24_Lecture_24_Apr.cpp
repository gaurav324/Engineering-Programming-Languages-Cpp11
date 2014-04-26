// Lecture 24 : 24th April 2014
// ----------------------------
//
// Action encounter(ObjInfo) = 0;
// If X returns EAT 
//   drand48() < eat_success_change(X, Y)
// Check collisions
//  - when an object crosses a border
//  - with the closest object
//  - using current stats
// Set the energy to 0 on die. The one that is doing the eating, he gains the energy 
// at some point in the future - schedule an event.
// Its a discreet thing. 
//
// Whenever you create an event make sure the event uses a SmartPointer for its object 
// and not regular pointers. 
// There isn't an encounter penalty if you bump into each other and it returns IGNORE.
// 
// When I cancel it clear it. 
// border_cross_event -> cancel();
// border_cross_event = 0;
// The problem is in various almost reentrant codepaths you try to cancel it again. It might be a 
// null pointer. 
//
//
// Procedure0 fi;
// Procedure1<int> g;
// Procedure2<Arg1, Arg2>
//
// Procedure0 fj;
// f()
// f = &::doit;
// f = &T::memfun and X;  <-- pointer to member function and object ref (have to come back and invent syntax)
// f = make_procedure(this, &t::mem_fun);
// f();
//

template <typename T>
Procedure0 make_procedure(T& x, void(T::*fun)(void)) {
	Procedure0_memfun<T>* fun = new ...;
	return Procedure0{fun};
}

class _Procedure0 {
	virtual void operator()(void) = 0;
	virtual _Procedure0* clone() = 0;
};

class Procedure0 {
	_Procedure0* ptr;
public:
	Procedure0();

	Procedure0(const Procedure0& proc);

	Procedure0& operator=(const Procedure0& proc);

	// This will delete the older pointer and create a new object of
	// the type Procedure0_fun
	Procedure0& operator=(void (*p)(void));

	// This will delete the older pointer and create a new object of 
	// the type Procedure0_memfun
	Procedure0& operator=(T& x, void(T::*p)(void));

	void operator()(void) {
		(*ptr)();
	}
};

class Procedure0_fun : public _Procedure0 {
	using Same = Procedure0_fun;
	void (*p)(void);
	Procedure0_fun(void (*p)(void)) {
		this->p = p;
	}

	void operator()(void) {
		(*p)();
	}

	Procedure0_fun* clone(void) const {
		return new decltype(*this)(*this);
	}
};

template <typename T>
class Procedure0_memfun : public _Procedure0 {
	T& obj;
	void (T::*p)(void);
	Procedure0_memfun(T& x, void(T::*p)(void)) : obj(x) {
		this->p = p;
	}

	void operator()(void) {
		(obj.*p)();
	}

	Procedure0_memfun* clone(void) const {
		return new decltype(*this)(*this);
	}
};