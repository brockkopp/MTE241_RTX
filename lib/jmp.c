#include "jmp.h"
int gSaveContext(jmp_buf jmpBuf)
{
	return setjmp( jmpBuf );
}

void gRestoreContext(jmp_buf jmpBuf)
{
	longjmp( jmpBuf , 1);
}
