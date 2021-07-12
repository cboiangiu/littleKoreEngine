#include <Kore/System.h>

#include "first_app.hpp"

namespace lke{
void FirstApp::run() {
    Kore::System::setCallback(update);
    // set more callbacks for key events here
    Kore::System::start();
}

void FirstApp::update() {
    // code that runs on each tick
}
}
