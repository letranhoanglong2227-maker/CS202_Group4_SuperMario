#include "UI/GUI.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <cassert>
#include <iostream>

int main() {
    GUI::SelectionMenu menu({
        {"PLAY", {{0.f, 0.f}, {100.f, 40.f}}, true},
        {"LOCKED", {{0.f, 50.f}, {100.f, 40.f}}, false},
        {"BACK", {{0.f, 100.f}, {100.f, 40.f}}, true},
    });

    assert(menu.selected() == 0);
    assert(!menu.handleEvent(sf::Event(
        sf::Event::KeyPressed{sf::Keyboard::Key::Down})));
    assert(menu.selected() == 2);

    const auto keyboardAction = menu.handleEvent(sf::Event(
        sf::Event::KeyPressed{sf::Keyboard::Key::Enter}));
    assert(keyboardAction && *keyboardAction == 2);

    assert(!menu.handleEvent(sf::Event(sf::Event::MouseMoved{{25, 20}}),
                             sf::Vector2f{25.f, 20.f}));
    assert(menu.selected() == 0);
    const auto mouseAction = menu.handleEvent(
        sf::Event(sf::Event::MouseButtonPressed{
            sf::Mouse::Button::Left, {25, 20}}),
        sf::Vector2f{25.f, 20.f});
    assert(mouseAction && *mouseAction == 0);

    menu.setFocused(false);
    assert(!menu.handleEvent(sf::Event(
        sf::Event::KeyPressed{sf::Keyboard::Key::Enter})));
    assert(!menu.setSelected(1));
    assert(menu.setSelected(2));

    GUI::SelectionMenu empty;
    assert(!empty.handleEvent(sf::Event(
        sf::Event::KeyPressed{sf::Keyboard::Key::Enter})));

    std::cout << "Person 4 GUI contracts passed\n";
    return 0;
}
