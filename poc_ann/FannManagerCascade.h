#pragma once
#include "FannManager.h"

/*
	Class FannManagerCascade
	mother class: FannManager
	Shortcut neural network with cascade training
*/


class FannManagerCascade : public FannManager
{
	// Cascade training parameters
	bool first_cascade;
	double output_change;
	double candidate_change;
	unsigned int output_stag;
	unsigned int candidate_stag;
	fann_type weight_multiplier;
	fann_type candidate_limit;
	unsigned int max_out_epoch;
	unsigned int max_candidate_epoch;
	unsigned int num_candidate_groups;
	unsigned int max_neuron;

	// Training parameters preset
	void set_high_precision();
	void set_low_precision();
	void set_cascade_tuning();
	
	double examine_train(FANN::training_algorithm_enum tal, FANN::activation_function_enum hact, FANN::activation_function_enum oact);

public:
	FannManagerCascade();
	~FannManagerCascade();
	
	// training function
	void train();

	// Callback
	friend int CascadeLogOut(FANN::neural_net &, FANN::training_data &, unsigned int, unsigned int, double, unsigned int, void *);

};

int CascadeLogOut(FANN::neural_net &net, FANN::training_data &train, unsigned int max_epochs, unsigned int epochs_between_reports, double desired_error, unsigned int epochs, void *user_data);

/*
	Struct EscapeFMC
	Escape encapsulation for callback
*/

struct EscapeFMC
{
	FannManagerCascade* fM;
	EscapeFMC::EscapeFMC() :fM(nullptr) {}
};