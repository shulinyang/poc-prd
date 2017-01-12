#include "main.h"
#include "DataProcess.h"


int main()
{
	DataProcess data;
	data.LoadRawData("keys_proper.data");
	data.ScaleAll();
	data.WriteScaleParameters("keys.scale.txt");
	data.Shuffle();
	data.WriteData("keys_ready.data", 1, 0.6);
	return 0;
}