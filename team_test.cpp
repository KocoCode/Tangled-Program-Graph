#include <iostream>
#include <vector>
#include "team.h"
#include "bidder.h"
#include "tpgdata.h"

using namespace std;

int main() {
    Team& team = Team::CreateTeam(0); // genTime
    srand48(3);
    Bidder& bidder1 = Bidder::CreateBidder(-1, 20, 20, 0); // action, featureDimension, maxProgSize, genTime
    Bidder& bidder2 = Bidder::CreateBidder(1, 20, 20, 0);
    team.addBidder(bidder1.getId());
    team.addBidder(bidder2.getId());

    Team& team2 = Team::CreateTeam(0);
    team2.addBidder(bidder1.getId());
    cout << "bidder1 refCount: " << bidder1.getRefCount() << endl;
    cout << "bidder2 refCount: " << bidder2.getRefCount() << endl;
    cout << "team refCount: " << team.getRefCount() << endl;
    cout << "team2 refCount: " << team2.getRefCount() << endl;

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