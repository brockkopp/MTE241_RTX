#include "scheduler.h"
/*
int PQ::release_processor( ) { return -2; }  
int PQ::change_priority( PCB * target, new_priority ) { return -2; }   
int PQ::process_switch( ) {return -2;}

int PQ::add_ready_process( PCB * target ) { return -2;}
int PQ::block_process (PCB * target, string reason ) {return -2; }
int PQ::unblock_process( PCB * target ) {return -2; }
*/
//Returns if a process is currently blocked on envelope
/*
int PQ::is_blocked_on_envelope( PCB * target ) {return -2; }
*/
/*
PCB * PQ::current_process {return NULL;} // Executing state

PQ * ready_procs() {return NULL;} // Ready to execute state
PQ * blocked_env_procs() {return NULL;} // Blocked on resource state
LL_List blocked_msg_recieve() {return NULL;}
*/
/*
int context_switch( PCB * next_proc ) {return -2;}
*/
int context_save( ) { return -2; }

