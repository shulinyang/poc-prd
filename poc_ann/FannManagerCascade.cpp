#include "FannManagerCascade.h"
#include <iostream>
static EscapeFMC esc;	// escape encapsulation for callback


FannManagerCascade::FannManagerCascade()
	:FannManager()
{
	first_cascade = true;

	num_layers = 2;
	layers = std::make_unique<unsigned int[]>(num_layers);
	layers[0] = num_input;
	layers[1] = num_output;
	set_low_precision();
}

void FannManagerCascade::set_high_precision()
{
	max_candidate_epoch = 700;
	max_out_epoch = 500;
	candidate_limit = 700;
	candidate_stag = 30;
	output_stag = 30;
	num_candidate_groups = 6;
	weight_multiplier = 0.2;
	candidate_change = 0.001;
	output_change = 0.001;
	max_neuron = 130;
	set_cascade_tuning();
}

void FannManagerCascade::set_low_precision()
{
	max_candidate_epoch = 50;
	max_out_epoch = 50;
	candidate_limit = 50;
	candidate_stag = 12;
	output_stag = 12;
	num_candidate_groups = 2;
	weight_multiplier = 0.4;
	candidate_change = 0.005;
	output_change = 0.005;
	max_neuron = 18;
	set_cascade_tuning();
}

FannManagerCascade::~FannManagerCascade(){}


void FannManagerCascade::set_cascade_tuning()
{
	net.set_cascade_output_change_fraction(output_change);
	net.set_cascade_output_stagnation_epochs(output_stag);
	net.set_cascade_candidate_change_fraction(candidate_change);
	net.set_cascade_candidate_stagnation_epochs(candidate_stag);
	net.set_cascade_weight_multiplier(weight_multiplier);
	net.set_cascade_candidate_limit(candidate_limit);
	net.set_cascade_max_out_epochs(max_out_epoch);
	net.set_cascade_max_cand_epochs(max_candidate_epoch);
	net.set_cascade_num_candidate_groups(num_candidate_groups);
}

void FannManagerCascade::train()
{
	if (!has_test_data)
		return;

	unsigned int neurons_between_reports = 2;
	//set_high_precision();
	net.create_shortcut(2, num_input, num_output);
	esc.fM = this;
	net.set_callback(print_callback, NULL);

	first_cascade = true;
	net.cascadetrain_on_data(train_data, max_neuron, neurons_between_reports, desired_error);
}

double FannManagerCascade::examine_train(FANN::training_algorithm_enum tal, FANN::activation_function_enum hact, FANN::activation_function_enum oact)
{
	net.set_training_algorithm(tal);
	net.set_activation_function_hidden(hact);
	net.set_activation_function_output(oact);
	esc.fM = this;
	net.set_callback(CascadeLogOut, NULL);
	net.cascadetrain_on_data(train_data, 8, 2, desired_error);

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

int CascadeLogOut(FANN::neural_net &net, FANN::training_data &train, unsigned int max_epochs, unsigned int epochs_between_reports, double desired_error, unsigned int epochs, void *user_data)
{
	double trainMSE = net.get_MSE();
	double testMSE = -1;

	FannManagerCascade* fM = esc.fM;

	if (fM->has_test_data && fM->over_training)
	{
		net.reset_MSE();
		net.test_data(fM->test_data);
		testMSE = net.get_MSE();
		net.test_data(fM->train_data);
		printf("%08d : %.08f       : %.08f       : %d ", epochs, trainMSE, testMSE, net.get_bit_fail());
	}
#ifdef DEBUG
	else
		std::cout << epochs << " : " << trainMSE << "	: " << net.get_bit_fail() << std::endl;
#endif // DEBUG

	// Memorizing Begin
	if (fM->first_cascade)
	{
		for (int i = 0; i<3; i++)
		{
			fM->min_training_MSE[i] = trainMSE;
			if (fM->has_test_data && fM->over_training)
				fM->min_testing_MSE[i] = testMSE;
		}
		fM->first_cascade = false;
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
