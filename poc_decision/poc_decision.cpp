#include "../poc_ann/FannManager.h"
#include <fstream>


int main()
{
	std::ios::sync_with_stdio(); // Syncronize cout and printf output
	
	FannManager fM;
	fM.load_data("keys_ready.data");
	fM.load_network("keys.net");
	std::ofstream file("scoring.txt", std::ios::app);
	if (file.is_open())
	{
		file << fM.test() << std::endl;
		file.close();
	}

	return 0;
}