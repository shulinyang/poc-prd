#pragma once
#include "floatfann.h"
#include "fann_cpp.h"
#include <vector>
#include <memory>

class FannManager
{
	FANN::neural_net net;
	FANN::training_data trainData;
	FANN::training_data testData;
	double desired_error;
	double learning_rate;
	double connection_rate;
	unsigned int num_layers;
	unsigned int num_input;
	std::unique_ptr<unsigned int[]> layers;
	unsigned int num_output;
	unsigned int max_iterations;
	unsigned int iterations_between_reports;
	FANN::training_algorithm_enum bestTrain;
	FANN::activation_function_enum bestActivationHidden;
	FANN::activation_function_enum bestActivationOutput;
	bool overtraining;
	bool haveTestData;
	FANN::neural_net MinANN[4];
	double MinTrainingMSE[4];
	double MinTestingMSE[4];
	
	void set_weigths();
	int examineTrain(FANN::training_algorithm_enum tal, FANN::activation_function_enum hact, FANN::activation_function_enum oact);

public:
	FannManager();
	~FannManager();
	void optimumAlgorithm();
	void optimumActivations();

	friend int logOut(FANN::neural_net&, FANN::training_data&, unsigned int, unsigned int, float, unsigned int, void*);

	void run();
	void save(std::string);
	void load_data(std::string);
};


int logOut(FANN::neural_net &net, FANN::training_data &train, unsigned int max_epochs, unsigned int epochs_between_reports, float desired_error, unsigned int epochs, void *user_data);

int print_callback(FANN::neural_net &net, FANN::training_data &train, unsigned int max_epochs, unsigned int epochs_between_reports, float desired_error, unsigned int epochs, void *user_data);