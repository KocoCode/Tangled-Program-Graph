#include <iostream>
#include <cassert>
#include "poolproxy.h"
#include "bidder.h"
#include "team.h"

using namespace std;

class PoolProxyTest {
private:
    PoolProxy& poolProxy;
    int featureDimension;
    int maxProgSize;
    int genTime;

public:
    PoolProxyTest(): poolProxy(PoolProxy::GetInstance()),
            featureDimension(10),
            maxProgSize(20),
            genTime(0) { }
    
    void bidderAssert(Bidder& bidder, int id, int action, int refCount) {
        assert(bidder.getId() == id);
        assert(bidder.getAction() == action);
        assert(bidder.getRefCount() == refCount);
    }

    void teamAssert(Team& team, int id, int refCount, bool isRoot, size_t bidderSize) {
        assert(team.getId() == id);
        assert(team.getRefCount() == refCount);
        assert(team.isRoot() == isRoot);
        assert(team.getBidders().size() == bidderSize);
    }
    
    void test() {
        int bidderId0 = poolProxy.bidderCreate(-1, featureDimension, maxProgSize, genTime);
        Bidder& bidder0 = poolProxy.bidderGet(bidderId0);
        bidderAssert(bidder0, 0, -1, 0);

        int bidderId1 = poolProxy.bidderCreate(-2, featureDimension, maxProgSize, genTime);
        Bidder& bidder1 = poolProxy.bidderGet(bidderId1);
        bidderAssert(bidder1, 1, -2, 0);

        int teamId0 = poolProxy.teamCreate(genTime);
        Team& team0 = poolProxy.teamGet(teamId0);
        teamAssert(team0, 0, 0, true, 0);

        poolProxy.teamAddBidder(teamId0, bidderId0);
        bidderAssert(bidder0, 0, -1, 1);
        teamAssert(team0, 0, 0, true, 1);
        poolProxy.teamAddBidder(teamId0, bidderId1);
        bidderAssert(bidder1, 1, -2, 1);
        teamAssert(team0, 0, 0, true, 2);
    }
};

int main() {
    PoolProxyTest test;
    test.test();
}
