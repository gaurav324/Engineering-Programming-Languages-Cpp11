#include "SmartPointer.h"

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

using Pointer = SmartPtr<Foo>;

int main() {
    { // first test
		Pointer p{new Foo};
		assert(p->id == 0);
	}
	assert(Foo::constructions == 1 && Foo::destructions == 1);
	cout << "first test passed\n";

    { // second test
		std::vector<Pointer> v;
		v.push_back(Pointer{new Foo});
		v.push_back(v[0]);
		v.push_back(v[0]);
		assert(v[0]->id == 1 && v[1]->id == 1);

		v.push_back(Pointer{nullptr}); 
		assert(Foo::constructions == 2 && Foo::destructions == 1);

		Pointer p{new Foo};
		v[1] = p;
		assert((*p).id == 2 && v[1]->id == 2);
		assert(Foo::constructions == 3 && Foo::destructions == 1);

		v.pop_back(); // remove nullptr
		v.pop_back(); // remove v[2] (references same object as v[0], object id 1)
		v.pop_back(); // remove v[1] (references same object as p, object id 2)
		assert(Foo::constructions == 3 && Foo::destructions == 1);

		v.pop_back(); // remove v[0], forces destruction of object id 1
		assert(Foo::constructions == 3 && Foo::destructions == 2);
	} // p goes out of scope, forces destruction of object id 2
	assert(Foo::constructions == 3 && Foo::destructions == 3);
	cout << "second test passed\n";
    
    {
        cout << "Constructions: " << Foo::constructions << endl;
        cout << "Destructions: " << Foo::destructions << endl;

        Pointer a{new Foo};
        //Pointer b{a};
        Pointer b (std::move(a));

        cout << "Id of b: " << b->id << endl;

        cout << "Constructions: " << Foo::constructions << endl;
        cout << "Destructions: " << Foo::destructions << endl;
 
        b = nullptr;
        cout << "Constructions: " << Foo::constructions << endl;
        cout << "Destructions: " << Foo::destructions << endl;
    }
}
