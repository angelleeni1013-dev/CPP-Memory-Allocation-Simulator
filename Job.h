#pragma once

#include<string>

class Job {

private:
	std::string id;
	int size;
	int time;

public:

	// constructor
	Job();
	Job(const std::string& id, int size, int time);

	// getter
	std::string getId() const;
	int getSize() const;
	int getTime() const;

	// setter
	void setId(const std::string& id);
	void setSize(int size);
	void setTime(int time);
};

