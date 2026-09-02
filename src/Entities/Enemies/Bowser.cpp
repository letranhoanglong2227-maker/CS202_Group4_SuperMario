#include "Entities/Enemies/Bowser.hpp"
#include "Core/Constants.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

Bowser::Bowser(const sf::Vector2f& pos)
    : Enemy() {
    setHealth(maxHealth);
    setDamage(2);  // High damage
    setPointsValue(5000);
    setSpeed(300.f);

    setPosition(pos);
    hitbox.setSize({2.f * CELL_SIZE, 140.f});
    hitbox.setPosition(pos);
    size = hitbox.getSize();

    movementComponent =
        std::make_unique<MovementComponent>(300.f, 2000.f, 0.f);

    // Initial scale
    entitySprite.setScale({2.0f, 2.0f});

    if (animationComponent) {
        animationComponent->addAnimation("walk", {
            sf::IntRect({1, 186}, {32, 35}),
            sf::IntRect({34, 186}, {32, 35}),
            sf::IntRect({67, 186}, {32, 35}) });
        animationComponent->addAnimation("breathe_fire", { sf::IntRect({100, 186}, {32, 35}), sf::IntRect({133, 186}, {32, 35}), sf::IntRect({166, 186}, {32, 35}), sf::IntRect({199, 186}, {32, 35}) });
    }
}

bool Bowser::isBreathingFire() const {
    return breathingFire;
}

void Bowser::breatheFire() {
    breathingFire = true;
}

void Bowser::jumpSlam() {
    if (movementComponent) {
        movementComponent->setVelocity(movementComponent->getVelocity().x, -350.f);
        jumpingSlam = true;
    }
}

void Bowser::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        health = 0;
        setDead(true);
    }
}

bool Bowser::hasPendingProjectile() const {
    return pendingProjectile.has_value();
}

std::optional<ProjectileSpawnRequest> Bowser::consumePendingProjectile() {
    auto res = pendingProjectile;
    pendingProjectile = std::nullopt;
    return res;
}

EnemyContactOutcome Bowser::handlePlayerContact(PlayerManager& player, PlayerEnemyContactKind kind, float horizontalDirection) {
    (void)kind;
    (void)horizontalDirection;
    if (dead) return EnemyContactOutcome{EnemyContactResult::None, 0, 0.f, false};

    if (player.isImmortal()) return EnemyContactOutcome{EnemyContactResult::None, 0, 0.f, false};
    player.takeDamage(damage);
    return EnemyContactOutcome{player.isDead() ? EnemyContactResult::PlayerKilled : EnemyContactResult::PlayerDamaged, 0, 0.f, false};
}

void Bowser::updateAnimation(float dt) {
    (void)dt;
    if (animationComponent) {
        if (breathingFire) {
            animationComponent->play("breathe_fire", dt);
        } else {
            animationComponent->play("walk", dt); // or jump_slam if you add one later
        }
    }
}

void Bowser::update(float dt) {
    if (dead) {
        pendingProjectile = std::nullopt;
        return;
    }

    attackTimer += dt;
    if (attackTimer >= 5.0f) {
        attackTimer = 0.f;
        breathingFire = false;
        jumpingSlam = false;
    } else if (attackTimer >= 2.0f && attackTimer < 3.5f) {
        if (!breathingFire) {
            breathingFire = true;
            float dirX = facingRight ? 1.f : -1.f;
            pendingProjectile = ProjectileSpawnRequest{
                ProjectileKind::BowserFire,
                {position.x + (dirX > 0 ? size.x : -32.f), position.y + 32.f},
                {dirX, 0.f},
                500.f,
                damage
            };
        }
    } else if (attackTimer >= 3.5f && !jumpingSlam) {
        breathingFire = false;
        jumpSlam();
    }

    float dirX = facingRight ? 1.f : -1.f;
    move(dirX, 0.f, dt);
    LivingEntity::update(dt);
}

void Bowser::render(sf::RenderTarget* target) {
    if (!target || dead) return;
    LivingEntity::render(target);

    constexpr float barWidth = 128.f;
    constexpr float barHeight = 10.f;
    sf::RectangleShape background({barWidth, barHeight});
    background.setPosition({position.x, position.y - 18.f});
    background.setFillColor(sf::Color(90, 15, 15));
    background.setOutlineColor(sf::Color::Black);
    background.setOutlineThickness(2.f);
    target->draw(background);

    sf::RectangleShape remaining(
        {barWidth * static_cast<float>(health) /
             static_cast<float>(maxHealth),
         barHeight});
    remaining.setPosition(background.getPosition());
    remaining.setFillColor(sf::Color(45, 210, 75));
    target->draw(remaining);
}
