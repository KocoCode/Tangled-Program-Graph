#include <iostream>
#include "instruction.h"

using std::cout;
using std::endl;

void printInstr(instruction instr) {
    int dstReg = ((instr & dstMask) >> dstShift).to_ulong();
    cout << "R[" << dstReg << "] <- ";
    instruction op = (instr & opMask) >> opShift;
    if (op == opSum)
        cout << "Sum ";
    else if (op == opDiff)
        cout << "Diff ";
    else if (op == opProd)
        cout << "Prod ";
    else if (op == opDiv)
        cout << "Div ";
    else if (op == opCos)
        cout << "Cos ";
    else if (op == opLog)
        cout << "Log ";
    else if (op == opExp)
        cout << "Exp ";
    else
        cout << "Cond ";
    
    if (op != opCos && op != opLog && op != opExp)
        cout << "R[" << dstReg << "] ";
    
    instruction mode = (instr & modeMask) >> modeShift;
    if (mode == mode0)
        cout << "R[" << ((instr & srcMask) >> srcShift).to_ulong() % REGISTER_SIZE << "]" << endl;
    else
        cout << "I[" << ((instr & srcMask) >> srcShift).to_ulong() << "(%dim)]" << endl;
}