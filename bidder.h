#pragma once
#include <vector>
#include <unordered_set>
#include "instruction.h"

using std::vector;
using std::unordered_set;

class Bidder {
private:
    int action;
    long ancestralGenTime;
    vector<instruction> prog;
    long featureDimension;
    int effProgSize;
    unordered_set<long long> effFeatures;
    long genTime;
    long id;
    vector <bool> isIntron;
    int refCount;
    vector <double> profile;
    double bidVal;

    Bidder(long action, long featureDimension, int maxProgSize, long genTime);
    Bidder(const Bidder &toCopy, long genTime);

    void markIntrons();
public:
    vector<double> REG;

    static Bidder& CreateBidder(long action, long featureDimension, int maxProgSize, long genTime);
    static Bidder& CreateBidder(const Bidder &toCopy, long genTime);

    Bidder() {}

    void printProg();
    long getId();
    void setId(long id);
    int getAction();
    void setBidVal(double bidVal);
    void clearReg();
    double bid(const vector<double> &feature);
    int getRefCount();
    void incRefCount();
    void decRefCount();
    bool mutateProg(double pDelete, double pAdd, double pSwap, double pMutate, int maxProgSize);

    friend bool operator<(const Bidder &lhs, const Bidder &rhs);
};
