#pragma once

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
        bool loadOk = playerTexture.loadFromFile("assets/textures/Mario_Luigi.png");
        if (!loadOk) {
            loadOk = playerTexture.loadFromFile("../assets/textures/Mario_Luigi.png");
        }
        if (loadOk) {
            std::cout << "Player Texture loaded!...\n";
        } else {
            std::cout << "Player Texture not found!...\n";
        }
        return loadOk;
    }

    static sf::Texture& getPlayerTexture() {
        return playerTexture;
    }
};
