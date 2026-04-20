#pragma once

#include "Job.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#define MAX_QUEUE 1024
#define MAX_JOB_SIZE 1000
#define MAX_JOB_TIME 10
#define JOB_FILE_NAME "job_list.txt"

class JobManager {

private:
	Job* queue[MAX_QUEUE];
	int front;
	int rear;
	int count;

	std::ifstream fileReader;
	int fileLineNum;

public:
	// constructor & destructor
	JobManager();
	~JobManager();

	// queue
	bool isEmpty();
	bool isFull();
	bool enqueue(const std::string& id, int size, int time);
	bool dequeue(Job& job);
	bool readJob();
	bool validateJob(int size, int time);
	bool getJob(Job& job);
};

