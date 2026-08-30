#include "Objects/Items/Fireball.hpp"
#include "Objects/Items/PowerUpObject.hpp"
#include "Core/Constants.hpp"

Fireball::Fireball(const sf::Vector2f& pos, float directionX)
    : LivingEntity(), dirX(directionX) {
    setPosition(pos);
    hitbox.setPosition(pos);
    hitbox.setSize({CELL_SIZE * 0.5f, CELL_SIZE * 0.5f});
    size = hitbox.getSize();

    movementComponent = std::make_unique<MovementComponent>(400.f, 1000.f, 0.f);
    movementComponent->setVelocity(dirX * 400.f, 0.f);

    setHealth(1);
    
    entitySprite.setScale({2.0f, 2.0f});
    animationComponent = std::make_unique<AnimationComponent>(
        entitySprite, 
        TextureItemManager::getItemTexture(), 
        0.05f
    );
    animationComponent->addAnimation("spin", {
        sf::IntRect({2, 70}, {8, 8}),
        sf::IntRect({11, 70}, {8, 8}),
        sf::IntRect({20, 70}, {8, 8}),
        sf::IntRect({29, 70}, {8, 8})
    });
}

void Fireball::reactToCollision() {
    setDead(true);
}

void Fireball::reactToBlockCollision() {
    if (movementComponent) {
        movementComponent->setVelocity(movementComponent->getVelocity().x, -400.f); // Bounce up
    }
}

bool Fireball::isExpired() const {
    return dead;
}

bool Fireball::canBeRemoved() const {
    return dead;
}

void Fireball::update(float dt) {
    if (dead) return;

    lifeTimer += dt;
    if (lifeTimer >= maxLife) {
        setDead(true);
        return;
    }

    if (movementComponent) {
        // Gravity
        movementComponent->setVelocity(movementComponent->getVelocity().x, movementComponent->getVelocity().y + 1200.f * dt);
        
        position += movementComponent->getVelocity() * dt;
        hitbox.setPosition(position);
        entitySprite.setPosition(position);
    }
    if (animationComponent) {
        animationComponent->play("spin", dt);
    }
}

void Fireball::render(sf::RenderTarget* target) {
    if (target && !dead) {
        target->draw(entitySprite);
    }
}
