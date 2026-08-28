#pragma once

#include "Levels/Managers/LevelManager.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class W3_LV3 : public ConfiguredLevel {
public:
    explicit W3_LV3(const std::vector<PlayerManager*>& players = {});
    void update(float dt) override;
    void render(sf::RenderTarget* target) override;

private:
    void updateMushroom(float dt);
    void positionBlindFold();

    static constexpr float reloadMushroom = 5.f;
    float reloadMushroomTimer{0.f};
    sf::Texture blindFoldTexture;
    sf::Sprite blindFold;
    bool blindFoldLoaded{false};
};
