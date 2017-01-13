#pragma once
#include "fann_cpp.h"
#include "floatfann.h"
#include <vector>

struct FannParameter
{
	double desired_error;
};


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
	std::vector<unsigned int> num_hidden;
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
	
public:
	FannManager();
	~FannManager();
	void OptimumAlgorithm();
	int ExamineTrain(FANN::training_algorithm_enum tal, FANN::activation_function_enum hact, FANN::activation_function_enum oact);
	friend int LogOut(FANN::neural_net&, FANN::training_data&, unsigned int, unsigned int, float, unsigned int, void*);
	void OptimumActivations();
	bool hasTestData() { return haveTestData; }
};

static const enum AlgoTrain
{
	FANN_TRAIN_INCREMENTAL = 0,
	FANN_TRAIN_BATCH,
	FANN_TRAIN_RPROP,
	FANN_TRAIN_QUICKPROP,
	FANN_TRAIN_SARPROP
};

static const enum AlgoActivation
{
	FANN_LINEAR = 0,
	FANN_SIGMOID,
	FANN_SIGMOID_STEPWISE,
	FANN_SIGMOID_SYMMETRIC,
	FANN_SIGMOID_SYMMETRIC_STEPWISE,
	FANN_GAUSSIAN,
	FANN_GAUSSIAN_SYMMETRIC,
	FANN_ELLIOT,
	FANN_ELLIOT_SYMMETRIC,
	FANN_LINEAR_PIECE,
	FANN_LINEAR_PIECE_SYMMETRIC,
	FANN_SIN_SYMMETRIC,
	FANN_COS_SYMMETRIC
};

int LogOut(FANN::neural_net &net, FANN::training_data &train,
	unsigned int max_epochs, unsigned int epochs_between_reports,
	float desired_error, unsigned int epochs, void *user_data);