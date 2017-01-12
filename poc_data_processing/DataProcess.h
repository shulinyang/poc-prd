#ifndef DATAPROCESS_H
#define DATAPROCESS_H
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

/*
	Extract from FannTool
	Original author: BlueKid
	License GNU GPL
*/

using namespace std;

int GetnItem(char *FileName);

class DataItem
{
public:
    int nhist;
    bool scale;
	
    void SetData(unsigned long i,double value);
    void Add(double value);
    // void DeleteData(unsigned long i);
    double* Histogram(int n=10);
    void Scale( double iminv,double imaxv );
    void Scale();
    inline void Swap(unsigned long i1,unsigned long i2) { std::swap(data[i1], data[i2]); }
    
	// Getter
	inline double GetData(unsigned long i) { return data[i]; }
	inline double GetMin() {return min; }
    inline double GetMinV() { return minv; }
    inline double GetMax() { return max; }
    inline double GetMaxV() { return maxv; }
	inline double GetAvg() { return avg; };
	// Setter
	inline void SetMinV(double value) { minv = value; }
    inline void SetMaxV(double value) { maxv = value; }
	inline void SetMin(double value) { min = value; }
	inline void SetMax(double value) { max = value; }
	
	// Constructor Destructor
	DataItem();
    virtual ~DataItem() { data.clear(); }
protected:
	double min, max;
	double minv, maxv;
	double sum;
	double hist[20];
	double avg;
	double stddev;
	vector<double> data;

};

// typedef pDataItem DataItem*;

class DataProcess
{
public:
	DataProcess();
	bool LoadRawData(char * filename);
	bool ParseData();
	bool WriteData(char *FileName,int nOut, float pTrain);
	bool WriteScaleParameters(char *FileName);
	DataItem *GetItem(int i);
	inline unsigned long GetNData(){return nData;}
	inline unsigned int GetNItem(){return nItem;}
	void ScaleAll();
	void Shuffle();
	virtual ~DataProcess();
protected:
	vector<double> line_data;
	int ReadLine(string line);
	vector<DataItem*> items;
	unsigned int nItem;
	unsigned long nData;

};


// Time Series Reader

class TimeSeri : public DataItem
{
public:
	TimeSeri();
	bool shuffle;
	bool LoadRawData(char *file);
	bool WriteData(char *file,float pTrain);
	bool WriteScaleParameters(char *FileName);
	unsigned long GetNData(){return (count-nInput-nOutput);}
	virtual ~TimeSeri(){ data.clear(); }

//    protected:
	long count;
	int nInput;
	int nOutput;
};


#endif // DATAPROCESS_H
