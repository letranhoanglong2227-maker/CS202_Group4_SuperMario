#include "States/Base/State.hpp"
#include "States/Menus/MainMenuState.hpp"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <cassert>
#include <filesystem>
#include <iostream>
#include <memory>

namespace {
void savePreview(sf::RenderTexture& target, StateStack& states, const std::filesystem::path& path) {
    target.clear(sf::Color(26, 35, 60));
    states.render(target);
    target.display();

    const sf::Image preview = target.getTexture().copyToImage();
    assert(preview.getSize() == sf::Vector2u(1280u, 720u));
    assert(preview.saveToFile(path));
}

void sendKey(StateStack& states, sf::Keyboard::Key key) {
    states.handleEvent(sf::Event(sf::Event::KeyPressed{key}));
}
}

int main(int argc, char** argv) {
    const std::filesystem::path outputDirectory = argc > 1 ? argv[1] : "build-p1-pre";

    sf::RenderTexture target({1280u, 720u});
    StateStack states(StateContext{nullptr});
    states.pushInitial(std::make_unique<MainMenuState>(states, states.context()));

    savePreview(target, states, outputDirectory / "P1MainMenuPreview.png");

    sendKey(states, sf::Keyboard::Key::Enter);
    assert(states.size() == 2);
    savePreview(target, states, outputDirectory / "P1EnterNamePreview.png");

    sendKey(states, sf::Keyboard::Key::Escape);
    sendKey(states, sf::Keyboard::Key::Down);
    sendKey(states, sf::Keyboard::Key::Enter);
    assert(states.size() == 2);
    savePreview(target, states, outputDirectory / "P1SettingsPreview.png");

    std::cout << "Person 1 application smoke passed: " << outputDirectory.string() << '\n';
    return 0;
}
