#ifndef H_PCB_INFO
#define H_PCB_INFO
#include <string>
//struct PcbInfo
//{
//	std::string	name;
//	unsigned int      processId;
//	unsigned int      priority;
//	unsigned int      stackSize;
//	unsigned int      processType;
//	void 			  (*address)();
//};

class PcbInfo {
public:
	std::string	name;
	unsigned int      processId;
	unsigned int      priority;
	unsigned int      stackSize;
	unsigned int      processType;
	void 			  (*address)();
};

#endif
