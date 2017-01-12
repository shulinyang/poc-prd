#include "main.h"
#include "DataProcess.h"


int main()
{
	DataProcess data;
	data.LoadRawData("keys.data");
	data.ScaleAll();
	data.WriteScaleParameters("keys.scale.txt");
	data.Shuffle();
	
	data.WriteData("keys_ready.data", data.GetNItem(), 0.6);
	return 0;
}