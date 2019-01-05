#pragma once
#include <vector>
#include <unordered_set>
#include "instruction.h"

using std::vector;
using std::unordered_set;

class Bidder {
private:
    int action;
    int ancestralGenTime;
    vector<instruction> prog;
    int featureDimension;
    int effProgSize;
    unordered_set<int> effFeatures;
    int genTime;
    int id;
    vector <bool> isIntron;
    int refCount;
    vector <double> profile;
    double bidVal;

    void markIntrons();
public:
    vector<double> REG;

    Bidder() {}
    Bidder(int action, int featureDimension, int maxProgSize, int genTime);
    Bidder(const Bidder &toCopy, int genTime);

    void printProg();
    int getId();
    void setId(int id);
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
