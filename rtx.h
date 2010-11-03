#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
using namespace std;

//Global Defines
#define DEBUG_MODE	1
#define EXIT_SUCCESS 	0
#define EXIT_ERROR	1

//debug.cpp
void debugMsg(string message);
int assure(bool condition, string message, string fileName, int lineNum, bool isFatal);

//timingServices.cpp
void test();

//primitives
int K_send_message(int dest_process_id, Msg_Env* msg_envelope);
Msg_Env* K_receive_message();
Msg_Env* K_request_msg_env();
int K_release_msg_env(Msg_Env* memory_block);
int K_release_processor();
int K_request_process_status(Msg_Env* memory_block);
int K_terminate();
int K_change_priority(int new_priority, int target_process_id);
int K_request_delay(int time_delay, int wakeup_code, Msg_Env* msg_envelope);
int K_send_console_chars(Msg_Env* msg_envelope);
int K_get_console_chars(Msg_Env* msg_envelope);
int K_get_trace_buffers(Msg_Env* msg_envelope);
