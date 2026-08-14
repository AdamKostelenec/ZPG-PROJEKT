// main.cpp
#include "Application.h"
#include <cstdio>
#include <cstdlib>

int main() {
    try {
        Application app(800, 600, "ZPG");
        app.initGame();   
        app.run();        // main
    }
    catch (const std::exception& e) {
        std::fprintf(stderr, "Exception: %s\n", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
