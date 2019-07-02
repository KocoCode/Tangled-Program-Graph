
#include <ale_interface.hpp>
#include <iostream>

using namespace std;

const int ALE_DIM_BEHAVIOURAL = 33600;
const int ALE_SENSOR_RANGE_HIGH = 255;
const int ALE_SENSOR_RANGE_LOW = 0;
const int MAX_NUM_FRAMES_PER_EPISODE = 6000;


int main(int argc, char *argv[]) {
    // Parameters
    int seed = 5566;
    char romBin[] = "./bowling.bin";

    // Init
    srand48(seed);

    // ALE
    ale::ALEInterface ale(romBin);
    ale.setMaxNumFrames(MAX_NUM_FRAMES_PER_EPISODE);

    ale::ActionVect legal_actions = ale.getMinimalActionSet();
    ale.resetGame();
    for (int i = 0; i < 10; ++i) {
        double reward = 0.0;
        while (!ale.gameOver()) {
            int atomicAction = (int)drand48() * 18;
            reward += ale.act(legal_actions[atomicAction]);
        }
        ale.resetGame();
        cout << reward << endl;
    }
}
