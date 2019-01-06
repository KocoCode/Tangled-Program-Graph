#pragma once
#include <unordered_set>
#include <vector>
#include "pool.h"

using std::unordered_set;
using std::vector;

class Team {
private:
    int genTime;
    int id;
    int refCount;
    RandomizedSet memberBidders;
    unordered_set<int> activeBidders;

public:
    Team() {};
    Team(int genTime);

    int getId();
    void setId(int id);
    int getGenTime();
    bool isRoot();
    int getRefCount();
    void incRefCount();
    void decRefCount();
    const RandomizedSet& getBidders();
    int randomBidder();
    int bidderSize();
    void addBidder(int id);
    void removeBidder(int id);
    bool findBidder(int id);
    void clearReg();
    int getAction(const vector<double> &state);
};
