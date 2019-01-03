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
    static long nextId;
    vector <bool> isIntron;
    int refCount;
    vector <double> profile;

    void markIntrons();
public:
    vector<double> REG;

    Bidder(long action, long genTime, long featureDimension, int maxProgSize);
    Bidder(const Bidder &toCopy, long genTime);

    void printProg();
    long getId();
    void setId(long id);
    void clearReg();
    double bid(const vector<double> &feature);
    void incRefCount();
    void decRefCount();
    bool mutateProg(double pDelete, double pAdd, double pSwap, double pMutate, int maxProgSize);
};