#include "Core/MyApp.hpp"

#include "States/Menus/MainMenuState.hpp"

#include <SFML/Graphics/View.hpp>
#include <SFML/System/Clock.hpp>

#include <algorithm>
#include <cstdint>
#include <memory>
#include <optional>

namespace {
constexpr sf::Vector2u LogicalViewSize{1560u, 960u};
constexpr sf::Vector2f LegacyUiCenter{640.f, 360.f};
constexpr std::uint64_t DesktopUsagePercent = 90u;
constexpr float MaximumDeltaSeconds = 0.1f;
}

sf::Vector2u MyApp::fitWindowToDesktop(sf::Vector2u desktopSize) noexcept {
    if (desktopSize.x == 0u || desktopSize.y == 0u) return LogicalViewSize;

    const auto maximumWidth = static_cast<unsigned int>(std::max<std::uint64_t>(
        1u, static_cast<std::uint64_t>(desktopSize.x) * DesktopUsagePercent / 100u));
    const auto maximumHeight = static_cast<unsigned int>(std::max<std::uint64_t>(
        1u, static_cast<std::uint64_t>(desktopSize.y) * DesktopUsagePercent / 100u));
    if (LogicalViewSize.x <= maximumWidth &&
        LogicalViewSize.y <= maximumHeight) {
        return LogicalViewSize;
    }

    const auto heightFromWidth = static_cast<unsigned int>(
        static_cast<std::uint64_t>(maximumWidth) * LogicalViewSize.y /
        LogicalViewSize.x);
    if (heightFromWidth <= maximumHeight) {
        return {maximumWidth, std::max(1u, heightFromWidth)};
    }

    const auto widthFromHeight = static_cast<unsigned int>(
        static_cast<std::uint64_t>(maximumHeight) * LogicalViewSize.x /
        LogicalViewSize.y);
    return {std::max(1u, widthFromHeight), maximumHeight};
}

sf::View MyApp::buildLetterboxedView(sf::Vector2u framebufferSize) {
    sf::View view(sf::FloatRect({0.f, 0.f}, sf::Vector2f(LogicalViewSize)));
    view.setCenter(LegacyUiCenter);
    if (framebufferSize.x == 0u || framebufferSize.y == 0u) return view;

    const float viewRatio = static_cast<float>(LogicalViewSize.x) /
                            static_cast<float>(LogicalViewSize.y);
    const float framebufferRatio = static_cast<float>(framebufferSize.x) /
                                   static_cast<float>(framebufferSize.y);
    sf::FloatRect viewport({0.f, 0.f}, {1.f, 1.f});
    if (framebufferRatio > viewRatio) {
        viewport.size.x = viewRatio / framebufferRatio;
        viewport.position.x = (1.f - viewport.size.x) / 2.f;
    } else if (framebufferRatio < viewRatio) {
        viewport.size.y = framebufferRatio / viewRatio;
        viewport.position.y = (1.f - viewport.size.y) / 2.f;
    }
    view.setViewport(viewport);
    return view;
}

MyApp::MyApp()
    : window(sf::VideoMode(fitWindowToDesktop(
          sf::VideoMode::getDesktopMode().size)), "Super Mario - Group 4"),
      audio(AudioSystem::instance()),
      states(StateContext{&window, &userData, &audio}) {
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
    window.setView(buildLetterboxedView(window.getSize()));
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
        if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            window.setView(buildLetterboxedView(resized->size));
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
