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

- operator-> means pointer. Hence return type will be T* type.

- operator* means *ptr means actual object should be returned. Hence, T&

---

###String with move
- nullptr is good to have if running default constructor and printing the string. This will return empty string instead of an infinite one.
```
char *buffer {nullptr}; 
```
- An object of a class can access private members of another object of the same class.
- `size = strlen(buffer); //sizeof(buffer)/sizeof(char); sizeof just gives the memory occupied of that element itself. sizeof(buffer) gives 4, sizeof(*buffer) gives 1. strlen is the only way to get the length.`
- '=' can be used for primary types.
- `std::ostream& operator<<(std::ostream& out, const T& t) { t.print(out); return out;}` something like this should be done outside T class to `cout` T object.
- `const char* s = "hello"; // OK`
- `char* s = "hello"; // NOT OK - conversion from std::string to char* has been deprecated. `
