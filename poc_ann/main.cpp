#ifdef _DEBUG
#define DEBUG		// compatibility
#endif

#include "FannManagerNormal.h"
#include "FannManagerCascade.h"
#include <iostream>
#include <fstream>

#define cascade 1

int main(int argc, char **argv)
{
	try
	{
		std::ios::sync_with_stdio(); // Syncronize cout and printf output
#if cascade
		FannManagerCascade fm;
#else
		FannManagerNormal fm;
#endif // cascade
		fm.load_train_data("keys_ready.data-train.dat");
		fm.load_test_data("keys_ready.data-test.dat");
		fm.optimumAlgorithm();
		fm.optimumActivations();
		//fm.optimumAlgorithm();
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