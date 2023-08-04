#include <iostream>
#include <string>
#include "findFile.h"


int main()
{
	std::string fileName;
	std::cout << "Enter the name of file to find: ";
	std::cin >> fileName;
	
	std::cout << findFile(fileName) << std::endl;
	return 0;
}

