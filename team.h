#pragma once
#include <unordered_set>
#include <vector>

using std::unordered_set;
using std::vector;

class Team {
private:
    long genTime;
    long id;
    bool isRootTeam;
    unordered_set<long> memberBidders;
    unordered_set<long> activeBidders;

public:
    Team() {};
    Team(long genTime);

    long getId();
    void setId(long id);
    bool isRoot();
    void setRoot(bool isRootTeam);
    void addBidder(long id);
    void clearReg();
    int getAction(const vector<double> &state);
};