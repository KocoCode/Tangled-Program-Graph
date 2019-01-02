#include <iostream>
#include <vector>
#include <cassert>
#include "bidder.h"


using namespace std;

int main() {
    srand48(3);
    Bidder bidder(0, 0, 20, 20); // action, genTime, featureDimension, maxProgSize
    bidder.printProg();

    cout << endl << "EXEC" << endl;
    vector<double> feature;
    for (int i = 0; i < 20; ++i)
        feature.push_back(i+1);
    cout << bidder.bid(feature) << endl;
/*
    Bidder bidder2(bidder, 1); // toCopy, genTime
    bidder2.printProg();
*/
    return 0;
}