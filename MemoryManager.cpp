#include "MemoryManager.h"

MemoryManager::MemoryManager() : top(0), maxPartitionSize(0){
	for (int i = 0; i < MAX_PARTITIONS; ++i)
		partitions[i] = NULL;
	readPartition();
}

MemoryManager::~MemoryManager() {
	for (int i = 0; i < MAX_PARTITIONS; ++i) {
		if (partitions[i] != NULL) {
			delete partitions[i];
			partitions[i] = NULL;
		}
	}
}

bool MemoryManager::isEmpty() {
	return top == 0;
}

bool MemoryManager::isFull() {
	return top == MAX_PARTITIONS;
}

bool MemoryManager::readPartition() {
	std::ifstream fileReader;
	fileReader.open(PARTITION_FILE_NAME);
	int fileLineNum = 0;

	int size = 0, startAddress = STARTING_ADDRESS;
	std::string line, extra;
	
	bool hasNewPartition = false;

	if (!fileReader.is_open()) {
		std::cout << "[ERROR] File '" << PARTITION_FILE_NAME << "' not found or can't open" << std::endl;
		return hasNewPartition;
	}
		
	while (!isFull() && std::getline(fileReader,line)) {
		fileLineNum++;

		if (line.empty()) continue;

		std::istringstream lineReader(line);
		if (lineReader >> size && !(lineReader >> extra)) {
			if (!validatePartition(size)) {
				std::cout << "[ERROR] Invalid partition size at line " << fileLineNum
					<< ". Expected size between 1 and " << MAX_PARTITION_SIZE <<"." << std::endl;
				continue;
			}

			partitions[top] = new Partition(size, startAddress);

			startAddress += size;
			if (size > maxPartitionSize)
				maxPartitionSize = size;

			top++;
			hasNewPartition = true;
		}
		else {
			std::cout << "[ERROR] Failed to read partition data at line " << fileLineNum
				<< ". Expected format: <int>" << std::endl;
		}
	}

	if (fileReader.peek() != EOF) {
		std::cout << "[WARNING] Only the first " << MAX_PARTITIONS
			<< " valid partitions are loaded. Extra data in file is ignored." << std::endl;
	}

	if (fileReader.is_open())
		fileReader.close();

	return hasNewPartition;
}

bool MemoryManager::validatePartition(int size) {
	return (size > 0 && size <= MAX_PARTITION_SIZE);
}

bool MemoryManager::allocation(const Job& job, int index) {
	if (index < 0 || index > top)
		return false;

	partitions[index]->setJob(job);
	partitions[index]->setIsFree(false);
	std::cout << "[INFO] Job " << job.getId() << " allocated to partition " << index + 1 << "." << std::endl;
	return true;
}

bool MemoryManager::deallocation() {
	bool hasDeallocated = false;
	for (int i = 0; i < top; i++) {
		if (partitions[i]->getJob().getTime() <= 0) {
			partitions[i]->setIsFree(true);
			hasDeallocated = true;
		}
	}
	return hasDeallocated;
}

bool MemoryManager::firstFit(const Job& job) {
	for (int i = 0; i < top; i++) {
		if (partitions[i]->getIsFree() && partitions[i]->getSize() >= job.getSize()) {
			allocation(job, i);
			return true;
		}
	}
	return false;
}

bool MemoryManager::bestFit(const Job& job) {
	int bestIndex = -1;

	for (int i = 0; i < top; i++) {
		if (partitions[i]->getIsFree() && partitions[i]->getSize() >= job.getSize()) {
			if (bestIndex == -1 || partitions[i]->getSize() < partitions[bestIndex]->getSize()) {
				bestIndex = i;
			}
		}
	}

	if (bestIndex != -1) {
		allocation(job, bestIndex);
		return true;
	}

	return false;
}

