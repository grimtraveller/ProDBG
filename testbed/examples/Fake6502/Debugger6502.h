#ifndef _DEBUGGER6502_H_
#define _DEBUGGER6502_H_

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct Debugger6502
{
    int runState;

} Debugger6502;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern Debugger6502* g_debugger;

#endif

