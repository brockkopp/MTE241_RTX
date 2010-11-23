#include <setjmp.h>
void gRestoreContext(jmp_buf jmpBuf);
int gSaveContext(jmp_buf jmpBuf);
