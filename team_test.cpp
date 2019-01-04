#include <iostream>
#include <vector>
#include "team.h"
#include "bidder.h"
#include "tpgdata.h"

using namespace std;

int main() {
    TPGData& tpgData = TPGData::GetInstance();

    Team& team = Team::CreateTeam();
    srand48(3);
    Bidder& bidder1 = Bidder::CreateBidder(-1, 20, 20, 0); // action, featureDimension, maxProgSize, genTime
    Bidder& bidder2 = Bidder::CreateBidder(-2, 20, 20, 0);
    team.addBidder(bidder1.getId());
    team.addBidder(bidder2.getId());

    vector<double> feature;
    for (int i = 0; i < 20; i++)
        feature.push_back(i+1);
    cout << bidder1.bid(feature) << endl;
    cout << bidder2.bid(feature) << endl;
    // Notice the initial registers
    cout << bidder1.bid(feature) << endl;
    cout << bidder2.bid(feature) << endl;
    team.clearReg();
    cout << bidder1.bid(feature) << endl;
    cout << bidder2.bid(feature) << endl;

    cout << endl << "Test getAction()" << endl;
    team.clearReg();
    cout << team.getAction(feature) << endl;
    return 0;
}