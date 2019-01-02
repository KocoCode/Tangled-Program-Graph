#include <algorithm>
#include <iostream>
#include <cmath>
#include "bidder.h"

#define SHOWEXEC

using std::reverse;
using std::fill;
using std::iter_swap;
using std::cos;
using std::log;
using std::fabs;
using std::exp;
using std::isfinite;
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

		for(int j = 0; j < instr.size(); j++)
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

void Bidder::clearReg() {
    fill(REG.begin(), REG.end(), 0);
}

double Bidder::bid(vector<double> feature) {
#ifdef SHOWEXEC
	cout << "FEATURE:";
	for(int i = 0; i < featureDimension; i++)
		cout << " " << feature[i];
	cout << endl << "REG:";
	for(int i = 0; i < REGISTER_SIZE; i++)
		cout << " " << REG[i];
	cout << endl;
#endif

	for(int i = 0; i < prog.size(); i++) {
		if(isIntron[i]) // Skip introns
			continue;
        instruction instr = prog[i];

		instruction mode = (instr & modeMask) >> modeShift;
		instruction op = (instr & opMask) >> opShift;

		// Should be between 0 and REGISTER_SIZE - 1

		int dstReg = ((instr & dstMask) >> dstShift).to_ulong();

#ifdef SHOWEXEC
		cout << i << ": " << instr << endl;
        cout << "R[" << dstReg << "] <- R[" << dstReg << "] ";
#endif

        double srcVal;
        // Rx <- op Rx Ry
		if(mode == mode0) {
			srcVal = REG[((instr & srcMask) >> srcShift).to_ulong() % REGISTER_SIZE];
#ifdef SHOWEXEC
			cout << "R[" << ((instr & srcMask) >> srcShift).to_ulong() % REGISTER_SIZE << "] ";
#endif
		}
        // Rx <- op Rx Iy
		else {
			srcVal = feature[((instr & srcMask) >> srcShift).to_ulong() % featureDimension];
#ifdef SHOWEXEC
			cout << "I[" << ((instr & srcMask) >> srcShift).to_ulong() % featureDimension << "] ";
#endif
		}

		if(op == opSum)
		{
			REG[dstReg] = REG[dstReg] + srcVal;
#ifdef SHOWEXEC
			cout << "Sum ";
#endif
		}
		else if(op == opDiff)
		{
			REG[dstReg] = REG[dstReg] - srcVal;
#ifdef SHOWEXEC
			cout << "Diff ";
#endif
		}
		else if(op == opProd)
		{
			REG[dstReg] = REG[dstReg] * srcVal;
#ifdef SHOWEXEC
			cout << "Prod ";
#endif
		}
		else if(op == opDiv)
		{
			REG[dstReg] = REG[dstReg] / srcVal;
#ifdef SHOWEXEC
			cout << "Div ";
#endif
		}
		else if(op == opCos)
		{
			REG[dstReg] = cos(srcVal);
#ifdef SHOWEXEC
			cout << "Cos ";
#endif
		}
		else if(op == opLog)
		{
			REG[dstReg] = log(fabs(srcVal));
#ifdef SHOWEXEC
			cout << "Log ";
#endif
		}
		else if(op == opExp)
		{
			REG[dstReg] = exp(srcVal);
#ifdef SHOWEXEC
			cout << "Exp ";
#endif
		}
		else if(op == opCond)
		{
			if(REG[dstReg] < srcVal)
				REG[dstReg] = -REG[dstReg];
#ifdef SHOWEXEC
			cout << "Cond ";
#endif
		}
		else
		{
			// die(__FILE__, __FUNCTION__, __LINE__, "bad operation");
		}

		if(isfinite(REG[dstReg]) == 0)
			REG[dstReg] = 0;

#ifdef SHOWEXEC
		cout << "REG";
		for(int i = 0; i < REGISTER_SIZE; i++)
			cout << " " << REG[i];
		cout << endl;
#endif
	}

	return REG[0];
}

void Bidder::incRefCount() {
    refCount++;
}

void Bidder::decRefCount() {
    refCount--;
}

bool Bidder::mutateProg(double pDelete, double pAdd, double pSwap, double pMutate, int maxProgSize) {
	bool changed = false;

	// Remove random instruction
	if(prog.size() > 1 && drand48() < pDelete) {
		int i = (int) (drand48() * prog.size());
		prog.erase(prog.begin() + i);

		changed = true;
	}

	// Insert random instruction
	if(prog.size() < maxProgSize && drand48() < pAdd) {
		instruction instr;

		for(int j = 0; j < instr.size(); j++)
			if(drand48() < 0.5) instr.flip(j);

		int i = (int) (drand48() * (prog.size() + 1));

		prog.insert(prog.begin() + i, instr);

		changed = true;
	}

	// Swap positions of two instructions
	if(prog.size() > 1 && drand48() < pSwap) {
		int i = (int) (drand48() * prog.size());
		int j;
		do {
			j = (int) (drand48() * prog.size());
		} while(i == j);

        iter_swap(prog.begin()+i, prog.begin()+j);

		changed = true;
	}

	// Flip single bit of random instruction
	if(drand48() < pMutate) {
		int i = (int) (drand48() * prog.size());
		int j = (int) (drand48() * prog[0].size());

		prog[i].flip(j);

		changed = true;
	}

	if(changed)
		markIntrons();

	return changed;
}