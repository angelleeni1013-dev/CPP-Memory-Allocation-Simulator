#include "Partition.h"

Partition::Partition() : size(0), startAddress(0), isFree(false) {
}

Partition::Partition(int size, int startAddress) : size(size), startAddress(startAddress), isFree(true) {
}

int Partition::getSize() const {
	return this->size;
}

int Partition::getStartAddress() const {
	return this->startAddress;
}

bool Partition::getIsFree() const {
	return this->isFree;
}

Job Partition::getJob() const {
	return this->job;
}

void Partition::setSize(int size) {
	this->size = size;
}

void Partition::setStartAddress(int startAddress) {
	this->startAddress = startAddress;
}

void Partition::setIsFree(bool isFree) {
	this->isFree = isFree;
}

void Partition::setJob(const Job& job) {
	this->job = job;
}

void Partition::decreaseJobTime() {
	int time = this->job.getTime();
	this->job.setTime(--time);
}