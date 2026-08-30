#include "Entities/Enemies/FlyingKoopa.hpp"
#include <cmath>

FlyingKoopa::FlyingKoopa(const sf::Vector2f& pos)
    : Koopa(pos), initialY(pos.y) {
    setPointsValue(300);
    if (animationComponent) {
        animationComponent->addAnimation("fly", { sf::IntRect({52, 37}, {16, 24}), sf::IntRect({69, 38}, {16, 23}) }); // Use walk frames for now, or specific if exists
    }
}

bool FlyingKoopa::isFlying() const {
    return hasWings;
}

EnemyContactOutcome FlyingKoopa::handlePlayerContact(PlayerManager& player, int collisionSide, float horizontalDirection) {
    if (dead) return EnemyContactOutcome{EnemyContactResult::None, 0, 0.f, false};
    
    if (hasWings) {
        bool isTop = (collisionSide == 1 || collisionSide == 0);
        if (isTop) {
            onStomped();
            return EnemyContactOutcome{EnemyContactResult::EnemyStomped, 200, -500.f, false};
        } else {
            player.takeDamage(damage);
            return EnemyContactOutcome{player.isDead() ? EnemyContactResult::PlayerKilled : EnemyContactResult::PlayerDamaged, 0, 0.f, false};
        }
    }
    
    return Koopa::handlePlayerContact(player, collisionSide, horizontalDirection);
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
