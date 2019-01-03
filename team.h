#pragma once
#include <unordered_set>

using std::unordered_set;

class Team {
private:
    long genTime;
    long id;
    unordered_set<long> memberBidders;
    unordered_set<long> activeBidders;
public:
    void addBidder(long id);
    void clearReg();
};