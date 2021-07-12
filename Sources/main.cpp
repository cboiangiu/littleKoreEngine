#include <Kore/global.h>

#include "first_app.hpp"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int kickstart(int argc, char** argv) {
    lke::FirstApp app{};
    
    try {
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

	return EXIT_SUCCESS;
}
