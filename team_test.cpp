#include <iostream>
#include <cassert>
#include <cmath>
#include "bidder.h"
#include "team.h"
#include "poolproxy.h"

using namespace std;

int main() {
    PoolProxy& poolProxy = PoolProxy::GetInstance();
    int featureDimension = 10;
    int maxProgSize = 300;
    int genTime = 0;

    int teamId = poolProxy.teamCreate(genTime);
    Team& team = poolProxy.teamGet(teamId);

    assert(team.getId() == teamId);
    assert(team.getGenTime() == genTime);
    assert(team.getRefCount() == 0);
    assert(team.isRoot());
    team.incRefCount();
    assert(team.getRefCount() == 1);
    assert(!team.isRoot());
    team.decRefCount();
    assert(team.getRefCount() == 0);
    assert(team.isRoot());

    int bidderId0 = poolProxy.bidderCreate(-1, featureDimension, maxProgSize, genTime);
    int bidderId1 = poolProxy.bidderCreate(-2, featureDimension, maxProgSize, genTime);
    poolProxy.teamAddBidder(teamId, bidderId0);
    poolProxy.teamAddBidder(teamId, bidderId1);

    assert(team.bidderSize() == 2);

    for (auto bidderId: team.getBidders()) {
        assert(0 <= bidderId && bidderId < 2);
    }
    int count[2] = {0};
    for (int i = 0; i < 100; ++i) {
        ++count[team.randomBidder()];
    }
    assert(count[0] != 0);
    assert(count[1] != 0);
    assert(count[0] + count[1] == 100);

    assert(team.findBidder(bidderId0));
    assert(team.findBidder(bidderId1));
    assert(!team.findBidder(100));

    vector<double> feature(10);
    count[0] = count[1] = 0;
    for (int i = 0; i < 100; ++i) {
        for (auto &f: feature)
            f = drand48();
        unordered_set<int> visitedTeams;
        ++count[2 + team.getAction(feature, visitedTeams)]; // -1 -> count[1], -2 -> count[0]
    }
    assert(count[0] != 0);
    assert(count[1] != 0);
    assert(count[0] + count[1] == 100);

    team.addOutcome(2);
    assert(team.getMeanOutcome() == 2);
    team.addOutcome(4);
    assert(team.getMeanOutcome() == 3);
    team.clearOutcomes();
    team.addOutcome(90);
    assert(team.getMeanOutcome() == 90);

    return 0;
}