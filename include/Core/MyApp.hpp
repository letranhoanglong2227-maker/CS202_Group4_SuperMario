#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

class MyApp {
public:
    MyApp();
    void run();

private:
    void processEvents();
    void update();
    void render();

    sf::RenderWindow window;
};

