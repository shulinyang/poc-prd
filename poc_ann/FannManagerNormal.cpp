#include "FannManagerNormal.h"
#include <iostream>
#include <iomanip>

static EscapeFM esc;	// escape encapsulation for callback

FannManagerNormal::FannManagerNormal()
	:FannManager()
{
	max_iterations = 30000;
	iterations_between_reports = 1000;
	num_layers = 7;
	layers = std::make_unique<unsigned int[]>(num_layers);
	layers[0] = num_input;

	for (unsigned int i = 1; i < (num_layers - 2); i++)
		layers[i] = 10;
	layers[num_layers - 1] = num_output;
}

FannManagerNormal::~FannManagerNormal()
{
}

void FannManagerNormal::train()
{
#ifdef DEBUG
	std::cout << "Start training." << std::endl;
#endif // DEBUG

	net.create_shortcut_array(num_layers, layers.get());
	net.init_weights(train_data);

	std::cout << "Max Epochs " << std::setw(8) << max_iterations << ". "
		<< "Desired Error: " << std::left << desired_error << std::right << std::endl;
	net.set_callback(print_callback, NULL);
	net.train_on_data(train_data, max_iterations, iterations_between_reports, desired_error);


	net.train_on_data(train_data, max_iterations,
		iterations_between_reports, desired_error);
}

double FannManagerNormal::examine_train(FANN::training_algorithm_enum tal, FANN::activation_function_enum hact, FANN::activation_function_enum oact)
{
	net.set_training_algorithm(tal);
	net.set_activation_function_hidden(hact);
	net.set_activation_function_output(oact);
	esc.fM = this;
	net.set_callback(logOut, NULL);
	net.train_on_data(train_data, 2000, 250, 0.0);

	double trainMSE = net.get_MSE();
	double testMSE = -1;
	if (has_test_data && over_training)
	{
		net.init_weights(train_data);
		net.reset_MSE();
		net.test_data(test_data);
		testMSE = net.get_MSE();
		return (trainMSE + testMSE) / 2;
	}
	else
		return trainMSE;
}

int logOut(FANN::neural_net &net, FANN::training_data &train, unsigned int max_epochs, unsigned int epochs_between_reports, double desired_error, unsigned int epochs, void *user_data)
{
	double trainMSE = net.get_MSE();
	double testMSE = -1;
	unsigned int newBitFail = net.get_bit_fail();

	FannManagerNormal* fM = esc.fM;
	if (fM->has_test_data && fM->over_training)
	{
		net.reset_MSE();
		net.test_data(fM->test_data);
		testMSE = net.get_MSE();
		net.test_data(fM->train_data);
	}

	// Memorizing Begin
	if (epochs == 1)
	{
		for (int i = 0; i<3; i++)
		{
			fM->min_training_MSE[i] = trainMSE;
			if (fM->has_test_data && fM->over_training)
				fM->min_testing_MSE[i] = testMSE;
		}
	}

	// Latest 
	fM->min_training_MSE[3] = trainMSE;
	fM->min_testing_MSE[3] = testMSE;

	// Minimum Training MSE
	if (fM->min_training_MSE[0]> trainMSE)
	{
		fM->min_training_MSE[0] = trainMSE;
		if (fM->has_test_data && fM->over_training)
			fM->min_testing_MSE[0] = testMSE;
	}
	if (fM->has_test_data && fM->over_training)
	{
		// Minimum Testing MSE
		if (fM->min_testing_MSE[1]> testMSE)
		{
			fM->min_training_MSE[1] = trainMSE;
			fM->min_testing_MSE[1] = testMSE;

		}
		// Minimum (Training MSE + Testing MSE )/2 
		if ((fM->min_testing_MSE[2] + fM->min_training_MSE[2])> (trainMSE + testMSE))
		{
			fM->min_training_MSE[2] = trainMSE;
			fM->min_testing_MSE[2] = testMSE;
		}
	}
	return 1;
}
