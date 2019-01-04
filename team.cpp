#include <algorithm>
#include "team.h"
#include "tpgdata.h"

using std::max_element;

Team& Team::CreateTeam() {
    Team team;
    TPGData::GetInstance().teamPool.insert(team);
    return TPGData::GetInstance().teamPool.get(team.getId());
}

long Team::getId() {
    return id;
}

void Team::setId(long id) {
    this->id = id;
}

void Team::addBidder(long id) {
    memberBidders.insert(id);
    TPGData::GetInstance().bidderPool.get(id).incRefCount();
}

void Team::clearReg() {
    TPGData& tpgData = TPGData::GetInstance();
    for (auto &bidderId: memberBidders) {
        Bidder& bidder = tpgData.bidderPool.get(bidderId);
        bidder.clearReg();
    }
}

struct BidderIdLookUpCompare {
    bool operator()(long lhsId, long rhsId) {
        Bidder lhsBidder = TPGData::GetInstance().bidderPool.get(lhsId);
        Bidder rhsBidder = TPGData::GetInstance().bidderPool.get(rhsId);
        return lhsBidder < rhsBidder;
    }
};

int Team::getAction(const vector<double> &state) {
    TPGData& tpgData = TPGData::GetInstance();
    for (auto &bidderId: memberBidders) {
        Bidder& bidder = tpgData.bidderPool.get(bidderId);
        bidder.setBidVal(bidder.bid(state));
    }
    long bestBidderId = *max_element(memberBidders.begin(), memberBidders.end(), BidderIdLookUpCompare());
    activeBidders.insert(bestBidderId);
    Bidder bestBidder = tpgData.bidderPool.get(bestBidderId);
    int action = bestBidder.getAction();
    if (action < 0)
        return action;

    // action is a teamId
    // return tpgData.teamPool.get(action).getAction();
}