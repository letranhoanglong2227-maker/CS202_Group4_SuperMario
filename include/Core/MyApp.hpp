#pragma once

#include "Audio/AudioSystem.hpp"
#include "Core/UserData.hpp"
#include "States/Base/State.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

class MyApp {
public:
    MyApp();
    void run();

    [[nodiscard]] static sf::Vector2u fitWindowToDesktop(
        sf::Vector2u desktopSize) noexcept;
    [[nodiscard]] static sf::View buildLetterboxedView(
        sf::Vector2u framebufferSize);

private:
    void processEvents();
    void update(float deltaSeconds);
    void render();

    sf::RenderWindow window;
    UserData userData;
    AudioSystem& audio;
    StateStack states;
};
