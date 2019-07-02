#include "tpg.h"
#include "featuremap.h"

#include <ale_interface.hpp>
#include <iostream>

using namespace std;

#define BACKGROUND_DETECTION true
#define QUANTIZE true
#define SECAM false
#define ALE_DIM_BEHAVIOURAL (QUANTIZE ? 1344 : 33600)
#define ALE_SENSOR_RANGE_HIGH (QUANTIZE ? 255 : (SECAM ? 7 : 128))
#define ALE_SENSOR_RANGE_LOW 0
#define MAX_NUM_FRAMES_PER_EPISODE 18000

vector<double> initUniformALE() {
    vector<double> behaviouralState;

    for (int i = 0; i < ALE_DIM_BEHAVIOURAL; ++i) {
        behaviouralState.push_back((int)(ALE_SENSOR_RANGE_LOW + (drand48() * (ALE_SENSOR_RANGE_HIGH - ALE_SENSOR_RANGE_LOW))));
    }

    return move(behaviouralState);
}

void train(TPG& tpg, ALEInterface& ale, FeatureMap& featureMap, int genTime) {
    vector < double > currentState; currentState.reserve(tpg.getNumFeatureDimension()); currentState.resize(tpg.getNumFeatureDimension());
    ActionVect legal_actions = ale.getMinimalActionSet();
    int prevId = -1;

    for (auto teamId : tpg.getRootTeams()) {
        ale.reset_game();
        double reward = 0.0;
        while (!ale.game_over()) {
            const ALEScreen screen = ale.getScreen();
            featureMap.getFeatures(screen, currentState, "");
            int atomicAction = tpg.getAction(teamId, currentState);
            reward += ale.act(legal_actions[atomicAction]);
            if (drand48() < 0.0005 and prevId != teamId) {
                tpg.addBehaviouralState(currentState);
                prevId = teamId;
            }
        }
        cout << "Gen: " << genTime << " Team ID: " << teamId << " Reward: " << reward << endl;
        tpg.addOutcome(teamId, reward);
    }
}

int main(int argc, char *argv[]) {
    // Parameters
    int seed = 0;
    bool visual = false;
    char romBin[] = "./frostbite.bin";

    // Init
    srand48(seed);

    // ALE
    ALEInterface ale;
    ale.setBool("color_averaging",false);
    ale.setInt("random_seed", seed);
    ale.setInt("max_num_frames_per_episode", MAX_NUM_FRAMES_PER_EPISODE);
    ale.setFloat("repeat_action_probability", 0.25);//this is the default
    ale.setBool("display_screen", visual);
    ale.setBool("sound", visual);
    ale.loadROM(romBin);
    FeatureMap featureMap("", QUANTIZE, SECAM, BACKGROUND_DETECTION);

    // TPG Init
    int numAtomicActions = ale.getMinimalActionSet().size();
    int numFeatureDimension = featureMap.numFeatures();

    TPG tpg(numAtomicActions, numFeatureDimension);
    tpg.initBehaviouralStates(initUniformALE);
    tpg.initTeams();

    // Time
  time_t rawtime;
  struct tm * timeinfo;

    for (int genTime = 1; genTime <= 10000; ++genTime) {
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        cerr << "Gen: " << genTime << endl;
        cout << "Time: " << asctime(timeinfo) << endl;
        tpg.genTeams(genTime);
        cerr << "genteams" << endl;
        train(tpg, ale, featureMap, genTime);
        cerr << "train" << endl;
        tpg.selTeams(genTime);
        cerr << "selteams" << endl;
        tpg.cleanup(genTime);
        cerr << "cleanup" << endl;
    }
}
