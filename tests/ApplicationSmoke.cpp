#include "States/Base/State.hpp"
#include "Core/AssetLocator.hpp"
#include "Core/UserData.hpp"
#include "Entities/EntityFactory.hpp"
#include "Levels/LevelFactory.hpp"
#include "Objects/Environment/WinFlag.hpp"
#include "States/Base/GameState.hpp"
#include "States/Menus/MainMenuState.hpp"
#include "States/Menus/DeathMenuState.hpp"
#include "States/Menus/LeaderboardState.hpp"
#include "States/Menus/MenuCharacterSelectionState.hpp"
#include "States/Menus/MenuWorldState.hpp"
#include "States/Menus/PauseMenuState.hpp"
#include "States/Menus/WinMenuState.hpp"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <algorithm>
#include <cassert>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

namespace {
sf::Image savePreview(sf::RenderTexture& target, StateStack& states,
                      const std::filesystem::path& path) {
    target.clear(sf::Color(26, 35, 60));
    states.render(target);
    target.display();

    const sf::Image preview = target.getTexture().copyToImage();
    assert(preview.getSize() == sf::Vector2u(1560u, 960u));
    assert(preview.saveToFile(path));
    return preview;
}

void sendKey(StateStack& states, sf::Keyboard::Key key) {
    states.handleEvent(sf::Event(sf::Event::KeyPressed{key}));
}
}

