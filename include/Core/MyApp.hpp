#pragma once

#include "States/Base/State.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

class MyApp {
public:
    MyApp();
    void run();

private:
    void processEvents();
    void update(float deltaSeconds);
    void render();

    sf::RenderWindow window;
    StateStack states;
};
