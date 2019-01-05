#include "poolproxy.h"

using namespace;

int PoolProxy::teamCreate(int genTime) {
    Team team(genTime);
    int teamId = teamPool.insert(team);
    addPendingRootTeam.insert(teamId);
    return teamId;
}

int PoolProxy::bidderCreate(int action, int featureDimension, int maxProgSize, int genTime) {
    Bidder bidder(action, featureDimension, maxProgSize, genTime);
    if (bidder.getAction() >= 0) {
        teamIncRef(bidder.getAction());
    }
    return bidderPool.insert(bidder);
}

int PoolProxy::bidderCreate(const Bidder &toCopy, long genTime) {
    Bidder bidder(toCopy, genTime);
    if (bidder.getAction() >= 0) {
        teamIncRef(bidder.getAction());
    }
    return bidderPool.insert(bidder);
}

void PoolProxy::teamRemove(int teamId) {
    for (auto bidderId : teamPool.get(teamId).getBidders()) {
        bidderDecRef(bidderId);
    }
    teamPool.remove(teamId);
}

void PoolProxy::bidderRemove(int bidderId) {
    int action = bidderPool.get(bidderId).getAction();
    if (action >= 0) {
        teamDecRef(action);
    }
    bidderPool.remove(bidderId);
}

int PoolProxy::teamRandom() {
    return teamPool.random();
}

int PoolProxy::teamRootRandom() {
    return teamPool.randomRootTeam();
}

int PoolProxy::bidderRandom() {
    return bidderPool.random();
}

Team& PoolProxy::teamGet(int teamId) {
    return teamPool.get(teamId);
}

Bidder& PoolProxy::bidderGet(int bidderId) {
    return bidderPool.get(bidderId);
}

int PoolProxy::teamSize() {
    return teamPool.size();
}

int PoolProxy::teamRootSize() {
    return teamPool.rootSize() + addPendingRootTeam.size() - removePendingRootTeam.size();
}

int PoolProxy::bidderSize() {
    return bidderPool.size();
}

void PoolProxy::bidderCleanup() {
    vector<int> gc, refTeams;
    for (auto& p : bidderPool) {
        if (p.second.getRefCount() == 0) {
            gc.push_back(p.first);
            int action = p.second.getAction();
            if (action >= 0) {
                refTeams.push_back(action);
            }
        }
    }
    for (auto id : gc) {
        bidderPool.remove(id);
    }
    for (auto teamId : refTeams) {
        teamDecRef(teamId);
    }
}

void PoolProxy::teamRootMaintain() {
    for (auto teamId : addPendingRootTeam) {
        teamPool.addRootTeam(teamId);
    }

    for (auto teamId : removePendingRootTeam) {
        teamPool.removeRootTeam(teamId);
    }
}

void PoolProxy::teamAddBidder(int teamId, int bidderId) {
    teamPool.get(teamId).addBidder(bidderId);
    bidderIncRef(bidderId);
}

void PoolProxy::teamRemoveBidder(int teamId, int bidderId) {
    teamPool.get(teamId).removeBidder(bidderId);
    bidderDecRef(bidderId);
}

void PoolProxy::teamIncRef(int teamId) {
    Team& team = teamPool.get(teamId);
    if (team.getRefCount() == 0) {
        addPendingRootTeam.erase(teamId);
        removePendingRootTeam.insert(teamId);
    }
    team.incRefCount();
}

void PoolProxy::teamDecRef(int teamId) {
    Team& team = teamPool.get(teamId);
    team.decRefCount();
    if (team.getRefCount() == 0) {
        addPendingRootTeam.insert(teamId);
        removePendingRootTeam.erase(teamId);
    }
}

void PoolProxy::bidderIncRef(int bidderId) {
    bidderPool.get(bidderId).incRefCount();
}

void PoolProxy::bidderDecRef(int bidderId) {
    bidderPool.get(bidderId).decRefCount();
}

int PoolProxy::numAtomic(int teamId) {
    return teamPool.get(teamId).numAtomic();
}
