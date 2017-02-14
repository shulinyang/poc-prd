#pragma once
#pragma warning(push, 0)
#include "doublefann.h"
#include "fann_cpp.h"
#pragma warning(pop)
#include <vector>
#include <memory>
#include <fstream>

#ifdef _DEBUG
#define DEBUG		// compatibility
#endif

typedef struct Score Score;

/*
	Struct Score
	Stock errors for FannManager
*/

struct Score
{
	double mean_error = 0;
	double max_error = 0;
	double min_error = 1;
};

/*
	Class FannManager
	Abstract class
	Shortcut neural network
*/


class FannManager
{
protected:
	FANN::neural_net net;	// neural network
	FANN::training_data train_data;	// Data for training
	FANN::training_data test_data;	// Data for testing
	
	// Neural network parameters
	double learning_rate;
	double connection_rate;
	unsigned int num_layers;
	unsigned int num_input;
	unsigned int num_output;
	std::unique_ptr<unsigned int[]> layers;

	// Training parameters
	FANN::training_algorithm_enum best_train;
	FANN::activation_function_enum best_activation_hidden;
	FANN::activation_function_enum best_activation_output;
	double desired_error;
	bool over_training;
	bool has_test_data;
	double min_training_MSE[4];
	double min_testing_MSE[4];
	Score score;

	// Private funtions
	virtual double examine_train(FANN::training_algorithm_enum tal, FANN::activation_function_enum hact, FANN::activation_function_enum oact) = 0;

public:
	FannManager();
	~FannManager();

	// Find optimised parameters
	void find_optimum_algorithm();
	void find_optimum_activations();
	
	// Operation on neural network
	virtual void train() = 0;
	double test();
	
	// Output functions
	void save(std::string);
	std::ofstream& write_score(std::ofstream& file);
	
	// Loading functions
	void load_train_data(std::string filename);
	void load_test_data(std::string filename);
	inline bool load_network(std::string filename) { return net.create_from_file(filename); }
};

int print_callback(FANN::neural_net &net, FANN::training_data &train, unsigned int max_epochs, unsigned int epochs_between_reports, double desired_error, unsigned int epochs, void *user_data);