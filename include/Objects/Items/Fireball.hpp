#pragma once

#include "Entities/Base/LivingEntity.hpp"
#include "Components/MovementComponent.hpp"
#include <memory>

class Fireball : public LivingEntity {
private:
    float dirX{ 1.f };
    float lifeTimer{ 0.f };
    float maxLife{ 3.f };

public:
    Fireball(const sf::Vector2f& pos, float directionX);
    ~Fireball() override = default;

    void reactToCollision();
    void reactToBlockCollision();
    
    bool isExpired() const;
    bool canBeRemoved() const;

    void update(float dt) override;
    void render(sf::RenderTarget* target) override;
};
