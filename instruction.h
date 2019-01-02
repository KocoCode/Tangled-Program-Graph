#include <bitset>

using std::bitset;

#define INPUTS 65536 /* Requires 16 bits. */
#define REGISTER_SIZE 8 /* Should match _dstMask. */

typedef bitset < 16 + 3 + 3 + 1 > instruction;

// 000 0000 0000 0000 0000 0001
const instruction modeMask(0x1);
// 000 0000 0000 0000 0000 1110
const instruction opMask(0xE);
// 000 0000 0000 0000 0111 0000
const instruction dstMask(0x70);
// 111 1111 1111 1111 1000 0000
const instruction srcMask(0x7FFF80);

// Rx <- op Rx Ry
const instruction mode0(0x0);
// Rx <- op Rx Iy
const instruction mode1(0x1);

const instruction opSum(0x0);
const instruction opDiff(0x1);
const instruction opProd(0x2);
const instruction opDiv(0x3);
const instruction opCos(0x4);
const instruction opLog(0x5);
const instruction opExp(0x6);
const instruction opCond(0x7);

const short modeShift = 0;
const short opShift = modeMask.count();
const short dstShift = modeMask.count() + opMask.count();
const short srcShift = modeMask.count() + opMask.count() + dstMask.count();

void printInstr(instruction instr);