#include "Tutorial_fixedTimeStepApp.h"

int main() {
	
	// allocation
	auto app = new Tutorial_fixedTimeStepApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}