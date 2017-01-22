#ifdef _DEBUG
#define DEBUG		// compatibility
#endif

#include "FannManager.h"
#include "FannManagerCascade.h"
#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
	try
	{
		std::ios::sync_with_stdio(); // Syncronize cout and printf output
		FannManager fm;
		fm.load_data("keys_ready.data");
		fm.optimumAlgorithm();
		fm.optimumActivations();
		fm.optimumAlgorithm();
		fm.train();
		std::ofstream file("scoring.txt", std::ios::app);
		if (file.is_open())
		{
			file << fm.test() << std::endl;
			file.close();
		}
		fm.save("keys");
	}
	catch (...)
	{
		std::cerr << std::endl << "Abnormal exception." << std::endl;
	}
	return 0;
}