#include "FannManagerCascade.h"
#include <iostream>
static EscapeFMC esc;

#define DEBUG

FannManagerCascade::FannManagerCascade()
	:FannManager()
{
	cascadeFirst = true;

	num_layers = 2;
	layers = std::make_unique<unsigned int[]>(num_layers);
	layers[0] = num_input;
	layers[1] = num_output;
	set_low_precision();
}

void FannManagerCascade::set_high_precision()
{
	MaxCandidateEpoch = 700;
	MaxOutEpoch = 700;
	CandidateLimit = 700;
	CandidateStag = 15;
	OutputStag = 15;
	NumCandidateGroups = 4;
	WeightMultiplier = 0.3;
	CandidateChange = 0.00005;
	OutputChange = 0.00005;
	MaxNeuron = 50;
	SetCascadeTuning();
}

void FannManagerCascade::set_low_precision()
{
	MaxCandidateEpoch = 50;
	MaxOutEpoch = 50;
	CandidateLimit = 50;
	CandidateStag = 12;
	OutputStag = 12;
	NumCandidateGroups = 2;
	WeightMultiplier = 0.4;
	CandidateChange = 0.005;
	OutputChange = 0.005;
	MaxNeuron = 18;
	SetCascadeTuning();
}

FannManagerCascade::~FannManagerCascade()
{
}



void FannManagerCascade::SetCascadeTuning()
{
	net.set_cascade_output_change_fraction(OutputChange);
	net.set_cascade_output_stagnation_epochs(OutputStag);
	net.set_cascade_candidate_change_fraction(CandidateChange);
	net.set_cascade_candidate_stagnation_epochs(CandidateStag);
	net.set_cascade_weight_multiplier(WeightMultiplier);
	net.set_cascade_candidate_limit(CandidateLimit);
	net.set_cascade_max_out_epochs(MaxOutEpoch);
	net.set_cascade_max_cand_epochs(MaxCandidateEpoch);
	net.set_cascade_num_candidate_groups(NumCandidateGroups);
}

void FannManagerCascade::train()
{
	if (!haveTestData)
		return;

	unsigned int neurons_between_reports = 2;
	set_high_precision();
	net.create_shortcut(2, num_input, num_output);
	esc.fM = this;
	net.set_callback(print_callback, NULL);

	// fann_set_train_error_function(ysa, FANN_ERRORFUNC_LINEAR);
	//SetFineTuning();

	cascadeFirst = true; // This will be needed in cont mode  in case net has just been loaded from a file
	net.cascadetrain_on_data(trainData, MaxNeuron, neurons_between_reports, desired_error);

}

double FannManagerCascade::examineTrain(FANN::training_algorithm_enum tal, FANN::activation_function_enum hact, FANN::activation_function_enum oact)
{
	net.set_training_algorithm(tal);
	net.set_activation_function_hidden(hact);
	net.set_activation_function_output(oact);
	esc.fM = this;
	net.set_callback(CascadeLogOut, NULL);
	net.cascadetrain_on_data(trainData, 8, 2, desired_error);

	double trainMSE = net.get_MSE();
	double testMSE = -1;
	if (haveTestData && overtraining)
	{
		net.init_weights(trainData);
		net.reset_MSE();
		net.test_data(testData);
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

	if (fM->haveTestData && fM->overtraining)
	{
		net.reset_MSE();
		net.test_data(fM->testData);
		testMSE = net.get_MSE();
		net.test_data(fM->trainData);
		printf("%08d : %.08f       : %.08f       : %d ", epochs, trainMSE, testMSE, net.get_bit_fail());
	}
#ifdef DEBUG
	else
		std::cout << epochs << " : " << trainMSE << "	: " << net.get_bit_fail() << std::endl;
#endif // DEBUG

	// Memorizing Begin
	if (fM->cascadeFirst)
	{
		for (int i = 0; i<3; i++)
		{
			fM->MinTrainingMSE[i] = trainMSE;
			if (fM->haveTestData && fM->overtraining)
				fM->MinTestingMSE[i] = testMSE;
		}
		fM->cascadeFirst = false;
	}

	// Latest 
	fM->MinTrainingMSE[3] = trainMSE;
	fM->MinTestingMSE[3] = testMSE;

	// Minimum Training MSE
	if (fM->MinTrainingMSE[0]> trainMSE)
	{
		fM->MinTrainingMSE[0] = trainMSE;
		if (fM->haveTestData && fM->overtraining)
			fM->MinTestingMSE[0] = testMSE;
	}
	if (fM->haveTestData && fM->overtraining)
	{
		// Minimum Testing MSE
		if (fM->MinTestingMSE[1]> testMSE)
		{
			fM->MinTrainingMSE[1] = trainMSE;
			fM->MinTestingMSE[1] = testMSE;
		}
		// Minimum (Training MSE + Testing MSE )/2 
		if ((fM->MinTestingMSE[2] + fM->MinTrainingMSE[2])> (trainMSE + testMSE))
		{
			fM->MinTrainingMSE[2] = trainMSE;
			fM->MinTestingMSE[2] = testMSE;
		}
	}

	return 1;
}
