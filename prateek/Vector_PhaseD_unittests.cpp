using namespace std;

#include <forward_list>
#include <iostream>
#include <cstdint>
#include <stdexcept>
#include <future>
#include <chrono>
#include "gtest/gtest.h"
#include "Vector.h"

#ifdef _MSC_VER
#define noexcept
#endif

using std::cout;
using std::endl;
using epl::Vector;

/*****************************************************************************************/
// Class Instrumentation
/*****************************************************************************************/
namespace{
  //Class Instrumentation
  class Foo {
  public:
    bool alive;
    
    static uint64_t constructions;
    static uint64_t destructions;
    static uint64_t copies;
    static uint64_t moves;
    static void reset(){ moves=copies=destructions=constructions=0; }

    Foo(void) { alive = true; ++constructions; }
    ~Foo(void) { destructions += alive;}
    Foo(const Foo&) noexcept { alive = true; ++copies; }
    Foo(Foo&& that) noexcept { that.alive = false; this->alive = true; ++moves;}
  };

  uint64_t Foo::constructions = 0;
  uint64_t Foo::destructions = 0;
  uint64_t Foo::copies = 0;
  uint64_t Foo::moves = 0;

/* base template (default) */
template <typename It>
struct iterator_traits {
	using value_type = typename It::value_type;
	using iterator_category = typename It::iterator_category;
};

} //namespace

/*****************************************************************************************/
// Phase C Tests
/*****************************************************************************************/
//TEST SUITE 1
//check begin(), end(), *, * modify, ++, !=

TEST(PhaseD, basic_iteration_functionality) {
	Vector<int> x1;

    for(int x = 0; x <= 10; x++)
       x1.push_back(x);
   Vector<int>::iterator it = x1.begin();
   Vector<int>::iterator it1 = it;
   it = it1;
   using T = Vector<int>::iterator::value_type;
   for(T y=1; it != x1.end(); ++it, ++y) {
       ++*it; --it; ++it; 
       it = it -1 ; it = it + 1;
       EXPECT_EQ(*it, y);
   }
}

TEST(PhaseD, constant_iterator) {
   Vector<int> x2;

   for(int x = 0; x <= 10; x++)
       x2.push_back(x);
   const Vector<int>& x1 = x2;
   Vector<int>::const_iterator it = x1.begin();
   Vector<int>::const_iterator it1 = it;
   it = it1;
   for(int y=0; it != x1.end(); ++it, ++y) {
       --it; ++it; 
       it = it -1 ; it = it + 1;
       //++*it; Uncomment should throw compiler error
       EXPECT_EQ(*it, y);
   }
}  

//Check for conversion from iterator to const_iterator
TEST(PhaseD, constant_iterator_equals_iterator) {
   Vector<int> x2;

   for(int x = 0; x <= 10; x++)
       x2.push_back(x);
   Vector<int>::iterator it = x2.begin();
   const Vector<int>& x1 = x2;
   Vector<int>::const_iterator it1 = it;
    //Check for conversion from const iterator to iterator should throw up compiler error.
   //it = it1;
   it1 = it;
   for(int y=0; it1 != x1.end(); ++it1, ++y) {
       //++*it1; //Uncomment should throw compiler error
       EXPECT_EQ(*it1, y);
   }
}

TEST(PhaseD, ItrExceptSevere){
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

TEST(PhaseD, ItrExceptModerate){
  Vector<int> x(1);
  Vector<int> y(1);
  auto itr = begin(x);
  auto itr1 = begin(y);
  try{
    if(itr == itr1)
        cout<<"must fail";
    FAIL();
  } catch (epl::invalid_iterator ii){
    EXPECT_EQ(ii.level, epl::invalid_iterator::MODERATE);
  }
}

TEST(PhaseD, direct_init_list){
	Vector<int> w{ 1, 2, 3 };
	EXPECT_EQ(3, w.size());
}

#define ARRAY_SIZE(X) (sizeof(X)/sizeof(*X))
TEST(PhaseD, copy_init_list){
  Vector<int> x = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 42, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int ans[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 42, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  EXPECT_EQ(ARRAY_SIZE(ans), x.size());
  for(unsigned int i=0; i<ARRAY_SIZE(ans); ++i)
    EXPECT_EQ(x[i], ans[i]);
}

TEST(PhaseD, range_for_loop){
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

TEST(PhaseD, iterator_constructor) {
    std::forward_list<int> fwd_list = { 34, 77, 16, 2 };
    Vector<int> x(fwd_list.begin(), fwd_list.end());
    EXPECT_EQ(4, x.size());

    Vector<int> y(x.begin(), x.end());
    y.push_back(6);
    EXPECT_EQ(4, x.size());
    EXPECT_EQ(5, y.size());

    int z1[7] = {7,6,5,4,3,2,1};
    Vector<int> z(z1, z1+7);
    EXPECT_EQ(7, z.size());

    Vector<int> q(x.begin(), x.begin());
    q.push_back(5);
    EXPECT_EQ(1, q.size());
}

//this is the main entry point for the program.  Other
//tests can be in other cpp files, as long as there is
//only one of these main functions.
