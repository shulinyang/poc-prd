#pragma once
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
	void set_high_precision();
	void set_low_precision();
	void SetCascadeTuning();
	double examineTrain(FANN::training_algorithm_enum tal, FANN::activation_function_enum hact, FANN::activation_function_enum oact);
public:
	FannManagerCascade();
	~FannManagerCascade();
	
	void train();
	friend int CascadeLogOut(FANN::neural_net &, FANN::training_data &, unsigned int, unsigned int, double, unsigned int, void *);

};

int CascadeLogOut(FANN::neural_net &net, FANN::training_data &train, unsigned int max_epochs, unsigned int epochs_between_reports, double desired_error, unsigned int epochs, void *user_data);

struct EscapeFMC
{
	FannManagerCascade* fM;
	EscapeFMC::EscapeFMC() :fM(nullptr) {}
};