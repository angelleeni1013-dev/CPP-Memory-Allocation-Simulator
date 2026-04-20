#include "JobManager.h"

JobManager::JobManager() : front(0), rear(0), count(0), fileLineNum(0) {
	for (int i = 0; i < MAX_QUEUE; ++i) {
		queue[i] = NULL;
	}

	fileReader.open(JOB_FILE_NAME);
	if (!fileReader.is_open())
		std::cout << "[ERROR] File '" << JOB_FILE_NAME << "' not found or can't open" << std::endl;

	readJob();
}

JobManager::~JobManager() {
	for (int i = 0; i < MAX_QUEUE; ++i) {
		if (queue[i] != NULL) {
			delete queue[i];
			queue[i] = NULL;
		}
	}

	if (fileReader.is_open())
		fileReader.close();
}

bool JobManager::isEmpty() {
	return count == 0;
}

bool JobManager::isFull() {
	return count == MAX_QUEUE;
}

bool JobManager::enqueue(const std::string& id, int size, int time) {
	if (isFull() || !validateJob(size, time))
		return false;

	queue[rear] = new Job(id, size, time);
	rear = (rear + 1) % MAX_QUEUE;
	count++;
	return true;
}

bool JobManager::dequeue(Job& job) {
	if (isEmpty())
		return false;

	job = *queue[front];
	delete queue[front];
	queue[front] = NULL;
	front = (front + 1) % MAX_QUEUE;
	count--;
	return true;
}

bool JobManager::readJob() {
	std::string id, line, extra;
	int size = 0, time = 0;
	bool hasNewJob = false;

	if (!fileReader.is_open())
		return hasNewJob;
		
		
	while (!isFull() && std::getline(fileReader, line)) {
		fileLineNum++;

		if (line.empty()) continue;

		std::istringstream lineReader(line);
		if (lineReader >> id >> size >> time && !(lineReader >> extra)) {
			if (!enqueue(id, size, time))
				std::cout << "[ERROR] Invalid job data at line " << fileLineNum
					<< ". Size must be between 1 and " << MAX_JOB_SIZE
					<< ". Time must be between 1 and " << MAX_JOB_TIME << "." << std::endl;
			else
				hasNewJob = true;
		}
		else {
			std::cout << "[ERROR] Failed to read job data at line " << fileLineNum
				<< ". Expected format: <string> <int> <int>" << std::endl;
		}
	}
	return hasNewJob;
}

bool JobManager::validateJob(int size, int time) {
	return (size > 0 && size <= MAX_JOB_SIZE) && (time > 0 && time <= MAX_JOB_TIME);
}

bool JobManager::getJob(Job& job) {
	if (isEmpty())
		readJob();

	return dequeue(job);
}