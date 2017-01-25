#include "FannManager.h"
#include <iostream>
#include <iomanip>

FannManager::FannManager()
	:desired_error(0.00001), learning_rate(0.7), num_layers(7), num_input(1), num_output(1)
{
	score = -1;
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

}


FannManager::~FannManager()
{
	net.destroy();
}


double FannManager::test()
{
	if (!haveTestData)
		return -1;
	std::cout << std::endl << "Testing network." << std::endl;
	score = 0.0;
#pragma omp parallel for num_threads(4)
	for (int i = 0; i < trainData.length_train_data(); ++i)
	{
		// Run the network on the test data
		fann_type *calc_out = net.run(trainData.get_input()[i]);

		std::cout << "XOR test (" << std::showpos << trainData.get_input()[i][0] << ", "
			<< trainData.get_input()[i][1] << ") -> " << *calc_out
			<< ", should be " << trainData.get_output()[i][0] << ", "
			<< "difference = " << std::noshowpos
			<< std::abs(*calc_out - trainData.get_output()[i][0]) << std::endl;
		score += abs(*calc_out - trainData.get_output()[i][0]);
	}
	score /= trainData.length_train_data();
	return score;
}


void FannManager::optimumAlgorithm()
{
#ifdef DEBUG
	std::cout << "Finding best training algorithm." << std::endl;
#endif // DEBUG

	net.create_shortcut_array(num_layers, layers.get());

	double min = 1, mse;

	for (int ta = FANN::TRAIN_RPROP; ta <= FANN::TRAIN_SARPROP; ta++)
	{
		net.init_weights(trainData);
		mse = examineTrain(static_cast<FANN::training_algorithm_enum>(ta), bestActivationHidden, bestActivationOutput);
		if (mse<min) {
			min = mse;
			bestTrain = static_cast<FANN::training_algorithm_enum>(ta);
		}
	}
#ifdef DEBUG
	std::cout << "Best training method is " << bestTrain << std::endl;
#endif // DEBUG

	net.set_training_algorithm(bestTrain);
	net.destroy();
}

void FannManager::optimumActivations() {
	if (!haveTestData)
		return;
#ifdef DEBUG
	std::cout << "Finding best activation functions." << std::endl;
#endif // DEBUG

	net.create_shortcut_array(num_layers, layers.get());

	double min = 1, mse;
	for (int i = 2; i<13; i++)
	{
		for (int j = 2; j<13; j++)
		{
			net.init_weights(trainData);
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


void FannManager::load_train_data(std::string filename)
{
	if (!trainData.read_train_from_file(filename))
		std::cerr << "Failed loading train data set." << std::endl;
}

void FannManager::load_test_data(std::string filename)
{
	if (testData.read_train_from_file(filename))
		haveTestData = true;
	else
		std::cerr << "Failed loading test data set." << std::endl;
}



void FannManager::save(std::string str)
{
	std::cout << std::endl << "Saving network." << std::endl;

	// Save the network in floating point and fixed point
	net.save(str+".net");
	unsigned int decimal_point = net.save_to_fixed(str+"_fixed.net");
	trainData.save_train_to_fixed(str+"_fixed.data", decimal_point);

}


int print_callback(FANN::neural_net &net, FANN::training_data &train,
	unsigned int max_epochs, unsigned int epochs_between_reports,
	float desired_error, unsigned int epochs, void *user_data)
{
	std::cout << "Epochs     " << std::setw(8) << epochs << ". "
		<< " Bit fail: " << net.get_bit_fail() <<" Current Error: " << std::left << net.get_MSE() << std::right << std::endl;
	return 0;
}

