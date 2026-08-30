#include "Core/MyApp.hpp"

#include "States/Menus/MainMenuState.hpp"

#include <SFML/Graphics/View.hpp>
#include <SFML/System/Clock.hpp>

#include <algorithm>
#include <memory>
#include <optional>

namespace {
constexpr sf::Vector2u WindowSize{1280u, 720u};
constexpr float MaximumDeltaSeconds = 0.1f;
}

MyApp::MyApp()
    : window(sf::VideoMode(WindowSize), "Super Mario - Group 4"),
      states(StateContext{&window}) {
    window.setVerticalSyncEnabled(true);
    window.setView(sf::View(sf::FloatRect({0.f, 0.f}, sf::Vector2f(WindowSize))));
    states.pushInitial(std::make_unique<MainMenuState>(states, states.context()));
}

void MyApp::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        processEvents();
        if (!window.isOpen()) {
            break;
        }

        update(std::min(clock.restart().asSeconds(), MaximumDeltaSeconds));
        if (states.empty() || states.quitRequested()) {
            window.close();
            break;
        }
        render();
    }
}

void MyApp::processEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            return;
        }
        if (event->is<sf::Event::Resized>()) {
            window.setView(sf::View(sf::FloatRect({0.f, 0.f}, sf::Vector2f(WindowSize))));
        }

        states.handleEvent(*event);
        if (states.empty() || states.quitRequested()) {
            return;
        }
    }
}

void MyApp::update(float deltaSeconds) {
    states.update(deltaSeconds);
}

void MyApp::render() {
    window.clear(sf::Color(26, 35, 60));
    states.render(window);
    window.display();
}
