#include "tpg.h"

#include <set>
#include <algorithm>

using namespace std;

TPG::TPG(int _numAtomicActions, int _numFeatureDimension,
    int _numBehaviouralStates, int _maxProgSize, int _Rsize,
    double _Rgap, double _pAddProfilePoint, double _pAtomic,
    double _pBidAdd, double _pBidDelete, double _pBidMutate,
    double _pBidSwap, double _pma, double _pmd, double _pmm,
    double _pmn, int _omega):
    numAtomicActions(_numAtomicActions),
    numFeatureDimension(_numFeatureDimension),
    numBehaviouralStates(_numBehaviouralStates),
    maxProgSize(_maxProgSize), Rsize(_Rsize), Rgap(_Rgap),
    pAddProfilePoint(_pAddProfilePoint), pAtomic(_pAtomic), pBidAdd(_pBidAdd),
    pBidDelete(_pBidDelete), pBidMutate(_pBidMutate),
    pBidSwap(_pBidSwap), pma(_pma), pmd(_pmd), pmm(_pmm), pmn(_pmn),
    omega(_omega) {

    PoolProxy& poolProxy = PoolProxy::GetInstance();
    poolProxy.behaviouralStates.resize(numBehaviouralStates);
    poolProxy.profiles.resize(numBehaviouralStates);
}

void TPG::genTeams(int genTime) {
    PoolProxy& poolProxy = PoolProxy::GetInstance();
    int numNewTeams = 0;

    while (poolProxy.teamRootSize() < Rsize) {
        auto parentId = poolProxy.teamPool.randomRootTeam();
        genTeams(genTime, parentId);
        numNewTeams++;
    }

    poolProxy.teamRootMaintain();
}

void TPG::genTeams(int genTime, int parentId) {
    PoolProxy& poolProxy = PoolProxy::GetInstance();
    int childId = poolProxy.teamCreate(genTime);

    // Add bidders from parentTeam
    for (auto id : poolProxy.teamGet(parentId).getBidders()) {
        poolProxy.teamAddBidder(childId, id);
    }

    // Remove bidders
    for (double b = 1.0; drand48() < b and poolProxy.teamGet(childId).bidderSize() > 2; b = b * pmd) {
        int bidderId;
        do {
            bidderId = poolProxy.teamGet(childId).randomBidder();
        } while (poolProxy.bidderGet(bidderId).getAction() < 0 and poolProxy.numAtomic(childId) < 2);
        poolProxy.teamRemoveBidder(childId, bidderId);
    }

    // Add bidders
    for (double b = 1.0; drand48() < b and poolProxy.teamGet(childId).bidderSize() < poolProxy.bidderSize(); b = b * pma) {
        int bidderId;
        do {
            bidderId = poolProxy.bidderRandom();
        } while (poolProxy.bidderGet(bidderId).getAction() == childId or poolProxy.teamFindBidder(childId, bidderId));
        poolProxy.teamAddBidder(childId, bidderId);
    }

    // Mutate bidders
    bool changedM = false;

    do  {
        vector<int> need_remove, need_add;
        for (auto& bidderId : poolProxy.teamGet(childId).getBidders()) {
            if (drand48() < pmm) {
                changedM = true;
                int newBidderId = poolProxy.bidderCreate(bidderId, genTime); // copy old bidder
                need_remove.push_back(bidderId);
                need_add.push_back(newBidderId);

                bool changedL = false;
                do {
                    changedL = poolProxy.bidderGet(newBidderId).mutateProg(pBidDelete, pBidAdd, pBidSwap, pBidMutate, maxProgSize);
                } while (changedL == false);

                genUniqueBidder(newBidderId);

                if (drand48() < pmn) {
                    if (genTime == 1 or (poolProxy.bidderGet(bidderId).getAction() < 0 and (poolProxy.numAtomic(childId) < 2 ? true : drand48() < pAtomic))) {
                        poolProxy.bidderSetAction(newBidderId, -1 - (int)(drand48() * numAtomicActions));
                    } else {
                        int id;
                        do {
                            id = poolProxy.teamRandom();
                        } while (id == childId or poolProxy.teamGet(id).getGenTime() == genTime);
                        poolProxy.bidderSetAction(newBidderId, id);
                    }
                }
            }
        }
        for (auto bidderId : need_remove) {
            poolProxy.teamRemoveBidder(childId, bidderId);
        }
        for (auto bidderId : need_add) {
            poolProxy.teamAddBidder(childId, bidderId);
        }
    } while (changedM == false);
}