bool MemoryManager::simulation(int type) {
	JobManager jobManager;
	bool hasError = false;

	if ((type != 1 && type != 2))
		hasError = true;

	if (isEmpty()) {
		std::cout << "[ERROR] No partition memory available. Simulation cannot proceed." << std::endl;
		hasError = true;
	}

	if (jobManager.isEmpty()) {
		std::cout << "[ERROR] No job available. Simulation cannot proceed." << std::endl;
		hasError = true;
	}

	if (hasError)
		return false;


	std::cout << "\nStart Memory Allocation Simulation." << std::endl;

	Job nextJob;
	bool hasNextJob = jobManager.getJob(nextJob);

	// Continue simulation as long as there are running jobs or new jobs to process
	// updateTime() return false mean no more job is running allow to update
	while (updateTime() || hasNextJob) {
		deallocation();

		std::cout << std::endl;
		while (hasNextJob) {
			bool hasAllocated = false;

			switch (type) {
				case 1: hasAllocated = firstFit(nextJob); break;
				case 2: hasAllocated = bestFit(nextJob); break;
			}

			if (!hasAllocated) {
				if (nextJob.getSize() > maxPartitionSize) {
					// If the job is too large to fit in any partition, skip it
					std::cout << "[ERROR] Job too large to be allocated: Id = "
					<< nextJob.getId()
					<< ", Size = " << nextJob.getSize()
					<< ". Skipped." << std::endl;
					hasNextJob = jobManager.getJob(nextJob);
					continue;
				} else {
					// Job could not be allocated now; wait until next time unit
					break;
				}
			}

			// Successfully allocated, try next job
			hasNextJob = jobManager.getJob(nextJob);
		}

		displayTable();
		if (hasNextJob) {
			std::cout << "[INFO] Next Job - Id: " << nextJob.getId()
				<< ", Size: " << nextJob.getSize()
				<< ", Time: " << nextJob.getTime()
				<< std::endl;
		}
		else {
			std::cout << "[INFO] No more jobs in the queue." << std::endl;
		}
		std::cout << std::string(MAX_LINE, '-') << std::endl;

		//std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	}

	std::cout << "\nMemory Allocation Simulation Completed." << std::endl;
	return true;
}

bool MemoryManager::updateTime() {
	bool hasUpdate = false;
	for (int i = 0; i < top; i++) {
		if (!partitions[i]->getIsFree()) {
			partitions[i]->decreaseJobTime();
			hasUpdate = true;
		}
	}
	return hasUpdate;
}

void MemoryManager::displayTable() {
	const int MAX_COLUMN_SIZE = 16;

	std::cout << std::string(MAX_LINE, '-') << std::endl;
	std::cout << "Memory Partition Table" << std::endl;
	std::cout << std::string(MAX_LINE, '-') << std::endl;
	std::cout << std::left
			<< std::setw(MAX_COLUMN_SIZE) << "Partition ID" << "|"
			<< std::setw(MAX_COLUMN_SIZE) << "Partition Size" << "|"
			<< std::setw(MAX_COLUMN_SIZE) << "Memory Address" << "|"
			<< std::setw(MAX_COLUMN_SIZE) << "Partition Status" << "|"
			<< std::setw(MAX_COLUMN_SIZE) << "Job ID" << "|"
			<< std::setw(MAX_COLUMN_SIZE) << "Job Size" << "|"
			<< std::setw(MAX_COLUMN_SIZE) << "Time Left"
			<< std::endl;
	std::cout << std::string(MAX_LINE, '-') << std::endl;

	for (int i = 0; i < top; i++) {
		std::string status = "Free";
		std::string jobId = "-";
		int jobSize = 0;
		int timeLeft = 0;

		if (!partitions[i]->getIsFree()) {
			status = "Busy";
			jobId = partitions[i]->getJob().getId();
			jobSize = partitions[i]->getJob().getSize();
			timeLeft = partitions[i]->getJob().getTime();
		}

		std::cout << std::left
			<< std::setw(MAX_COLUMN_SIZE) << i+1 << "|"
			<< std::setw(MAX_COLUMN_SIZE) << partitions[i]->getSize() << "|"
			<< std::setw(MAX_COLUMN_SIZE) << partitions[i]->getStartAddress() << "|"
			<< std::setw(MAX_COLUMN_SIZE) << status << "|"
			<< std::setw(MAX_COLUMN_SIZE) << jobId.substr(0, MAX_COLUMN_SIZE) << "|"
			<< std::setw(MAX_COLUMN_SIZE) << jobSize << "|"
			<< std::setw(MAX_COLUMN_SIZE) << timeLeft
			<< std::endl;
	}
	std::cout << std::string(MAX_LINE, '-') << std::endl;
}

