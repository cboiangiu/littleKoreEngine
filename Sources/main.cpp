#include <Kore/global.h>

#include "first_app.hpp"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int kickstart(int argc, char** argv) {
    try {
        lke::FirstApp::instance()->run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

	return EXIT_SUCCESS;
}
