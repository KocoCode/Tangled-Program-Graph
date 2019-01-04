#pragma once
#include "pool.h"
#include "bidder.h"
#include "team.h"

class TPGData {
private:
    TPGData() {}

public:
    ~TPGData() {}
    TPGData(TPGData const&) = delete;
    void operator=(TPGData const&) = delete;
    static TPGData& GetInstance() {
        static TPGData instance;
        return instance;
    }

    Pool<Bidder> bidderPool;
    Pool<Team> teamPool;
};