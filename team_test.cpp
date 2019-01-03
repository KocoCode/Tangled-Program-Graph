#include <iostream>
#include <vector>
#include "team.h"
#include "bidder.h"
#include "tpgdata.h"

using namespace std;

int main() {
    Team team;
    srand48(3);
    Bidder bidder1(0, 0, 20, 20); // action, genTime, featureDimension, maxProgSize
    Bidder bidder2(0, 0, 20, 20); // action, genTime, featureDimension, maxProgSize
    team.addBidder(bidder1.getId());
    team.addBidder(bidder2.getId());

    // Now bidder1 and bidder2 are DIFFERENT instances from the ones in the bidderPool
    // Should get references of them from bidderPool
    Bidder& bb1 = TPGData::GetInstance().bidderPool.get(bidder1.getId());
    Bidder& bb2 = TPGData::GetInstance().bidderPool.get(bidder2.getId());
    // Better way to solve this?
    // write a static CreateBidder function for Bidder
    // Bidder& bidder1 = Bidder::CreateBidder(0, 0, 20, 20);

    vector<double> feature;
    for (int i = 0; i < 20; i++)
        feature.push_back(i+1);
    cout << bb1.bid(feature) << endl;
    cout << bb2.bid(feature) << endl;
    // Notice the initial registers
    cout << bb1.bid(feature) << endl;
    cout << bb2.bid(feature) << endl;
    team.clearReg();
    cout << bb1.bid(feature) << endl;
    cout << bb2.bid(feature) << endl;
    return 0;
}