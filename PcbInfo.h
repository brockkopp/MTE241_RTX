struct PcbInfo
{
	std::string	name;
	unsigned int    processId;
	unsigned int    priority;
	unsigned int    stackSize;
	unsigned int    processType;
	void*         	address;
};
