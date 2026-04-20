#include"MemoryManager.h"

bool isValidInput();

int main () {
	while (true) {
		int type = 0;

		std::cout << "\nMemory Allocation Strategy Menu:" << std::endl;
		std::cout << "  1) First Fit" << std::endl;
		std::cout << "  2) Best Fit" << std::endl;
		std::cout << "  0) Exit Program" << std::endl;
		std::cout << "Enter your choice (0-2): ";
		std::cin >> type;
		while ((type < 0 || type > 2) || !isValidInput()) {
			std::cout << "Invalid input. Enter your choice (0-2): ";
			std::cin >> type;
		}

		if (type == 0) {
			break;
		} else {
			std::cout << std::endl;
			MemoryManager memoryManager;
			if (!memoryManager.simulation(type)) {
				break;
			}
		}
	}
	std::cout << std::endl;
	std::cout << "Program terminated" << std::endl;
	return 0;
}

bool isValidInput() {
	if (std::cin.fail() || std::cin.peek() != '\n') {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return false;
	}
	else {
		std::cin.ignore();
		return true;
	}
}