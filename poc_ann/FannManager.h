#pragma once
#pragma warning(push, 0)
#include "doublefann.h"
#include "fann_cpp.h"
#pragma warning(pop)
#include <vector>
#include <memory>

#ifdef _DEBUG
#define DEBUG		// compatibility
#endif


class FannManager
{
protected:
	FANN::neural_net net;
	FANN::training_data trainData;
	FANN::training_data testData;
	double desired_error;
	double learning_rate;
	double connection_rate;
	unsigned int num_layers;
	unsigned int num_input;
	unsigned int num_output;

	FANN::training_algorithm_enum bestTrain;
	FANN::activation_function_enum bestActivationHidden;
	FANN::activation_function_enum bestActivationOutput;
	bool overtraining;
	bool haveTestData;
	double MinTrainingMSE[4];
	double MinTestingMSE[4];
	double score;
	std::unique_ptr<unsigned int[]> layers;
	virtual double examineTrain(FANN::training_algorithm_enum tal, FANN::activation_function_enum hact, FANN::activation_function_enum oact) = 0;

public:
	FannManager();
	~FannManager();
	void optimumAlgorithm();
	void optimumActivations();
	inline bool load_network(std::string filename) {return net.create_from_file(filename);}
	double test();
	void save(std::string);
	virtual void train() = 0;
	void load_train_data(std::string filename);
	void load_test_data(std::string filename);
};

int print_callback(FANN::neural_net &net, FANN::training_data &train, unsigned int max_epochs, unsigned int epochs_between_reports, double desired_error, unsigned int epochs, void *user_data);