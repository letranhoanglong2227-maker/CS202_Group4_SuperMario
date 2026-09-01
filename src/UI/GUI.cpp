#include "UI/GUI.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <utility>

namespace GUI {

SelectionMenu::SelectionMenu(std::vector<MenuItem> items)
    : m_items(std::move(items)) {
    if (!m_items.empty() && !m_items.front().enabled) {
        moveSelection(1);
    }
}

std::optional<std::size_t> SelectionMenu::handleEvent(
    const sf::Event& event, std::optional<sf::Vector2f> pointerPosition) {
    if (!m_focused || m_items.empty()) {
        return std::nullopt;
    }

    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        switch (key->code) {
        case sf::Keyboard::Key::Up:
        case sf::Keyboard::Key::Left:
            moveSelection(-1);
            break;
        case sf::Keyboard::Key::Down:
        case sf::Keyboard::Key::Right:
            moveSelection(1);
            break;
        case sf::Keyboard::Key::Enter:
        case sf::Keyboard::Key::Space:
            if (m_items[m_selected].enabled) {
                return m_selected;
            }
            break;
        default:
            break;
        }
        return std::nullopt;
    }

    if (event.is<sf::Event::MouseMoved>() && pointerPosition) {
        if (const auto hovered = itemAt(*pointerPosition)) {
            m_selected = *hovered;
        }
        return std::nullopt;
    }

    if (const auto* click = event.getIf<sf::Event::MouseButtonPressed>();
        click && click->button == sf::Mouse::Button::Left && pointerPosition) {
        if (const auto selected = itemAt(*pointerPosition)) {
            m_selected = *selected;
            return selected;
        }
    }
    return std::nullopt;
}

void SelectionMenu::render(sf::RenderTarget& target, const sf::Font& font,
                           unsigned characterSize) const {
    for (std::size_t index = 0; index < m_items.size(); ++index) {
        const MenuItem& item = m_items[index];
        sf::RectangleShape button(item.bounds.size);
        button.setPosition(item.bounds.position);
        button.setFillColor(!item.enabled
            ? sf::Color(65, 70, 80, 190)
            : index == m_selected && m_focused
                ? sf::Color(190, 52, 46, 238)
                : sf::Color(22, 36, 58, 225));
        button.setOutlineColor(index == m_selected && m_focused
            ? sf::Color(255, 226, 92)
            : sf::Color(225, 235, 245));
        button.setOutlineThickness(index == m_selected && m_focused ? 3.f : 1.f);
        target.draw(button);

        sf::Text text(font, item.label, characterSize);
        text.setFillColor(item.enabled ? sf::Color::White
                                       : sf::Color(150, 155, 165));
        const sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.position + textBounds.size / 2.f);
        text.setPosition(item.bounds.position + item.bounds.size / 2.f);
        target.draw(text);
    }
}

void SelectionMenu::setFocused(bool focused) noexcept {
    m_focused = focused;
}

bool SelectionMenu::isFocused() const noexcept {
    return m_focused;
}

bool SelectionMenu::setSelected(std::size_t index) noexcept {
    if (index >= m_items.size() || !m_items[index].enabled) {
        return false;
    }
    m_selected = index;
    return true;
}

std::size_t SelectionMenu::selected() const noexcept {
    return m_selected;
}

const std::vector<MenuItem>& SelectionMenu::items() const noexcept {
    return m_items;
}

void SelectionMenu::moveSelection(int direction) noexcept {
    if (m_items.empty() || direction == 0) {
        return;
    }
    const auto count = static_cast<long long>(m_items.size());
    for (std::size_t offset = 1; offset <= m_items.size(); ++offset) {
        const auto signedIndex = static_cast<long long>(m_selected) +
            static_cast<long long>(direction) * static_cast<long long>(offset);
        const std::size_t candidate = static_cast<std::size_t>(
            (signedIndex % count + count) % count);
        if (m_items[candidate].enabled) {
            m_selected = candidate;
            return;
        }
    }
}

std::optional<std::size_t> SelectionMenu::itemAt(
    sf::Vector2f position) const noexcept {
    for (std::size_t index = 0; index < m_items.size(); ++index) {
        if (m_items[index].enabled && m_items[index].bounds.contains(position)) {
            return index;
        }
    }
    return std::nullopt;
}

} // namespace GUI
