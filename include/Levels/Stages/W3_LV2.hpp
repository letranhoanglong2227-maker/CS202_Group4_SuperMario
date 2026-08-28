#pragma once

#include "Levels/Managers/LevelManager.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class W3_LV2 : public ConfiguredLevel {
public:
    explicit W3_LV2(const std::vector<PlayerManager*>& players = {});
    void update(float dt) override;
    void render(sf::RenderTarget* target) override;

private:
    void positionBlindFold();

    sf::Texture blindFoldTexture;
    sf::Sprite blindFold;
    bool blindFoldLoaded{false};
};
