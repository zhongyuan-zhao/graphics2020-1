#include "Application.h"

int main(int argc, char** argv)
{
	Horizon::Application app;
	try
	{
		app.init();
		app.run();
		app.shutdown();
	}
	catch (const std::runtime_error& error)
	{
		std::cerr << error.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}