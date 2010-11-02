#include "rtx.h"

int main()
{
	test();
	debugMsg("\nRTX INITIALIZED\n\n");
	//string msg = "RTX Terminated\n\n";
	assure(false,"testError", __FILE__, __LINE__, false);
	debugMsg("\n\nRTX TERMINATED SUCCESSFULLY\n\n");
	return EXIT_SUCCESS;
}
