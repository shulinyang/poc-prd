#pragma once
#include "floatfann.h"
#include "fann_cpp.h"
#include "FannManager.h"

class FannManagerCascade : public FannManager
{
	bool cascadeFirst;
	double OutputChange;
	double CandidateChange;
	unsigned int OutputStag;
	unsigned int CandidateStag;
	fann_type WeightMultiplier;
	fann_type CandidateLimit;
	unsigned int MaxOutEpoch;
	unsigned int MaxCandidateEpoch;
	unsigned int NumCandidateGroups;
	unsigned int MaxNeuron;

public:
	FannManagerCascade();
	~FannManagerCascade();
	void SetCascadeTuning();
	void train();
	friend int CascadeLogOut(FANN::neural_net &, FANN::training_data &, unsigned int, unsigned int, float, unsigned int, void *);

};

int CascadeLogOut(FANN::neural_net &net, FANN::training_data &train, unsigned int max_epochs, unsigned int epochs_between_reports, float desired_error, unsigned int epochs, void *user_data);

struct EscapeFMC
{
	FannManagerCascade* fM;
	EscapeFMC::EscapeFMC() :fM(nullptr) {}
};