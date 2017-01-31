#include "DataProcess.h"
#include <iostream>
#include <string>

int main()
{
	DataProcess data;
	std::string filename;
	std::cout << "Input basename file: ";
	std::cin >> filename;
	filename += "_proper.data";
	data.LoadRawData(filename);
	data.ScaleAll();
	data.WriteScaleParameters("keys.scale.txt");
	data.Shuffle();
	data.WriteData(filename, 1, 0.75);
	return 0;
}