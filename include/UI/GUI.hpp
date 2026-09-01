#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace sf {
class Font;
class RenderTarget;
}

namespace GUI {

struct MenuItem {
    std::string label;
    sf::FloatRect bounds;
    bool enabled{true};
};

// Reports action values only; the owning State decides what each action does.
class SelectionMenu {
public:
    explicit SelectionMenu(std::vector<MenuItem> items = {});

    [[nodiscard]] std::optional<std::size_t> handleEvent(
        const sf::Event& event,
        std::optional<sf::Vector2f> pointerPosition = std::nullopt);
    void render(sf::RenderTarget& target, const sf::Font& font,
                unsigned characterSize = 28) const;

    void setFocused(bool focused) noexcept;
    [[nodiscard]] bool isFocused() const noexcept;
    [[nodiscard]] bool setSelected(std::size_t index) noexcept;
    [[nodiscard]] std::size_t selected() const noexcept;
    [[nodiscard]] const std::vector<MenuItem>& items() const noexcept;

private:
    void moveSelection(int direction) noexcept;
    [[nodiscard]] std::optional<std::size_t> itemAt(
        sf::Vector2f position) const noexcept;

    std::vector<MenuItem> m_items;
    std::size_t m_selected{};
    bool m_focused{true};
};

} // namespace GUI
