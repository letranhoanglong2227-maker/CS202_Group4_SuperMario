#pragma once

#include "Core/AssetLocator.hpp"
#include "Entities/Base/LivingEntity.hpp"
#include <string>
#include <iostream>

class Character : public LivingEntity {
protected:
    std::string characterName;
    bool jumping{ false };
    bool grounded{ false };
    float jumpForce{ -500.f };

public:
    Character(const std::string& name = "Character");
    virtual ~Character() = default;

    const std::string& getCharacterName() const;
    void setCharacterName(const std::string& name);

    bool isJumping() const;
    void setJumping(bool j);

    bool isGrounded() const;
    void setGrounded(bool g);

    virtual void jump();
};

class TexturePlayerManager {
private:
    inline static sf::Texture playerTexture;
public:
    static bool setupTexture() {
        const auto path = AssetLocator::find("assets/textures/Mario_Luigi.png");
        const bool loadOk = path && playerTexture.loadFromFile(*path);
        if (!loadOk)
            std::cerr << AssetLocator::missingMessage(
                             "assets/textures/Mario_Luigi.png")
                      << '\n';
        return loadOk;
    }

    static sf::Texture& getPlayerTexture() {
        return playerTexture;
    }
};
