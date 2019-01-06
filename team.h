#pragma once
#include <unordered_set>
#include <vector>

using std::unordered_set;
using std::vector;

class Team {
private:
    int genTime;
    int id;
    int refCount;
    unordered_set<int> memberBidders;
    unordered_set<int> activeBidders;

public:
    Team() {};
    Team(int genTime);

    int getId();
    void setId(int id);
    bool isRoot();
    int getRefCount();
    void incRefCount();
    void decRefCount();
    const unordered_set<int>& getBidders();
    void addBidder(int id);
    void removeBidder(int id);
    bool findBidder(int id);
    void clearReg();
    int getAction(const vector<double> &state);
};
