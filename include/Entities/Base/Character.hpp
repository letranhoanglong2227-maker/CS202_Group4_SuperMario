#pragma once

#include "Entities/Base/LivingEntity.hpp"
#include <string>

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
