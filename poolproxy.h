#pragma once
#include "pool.h"
#include "bidder.h"
#include "team.h"

#include <deque>
#include <set>
#include <unordered_set>

using std::deque;
using std::multiset;
using std::unordered_set;

class PoolProxy {
private:
    PoolProxy() {}
    void teamPendAddRoot(int teamId);
    void teamPendRemoveRoot(int teamId);
    void bidderRemove(int bidderId);
    void teamIncRef(int teamId);
    void teamDecRef(int teamId);
    void bidderIncRef(int bidderId);
    void bidderDecRef(int bidderId);

    unordered_set<int> addPendingRootTeam, removePendingRootTeam;
    deque<vector<double>> behaviouralStates;
    deque<multiset<double>> profiles;

public:
    ~PoolProxy() {}
    PoolProxy(PoolProxy const&) = delete;
    void operator=(PoolProxy const&) = delete;
    static PoolProxy& GetInstance() {
        static PoolProxy instance;
        return instance;
    }

    BidderPool<Bidder> bidderPool;
    TeamPool<Team> teamPool;

    int teamCreate(int genTime);
    int bidderCreate(int action, int featureDimension, int maxProgSize, int genTime);
    int bidderCreate(const Bidder &toCopy, long genTime);
    void teamRemove(int teamId);
    int teamRandom();
    int teamRootRandom();
    int bidderRandom();
    Team& teamGet(int teamId);
    Bidder& bidderGet(int bidderId);
    int teamSize();
    int teamRootSize();
    int bidderSize();
    void bidderCleanup();
    void teamRootMaintain();
    void teamAddBidder(int teamId, int bidderId);
    void teamRemoveBidder(int teamId, int bidderId);
    void bidderSetAction(int bidderId, int action);
    bool teamFindBidder(int teamId, int bidderId);
    int numAtomic(int teamId);
};
