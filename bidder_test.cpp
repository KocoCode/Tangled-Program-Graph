#include <iostream>
#include <cassert>
#include "bidder.h"
#include "poolproxy.h"

using namespace std;

int main() {
    PoolProxy& poolProxy = PoolProxy::GetInstance();
    int featureDimension = 10;
    int maxProgSize = 300;
    int genTime = 0;
    vector<double> feature(10, drand48());

    int bidderId = poolProxy.bidderCreate(-1, featureDimension, maxProgSize, genTime);
    Bidder& bidder = poolProxy.bidderGet(bidderId);

    assert(bidder.getId() == bidderId);
    assert(bidder.getAction() == -1);
    bidder.setAction(-2);
    assert(bidder.getAction() == -2);
    bidder.bid(feature); // verify that this function runs without error

    assert(bidder.getRefCount() == 0);
    bidder.incRefCount();
    assert(bidder.getRefCount() == 1);
    bidder.decRefCount();
    assert(bidder.getRefCount() == 0);

    return 0;
}