#include "globals.h"
#include "code.h"

static int emitLoc = 0;
static int highEmitLoc = 0;

void emitComment(char* c) {
	if (TraceCode) { fprintf(code, "* %s\n", c); }
}

void emitRO(char* op, int r, int s, int t, char* c) {
	fprintf(code, "%3d:  %5s  %d,%d,%d ",
		emitLoc++, op, r, s, t);
	if (TraceCode) { fprintf(code, "  %s", c); }
	fprintf(code, "\n");
	if (highEmitLoc < emitLoc) { highEmitLoc = emitLoc; }
} /* emitRO */

void emitRM(char* op, int r, int d, int s, char* c) {
	fprintf(code, "%3d:  %5s  %d,%d(%d) ", emitLoc++, op, r, d, s);
	if (TraceCode) { fprintf(code, "  %s", c); }
	fprintf(code, "\n");
	if (highEmitLoc < emitLoc) { highEmitLoc = emitLoc; }
} /* emitRM */

int emitSkip(int howMany) {
	int i = emitLoc;
	emitLoc += howMany;
	if (highEmitLoc < emitLoc) { highEmitLoc = emitLoc; }
	return i;
} /* emitSkip */

void emitBackup(int loc) {
	if (loc > highEmitLoc) { emitComment("BUG in emitBackup"); }
	emitLoc = loc;
} /* emitBackup */

void emitRestore(void) {
	emitLoc = highEmitLoc;
}

void emitRM_Abs(char* op, int r, int a, char* c) {
	fprintf(code, "%3d:  %5s  %d,%d(%d) ",
		emitLoc, op, r, a - (emitLoc + 1), pc);
	emitLoc++;
	if (TraceCode) { fprintf(code, "  %s", c); }
	fprintf(code, "\n");
	if (highEmitLoc < emitLoc) { highEmitLoc = emitLoc; }
} /* emitRM_Abs */

