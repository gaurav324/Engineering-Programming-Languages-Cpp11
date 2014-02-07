###Phase 1A
- std::move takes actual object as parameter and not a pointer.

- operator delete(ptr) just deallocates the spaces. thats it. Use that instead of operator delete[]

- new (pointer) T() is called placement new.

- ptr->~ClassName(); it just destructs the object pointed by ptr.

- call: operator[](i) or *(head + i)

- No const in the return of assignment operator, but should be in the param

- Copy constructor can access private members of param

- a non-const method can not be called for a const object

- a const method can be called from (this) or from a non-const object

- if defined just copy const., and not assignment const. and if you use assignment, compiler says that the method is deleted. Assign the trio always if assigning any one of them.


---

###smart_pointer

- operator= returns Klass& type and returns *this.

- operator-> returns actual object hence T*

- operator* returns a pointer hence T&

