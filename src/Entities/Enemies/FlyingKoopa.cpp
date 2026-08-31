#include "Entities/Enemies/FlyingKoopa.hpp"
#include <cmath>

FlyingKoopa::FlyingKoopa(const sf::Vector2f& pos)
    : Koopa(pos), initialX(pos.x), initialY(pos.y) {
    setPointsValue(400);
    setSpeed(300.f);
    setFacingRight(true);
    if (animationComponent) {
        animationComponent->addAnimation("fly", {
            sf::IntRect({86, 37}, {16, 24}),
            sf::IntRect({103, 38}, {16, 23})});
    }
}

bool FlyingKoopa::isFlying() const {
    return hasWings;
}

EnemyContactOutcome FlyingKoopa::handlePlayerContact(PlayerManager& player, PlayerEnemyContactKind kind, float horizontalDirection) {
    if (dead) return EnemyContactOutcome{EnemyContactResult::None, 0, 0.f, false};
    
    if (hasWings) {
        bool isTop = (kind == PlayerEnemyContactKind::Stomp);
        if (isTop) {
            onStomped();
            return EnemyContactOutcome{EnemyContactResult::EnemyStomped, 200, -500.f, false};
        } else {
            if (player.isImmortal()) return EnemyContactOutcome{EnemyContactResult::None, 0, 0.f, false};
            player.takeDamage(damage);
            return EnemyContactOutcome{player.isDead() ? EnemyContactResult::PlayerKilled : EnemyContactResult::PlayerDamaged, 0, 0.f, false};
        }
    }
    
    return Koopa::handlePlayerContact(player, kind, horizontalDirection);
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
        const float offsetY = std::sin(flyTimer) * 40.f;
        position.y = initialY + offsetY;
        const float dirX = facingRight ? 1.f : -1.f;
        position.x += dirX * speed * dt;
        if (position.x <= initialX - patrolRadius) {
            position.x = initialX - patrolRadius;
            setFacingRight(true);
        } else if (position.x >= initialX + patrolRadius) {
            position.x = initialX + patrolRadius;
            setFacingRight(false);
        }
        hitbox.setPosition(position);
        entitySprite.setPosition(position);
        updateAnimation(dt);
    } else {
        Koopa::update(dt);
    }
}
