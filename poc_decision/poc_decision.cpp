#include "../poc_ann/FannManagerNormal.h"
#include "../poc_ann/FannManagerCascade.h"
#include <fstream>


int main()
{
	std::ios::sync_with_stdio(); // Syncronize cout and printf output
	
	FannManagerCascade fM;
	fM.load_test_data("keys_ready.data-test.dat");
	fM.load_network("keys.net");
	std::ofstream file("scoring.txt", std::ios::app);
	if (file.is_open())
	{
		file << fM.test() << std::endl;
		file.close();
	}

	return 0;
}