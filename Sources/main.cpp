#include <Kore/System.h>

int kickstart(int argc, char** argv) {
	Kore::System::init("Kore window", 800, 600);

	Kore::System::start();

	return 0;
}
