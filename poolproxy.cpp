#include "poolproxy.h"

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

const RandomizedSet& PoolProxy::getTeams() {
    return teamPool.teams();
}

const RandomizedSet& PoolProxy::getRootTeams() {
    return teamPool.rootTeams();
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
    vector<int> gc;
    for (auto& p : bidderPool) {
        if (p.second.getRefCount() == 0) {
            gc.push_back(p.first);
        }
    }
    for (auto id : gc) {
        for (int i = 0; i < behaviouralStates.size(); ++i) {
            auto it = profiles[i].find(bidderPool.get(id).bid(behaviouralStates[i]));
            profiles[i].erase(it);
        }
        bidderRemove(id);
    }
}

void PoolProxy::teamRootMaintain() {
    for (auto teamId : addPendingRootTeam) {
        teamPool.addRootTeam(teamId);
    }

    for (auto teamId : removePendingRootTeam) {
        teamPool.removeRootTeam(teamId);
    }

    addPendingRootTeam.clear();
    removePendingRootTeam.clear();
}

void PoolProxy::teamAddBidder(int teamId, int bidderId) {
    teamPool.get(teamId).addBidder(bidderId);
    bidderIncRef(bidderId);
}

void PoolProxy::teamRemoveBidder(int teamId, int bidderId) {
    teamPool.get(teamId).removeBidder(bidderId);
    bidderDecRef(bidderId);
}

void PoolProxy::bidderSetAction(int bidderId, int action) {
    Bidder& bidder = bidderPool.get(bidderId);
    if (action == bidder.getAction())
        return;
    if (bidder.getAction() >= 0)
        teamDecRef(bidder.getAction());
    bidder.setAction(action);
    if (action >= 0)
        teamIncRef(action);
}

bool PoolProxy::teamFindBidder(int teamId, int bidderId) {
    return teamPool.get(teamId).findBidder(bidderId);
}

void PoolProxy::teamIncRef(int teamId) {
    Team& team = teamPool.get(teamId);
    if (team.getRefCount() == 0) {
        teamPendRemoveRoot(teamId);
    }
    team.incRefCount();
}

void PoolProxy::teamDecRef(int teamId) {
    Team& team = teamPool.get(teamId);
    team.decRefCount();
    if (team.getRefCount() == 0) {
        teamPendAddRoot(teamId);
    }
}

void PoolProxy::bidderIncRef(int bidderId) {
    bidderPool.get(bidderId).incRefCount();
}

void PoolProxy::bidderDecRef(int bidderId) {
    bidderPool.get(bidderId).decRefCount();
}

int PoolProxy::numAtomic(int teamId) {
    unordered_set<int> atomicActions;
    for (auto bidderId : teamPool.get(teamId).getBidders()) {
        int action = bidderPool.get(bidderId).getAction();
        if (action < 0) atomicActions.insert(action);
    }
    return atomicActions.size();
}

void PoolProxy::teamPendAddRoot(int teamId) {
    addPendingRootTeam.insert(teamId);
    removePendingRootTeam.erase(teamId);
}

void PoolProxy::teamPendRemoveRoot(int teamId) {
    addPendingRootTeam.erase(teamId);
    removePendingRootTeam.insert(teamId);
}
