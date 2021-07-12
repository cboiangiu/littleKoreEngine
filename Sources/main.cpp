#include <Kore/System.h>
#include <Kore/Math/Matrix.h>
#include <Kore/Math/Vector.h>

int kickstart(int argc, char** argv) {
	Kore::System::init("Kore window", 800, 600);

    Kore::mat4 matrix;
    Kore::vec4 vec;
    
    auto test = matrix * vec;
    
	Kore::System::start();

	return 0;
}
