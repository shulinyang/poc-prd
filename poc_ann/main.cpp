#ifdef _DEBUG
#define DEBUG		// compatibility
#endif

#include "floatfann.h"
#include "fann_cpp.h"
#include "FannManager.h"
#include "FannManagerCascade.h"
#include <iostream>

int main(int argc, char **argv)
{
	try
	{
		std::ios::sync_with_stdio(); // Syncronize cout and printf output
		FannManagerCascade fm;
		fm.load_data("keys_ready.data");
		//fm.optimumAlgorithm();
		//fm.optimumActivations();
		fm.train();
		fm.test();
		fm.save("keys");
	}
	catch (...)
	{
		std::cerr << std::endl << "Abnormal exception." << std::endl;
	}
	return 0;
}