// Ameya is taking class.


// In normal circumstances.
foo () {
    int  *p = new int;

    delete p;
}

// What if we have a global int *q and
//
// int *q;
foo() {
    int  *p = new int;

    q = p

    delete p; // Haha, this is bad.
}

// So let us create a smart pointer and keep track of references and enjoy. Taraaah !!

// Ameya bhaji is telling us that ik container bana lao.. us de vich T* and count
// dovan nu paa dao.. main b ehi socheya hi par, saala jadon nava pointer banana waa,
// fir reflect back kiven hoyega saare blocks de vich.
//
// Isda matlab aa count nu har block de vich nahi, balki bahar kite store kar lao. 
// and make everyone point to that count.
//
// Gal badi simple jahi hai, bas ik control block type cheej bana lao and 
// share that block idhar udhar. Now when u change something in the shared control
// block, apne aap saare gal baat theek ho jayegi.
//
// Ik vaare, delete and delete[] da farak check kar hi lae.. saala isne badi bund lae rakhi hai.
//
//
// What is I do 
SmartPointer a;

SmartPointer b (std::move(a));

// Now, if we make a.cntl = null, then what will happen.
