//Sir, is saying something Event driven simulation and I am thinking about project 2 k jo number kat gaye. Som babu notes
//bana rahe hain. Dude hain woh.
//
//yadav g meri screen pe dekh rahe hain, unke 6 test case fail huye hain .. haunsla de rahe hain.
//akash bhi dekh raha hai, saala paper padta rehta hai class mein aa k.. vehla...chuuuuuuutiiiyaaa..
//prad saala class mein hi khayega aa k. Pizza lae k aaya, bukh to mujhe bhi lage hai.. 
//
//@akash: notes/communication khel raha hai..
//This is for future reading.. something like dairy writing. just for the reecords. git pe rahega.
//ghanta kisi ko samajh lagega.
//
//============= 

// Gaane ho jaye ek.. ni tun vichdan vichdan kardi hain, jad vichde ge pata lag juga...

// I guess sir is describing the project.
EventQueue - priority queue // ordered by time.

// I was just thinking whether can I do this project in python. shit I think.
main() {

    initialize_world();
    while(!done) {
        Event e = EventQueue.next();
        now = e.time;
        e.handle(); // Sir want to focus on this guy.
        // Reasonable in size and reasonable in ..
    }
}

std::function<void(void)> f = ...;

// Not a polymorphic class.
class Event {
    double time;
    std::function<void<void> handle;


}

// Can we inline the actual call. We did it using virtual thing.
//
// std_function<void(void)> f;
// f = ...; (takes LifeForm::eat and &obj_x) 
// f(); 
//
// Something saying something about std::bind. Sir, does not want us to go this direction.
// Sir, want us to inline the f() call.
//
// Anyone looked at the c+++ lambdas.. or Java-8 lambdas.
//

// Lambdas in C++.
// x.doit(); // Instead, of actually calling, but we want to capture that invoke doit on x.
//
// class myFan {
//  T& obj;
//  void Op()(void) { obj.doit(); }
// }
//
// Capture expression. It can be empty. Sir want to capture x by reference.
// [&x] (void) { x.doit(); } 
//
//
