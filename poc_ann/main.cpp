
#include "floatfann.h"
#include "fann_cpp.h"
#include <iostream>
#include <iomanip>

int print_callback(FANN::neural_net &net, FANN::training_data &train,
	unsigned int max_epochs, unsigned int epochs_between_reports,
	float desired_error, unsigned int epochs, void *user_data)
{
	std::cout << "Epochs     " << std::setw(8) << epochs << ". "
		<< "Current Error: " << std::left << net.get_MSE() << std::right << std::endl;
	return 0;
}

void xor_test()
{
	std::cout << std::endl << "XOR test started." << std::endl;

	const float learning_rate = 0.7f;
	const unsigned int num_layers = 3;
	const unsigned int num_input = 2;
	const unsigned int num_hidden = 3;
	const unsigned int num_output = 1;
	const float desired_error = 0.001f;
	const unsigned int max_iterations = 300000;
	const unsigned int iterations_between_reports = 1000;

	std::cout << std::endl << "Creating network." << std::endl;

	FANN::neural_net net;
	net.create_standard(num_layers, num_input, num_hidden, num_output);

	net.set_learning_rate(learning_rate);

	net.set_activation_steepness_hidden(1.0);
	net.set_activation_steepness_output(1.0);

	net.set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC_STEPWISE);
	net.set_activation_function_output(FANN::SIGMOID_SYMMETRIC_STEPWISE);

	// Set additional properties such as the training algorithm
	//net.set_training_algorithm(FANN::TRAIN_QUICKPROP);

	// Output network type and parameters
	std::cout << std::endl << "Network Type                         :  ";
	switch (net.get_network_type())
	{
	case FANN::LAYER:
		std::cout << "LAYER" << std::endl;
		break;
	case FANN::SHORTCUT:
		std::cout << "SHORTCUT" << std::endl;
		break;
	default:
		std::cout << "UNKNOWN" << std::endl;
		break;
	}
	net.print_parameters();

	std::cout << std::endl << "Training network." << std::endl;

	FANN::training_data data;
	if (data.read_train_from_file("xor.data"))
	{
		// Initialize and train the network with the data
		net.init_weights(data);

		std::cout << "Max Epochs " << std::setw(8) << max_iterations << ". "
			<< "Desired Error: " << std::left << desired_error << std::right << std::endl;
		net.set_callback(print_callback, NULL);
		net.train_on_data(data, max_iterations,
			iterations_between_reports, desired_error);

		std::cout << std::endl << "Testing network." << std::endl;

		for (unsigned int i = 0; i < data.length_train_data(); ++i)
		{
			// Run the network on the test data
			fann_type *calc_out = net.run(data.get_input()[i]);

			std::cout << "XOR test (" << std::showpos << data.get_input()[i][0] << ", "
				<< data.get_input()[i][1] << ") -> " << *calc_out
				<< ", should be " << data.get_output()[i][0] << ", "
				<< "difference = " << std::noshowpos
				<< fann_abs(*calc_out - data.get_output()[i][0]) << std::endl;
		}

		std::cout << std::endl << "Saving network." << std::endl;

		// Save the network in floating point and fixed point
		net.save("xor_float.net");
		unsigned int decimal_point = net.save_to_fixed("xor_fixed.net");
		data.save_train_to_fixed("xor_fixed.data", decimal_point);

		std::cout << std::endl << "XOR test completed." << std::endl;
	}
}

int main(int argc, char **argv)
{
	try
	{
		std::ios::sync_with_stdio(); // Syncronize cout and printf output
		xor_test();
	}
	catch (...)
	{
		std::cerr << std::endl << "Abnormal exception." << std::endl;
	}
	return 0;
}