#pragma once
#include "pool.h"
#include "bidder.h"
#include "team.h"

class PoolProxy {
private:
    PoolProxy() {}
    void teamIncRef(int);
    void teamDecRef(int);
    void bidderIncRef(int);
    void bidderDecRef(int);
    void teamPendAddRoot(int);
    void teamPendRemoveRoot(int);
    void bidderRemove(int bidderId);

    unordered_set<int> addPendingRootTeam, removePendingRootTeam;

public:
    ~PoolProxy() {}
    PoolProxy(PoolProxy const&) = delete;
    void operator=(PoolProxy const&) = delete;
    static PoolProxy& GetInstance() {
        static PoolProxy instance;
        return instance;
    }

    Pool<Bidder> bidderPool;
    Pool<Team> teamPool;

    int teamCreate(int genTime);
    int bidderCreate(int action, int featureDimension, int maxProgSize, int genTime);
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
    void teamAddBidder(int bidderId);
    void teamRemoveBidder(int bidderId);
    void teamIncRef(int teamId);
    void teamDecRef(int teamId);
    void bidderIncRef(int bidderId);
    void bidderDecRef(int bidderId);
    int numAtomic(int teamId);
};
