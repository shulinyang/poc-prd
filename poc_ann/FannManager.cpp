#include "FannManager.h"


FannManager::FannManager()
	:desired_error(0.001), learning_rate(0.7), num_layers(3), num_input(1), num_output(1), max_iterations(30000), iterations_between_reports(1000)
{
	bestTrain = FANN::TRAIN_RPROP;
	bestActivationHidden = bestActivationOutput = FANN::SIGMOID_SYMMETRIC_STEPWISE;
	net.set_activation_function_hidden(bestActivationHidden);
	net.set_activation_function_output(bestActivationOutput);
	net.set_learning_rate(learning_rate);

	num_hidden.resize(num_layers - num_input - num_output);
	for (size_t i = 0; i < num_hidden.size(); i++)
		num_hidden[i] = 3;
}


FannManager::~FannManager()
{
	net.destroy();
}

void FannManager::optimumAlgorithm()
{
	if (num_layers == 5)
		net.create_sparse(connection_rate, 5, num_input, num_hidden[0], num_hidden[1], num_hidden[2], num_output);
	if (num_layers == 4)
		net.create_sparse(connection_rate, 4, num_input, num_hidden[0], num_hidden[1], num_output);
	else
		net.create_sparse(connection_rate, 3, num_input, num_hidden[0], num_output);

	int min = 1, mse;
	for (int ta = FANN::TRAIN_INCREMENTAL; ta <= FANN::TRAIN_SARPROP; ta++)
	{
		mse = examineTrain(static_cast<FANN::training_algorithm_enum>(ta), bestActivationHidden, bestActivationOutput);
		if (mse<min) {
			min = mse;
			bestTrain = static_cast<FANN::training_algorithm_enum>(ta);
		}
	}
	net.set_training_algorithm(bestTrain);
	net.destroy();
}

void FannManager::optimumActivations() {
	if (haveTestData)
		return;

	if (num_layers == 5)
		net.create_sparse(connection_rate, 5, num_input, num_hidden[0], num_hidden[1], num_hidden[2], num_output);
	if (num_layers == 4)
		net.create_sparse(connection_rate, 4, num_input, num_hidden[0], num_hidden[1], num_output);
	else
		net.create_sparse(connection_rate, 3, num_input, num_hidden[0], num_output);

	int min = 1, mse;
	for (int i = 0; i<13; i++)
	{
		for (int j = 0; j<13; j++)
		{
			set_weigths();
			mse = examineTrain(bestTrain, static_cast<FANN::activation_function_enum>(i), (FANN::activation_function_enum)j); 
			if (mse<min)
			{
				min = mse;
				bestActivationHidden = static_cast<FANN::activation_function_enum>(i);
				bestActivationOutput = static_cast<FANN::activation_function_enum>(j);
			}
		}
	}
	net.set_activation_function_hidden(bestActivationHidden);
	net.set_activation_function_output(bestActivationOutput);
	net.destroy();
}

void FannManager::run()
{

}



int FannManager::examineTrain(FANN::training_algorithm_enum tal, FANN::activation_function_enum hact, FANN::activation_function_enum oact) 
{
	net.set_training_algorithm(tal);
	net.set_activation_function_hidden(hact);
	net.set_activation_function_output(oact);
	net.set_callback(logOut, this);
	net.train_on_data(trainData, 2000, 250, 0.0);

	double trainMSE = net.get_MSE();
	double testMSE = -1;
	if (haveTestData && overtraining)
	{
		set_weigths();
		net.reset_MSE();
		net.test_data(testData);
		testMSE = net.get_MSE();
		return (trainMSE + testMSE) / 2;
	}
	else
		return trainMSE;
}


void FannManager::set_weigths()
{
	net.init_weights(trainData);
}

int logOut(FANN::neural_net &net, FANN::training_data &train, unsigned int max_epochs, unsigned int epochs_between_reports, float desired_error, unsigned int epochs, void *user_data)
{
	double trainMSE = net.get_MSE();
	double testMSE = -1;
	unsigned int newBitFail = net.get_bit_fail();
	FannManager* fM = reinterpret_cast<FannManager*>(user_data);
	if (fM->hasTestData() && fM->overtraining)
	{
		net.reset_MSE();
		net.test_data(fM->testData);
		testMSE = net.get_MSE();
		net.test_data(fM->trainData);
	}

	// Memorizing Begin
	if (epochs == 1)
	{
		for (int i = 0; i<3; i++)
		{
			fM->MinTrainingMSE[i] = trainMSE;
			fM->MinANN[i] = FANN::neural_net(net);
			if (fM->hasTestData() && fM->overtraining)
				fM->MinTestingMSE[i] = testMSE;
		}
		fM->MinANN[3] = FANN::neural_net(net);
	}

	// Latest 
	fM->MinTrainingMSE[3] = trainMSE;
	fM->MinTestingMSE[3] = testMSE;

	// Minimum Training MSE
	if (fM->MinTrainingMSE[0]> trainMSE) {
		//if (fM->MinANN[0] != NULL) fM->MinANN[0]);
		fM->MinANN[0] = FANN::neural_net(net);
		fM->MinTrainingMSE[0] = trainMSE;
		if (fM->hasTestData() && fM->overtraining)
			fM->MinTestingMSE[0] = testMSE;

	}
	if (fM->hasTestData() && fM->overtraining) {
		// Minimum Testing MSE
		if (fM->MinTestingMSE[1]> testMSE) {
			//if (fM->MinANN[1]) fann_destroy(MinANN[1]);
			fM->MinANN[1] = FANN::neural_net(net);
			fM->MinTrainingMSE[1] = trainMSE;
			fM->MinTestingMSE[1] = testMSE;

		}
		// Minimum (Training MSE + Testing MSE )/2 
		if ((fM->MinTestingMSE[2] + fM->MinTrainingMSE[2])> (trainMSE + testMSE)) {
			//if (fM->MinANN[2]) fann_destroy(MinANN[2]);
			fM->MinANN[2] = FANN::neural_net(net);
			fM->MinTrainingMSE[2] = trainMSE;
			fM->MinTestingMSE[2] = testMSE;
		}
	}
	return 1;
}