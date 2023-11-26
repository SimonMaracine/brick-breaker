#include <iostream>

#include "engine/engine.hpp"

int main() {
    bb::ApplicationProperties properties;

    try {
        bb::Application application {properties};
        application.run();
    } catch (bb::RuntimeError error) {
        std::cout << "An error occurred: " << error << '\n';
        return 1;
    }

    return 0;
}
