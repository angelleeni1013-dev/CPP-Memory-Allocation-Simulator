#pragma once

#include"Job.h"

class Partition {

private:
	int size;
	int startAddress;
	bool isFree;
	Job job;

public:
	// constructor
	Partition();
	Partition(int size, int startAddress);

	//getter
	int getSize() const;
	int getStartAddress() const;
	bool getIsFree() const;
	Job getJob() const;

	//setter
	void setSize(int size);
	void setStartAddress(int startAddress);
	void setIsFree(bool isFree);
	void setJob(const Job& job);

	void decreaseJobTime();
};

