#include "States/Menus/MenuLevelState.hpp"

#include "Core/AssetLocator.hpp"
#include "Core/UserData.hpp"
#include "Entities/EntityFactory.hpp"
#include "Levels/LevelFactory.hpp"
#include "States/Base/GameState.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <memory>
#include <vector>

namespace {
std::vector<GUI::MenuItem> levelItems(const UserData* data, int worldId) {
    (void)data;
    (void)worldId;
    return {
        {"LEVEL 1", {{160.f, 360.f}, {280.f, 90.f}}, true},
        {"LEVEL 2", {{500.f, 360.f}, {280.f, 90.f}}, true},
        {"LEVEL 3", {{840.f, 360.f}, {280.f, 90.f}}, true},
    };
}

std::optional<sf::Vector2f> pointerPosition(
    const sf::Event& event, sf::RenderWindow* window) {
    if (!window) return std::nullopt;
    if (const auto* moved = event.getIf<sf::Event::MouseMoved>())
        return window->mapPixelToCoords(moved->position);
    if (const auto* clicked = event.getIf<sf::Event::MouseButtonPressed>())
        return window->mapPixelToCoords(clicked->position);
    return std::nullopt;
}

}

MenuLevelState::MenuLevelState(
    StateStack& stack, StateContext context, int worldId)
    : State(stack, context), m_worldId(worldId),
      m_menu(levelItems(context.userData, worldId)) {
    if (const auto path = AssetLocator::find(
            "assets/textures/selectionBackground2.png"))
        m_backgroundLoaded = m_backgroundTexture.loadFromFile(*path);
    if (const auto path = AssetLocator::find(
            "assets/fonts/Super-Mario-Bros--3.ttf"))
        m_fontLoaded = m_font.openFromFile(*path);
    if (const auto path = AssetLocator::find(
            "assets/fonts/American Captain.ttf"))
        m_titleFontLoaded = m_titleFont.openFromFile(*path);
}

void MenuLevelState::handleEvent(const sf::Event& event) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>();
        key && key->code == sf::Keyboard::Key::Escape) {
        (void)requestPop();
        return;
    }
    if (const auto selected = m_menu.handleEvent(
            event, pointerPosition(event, context().window)))
        launch(*selected);
}

void MenuLevelState::update(float) {}

void MenuLevelState::render(sf::RenderTarget& target) {
    const auto view = target.getView();
    const auto topLeft = view.getCenter() - view.getSize() / 2.f;
    if (m_backgroundLoaded) {
        sf::Sprite background(m_backgroundTexture);
        const auto size = m_backgroundTexture.getSize();
        background.setPosition(topLeft);
        background.setScale({view.getSize().x / static_cast<float>(size.x),
                             view.getSize().y / static_cast<float>(size.y)});
        target.draw(background);
    } else {
        sf::RectangleShape background(view.getSize());
        background.setPosition(topLeft);
        background.setFillColor(sf::Color(40, 105, 160));
        target.draw(background);
    }
    if (m_fontLoaded) {
        const sf::Font& titleFont = m_titleFontLoaded ? m_titleFont : m_font;
        sf::Text title(titleFont,
            "WORLD " + std::to_string(m_worldId) + " - CHOOSE A LEVEL", 38);
        const auto bounds = title.getLocalBounds();
        title.setOrigin(bounds.position + bounds.size / 2.f);
        title.setPosition({640.f, 190.f});
        title.setFillColor(sf::Color(255, 220, 70));
        title.setOutlineColor(sf::Color::Black);
        title.setOutlineThickness(3.f);
        target.draw(title);
        m_menu.render(target, m_font, 27);
        sf::Text modeHint(
            titleFont,
            "CHOOSE ANY LEVEL - EVERY SCORE COUNTS",
            21);
        const auto hintBounds = modeHint.getLocalBounds();
        modeHint.setOrigin(hintBounds.position + hintBounds.size / 2.f);
        modeHint.setPosition({640.f, 520.f});
        modeHint.setFillColor(sf::Color::White);
        modeHint.setOutlineColor(sf::Color::Black);
        modeHint.setOutlineThickness(2.f);
        target.draw(modeHint);
        if (!m_error.empty()) {
            sf::Text error(m_font, m_error, 18);
            error.setFillColor(sf::Color(255, 110, 90));
            error.setPosition({150.f, 540.f});
            target.draw(error);
        }
    }
}

void MenuLevelState::launch(std::size_t selected) {
    const int levelId = static_cast<int>(selected) + 1;
    const bool luigi = context().userData &&
                       context().userData->getCharacter() == 1;
    auto player = EntityFactory::createPlayer(luigi ? "Luigi" : "Mario");
    if (!player) {
        m_error = "Could not create the selected character";
        return;
    }
    auto level = createConfiguredLevel(m_worldId, levelId, {player.get()});
    if (!level) {
        m_error = "Invalid world/level selection";
        return;
    }
    auto game = std::make_unique<GameState>(
        stateStack(), context(), m_worldId, levelId,
        std::move(player), std::move(level));
    if (!game->isReady()) {
        m_error = game->loadError();
        return;
    }
    if (context().userData)
        context().userData->setCurrentLevel(m_worldId, levelId);
    m_error.clear();
    (void)requestReplace(std::move(game));
}
