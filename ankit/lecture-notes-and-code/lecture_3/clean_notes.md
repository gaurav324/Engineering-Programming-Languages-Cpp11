## lecture 3: constructors, templates

Following code gives a run time exception. Since `new` allocates and initializes the memory for an array of objects of type foo. It stores extra information (size of the array) at index -1. Pointer p points to 0th index.

```
#include<iostream>
#include<cstdint>

class Foo{
    public:
        Foo(void) { std::cout << "default constructor\n"; }
        ~Foo(void) { std::cout << "destructor\n"; }
};

int main(void){
    Foo * p = new Foo[100]; 
    delete p;
}

```

Above code can be easily fixed using:
```
delete[] p
```

---

### Operator syntax `X+Y` vs Function Syntax `operator+(X,Y)`

There is no difference between operator syntax and function call syntax for any method **except** `new` and `delete`

**`new`**

`new` used with **operator syntax** : space is **allocated** and **initialized**.

`new` used with **function syntax** : splice is just **allocated**. Not initialized.

---

Update: Prateek (01/26)

If creating an array of objects, you would need to have default constructor for that class, else compilation will fails.

***When You Need a Default Constructor***

Consider arrays of objects. The act of creating an array of objects accomplishes two tasks: It allocates contiguous memory space for all the objects and it calls the default constructor on each object. C++ fails to provide any syntax to tell the array creation code directly to call a different constructor. For example, if you do not define a default constructor for the SpreadsheetCell class, the following code does not compile:

```
SpreadsheetCell cells[3]; // FAILS compilation without default constructor 
SpreadsheetCell* myCellp = new SpreadsheetCell[10]; // Also FAILS
```
