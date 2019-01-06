#include <algorithm>
#include "team.h"
#include "poolproxy.h"

using std::sort;
using std::accumulate;

Team::Team(int genTime): genTime(genTime) {
    refCount = 0;
}

int Team::getId() {
    return id;
}

void Team::setId(int id) {
    this->id = id;
}

int Team::getGenTime() {
    return genTime;
}

bool Team::isRoot() {
    return refCount == 0;
}

int Team::getRefCount() {
    return refCount;
}

void Team::incRefCount() {
    refCount++;
}

void Team::decRefCount() {
    refCount--;
}

const RandomizedSet& Team::getBidders() {
    return memberBidders;
}

int Team::randomBidder() {
    return memberBidders.random();
}

int Team::bidderSize() {
    return memberBidders.size();
}

void Team::addBidder(int id) {
    memberBidders.insert(id);
}

void Team::removeBidder(int id) {
    memberBidders.remove(id);
}

bool Team::findBidder(int id) {
    return memberBidders.find(id);
}

void Team::clearReg() {
    for (auto bidderId: memberBidders) {
        PoolProxy::GetInstance().bidderPool.get(bidderId).clearReg();
    }
}

struct BidderIdLookUpCompare {
    bool operator()(int lhsId, int rhsId) {
        Bidder lhsBidder = PoolProxy::GetInstance().bidderPool.get(lhsId);
        Bidder rhsBidder = PoolProxy::GetInstance().bidderPool.get(rhsId);
        return lhsBidder < rhsBidder;
    }
};

int Team::getAction(const vector<double> &state, unordered_set<int>& visitedTeams) {
    PoolProxy& poolProxy = PoolProxy::GetInstance();
    clearReg();
    visitedTeams.insert(id);
    for (auto bidderId: memberBidders) {
        Bidder& bidder = poolProxy.bidderPool.get(bidderId);
        bidder.setBidVal(bidder.bid(state)); // better skip those whose action is a visitedTeam
    }
    vector<int> bidderSorted(memberBidders.begin(), memberBidders.end());
    sort(bidderSorted.begin(), bidderSorted.end(), BidderIdLookUpCompare());
    for (auto it = bidderSorted.rbegin(); it != bidderSorted.rend(); ++it) {
        int bidderId = *it;
        Bidder bidder = poolProxy.bidderPool.get(bidderId);
        int action = bidder.getAction();
        if (action < 0) {
            activeBidders.insert(bidderId);
            return action;
        }
        else if (visitedTeams.find(action) == visitedTeams.end()) {
            activeBidders.insert(bidderId);
            return poolProxy.teamPool.get(action).getAction(state, visitedTeams);
        }
    }
}

void Team::clearOutcomes() {
    outcomes.clear();
}

void Team::addOutcome(double outcome) {
    outcomes.push_back(outcome);
}

double Team::getMeanOutcome() {
    return accumulate(outcomes.begin(), outcomes.end(), 0.0) / outcomes.size();
}