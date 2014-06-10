#include <iostream>
#include <cstdint>
#include <cstring>

using std::cout;
using std::endl;


/* C++ retained one very stupid and annoying feature from C (OK, maybe more than just one)
 * When C++ was developed, the founders held out naive hope that someone would be able to
 * write a single-pass compiler for the language (Java and C# have no such illusions)
 * As a result, the language requires that all symbols (i.e., all names) must be 
 * at least declared before they are used. 
 *
 * Well, I wanted to add print statements to my constructors in String
 * that means the constructors will invoke the ostream operator << 
 * Since the ostream operator << is defined as a global scope function (not a member)
 * the definition of this function (i.e., the "overload") must be written after
 * the end of the String class definition. 
 * Since I'm calling the operator inside the class definition, I need to include
 * a declaration of the operator (many people refer to these declarations as "function prototypes").
 * Well, that's all fine, except that the 2nd argument to the operator is a String....
 *
 * So, to write String, I have to first declare operator<< for ostream
 * but to declare operator<< for ostream, I have to first declare String....
 * Yeah, it's stupid.
 * The workaround is to actually *declare* (not define) String, as shown below.
 */
class String;
std::ostream& operator<<(std::ostream&, const String&);

class String {
private:
	char* buffer{nullptr};
	uint32_t len{0};
public:
	String(void) = default;
	String(const char* input) { // not explicit
		len = strlen(input);
		buffer = new char[len];
		for (uint32_t k = 0; k < len; k += 1) {
			buffer[k] = input[k];
		}
	}

	/* copy semantics */
	String(const String& that) {
		cout << "copy construction from " << that << endl;
		copy(that);
	}

	String& operator=(const String& that) {
		cout << "copy (re)assignment " << *this << " changed to " << that << endl;
		if (this != &that) {
			destroy();
			copy(that);
		}
		return *this;
	}

	/* move semantics */
	/* when an object is "moved" we assume that the original object (the right-hand-side
	 * in an assignment statement) is a temporary object. Since that object is a temporary
	 * we know that this move operation is the last even in that object's lifetime (it's a temporary!)
	 * Therefore, the state of the RHS (right-hand-side) object is unimportant -- that object state
	 * cannot be used.
	 * A move constructor and a move assignment operator exploit this knowledge to perform 
	 * faster initialization and/or reassignment. Typically, move semantics are implemented
	 * with "shallow copies". 
	 *
	 * There is one very important caveat to move operators in C++. The concept of "move"
	 * is a design concept, it is NOT part of the language. The compiler treats the RHS
	 * just like it treats any other object. That means, the RHS object WILL BE DESTRUCTED
	 * as soon as the move operation completes (it's a temporary!).
	 * As a designer, therefore, we must implement our move operators so that the RHS
	 * destructor is rendered harmless. We can't stop it from running, so we set the internal
	 * state to null pointers (or the equivalent). 
	 *
	 * In this example, my move constructor is fairly normal. This example is how I recommend
	 * you think about move operators (and how you should probably write them)
	 * My move assignment is implemented in a slightly less orthodox manner. That operator
	 * performs re-assignment. So, it has to deallocate the LHS object and move 
	 * the internal state of the RHS. Normally, we'd do the deallocate directly in the 
	 * assignment operator (calling destroy() in this example). However, I *know* 
	 * that the RHS will have its destructor invoked. So, by swapping the internal
	 * states of the LHS and RHS objects, I accomplish both tasks -- I first move the
	 * internal state of the RHS, and then (via the RHS' destructor) deallocate the LHS.
	 */
	String(String&& rhs) {
		cout << "move construction from " << rhs << endl;
		buffer = rhs.buffer;
		len = rhs.len;
		rhs.buffer = nullptr; // essential, to make rhs.~String() into a no-op
		rhs.len = 0; // somewhat pointless, but for completeness, I'm erasing the entire state of rhs
	}

	String& operator=(String&& rhs) {
		cout << "move (re)assignment of " << *this << " from " << rhs << endl;
		/* NOTE this == &rhs is not possible for rvalue references, so we don't check for it 
		 * (why is it not possible? the answer to this question is left to the student) */
		std::swap(buffer, rhs.buffer);
		std::swap(len, rhs.len);
		return *this;
	}

	~String(void) {
		destroy();
	}

	void print(std::ostream& out) const {
		if (buffer) {
			for (uint32_t k = 0; k < len; k += 1) {
				out << buffer[k];
			}
		}
	}

	/* we need some methods, or we can't create temporaries. String concatenation is the obvious operation */
	void concat(const String& rhs) {
		cout << "concat " << *this << " and " << rhs << endl;
		if (rhs.len == 0) { return; }
		const char* p = buffer;
		buffer = new char[len + rhs.len];
		for (uint32_t k = 0; k < len; k += 1) {
			buffer[k] = p[k];
		}
		for (uint32_t k = 0; k < rhs.len; k += 1) {
			buffer[len + k] = rhs.buffer[k];
		}

		len += rhs.len;
		delete[] p;
	}

	/* technically, this method should return String&
	 * however, I've never met a programmer that writes code like x = y += z;
	 * (at least not a programmer I respect), so I see no reason to support such things */
	void operator+=(const String& rhs) { concat(rhs); }

	String operator+(const String& rhs) const {
		String result(*this); // copy construction
		result += rhs; // concat
		return result;
	}

private:
	void copy(const String& that) {
		this->len = that.len;
		if (len == 0) {
			buffer = nullptr;
		} else {
			this->buffer = new char[len];
			for (uint32_t k = 0; k < len; k += 1) {
				buffer[k] = that.buffer[k];
			}
		}
	}

	void destroy(void) { delete [] buffer; }
};

std::ostream& operator<<(std::ostream& out, const String& myString) {
	myString.print(out);
	return out;
}


int main(void) {
	String s {"Hello World"};
	s = s + s;
	cout << s << endl;
}











