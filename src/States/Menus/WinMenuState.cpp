#include "States/Menus/WinMenuState.hpp"

#include "Core/AssetLocator.hpp"
#include "Core/UserData.hpp"
#include "Entities/EntityFactory.hpp"
#include "Levels/LevelFactory.hpp"
#include "States/Base/GameState.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>

WinMenuState::WinMenuState(
    StateStack& stack, StateContext context, int worldId, int levelId)
    : State(stack, context), m_worldId(worldId), m_levelId(levelId) {
    if (const auto path = AssetLocator::find(
            "assets/fonts/Super-Mario-Bros--3.ttf"))
        m_fontLoaded = m_font.openFromFile(*path);
    if (const auto path = AssetLocator::find(
            "assets/fonts/American Captain.ttf"))
        m_readableFontLoaded = m_readableFont.openFromFile(*path);
}

void WinMenuState::handleEvent(const sf::Event& event) {
    const auto* key = event.getIf<sf::Event::KeyPressed>();
    if (!key) return;
    if (key->code == sf::Keyboard::Key::Escape) {
        (void)requestPopToRoot();
        return;
    }
    if (key->code != sf::Keyboard::Key::Enter &&
        key->code != sf::Keyboard::Key::Space) {
        return;
    }
    if (m_worldId == 3 && m_levelId == 3) {
        (void)requestPopToRoot();
        return;
    }
    continueToNextStage();
}

void WinMenuState::update(float) {}

void WinMenuState::render(sf::RenderTarget& target) {
    const auto view = target.getView();
    sf::RectangleShape background(view.getSize());
    background.setPosition(view.getCenter() - view.getSize() / 2.f);
    background.setFillColor(sf::Color(8, 30, 24));
    target.draw(background);
    if (!m_fontLoaded) return;
    sf::Text title(m_font,
                   m_worldId == 3 && m_levelId == 3
                       ? "GAME COMPLETE"
                       : "YOU WIN!",
                   64);
    auto bounds = title.getLocalBounds();
    title.setOrigin(bounds.position + bounds.size / 2.f);
    title.setPosition({640.f, 230.f});
    title.setFillColor(sf::Color(255, 220, 70));
    target.draw(title);
    const sf::Font& readableFont =
        m_readableFontLoaded ? m_readableFont : m_font;
    sf::Text stage(readableFont,
        "WORLD " + std::to_string(m_worldId) + " - LEVEL " +
            std::to_string(m_levelId), 34);
    bounds = stage.getLocalBounds();
    stage.setOrigin(bounds.position + bounds.size / 2.f);
    stage.setPosition({640.f, 345.f});
    stage.setFillColor(sf::Color::White);
    target.draw(stage);
    std::string hint;
    if (!m_error.empty()) {
        hint = m_error;
    } else if (m_worldId == 3 && m_levelId == 3) {
        hint = "PRESS ENTER TO RETURN TO MENU";
    } else {
        // Keep Group5's short wording: the display font becomes difficult to
        // scan when it is forced into one long stage-description line.
        hint = "PRESS ENTER TO CONTINUE";
    }
    sf::Text hintText(m_error.empty() ? m_font : readableFont, hint,
                      m_error.empty() ? 30u : 24u);
    bounds = hintText.getLocalBounds();
    hintText.setOrigin(bounds.position + bounds.size / 2.f);
    hintText.setPosition({640.f, 480.f});
    hintText.setFillColor(m_error.empty() ? sf::Color(210, 230, 220)
                                          : sf::Color(255, 120, 100));
    target.draw(hintText);
}

void WinMenuState::continueToNextStage() {
    UserData* data = context().userData;
    if (!data) {
        m_error = "PLAYER DATA IS UNAVAILABLE";
        return;
    }

    int nextWorld = m_worldId;
    int nextLevel = m_levelId + 1;
    if (nextLevel > 3) {
        nextLevel = 1;
        ++nextWorld;
    }
    if (nextWorld > 3) {
        m_error = "THERE IS NO NEXT LEVEL";
        return;
    }
    auto player = EntityFactory::createPlayer(
        data->getCharacter() == 1 ? "Luigi" : "Mario");
    if (!player) {
        m_error = "PLAYER COULD NOT BE CREATED";
        return;
    }
    auto level = createConfiguredLevel(nextWorld, nextLevel, {player.get()});
    if (!level || !level->isLoaded() || !level->getWorldBounds()) {
        m_error = "NEXT LEVEL COULD NOT BE LOADED";
        return;
    }
    if (data->getPlayerForm() == 2) player->setFire(true);
    else if (data->getPlayerForm() == 1) player->setBig(true);

    auto game = std::make_unique<GameState>(
        stateStack(), context(), nextWorld, nextLevel,
        std::move(player), std::move(level));
    if (!game->isReady()) {
        m_error = game->loadError();
        return;
    }
    data->setCurrentLevel(nextWorld, nextLevel);
    m_error.clear();
    (void)requestReplace(std::move(game));
}
