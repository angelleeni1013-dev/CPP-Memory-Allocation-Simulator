#include "Job.h"

Job::Job() : id(""), size(0), time(0) {
}

Job::Job(const std::string& id, int size, int time) : id(id), size(size), time(time) {
}

std::string Job::getId() const {
    return id;
}

int Job::getSize() const {
    return size;
}

int Job::getTime() const {
    return time;
}

void Job::setId(const std::string& id) {
    this->id = id;
}

void Job::setSize(int size) {
    this->size = size;
}

void Job::setTime(int time) {
    this->time = time;
}