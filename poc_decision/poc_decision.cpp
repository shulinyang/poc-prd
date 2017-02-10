#include "../poc_ann/FannManagerNormal.h"
#include "../poc_ann/FannManagerCascade.h"
#include <fstream>
#include <iostream>

int main()
{
	std::ios::sync_with_stdio(); // Syncronize cout and printf output
	
	FannManagerCascade fM;
	std::string filename;
	std::cout << "Input basename file: ";
	std::cin >> filename;
	fM.load_test_data(filename+"_proper.data-test.dat");
	fM.load_network("keys.net");
	std::ofstream file("scoring.txt", std::ios::app);
	fM.test();
	if (file.is_open())
	{
		fM.write_score(file);
		file.close();
	}

	return 0;
}