#include <cstdint>
#include <stdexcept>
#include "gtest/gtest.h"
#include "Vector.h"
#include <list>

//define PHASE_C

using epl::Vector;
using namespace std;

#ifdef PHASE_C
class Foo {
  public:
    bool alive;
    uint64_t val{0};
    
    static uint64_t constructions;
    static uint64_t destructions;
    static uint64_t copies;
    static void reset(){ copies=destructions=constructions=0; }

    Foo(uint64_t value) {alive = true; ++constructions; val = value;}
    Foo(uint64_t value1, uint64_t value2) {alive = true; ++constructions; val = value1 * value2;}
    Foo(void) { alive = true; ++constructions; }
    ~Foo(void) { destructions += alive; }
    Foo(const Foo& that) noexcept { alive = true; ++copies; val = that.val;}
    Foo(Foo&& that) noexcept { that.alive = false; this->alive = true; val = that.val;}
    Foo& operator=(const Foo& that) noexcept { alive=true; copies += 1; val = that.val; return *this; }
    bool operator==(const Foo& that) { if (val == that.val) {return true;} else {return false;} }

    void print(std::ostream& out) const {out << val;}
    //Foo& operator=(Foo&& that) noexcept { that.alive = false; this->alive = true; return *this; }
  };
  uint64_t Foo::constructions = 0;
  uint64_t Foo::destructions = 0;
  uint64_t Foo::copies = 0;

std::ostream& operator<<(std::ostream& out, Foo f) {f.print(out); return out;}


TEST(PhaseC, direct_init_list){
	Vector<int> w{ 1, 2, 3 };
	EXPECT_EQ(3, w.size());
}

