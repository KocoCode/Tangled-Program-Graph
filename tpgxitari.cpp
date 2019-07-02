#include "tpg.h"

#include <ale_interface.hpp>
#include <iostream>

using namespace std;

const int ALE_DIM_BEHAVIOURAL = 33600;
const int ALE_SENSOR_RANGE_HIGH = 255;
const int ALE_SENSOR_RANGE_LOW = 0;
const int MAX_NUM_FRAMES_PER_EPISODE = 18000;

vector<double> initUniformALE() {
    vector<double> behaviouralState;

    for (int i = 0; i < ALE_DIM_BEHAVIOURAL; ++i) {
        behaviouralState.push_back((int)(ALE_SENSOR_RANGE_LOW + (drand48() * (ALE_SENSOR_RANGE_HIGH - ALE_SENSOR_RANGE_LOW))));
    }

    return move(behaviouralState);
}

void train(TPG& tpg, ale::ALEInterface& ale, int genTime) {
    vector < double > currentState; currentState.reserve(tpg.getNumFeatureDimension()); currentState.resize(tpg.getNumFeatureDimension());
    ale::ActionVect legal_actions = ale.getMinimalActionSet();
    for (auto teamId : tpg.getRootTeams()) {
        ale.resetGame();
        double reward = 0.0;
        while (!ale.gameOver()) {
            const ale::ALEScreen screen = ale.getScreen();
            for (int i = 0; i < ALE_DIM_BEHAVIOURAL / 20; ++i) {
                currentState[i] = screen.getArray()[i];
            }
            int atomicAction = tpg.getAction(teamId, currentState);
            reward += ale.act(legal_actions[atomicAction]);
            if (drand48() < 0.0005) {
                tpg.addBehaviouralState(currentState);
            }
        }
        cout << "Gen: " << genTime << " Team ID: " << teamId << " Reward: " << reward << endl;
        tpg.addOutcome(teamId, reward);
    }
}

int main(int argc, char *argv[]) {
    // Parameters
    int seed = 0;
    char romBin[] = "./bowling.bin";

    // Init
    srand48(seed);

    // ALE
    ale::ALEInterface ale(romBin);
    ale.setMaxNumFrames(MAX_NUM_FRAMES_PER_EPISODE);

    // TPG Init
    int numAtomicActions = ale.getMinimalActionSet().size();
    int numFeatureDimension = ALE_DIM_BEHAVIOURAL;

    TPG tpg(numAtomicActions, numFeatureDimension);
    tpg.initBehaviouralStates(initUniformALE);
    tpg.initTeams();

    for (int genTime = 1; genTime <= 10000; ++genTime) {
        cerr << "Gen: " << genTime << endl;
        tpg.genTeams(genTime);
        cerr << "genteams" << endl;
        train(tpg, ale, genTime);
        cerr << "train" << endl;
        tpg.selTeams(genTime);
        cerr << "selteams" << endl;
        tpg.cleanup(genTime);
        cerr << "cleanup" << endl;
    }
}
