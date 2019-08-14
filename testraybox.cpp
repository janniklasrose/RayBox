// testraybox.cpp

#include "raybox.hpp"
#include <iostream>
#include <cassert>

void print (bool test) {
    std::cout << "The ray does" << (test?"":" not") << " intersect the box\n";
}

bool hit () {
    Box box( Vector3(1,1,1), Vector3(2,2,2) );
    Ray ray( Vector3(0,0,0), Vector3(3,4,5) );
    bool test = box.intersect(ray, 0, 1);
    print(test);
    return test; // is supposed to hit
}

bool miss () {
    Box box( Vector3(2,2,2), Vector3(3,3,3) );
    Ray ray( Vector3(0,0,0), Vector3(1,1,1) );
    bool test = box.intersect(ray, 0, 1);
    print(test);
    return !test; // is supposed to miss
}

bool inside () {
    Box box( Vector3(0,0,0), Vector3(2,2,2) );
    Ray ray( Vector3(1,1,1), Vector3(3,4,5) );
    bool test = box.intersect(ray, 0, 1);
    print(test);
    return test; // is supposed to hit
}

int main () {
    assert(hit());
    assert(miss());
    assert(inside());
    return 0;
}

