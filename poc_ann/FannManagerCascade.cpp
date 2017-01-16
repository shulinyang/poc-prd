#include "FannManagerCascade.h"

static EscapeFMC esc;

FannManagerCascade::FannManagerCascade()
	:FannManager()
{
	cascadeFirst = true;
	MaxCandidateEpoch = 150;
	CandidateLimit = 150;
	CandidateStag = 12;
	NumCandidateGroups = 2;
	WeightMultiplier = 0.4;
	MaxOutEpoch = 150;
	CandidateChange = 0.01;
	OutputChange = 0.01;
	MaxNeuron = 20;
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

	unsigned int max_neurons = MaxNeuron;
	unsigned int neurons_between_reports = 1;

	net.create_shortcut(2, num_input, num_output);
	esc.fM = this;
	net.set_callback(CascadeLogOut, NULL);

	// fann_set_train_error_function(ysa, FANN_ERRORFUNC_LINEAR);
	//SetFineTuning();
	SetCascadeTuning();

	cascadeFirst = true; // This will be needed in cont mode  in case net has just been loaded from a file
	net.cascadetrain_on_data(trainData, max_neurons, neurons_between_reports, desired_error);

}

int CascadeLogOut(FANN::neural_net &net, FANN::training_data &train, unsigned int max_epochs, unsigned int epochs_between_reports, float desired_error, unsigned int epochs, void *user_data)
{
	double trainMSE = net.get_MSE();
	double testMSE = -1;
	unsigned int newBitFail = net.get_bit_fail();

	FannManagerCascade* fM = esc.fM;

	if (fM->haveTestData && fM->overtraining)
	{
		net.reset_MSE();
		net.test_data(fM->testData);
		testMSE = net.get_MSE();
		net.test_data(fM->trainData);
		printf("%08d : %.08f       : %.08f       : %d ", epochs, trainMSE, testMSE, newBitFail);
	}
	else
		printf("%08d : %.08f       : %d  ", epochs, trainMSE, newBitFail);

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