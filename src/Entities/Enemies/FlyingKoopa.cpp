#include "Entities/Enemies/FlyingKoopa.hpp"
#include <cmath>

FlyingKoopa::FlyingKoopa(const sf::Vector2f& pos)
    : Koopa(pos), initialY(pos.y) {
    setPointsValue(300);
}

bool FlyingKoopa::isFlying() const {
    return hasWings;
}

void FlyingKoopa::onStomped() {
    if (hasWings) {
        hasWings = false;
        setPointsValue(200);
    } else {
        Koopa::onStomped();
    }
}

void FlyingKoopa::updateAnimation(float dt) {
    (void)dt;
    if (animationComponent) {
        if (hasWings) {
            animationComponent->play("fly", dt);
        } else {
            Koopa::updateAnimation(dt);
        }
    }
}

void FlyingKoopa::update(float dt) {
    if (dead) return;

    if (hasWings) {
        flyTimer += dt * 3.f;
        float offsetY = std::sin(flyTimer) * 40.f;
        position.y = initialY + offsetY;
        float dirX = facingRight ? 1.f : -1.f;
        position.x += dirX * speed * dt;
        hitbox.setPosition(position);
        entitySprite.setPosition(position);
    } else {
        Koopa::update(dt);
    }
}
