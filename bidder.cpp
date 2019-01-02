#include <algorithm>
#include <iostream>
#include "bidder.h"

using std::reverse;
using std::cout;
using std::endl;

long Bidder::nextId = 0;

void Bidder::markIntrons() {
    /*
    Mark introns in prog
    Count effProgSize
    Set effFeatures
    */
    bitset <REGISTER_SIZE> isEffReg;

    isIntron.clear();
    effProgSize = 0;
    effFeatures.clear();

    // Mark the first register
    isEffReg.reset();
    isEffReg.set(0, true);

    for(auto it = prog.rbegin(); it != prog.rend(); it++) {
        int reg = ((*it & dstMask) >> dstShift).to_ulong(); // Get destination register

        // Destination register is effective
        if(isEffReg.test(reg)) {
            isIntron.push_back(false); // for efficiency, reverse at last
            effProgSize++;

            instruction op = (*it & opMask) >> opShift;

            // If the operation is unary, remove destination register from isEffReg set
            if(op == opCos || op == opLog || op == opExp)
                isEffReg.set(reg, false);

            instruction mode = (*it & modeMask) >> modeShift;

             // Rx <- op Rx Ry; Ry is effective
            if(mode == mode0) {
                int reg = ((*it & srcMask) >> srcShift).to_ulong() % REGISTER_SIZE;
                isEffReg.set(reg, true);
            }
            // Rx <- op Rx Iy; Get feature index y
            else {
                long feature = ((*it & srcMask) >> srcShift).to_ulong() % featureDimension;
                effFeatures.insert(feature);
            }
        }
        // Not effective, mark as intron
        else {
            isIntron.push_back(true); // for efficiency, reverse at last
        }
    }
    reverse(isIntron.begin(), isIntron.end());
}

Bidder::Bidder(long action, long genTime, long featureDimension, int maxProgSize):
    action(action), ancestralGenTime(genTime), genTime(genTime), featureDimension(featureDimension) {
	id = nextId++;
	refCount = 0;
    REG = vector<double>(REGISTER_SIZE, 0);

	int progSize = 1 + ((int) (drand48() * maxProgSize));

	for(int i = 0; i < progSize; i++) {
        instruction instr;

		for(size_t j = 0; j < instr.size(); j++)
			if(drand48() < 0.5)
                instr.flip(j);

		prog.push_back(instr);
	}

	markIntrons();
}

Bidder::Bidder(const Bidder &toCopy, long genTime): genTime(genTime) {
	action = toCopy.action;
	ancestralGenTime = toCopy.ancestralGenTime;
	featureDimension = toCopy.featureDimension;
	id = nextId++;
    refCount = 0;
    REG = vector<double>(REGISTER_SIZE, 0);

	for(auto instr: toCopy.prog)
		prog.push_back(instr);

	markIntrons();
}

void Bidder::printProg() {
    int count = 0;
    cout << "Generation Time: " << genTime << endl;
    for (auto instr: prog) {
        cout << count << ": Intron? " << (isIntron[count]? "Yes": "No") << endl;
        cout << instr << endl;
        printInstr(instr);
        ++count;
    }
}