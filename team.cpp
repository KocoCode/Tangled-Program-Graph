#include "team.h"
#include "tpgdata.h"

void Team::addBidder(long id) {
    memberBidders.insert(id);
}

void Team::clearReg() {
    TPGData& tpgData = TPGData::GetInstance();
    for (auto &bidderId: memberBidders) {
        Bidder& bidder = tpgData.bidderPool.get(bidderId);
        bidder.clearReg();
    }
}