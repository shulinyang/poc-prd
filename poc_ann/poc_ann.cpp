#ifdef _DEBUG
#define DEBUG		// compatibility
#endif

#include "FannManagerNormal.h"
#include "FannManagerCascade.h"
#include <iostream>
#include <fstream>

#define cascade 0

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
		std::string filename;
		std::cout << "Input basename file: ";
		std::cin >> filename;
		fm.load_train_data(filename+"_proper.data-train.dat");
		fm.load_test_data(filename+"_proper.data-test.dat");
		//fm.optimumAlgorithm();
		//fm.optimumActivations();
		//fm.optimumAlgorithm();
		fm.train();
		std::ofstream file("scoring.txt", std::ios::app);
		fm.test();
		if (file.is_open())
		{
			fm.write_score(file);
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