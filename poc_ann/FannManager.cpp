#include "FannManager.h"
#include <iostream>
#include <iomanip>

static EscapeFM esc;

FannManager::FannManager()
	:desired_error(0.0005), learning_rate(0.7), num_layers(4), num_input(1), num_output(1), max_iterations(30000), iterations_between_reports(1000)
{
	connection_rate = 1;
	haveTestData = false;
	overtraining = false;
	bestTrain = FANN::TRAIN_RPROP;
	bestActivationHidden = bestActivationOutput = FANN::SIGMOID_SYMMETRIC_STEPWISE;
	net.set_activation_function_hidden(bestActivationHidden);
	net.set_activation_function_output(bestActivationOutput);
	net.set_learning_rate(learning_rate);
	net.set_activation_steepness_hidden(1.0);
	net.set_activation_steepness_output(1.0);

	//for (int i = 0; i < 4; i++)
	//	MinANN[i] = NULL;
	
	layers = std::make_unique<unsigned int[]>(num_layers);
	layers[0] = num_input;
	for (size_t i = 1; i < (num_layers - 2); i++)
		layers[i] = 3;
	layers[num_layers - 1] = num_output;
}


FannManager::~FannManager()
{
	net.destroy();
}

void FannManager::optimumAlgorithm()
{
	net.create_shortcut_array(num_layers, layers.get());

	double min = 1, mse;
	for (int ta = FANN::TRAIN_INCREMENTAL; ta <= FANN::TRAIN_SARPROP; ta++)
	{
		set_weigths();
		mse = examineTrain(static_cast<FANN::training_algorithm_enum>(ta), bestActivationHidden, bestActivationOutput);
		if (mse<min) {
			min = mse;
			bestTrain = static_cast<FANN::training_algorithm_enum>(ta);
		}
	}
#ifdef DEBUG
	std::cout << "Best training method is " << bestTrain<< std::endl;
#endif // DEBUG

	net.set_training_algorithm(bestTrain);
	net.destroy();
}

void FannManager::optimumActivations() {
	if (!haveTestData)
		return;

	net.create_shortcut_array(num_layers, layers.get());
	
	double min = 1, mse;
	for (int i = 2; i<13; i++)
	{
		for (int j = 2; j<13; j++)
		{
			set_weigths();
			mse = examineTrain(bestTrain, static_cast<FANN::activation_function_enum>(i), (FANN::activation_function_enum)j); 
#ifdef DEBUG
			std::cout << static_cast<FANN::activation_function_enum>(i) << " " << (FANN::activation_function_enum)j << " mse: " << mse << std::endl;
#endif

			if (mse<min)
			{
				min = mse;
				bestActivationHidden = static_cast<FANN::activation_function_enum>(i);
				bestActivationOutput = static_cast<FANN::activation_function_enum>(j);
			}
		}
	}
#ifdef DEBUG
	std::cout << "Activation function for hidden layer is " << bestActivationHidden << "\n";
	std::cout << "Activation function for output layer is " << bestActivationOutput << std::endl;
#endif // DEBUG

	net.set_activation_function_hidden(bestActivationHidden);
	net.set_activation_function_output(bestActivationOutput);
	net.destroy();
}

void FannManager::train()
{
	net.create_shortcut_array(num_layers, layers.get());
	set_weigths();

	std::cout << "Max Epochs " << std::setw(8) << max_iterations << ". "
		<< "Desired Error: " << std::left << desired_error << std::right << std::endl;
	net.set_callback(print_callback, this);
	net.train_on_data(trainData, max_iterations, iterations_between_reports, desired_error);


	net.train_on_data(trainData, max_iterations,
		iterations_between_reports, desired_error);
}

void FannManager::test()
{
	std::cout << std::endl << "Testing network." << std::endl;

	for (unsigned int i = 0; i < trainData.length_train_data(); ++i)
	{
		// Run the network on the test data
		fann_type *calc_out = net.run(trainData.get_input()[i]);

		std::cout << "XOR test (" << std::showpos << trainData.get_input()[i][0] << ", "
			<< trainData.get_input()[i][1] << ") -> " << *calc_out
			<< ", should be " << trainData.get_output()[i][0] << ", "
			<< "difference = " << std::noshowpos
			<< std::abs(*calc_out - trainData.get_output()[i][0]) << std::endl;
	}
}

void FannManager::load_data(std::string str)
{
	trainData.read_train_from_file(str+"-train.dat");
	
	if (testData.read_train_from_file(str + "-test.dat"))
		haveTestData = true;
}


double FannManager::examineTrain(FANN::training_algorithm_enum tal, FANN::activation_function_enum hact, FANN::activation_function_enum oact) 
{
	net.set_training_algorithm(tal);
	net.set_activation_function_hidden(hact);
	net.set_activation_function_output(oact);
	esc.fM = this;
	net.set_callback(logOut, NULL);
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

void FannManager::save(std::string str)
{
	std::cout << std::endl << "Saving network." << std::endl;

	// Save the network in floating point and fixed point
	net.save(str+".net");
	unsigned int decimal_point = net.save_to_fixed(str+"_fixed.net");
	trainData.save_train_to_fixed(str+"_fixed.data", decimal_point);

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

	FannManager* fM = esc.fM;
	if (fM->haveTestData && fM->overtraining)
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
			if (fM->haveTestData && fM->overtraining)
				fM->MinTestingMSE[i] = testMSE;
		}
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

int print_callback(FANN::neural_net &net, FANN::training_data &train,
	unsigned int max_epochs, unsigned int epochs_between_reports,
	float desired_error, unsigned int epochs, void *user_data)
{
	std::cout << "Epochs     " << std::setw(8) << epochs << ". "
		<< "Current Error: " << std::left << net.get_MSE() << std::right << std::endl;
	return 0;
}