void TPG::genUniqueBidder(int bidderId) {
    PoolProxy& poolProxy = PoolProxy::GetInstance();
    auto& profiles = poolProxy.profiles;
    const auto& behaviouralStates = poolProxy.behaviouralStates;
    vector<double> profile;

    bool stop = false;
    while (stop == false) {
        profile.clear();
        for (int i = 0; i < (int)behaviouralStates.size(); ++i) {
            profile.push_back(poolProxy.bidderGet(bidderId).bid(behaviouralStates[i]));
        }

        bool success = true;
        for (int i = 0; i < (int)profiles.size(); ++i) {
            auto it = profiles[i].lower_bound(profile[i]);
            if (it != profiles[i].end()) {
                if (fabs(profile[i] - *it) < BID_EPSILON) {
                    success = false;
                    break;
                }
            }
            if (it != profiles[i].begin()) {
                --it;
                if (fabs(profile[i] - *it) < BID_EPSILON) {
                    success = false;
                    break;
                }
            }
        }
        if (success) {
            stop = true;
        } else {
            bool changedL = false;
            do {
                changedL = poolProxy.bidderGet(bidderId).mutateProg(pBidDelete, pBidAdd, pBidSwap, pBidMutate, maxProgSize);
            } while (changedL == false);
        }
    }
    for (int i = 0; i < (int)profiles.size(); ++i) {
        profiles[i].insert(profile[i]);
    }
}

void TPG::initTeams() {
    PoolProxy& poolProxy = PoolProxy::GetInstance();

    int keep = Rsize - floor(Rsize * Rgap);

    for (int i = 0; i < keep; ++i) {
        // Generate 2 different atomic actions
        int a1, a2;

        a1 = -1 - ((int)(drand48() * numAtomicActions));

        do {
            a2 = -1 - ((int)(drand48() * numAtomicActions));
        } while (a1 == a2);

        int teamId = poolProxy.teamCreate(1); // genTime = 1
        int bidderId;
        bidderId = poolProxy.bidderCreate(a1, numFeatureDimension, maxProgSize, 1);
        genUniqueBidder(bidderId);
        poolProxy.teamAddBidder(teamId, bidderId);
        bidderId = poolProxy.bidderCreate(a2, numFeatureDimension, maxProgSize, 1);
        genUniqueBidder(bidderId);
        poolProxy.teamAddBidder(teamId, bidderId);
    }

    for (auto teamId : poolProxy.getTeams()) {
        int size = max(min((int)(drand48() * (omega + 1)), (int)poolProxy.bidderSize() - 2), 0);
        unordered_set<int> bidderUsed;
        for (auto bidderId : poolProxy.teamGet(teamId).getBidders()) {
            bidderUsed.insert(bidderId);
        }

        while (poolProxy.teamGet(teamId).bidderSize() < size) {
            int bidderId = poolProxy.bidderRandom();

            while (bidderUsed.find(bidderId) != bidderUsed.end()) {
                bidderId = poolProxy.bidderRandom();
            }
            poolProxy.teamAddBidder(teamId, bidderId);
            bidderUsed.insert(bidderId);
        }
    }

    poolProxy.teamRootMaintain();
}

void TPG::selTeams(int genTeam) {
    PoolProxy& poolProxy = PoolProxy::GetInstance();
    vector<pair<int, double>> teams;

    for (auto teamId : poolProxy.getRootTeams()) {
        teams.push_back({teamId, poolProxy.teamGet(teamId).getMeanOutcome()});
    }

    int needDeleted = floor(teams.size() * Rgap);
    partial_sort(teams.begin(), teams.begin() + needDeleted, teams.end(),
      [](const pair<int, double>& l, const pair<int, double>& r){
      return l.second < r.second;});

    for (int i = 0; i < needDeleted; ++i) {
        poolProxy.teamRemove(teams[i].first);
    }
}

void TPG::cleanup(int genTeam) {
    PoolProxy& poolProxy = PoolProxy::GetInstance();
    poolProxy.bidderCleanup();
}

void TPG::initBehaviouralStates(function<vector<double>()> initFunc) {
    PoolProxy& poolProxy = PoolProxy::GetInstance();
    for (int i = 0; i < numBehaviouralStates; ++i) {
        poolProxy.behaviouralStates[i] = move(initFunc());
    }
}

void TPG::addBehaviouralState(const vector<double> behaviouralState) {
    PoolProxy& poolProxy = PoolProxy::GetInstance();
    multiset<double> profiles;
    for (auto& p : poolProxy.bidderPool) {
        auto& bidder = p.second;
        profiles.insert(bidder.bid(behaviouralState));
    }
    poolProxy.behaviouralStates.pop_front();
    poolProxy.behaviouralStates.push_back(behaviouralState);
    poolProxy.profiles.pop_front();
    poolProxy.profiles.push_back(move(profiles));
}
