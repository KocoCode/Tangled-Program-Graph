#include <iostream>
#include <vector>
#include <cassert>
#include "bidder.h"
#include "tpgdata.h"


using namespace std;

int main() {
    srand48(3);
    TPGData& tpgData = TPGData::GetInstance();
    Bidder& bidder1 = Bidder::CreateBidder(0, 20, 20, 0); // action, featureDimension, maxProgSize, genTime
    cout << "Bidder1: " << bidder1.getId() << endl;
    bidder1.printProg();

    cout << endl;
    Bidder& bidder2 = Bidder::CreateBidder(0, 20, 20, 0);
    cout << "Bidder2: " << bidder2.getId() << endl;
    bidder2.printProg();

    cout << endl;
    Bidder& bidder3 = Bidder::CreateBidder(bidder2, 1);
    cout << "Bidder3: " << bidder3.getId() << endl;
    bidder3.printProg();

    return 0;
}