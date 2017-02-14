#include "FannManager.h"
#include <iostream>
#include <iomanip>


FannManager::FannManager()
	:desired_error(0.000001), learning_rate(0.7), num_layers(7), num_input(1), num_output(1)
{
	connection_rate = 1;
	has_test_data = false;
	over_training = false;
	best_train = FANN::TRAIN_SARPROP;
	best_activation_hidden = FANN::COS_SYMMETRIC;
	best_activation_output = FANN::ELLIOT_SYMMETRIC;
	net.set_activation_function_hidden(best_activation_hidden);
	net.set_activation_function_output(best_activation_output);
	net.set_learning_rate(learning_rate);
	net.set_activation_steepness_hidden(0.25);
	net.set_activation_steepness_output(0.25);
}


FannManager::~FannManager()
{
	net.destroy();
}


double FannManager::test()
{
	if (!has_test_data)
		return -1;
	std::cout << std::endl << "Testing network." << std::endl;
	score.mean_error = 0.0;

	for (unsigned int i = 0; i < test_data.length_train_data(); ++i)
	{
		// Run the network on the test data
		fann_type *calc_out = net.run(test_data.get_input()[i]);

		std::cout << "XOR test (" << std::showpos << test_data.get_input()[i][0] << ", "
			<< test_data.get_input()[i][1] << ") -> " << *calc_out
			<< ", should be " << test_data.get_output()[i][0] << ", "
			<< "difference = " << std::noshowpos
			<< std::abs(*calc_out - test_data.get_output()[i][0]) << std::endl;
		score.max_error = max(score.max_error, std::abs(*calc_out - test_data.get_output()[i][0]));
		score.min_error = min(score.min_error, std::abs(*calc_out - test_data.get_output()[i][0]));
		score.mean_error += abs(*calc_out - test_data.get_output()[i][0]);
	}
	score.mean_error /= test_data.length_train_data();
	return score.mean_error;
}


void FannManager::find_optimum_algorithm()
{
#ifdef DEBUG
	std::cout << "Finding best training algorithm." << std::endl;
#endif // DEBUG

	net.create_shortcut_array(num_layers, layers.get());

	double min = 1, mse;

	for (int ta = FANN::TRAIN_RPROP; ta <= FANN::TRAIN_SARPROP; ta++)
	{
		net.init_weights(train_data);
		mse = examine_train(static_cast<FANN::training_algorithm_enum>(ta), best_activation_hidden, best_activation_output);
		if (mse<min) {
			min = mse;
			best_train = static_cast<FANN::training_algorithm_enum>(ta);
		}
	}
#ifdef DEBUG
	std::cout << "Best training method is " << best_train << std::endl;
#endif // DEBUG

	net.set_training_algorithm(best_train);
	net.destroy();
}

void FannManager::find_optimum_activations() {
	if (!has_test_data)
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
			net.init_weights(train_data);
			mse = examine_train(best_train, static_cast<FANN::activation_function_enum>(i), (FANN::activation_function_enum)j);
#ifdef DEBUG
			std::cout << static_cast<FANN::activation_function_enum>(i) << " " << (FANN::activation_function_enum)j << " mse: " << mse << std::endl;
#endif

			if (mse<min)
			{
				min = mse;
				best_activation_hidden = static_cast<FANN::activation_function_enum>(i);
				best_activation_output = static_cast<FANN::activation_function_enum>(j);
			}
		}
	}
#ifdef DEBUG
	std::cout << "Activation function for hidden layer is " << best_activation_hidden << "\n";
	std::cout << "Activation function for output layer is " << best_activation_output << std::endl;
#endif // DEBUG

	net.set_activation_function_hidden(best_activation_hidden);
	net.set_activation_function_output(best_activation_output);
	net.destroy();
}

std::ofstream & FannManager::write_score(std::ofstream& file)
{
	file << std::string("Mean error: ") << score.mean_error << " max error: " << score.max_error << " min error: "<< score.min_error << std::endl;
	return file;
}


void FannManager::load_train_data(std::string filename)
{
	if (!train_data.read_train_from_file(filename))
		std::cerr << "Failed loading train data set." << std::endl;
}

void FannManager::load_test_data(std::string filename)
{
	if (test_data.read_train_from_file(filename))
		has_test_data = true;
	else
		std::cerr << "Failed loading test data set." << std::endl;
}



void FannManager::save(std::string str)
{
	std::cout << std::endl << "Saving network." << std::endl;

	// Save the network in floating point and fixed point
	net.save(str+".net");
	unsigned int decimal_point = net.save_to_fixed(str+"_fixed.net");
	train_data.save_train_to_fixed(str+"_fixed.data", decimal_point);

}


int print_callback(FANN::neural_net &net, FANN::training_data &train,
	unsigned int max_epochs, unsigned int epochs_between_reports,
	double desired_error, unsigned int epochs, void *user_data)
{
	std::cout << "Epochs     " << std::setw(8) << epochs << ". "
		<< " Bit fail: " << net.get_bit_fail() <<" Current Error: " << std::left << net.get_MSE() << std::right << std::endl;
	return 0;
}

