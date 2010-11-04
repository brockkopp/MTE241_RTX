#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
using namespace std;

//Project wide variables
#define DEBUG_MODE 1

#define EXIT_SUCCESS 	0
#define EXIT_ERROR	1

//Messaging Constants
#define SEND 0
#define RECEIVE 1
	//Msg Types
#define STD_MSG 0
#define RECEIVE_ACK 1
#define DELAY_REQUEST 2
#define WAKE_UP 3

