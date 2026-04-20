#pragma once

#include"Partition.h"
#include"JobManager.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <thread>
#include <chrono>

#define STARTING_ADDRESS 10
#define MAX_PARTITIONS 10
#define MAX_PARTITION_SIZE 1000
#define MAX_LINE 118
#define PARTITION_FILE_NAME "partition_list.txt"

class MemoryManager {

private:
	Partition* partitions[MAX_PARTITIONS];
	int top;
	int maxPartitionSize;

public:
	
	//constructor
	MemoryManager();
	~MemoryManager();

	bool isEmpty();
	bool isFull();

	bool readPartition();
	bool validatePartition(int size);

	bool allocation(const Job& job, int index);
	bool deallocation();

	//First-Fit
	bool firstFit(const Job& job);

	//Best-Fit
	bool bestFit(const Job& job);

	bool simulation(int type);

	bool updateTime();

	void displayTable();
};