#include <iostream>
#include <cassert>
#include "poolproxy.h"
#include "bidder.h"
#include "team.h"

#define myassert(EX, LL) (void)((EX) || (__assert (#EX, __FILE__, LL),0))

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
    
    void bidderAssert(int lineno, Bidder& bidder, int id, int action, int refCount) {
        myassert(bidder.getId() == id, lineno);
        myassert(bidder.getAction() == action, lineno);
        myassert(bidder.getRefCount() == refCount, lineno);
    }

    void bidderAssert(int lineno, int id, int action, int refCount) {
        Bidder& bidder = poolProxy.bidderGet(id);
        myassert(bidder.getAction() == action, lineno);
        myassert(bidder.getRefCount() == refCount, lineno);
    }

    void teamAssert(int lineno, Team& team, int id, int refCount, bool isRoot, size_t bidderSize) {
        myassert(team.getId() == id, lineno);
        myassert(team.getRefCount() == refCount, lineno);
        myassert(team.isRoot() == isRoot, lineno);
        myassert(team.getBidders().size() == bidderSize, lineno);
    }

    void teamAssert(int lineno, int id, int refCount, bool isRoot, size_t bidderSize) {
        Team& team = poolProxy.teamGet(id);
        myassert(team.getRefCount() == refCount, lineno);
        myassert(team.isRoot() == isRoot, lineno);
        myassert(team.getBidders().size() == bidderSize, lineno);
    }
    
    void poolProxyAssert(int lineno, int teamSize, int teamRootSize, int bidderSize) {
        myassert(poolProxy.teamSize() == teamSize, lineno);
        myassert(poolProxy.teamRootSize() == teamRootSize, lineno);
        myassert(poolProxy.bidderSize() == bidderSize, lineno);
    }

    void test() {
        int bidderId0 = poolProxy.bidderCreate(-1, featureDimension, maxProgSize, genTime);
        Bidder& bidder0 = poolProxy.bidderGet(bidderId0);
        bidderAssert(__LINE__, bidder0, 0, -1, 0);

        int bidderId1 = poolProxy.bidderCreate(-2, featureDimension, maxProgSize, genTime);
        Bidder& bidder1 = poolProxy.bidderGet(bidderId1);
        bidderAssert(__LINE__, bidder1, 1, -2, 0);

        int teamId0 = poolProxy.teamCreate(genTime);
        Team& team0 = poolProxy.teamGet(teamId0);
        teamAssert(__LINE__, team0, 0, 0, true, 0);

        poolProxy.teamAddBidder(teamId0, bidderId0);
        bidderAssert(__LINE__, bidder0, 0, -1, 1);
        teamAssert(__LINE__, team0, 0, 0, true, 1);
        poolProxy.teamAddBidder(teamId0, bidderId1);
        bidderAssert(__LINE__, bidder1, 1, -2, 1);
        teamAssert(__LINE__, team0, 0, 0, true, 2);

        poolProxyAssert(__LINE__, 1, 1, 2);

        //////

        int teamId1 = poolProxy.teamCreate(genTime);
        int bidderId2 = poolProxy.bidderCreate(-1, featureDimension, maxProgSize, genTime);
        int bidderId3 = poolProxy.bidderCreate(-1, featureDimension, maxProgSize, genTime);
        int bidderId4 = poolProxy.bidderCreate(-1, featureDimension, maxProgSize, genTime);
        int bidderId5 = poolProxy.bidderCreate(-1, featureDimension, maxProgSize, genTime);
        poolProxy.teamAddBidder(teamId1, bidderId2);
        poolProxy.teamAddBidder(teamId1, bidderId3);
        poolProxy.teamAddBidder(teamId1, bidderId4);
        poolProxy.teamAddBidder(teamId1, bidderId5);
        bidderAssert(__LINE__, bidderId2, -1, 1);
        bidderAssert(__LINE__, bidderId3, -1, 1);
        bidderAssert(__LINE__, bidderId4, -1, 1);
        bidderAssert(__LINE__, bidderId5, -1, 1);
        teamAssert(__LINE__, teamId1, 0, true, 4);
        poolProxyAssert(__LINE__, 2, 2, 6);

        poolProxy.teamRootMaintain();

        for (int i = 0; i < 100; ++i) {
            int bidderId = poolProxy.bidderRandom();
            assert(0 <= bidderId && bidderId < 6);
        }
        for (int i = 0; i < 100; ++i) {
            int teamId = poolProxy.teamRandom();
            assert(0 <= teamId && teamId < 2);
        }
        for (int i = 0; i < 100; ++i) {
            int teamId = poolProxy.teamRootRandom();
            assert(0 <= teamId && teamId < 2);
        }

        //////

        poolProxy.bidderSetAction(bidderId3, -2);
        bidderAssert(__LINE__, bidderId3, -2, 1);

        poolProxy.bidderSetAction(bidderId4, teamId0);
        bidderAssert(__LINE__, bidderId4, 0, 1);
        teamAssert(__LINE__, teamId0, 1, false, 2);
        
        poolProxyAssert(__LINE__, 2, 1, 6);

        for (int i = 0; i < 100; ++i) {
            int teamId = poolProxy.teamRandom();
            assert(0 <= teamId && teamId < 2);
        }
        bool hasTeamId0 = false;
        for (int i = 0; i < 100; ++i) {
            int teamId = poolProxy.teamRootRandom();
            assert(0 <= teamId && teamId < 2);
            if (teamId == 0)
                hasTeamId0 = true;
        }
        assert(hasTeamId0); // very unlikely to fail; should have around half of those be 0

        poolProxy.teamRootMaintain();

        for (int i = 0; i < 100; ++i) {
            int teamId = poolProxy.teamRandom();
            assert(0 <= teamId && teamId < 2);
        }
        for (int i = 0; i < 100; ++i) {
            int teamId = poolProxy.teamRootRandom();
            assert(teamId == 1);
        }

        //////

        poolProxy.teamRemoveBidder(teamId1, bidderId5);
        bidderAssert(__LINE__, bidderId5, -1, 0);
        teamAssert(__LINE__, teamId1, 0, true, 3);

        poolProxyAssert(__LINE__, 2, 1, 6);
        poolProxy.bidderCleanup();
        poolProxyAssert(__LINE__, 2, 1, 5);

        poolProxy.teamRemoveBidder(teamId1, bidderId4);
        bidderAssert(__LINE__, bidderId4, 0, 0);
        teamAssert(__LINE__, teamId1, 0, true, 2);

        teamAssert(__LINE__, teamId0, 1, false, 2);
        poolProxyAssert(__LINE__, 2, 1, 5);

        poolProxy.bidderCleanup();

        teamAssert(__LINE__, teamId0, 0, true, 2);
        poolProxyAssert(__LINE__, 2, 2, 4);

        for (int i = 0; i < 100; ++i) {
            int teamId = poolProxy.teamRandom();
            assert(0 <= teamId && teamId < 2);
        }
        for (int i = 0; i < 100; ++i) {
            int teamId = poolProxy.teamRootRandom();
            assert(teamId == 1);
        }

        poolProxy.teamRootMaintain();

        for (int i = 0; i < 100; ++i) {
            int teamId = poolProxy.teamRandom();
            assert(0 <= teamId && teamId < 2);
        }
        hasTeamId0 = false;
        for (int i = 0; i < 100; ++i) {
            int teamId = poolProxy.teamRootRandom();
            assert(0 <= teamId && teamId < 2);
            if (teamId == 0)
                hasTeamId0 = true;
        }
        assert(hasTeamId0); // very unlikely to fail; should have around half of those be 0


/*
        cout << poolProxy.teamRootSize() << endl; // 1
        poolProxy.bidderCleanup(); // bidder4
        teamAssert(__LINE__, teamId0, 0, true, 2);
        cout << poolProxy.teamRootSize() << endl; // 3; root_rs and addPendingRootTeam intersects
        poolProxyAssert(__LINE__, 2, 1, 4);
*/

        //poolProxy.teamRootMaintain();
        //teamAssert(__LINE__, teamId0, -1, false, 2); // dec twice; no need to dec in cleanup
        //poolProxyAssert(__LINE__, 2, 1, 4);
    }
};

int main() {
    PoolProxyTest test;
    test.test();
}
