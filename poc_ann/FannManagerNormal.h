#pragma once
#include <memory>
#include "FannManager.h"

typedef struct EscapeFM escapeFM;

/*
	Class FannManagerNormal
	mother class: FannManager
	Shortcut neural network with normal training
*/

class FannManagerNormal : public FannManager
{
	// Normal training parameters
	unsigned int max_iterations;
	unsigned int iterations_between_reports;
	
	double examine_train(FANN::training_algorithm_enum tal, FANN::activation_function_enum hact, FANN::activation_function_enum oact);

public:
	FannManagerNormal();
	~FannManagerNormal();
	
	// Training function
	void train();

	// Callback
	friend int logOut(FANN::neural_net&, FANN::training_data&, unsigned int, unsigned int, double, unsigned int, void*);
};

int logOut(FANN::neural_net &net, FANN::training_data &train, unsigned int max_epochs, unsigned int epochs_between_reports, double desired_error, unsigned int epochs, void *user_data);

/*
	struct EscapeFM
	Escape encapsulation for callback
*/


struct EscapeFM
{
	FannManagerNormal* fM;
	EscapeFM::EscapeFM() :fM(nullptr) {}
};