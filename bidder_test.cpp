#include <iostream>
#include <cassert>
#include "bidder.h"

using namespace std;

int main() {
    srand48(3);
    Bidder bidder(0, 0, 20, 20);
    bidder.printProg();

    cout << endl;

    Bidder bidder2(bidder, 1);
    bidder2.printProg();

    return 0;
}