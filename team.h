#pragma once
#include <unordered_set>
#include <vector>

using std::unordered_set;
using std::vector;

class Team {
private:
    long genTime;
    long id;
    int refCount;
    unordered_set<long> memberBidders;
    unordered_set<long> activeBidders;

    Team(long genTime);
public:
    static Team& CreateTeam(long genTime);

    Team() {};

    long getId();
    void setId(long id);
    bool isRoot();
    int getRefCount();
    void incRefCount();
    void decRefCount();
    void addBidder(long id);
    void removeBidder(long id);
    void clearReg();
    int getAction(const vector<double> &state);
};