int main(int argc, char** argv) {
    const std::filesystem::path outputDirectory = argc > 1 ? argv[1] : "build-p1-pre";

    sf::RenderTexture target({1560u, 960u});

    const auto blindFoldPath = AssetLocator::find(
        "assets/textures/BlindFold.png");
    sf::Image blindFoldImage;
    assert(blindFoldPath && blindFoldImage.loadFromFile(*blindFoldPath));
    sf::Vector2u transparentMin = blindFoldImage.getSize();
    sf::Vector2u transparentMax{};
    bool hasTransparentPixel = false;
    for (unsigned int y = 0; y < blindFoldImage.getSize().y; ++y) {
        for (unsigned int x = 0; x < blindFoldImage.getSize().x; ++x) {
            if (blindFoldImage.getPixel({x, y}).a == 255) continue;
            hasTransparentPixel = true;
            transparentMin.x = std::min(transparentMin.x, x);
            transparentMin.y = std::min(transparentMin.y, y);
            transparentMax.x = std::max(transparentMax.x, x);
            transparentMax.y = std::max(transparentMax.y, y);
        }
    }
    assert(hasTransparentPixel);
    std::cout << "BlindFold aperture: " << transparentMin.x << ','
              << transparentMin.y << " to " << transparentMax.x << ','
              << transparentMax.y << " alpha "
              << static_cast<int>(blindFoldImage.getPixel(transparentMin).a)
              << '\n';
    sf::View view = target.getView();
    view.setCenter({640.f, 360.f});
    target.setView(view);
    StateStack states(StateContext{nullptr});
    states.pushInitial(std::make_unique<MainMenuState>(states, states.context()));

    savePreview(target, states, outputDirectory / "P1MainMenuPreview.png");

    sendKey(states, sf::Keyboard::Key::Down);
    sendKey(states, sf::Keyboard::Key::Enter);
    assert(states.size() == 2);
    savePreview(target, states, outputDirectory / "P1EnterNamePreview.png");

    sendKey(states, sf::Keyboard::Key::Escape);
    sendKey(states, sf::Keyboard::Key::Down);
    sendKey(states, sf::Keyboard::Key::Down);
    sendKey(states, sf::Keyboard::Key::Enter);
    assert(states.size() == 2);
    savePreview(target, states, outputDirectory / "P1SettingsPreview.png");

    UserData progression("Font Smoke");
    StateStack characterStates(StateContext{nullptr, &progression});
    characterStates.pushInitial(
        std::make_unique<MenuCharacterSelectionState>(
            characterStates, characterStates.context()));
    savePreview(target, characterStates,
                outputDirectory / "P1CharacterSelectionPreview.png");

    StateStack pauseStates(StateContext{nullptr, &progression});
    pauseStates.pushInitial(std::make_unique<PauseMenuState>(
        pauseStates, pauseStates.context(), [] { return true; }));
    savePreview(target, pauseStates,
                outputDirectory / "P1PauseMenuPreview.png");

    StateStack deathStates(StateContext{nullptr, &progression});
    deathStates.pushInitial(std::make_unique<DeathMenuState>(
        deathStates, deathStates.context(), [] { return true; }));
    savePreview(target, deathStates,
                outputDirectory / "P1DeathMenuPreview.png");

    StateStack leaderboardStates(StateContext{nullptr, &progression});
    leaderboardStates.pushInitial(std::make_unique<LeaderboardState>(
        leaderboardStates, leaderboardStates.context()));
    savePreview(target, leaderboardStates,
                outputDirectory / "P1LeaderboardPreview.png");

    StateStack worldStates(StateContext{nullptr, &progression});
    worldStates.pushInitial(std::make_unique<MenuWorldState>(
        worldStates, worldStates.context()));
    savePreview(target, worldStates,
                outputDirectory / "P1WorldMenuPreview.png");
    sendKey(worldStates, sf::Keyboard::Key::Enter);
    assert(worldStates.size() == 1);
    savePreview(target, worldStates,
                outputDirectory / "P1LevelMenuPreview.png");

    StateStack winStates(StateContext{nullptr, &progression});
    winStates.pushInitial(std::make_unique<WinMenuState>(
        winStates, winStates.context(), 1, 1));
    savePreview(target, winStates,
                outputDirectory / "P1WinMenuPreview.png");

    StateStack qaSelection(StateContext{nullptr, &progression});
    qaSelection.pushInitial(std::make_unique<MenuWorldState>(
        qaSelection, qaSelection.context()));
    sendKey(qaSelection, sf::Keyboard::Key::Down);
    sendKey(qaSelection, sf::Keyboard::Key::Down);
    sendKey(qaSelection, sf::Keyboard::Key::Enter); // World 3
    sendKey(qaSelection, sf::Keyboard::Key::Down);
    sendKey(qaSelection, sf::Keyboard::Key::Down);
    sendKey(qaSelection, sf::Keyboard::Key::Enter); // Level 3
    assert(progression.getCurrentWorld() == 3 &&
           progression.getCurrentLevel() == 3);
    savePreview(target, qaSelection,
                outputDirectory / "P1QAW3LV3Preview.png");
    sendKey(qaSelection, sf::Keyboard::Key::Escape);
    assert(qaSelection.size() == 2); // GameState opened its PauseMenu.

    for (int world = 1; world <= 3; ++world) {
        for (int stage = 1; stage <= 3; ++stage) {
            auto player = EntityFactory::createPlayer("Mario");
            assert(player);
            auto level = createConfiguredLevel(world, stage, {player.get()});
            assert(level && level->isLoaded());
            StateStack gameStates(StateContext{nullptr, &progression});
            auto game = std::make_unique<GameState>(
                gameStates, gameStates.context(), world, stage,
                std::move(player), std::move(level));
            GameState* gameView = game.get();
            gameStates.pushInitial(std::move(game));
            gameStates.update(0.016f);
            const sf::Image preview = savePreview(
                target, gameStates,
                outputDirectory /
                    ("P1GameplayW" + std::to_string(world) + "LV" +
                     std::to_string(stage) + "Preview.png"));
            if (world == 3 && stage == 2) {
                bool marioVisibleThroughBlindFold = false;
                for (unsigned int y = 480; y < 600; ++y) {
                    for (unsigned int x = 280; x < 420; ++x) {
                        const sf::Color pixel = preview.getPixel({x, y});
                        if (pixel.r > 150 && pixel.g < 100 && pixel.b < 100) {
                            marioVisibleThroughBlindFold = true;
                        }
                    }
                }
                assert(marioVisibleThroughBlindFold);
            }
            if (world == 1 && stage == 1) {
                gameView->activePlayer().setImmortal(true, 10.f);
                gameStates.update(2.01f);
                savePreview(
                    target, gameStates,
                    outputDirectory / "P1GameplayW1LV1PeteyFirePreview.png");
            }

            if (world != 3 || stage != 3) {
                WinFlag* flag = nullptr;
                for (const auto& entity : gameView->activeLevel().getEntities()) {
                    if (auto* candidate = dynamic_cast<WinFlag*>(entity.get())) {
                        flag = candidate;
                        break;
                    }
                }
                assert(flag);
                gameView->activePlayer().setImmortal(true, 10.f);
                gameView->activePlayer().setPosition(
                    flag->hitbox.getPosition());
                gameStates.update(0.01f);
                assert(flag->isActivated());
                gameStates.update(0.5f);
                savePreview(
                    target, gameStates,
                    outputDirectory /
                        ("P1GoalW" + std::to_string(world) + "LV" +
                         std::to_string(stage) + "Preview.png"));
            }
        }
    }

    {
        auto luigi = EntityFactory::createPlayer("Luigi");
        assert(luigi);
        auto level = createConfiguredLevel(3, 3, {luigi.get()});
        assert(level && level->isLoaded());
        StateStack luigiStates(StateContext{nullptr, &progression});
        luigiStates.pushInitial(std::make_unique<GameState>(
            luigiStates, luigiStates.context(), 3, 3,
            std::move(luigi), std::move(level)));
        luigiStates.update(0.016f);
        savePreview(target, luigiStates,
                    outputDirectory / "P1GameplayW3LV3LuigiPreview.png");
    }

    {
        auto powered = EntityFactory::createPlayer("Mario");
        assert(powered && powered->setBig(true));
        auto level = createConfiguredLevel(3, 3, {powered.get()});
        assert(level && level->isLoaded());
        StateStack poweredStates(StateContext{nullptr, &progression});
        poweredStates.pushInitial(std::make_unique<GameState>(
            poweredStates, poweredStates.context(), 3, 3,
            std::move(powered), std::move(level)));
        poweredStates.update(0.016f);
        savePreview(target, poweredStates,
                    outputDirectory / "P1GameplayW3LV3PoweredPreview.png");
    }

    std::cout << "Application smoke passed: " << outputDirectory.string() << '\n';
    return 0;
}