#define ARRAY_SIZE(X) (sizeof(X)/sizeof(*X))
TEST(PhaseC, copy_init_list){
  Vector<int> x = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 42, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int ans[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 42, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  EXPECT_EQ(ARRAY_SIZE(ans), x.size());
  for(unsigned int i=0; i<ARRAY_SIZE(ans); ++i)
    EXPECT_EQ(x[i], ans[i]);
}

TEST(PhaseC, range_for_loop){
	Vector<int32_t> x(10);

	int32_t k = 0;
	for (auto& v : x) {
		v = k++;
	}

	const Vector<int32_t>& y = x;
	int32_t s = 0;
	for (const auto& v : y) {
		s += v;
	}

	EXPECT_EQ(45, s);
}

TEST(PhaseC, ItrExceptSevere){
  Vector<int> x(1);
  auto itr = begin(x);
  x.pop_back();
  try{
    int a;
    a = *itr;
    FAIL();
    *itr = a;
  } catch (epl::invalid_iterator ii){
    EXPECT_EQ(ii.level, epl::invalid_iterator::SEVERE);
  }
}

TEST(PhaseC, ItrExceptModerate){
	//Vector<int>  x(3), y{1,2,3};
	Vector<int>  x(3), y(3);
	auto xi = begin(x);
	
	x = y;
	try{
		*xi = 5;
    FAIL();
	}
	catch(epl::invalid_iterator ex){
    EXPECT_EQ(ex.level, epl::invalid_iterator::MODERATE);
	}
}

TEST(PhaseC, ItrExceptMild){
  Vector<int> x(3);
  auto itr = begin(x);
  x.pop_back();
  try{
    volatile int a;
    a = *itr;
    FAIL();
    *itr = a;
  } catch (epl::invalid_iterator ii){
    EXPECT_EQ(ii.level, epl::invalid_iterator::MILD);
  }
}

TEST(PhaseC, InitializerListNanda) {
    Vector<int> x{1, 2, 3, 4, 5};
    auto x_itr = x.begin();

    // Test-1
    EXPECT_EQ(*(x_itr + 2), 3);

    Vector<int> y(x.begin() + 1, x.end() - 1);
    auto y_itr_s = y.begin();
    for (;y_itr_s != y.end(); ++y_itr_s) {}
    
    // Test-2(a)
    EXPECT_EQ(*(--y_itr_s), 4);
    // Test-2(b)
    EXPECT_EQ(*(y.end() - 1), 4);

    list<int> l{10, 11, 12, 13, 14};
    Vector<int> z(l.begin(), l.end());
    // Test-3(a)
    EXPECT_EQ(*(z.begin() + 2), 12);
    // Test-3(b)
    EXPECT_EQ(*(z.end() - 1), 14);

    // Change something and check end de-reference throws an severe.
    try {
        Vector<int> x1{1, 2, 3, 4, 5};
        auto itr = x1.end();
        x1.push_front(15);
        x1.pop_front();
        cout << *itr;
    } catch (epl::invalid_iterator ii){
        EXPECT_EQ(ii.level, epl::invalid_iterator::SEVERE);
    }
}

TEST(PhaseC, EmplaceBackNanda) {
    Vector<Foo> x{1};
    x.pop_front();

    x.emplace_back(2, 10);
    x.push_front(Foo(1));
    x.push_back(3);

    // Test - 1
    EXPECT_EQ(x[0].val + x[1].val + x[2].val, 24);

    // Test - 2
    const Vector<Foo> y(x);
    auto const_itr = y.begin();
    
    auto non_const_itr = x.begin();
    // This should not even compile.
    //non_const_itr = const_itr;
}

TEST(PhaseC, ExceptionsSEVERENanda) {

    try {
        Vector<int> x {1, 2, 3, 4, 5};
   
        // Check what is moved, what is left.
        auto i1 = x.begin();
        Vector<int> y;
        y = std::move(x);
        *i1 = 10;
    } catch (epl::invalid_iterator ii){
        EXPECT_EQ(ii.level, epl::invalid_iterator::SEVERE);
    }

    try {
        Vector<Foo> v{1, 2, 3, 4};
        auto it1 = v.begin();
        it1 += 2;
        
        v = std::move(Vector<Foo>{1});
        *it1 = 10;
    } catch (epl::invalid_iterator ii){
        EXPECT_EQ(ii.level, epl::invalid_iterator::SEVERE);
    } 

    try {
        Vector<int> x {1, 2, 3, 4, 5};
        auto it = x.begin();
        it += 1;
        x.pop_front();
        x.pop_front();
        
        // This should not raise SEVERE expectation, but throw
        // a pop-front corresponding WARNING.
        EXPECT_EQ(it, x.end() - 4);
    
    } catch (epl::invalid_iterator ii){
        EXPECT_EQ(ii.level, epl::invalid_iterator::WARNING);
    } 
}

TEST(PhaseC, ExceptionsMODERATENanda) {
    try {
        Vector<Foo> v{1, 2, 3, 4};
        auto it1 = v.begin();
        
        v = std::move(Vector<Foo>{1});
        // Any assignment operation should result in moderate exception.
        // Even though there is no reallocation.
        it1 += 1;
    } catch (epl::invalid_iterator ii){
        EXPECT_EQ(ii.level, epl::invalid_iterator::MODERATE);
    } 

    try {
        Vector<Foo> v{1, 2, 3, 4};
        auto it1 = v.begin();
        
        // Would result in reallocation.
        v.push_back(5);
        it1 += 1;
        
    } catch (epl::invalid_iterator ii){
        EXPECT_EQ(ii.level, epl::invalid_iterator::MODERATE);
    }
}

TEST(PhaseC, ExceptionsMILDNanda) {
    try {
        Vector<Foo> v{1, 2, 3, 4};
        auto it1 = v.begin();
        
        v.pop_back();
        it1 += 1;
    } catch (epl::invalid_iterator ii){
        EXPECT_EQ(ii.level, epl::invalid_iterator::MILD);
    } 

    try {
        Vector<Foo> v{1, 2, 3, 4};
        auto it1 = v.begin();
        
        // Would not result in reallocation.
        v.pop_back();
        v.push_back(5);
        it1 += 1;
        
    } catch (epl::invalid_iterator ii){
        EXPECT_EQ(ii.level, epl::invalid_iterator::MILD);
    }
}

TEST(PhaseC, ExceptionsWARNINGNanda) {
    try {
        Vector<Foo> v;
        auto it1 = v.begin();
        
        v.push_front(4);
        it1 += 1;
    } catch (epl::invalid_iterator ii){
        EXPECT_EQ(ii.level, epl::invalid_iterator::WARNING);
    } 

    try {
        Vector<Foo> v(2);
        v.push_back(1);
        auto it1 = v.begin();
        
        v.pop_front();
        // Because this is de-reference, this would be SEVERE.
        *it1 = 10;
        
    } catch (epl::invalid_iterator ii){
        EXPECT_EQ(ii.level, epl::invalid_iterator::SEVERE);
    }
}


#endif
