#include "Entities/Players/PlayerManager.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>

PlayerManager::PlayerManager(int pId, const std::string& name)
    : Character(name), playerId(pId) {
    hitbox.setSize(sf::Vector2f(32.f, 32.f));
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

void PlayerManager::setBig(bool big) {
    is_big = big;
    updateHitboxSize();
}

bool PlayerManager::isFire() const {
    return is_fire;
}

void PlayerManager::setFire(bool fire) {
    is_fire = fire;
    if (is_fire) {
        is_big = true;
    }
    updateHitboxSize();
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
        hitbox.setSize(sf::Vector2f(32.f, 64.f));
    } else {
        hitbox.setSize(sf::Vector2f(32.f, 32.f));
    }
    size = hitbox.getSize();
}

void PlayerManager::handleInput(float dt) {
    if (dead || isTransforming) return;

    if (playerId == 1) {
        // Player 1 controls: A/D to move, W/J to jump
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            move(-1.f, 0.f, dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            move(1.f, 0.f, dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J)) {
            jump();
        }
    } else {
        // Player 2 controls: Left/Right to move, Up/Numpad1 to jump
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            move(-1.f, 0.f, dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            move(1.f, 0.f, dt);
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
