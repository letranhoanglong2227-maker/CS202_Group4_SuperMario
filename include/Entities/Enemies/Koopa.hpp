#pragma once

#include "Entities/Base/Enemy.hpp"

class Koopa : public Enemy {
protected:
    bool inShell{ false };
    bool shellKicked{ false };
    float shellSpeed{ 300.f };

public:
    Koopa(const sf::Vector2f& pos = { 0.f, 0.f });
    ~Koopa() override = default;

    bool isInShell() const;
    bool isShellKicked() const;
    void kickShell(bool toRight);

    void onStomped() override;
    void updateAnimation(float dt) override;
    void update(float dt) override;
};
