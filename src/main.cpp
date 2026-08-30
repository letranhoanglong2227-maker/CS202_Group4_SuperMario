#include "Core/MyApp.hpp"

#include <exception>
#include <iostream>

int main() {
    try {
        MyApp app;
        app.run();
    } catch (const std::exception& error) {
        std::cerr << "Super Mario failed to start: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
