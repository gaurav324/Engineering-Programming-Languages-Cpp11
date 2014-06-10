#include <iostream>
#pragma once

#ifndef _InstanceCounter_h
#define _InstanceCounter_h

using namespace std;
class InstanceCounter {
public:
        static int counter;
        InstanceCounter(void) {
            //cout << "Created" << endl;
                ++counter;
        }
};

#endif /* _InstanceCounter_h */
