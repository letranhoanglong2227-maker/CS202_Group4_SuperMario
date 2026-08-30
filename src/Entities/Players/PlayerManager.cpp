#include "Entities/Players/PlayerManager.hpp"
#include "Core/Constants.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>

PlayerManager::PlayerManager(int pId, const std::string& name)
    : Character(name), playerId(pId) {
    hitbox.setSize({CELL_SIZE, CELL_SIZE});
    size = hitbox.getSize();
    movementComponent = std::make_unique<MovementComponent>(200.f, 1000.f, 800.f);
}

int PlayerManager::getPlayerId() const {
    return playerId;
}

void PlayerManager::setPlayerId(int pId) {
    playerId = pId;
}

bool PlayerManager::isBig() const {
    return is_big;
}

bool PlayerManager::setBig(bool big, bool canGrow) {
    if (big && !canGrow) return false;
    if (is_big == big) return true;
    
    is_big = big;
    if (!is_big) {
        is_fire = false;
        position.y += CELL_SIZE; // Shrink: move top down, keep feet anchored
    } else {
        position.y -= CELL_SIZE; // Grow: move top up, keep feet anchored
    }
    
    updateHitboxSize();
    hitbox.setPosition(position);
    entitySprite.setPosition(position);
    return true;
}

bool PlayerManager::isFire() const {
    return is_fire;
}

bool PlayerManager::setFire(bool fire, bool canGrow) {
    if (fire && !is_big && !canGrow) return false;
    
    if (fire) {
        if (!is_big) {
            setBig(true, canGrow);
        }
        is_fire = true;
    } else {
        is_fire = false;
    }
    
    updateHitboxSize();
    return true;
}

void PlayerManager::resetForRespawn(const sf::Vector2f& spawnPosition) {
    health = 1;
    dead = false;
    
    if (movementComponent) {
        movementComponent->setVelocity({0.f, 0.f});
    }
    
    setGrounded(true);
    jumping = false;
    
    is_fire = false;
    is_big = false;
    updateHitboxSize();
    
    buffs.clear();
    
    immortal = false;
    isFlashing = false;
    flashTimer = 0.f;
    
    horizontalControlsInverted = false;
    isTransforming = false;
    transformationTimer = 0.f;
    shootTimer = 0.f;
    
    setPosition(spawnPosition);
}

bool PlayerManager::canShoot() const {
    return is_fire && !dead && shootTimer <= 0.f;
}

std::optional<ProjectileSpawnRequest> PlayerManager::shoot(float direction) {
    if (!canShoot()) return std::nullopt;
    
    shootTimer = shootCooldown;
    
    float dirX = (direction >= 0.f) ? 1.f : -1.f;
    float spawnX = position.x + (dirX > 0 ? size.x : -16.f);
    float spawnY = position.y + 16.f;
    
    return ProjectileSpawnRequest{
        ProjectileKind::Fireball,
        {spawnX, spawnY},
        {dirX, 0.f},
        400.f,
        1
    };
}

bool PlayerManager::isImmortal() const {
    return immortal;
}

void PlayerManager::setImmortal(bool val, float duration) {
    immortal = val;
    invincibilityDuration = duration;
    flashTimer = 0.f;
    isFlashing = val;
}

bool PlayerManager::areHorizontalControlsInverted() const noexcept {
    return horizontalControlsInverted;
}

void PlayerManager::setHorizontalControlsInverted(bool inverted) noexcept {
    horizontalControlsInverted = inverted;
}

void PlayerManager::addBuff(std::unique_ptr<PlayerBuff> buff) {
    if (!buff) return;
    buff->apply(this);
    buffs.push_back(std::move(buff));
}

void PlayerManager::removeBuff(const std::string& type) {
    buffs.erase(
        std::remove_if(buffs.begin(), buffs.end(),
            [this, &type](const std::unique_ptr<PlayerBuff>& b) {
                if (b && b->getType() == type) {
                    b->remove(this);
                    return true;
                }
                return false;
            }),
        buffs.end());
}

void PlayerManager::updateHitboxSize() {
    if (is_big || is_fire) {
        hitbox.setSize({CELL_SIZE, 2.f * CELL_SIZE});
    } else {
        hitbox.setSize({CELL_SIZE, CELL_SIZE});
    }
    size = hitbox.getSize();
}

void PlayerManager::handleInput(float dt) {
    if (dead || isTransforming) return;

    const float leftDirection = horizontalControlsInverted ? 1.f : -1.f;
    const float rightDirection = -leftDirection;

    if (playerId == 1) {
        // Player 1 controls: A/D to move, W/J to jump
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            move(leftDirection, 0.f, dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            move(rightDirection, 0.f, dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            jump();
        }
    } else {
        // Player 2 controls: Left/Right to move, Up/Numpad1 to jump
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            move(leftDirection, 0.f, dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            move(rightDirection, 0.f, dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad1)) {
            jump();
        }
    }
}

void PlayerManager::takeDamage(int amount) {
    if (immortal || dead) return;

    if (is_fire) {
        setFire(false);
        setBig(true);
        setImmortal(true, 2.0f);
    } else if (is_big) {
        setBig(false);
        setImmortal(true, 2.0f);
    } else {
        Character::takeDamage(amount);
    }
}

void PlayerManager::update(float dt) {
    handleInput(dt);

    // Update buffs
    for (auto& buff : buffs) {
        if (buff) {
            buff->update(dt);
        }
    }

    // Clean expired buffs
    buffs.erase(
        std::remove_if(buffs.begin(), buffs.end(),
            [this](const std::unique_ptr<PlayerBuff>& b) {
                if (b && b->isExpired()) {
                    b->remove(this);
                    return true;
                }
                return false;
            }),
        buffs.end());

    if (shootTimer > 0.f) {
        shootTimer -= dt;
    }

    // Update invincibility & flashing
    if (immortal) {
        flashTimer += dt;
        if (flashTimer >= invincibilityDuration) {
            immortal = false;
            isFlashing = false;
            flashTimer = 0.f;
        }
    }

    Character::update(dt);
}
