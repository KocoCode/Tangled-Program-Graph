#include <algorithm>
#include "team.h"
#include "poolproxy.h"

using std::max_element;

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

const unordered_set<int>& Team::getBidders() {
    return memberBidders;
}

int Team::bidderSize() {
    return memberBidders.size();
}

void Team::addBidder(int id) {
    memberBidders.insert(id);
}

void Team::removeBidder(int id) {
    memberBidders.erase(id);
}

bool Team::findBidder(int id) {
    return memberBidders.find(id) != memberBidders.end();
}

/*
void Team::clearReg() {
    TPGData& tpgData = TPGData::GetInstance();
    for (auto &bidderId: memberBidders) {
        Bidder& bidder = tpgData.bidderPool.get(bidderId);
        bidder.clearReg();
    }
}

struct BidderIdLookUpCompare {
    bool operator()(int lhsId, int rhsId) {
        Bidder lhsBidder = TPGData::GetInstance().bidderPool.get(lhsId);
        Bidder rhsBidder = TPGData::GetInstance().bidderPool.get(rhsId);
        return lhsBidder < rhsBidder;
    }
};

int Team::getAction(const vector<double> &state) {
    TPGData& tpgData = TPGData::GetInstance();
    clearReg();
    for (auto &bidderId: memberBidders) {
        Bidder& bidder = tpgData.bidderPool.get(bidderId);
        bidder.setBidVal(bidder.bid(state));
    }
    int bestBidderId = *max_element(memberBidders.begin(), memberBidders.end(), BidderIdLookUpCompare());
    activeBidders.insert(bestBidderId);
    Bidder bestBidder = tpgData.bidderPool.get(bestBidderId);
    int action = bestBidder.getAction();
    if (action < 0)
        return action;

    // action is a teamId
    return tpgData.teamPool.get(action).getAction(state);
}
*/
