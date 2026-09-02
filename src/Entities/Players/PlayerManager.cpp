#include "Entities/Players/PlayerManager.hpp"
#include "Core/Constants.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>

PlayerManager::PlayerManager(int pId, const std::string& name)
    : Character(name), playerId(pId) {
    health = name == "Luigi" ? 4 : 3;
    maxAirJumps = name == "Luigi" ? 0 : 1;
    airJumpsRemaining = maxAirJumps;
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

    const float oldHeight = hitbox.getSize().y;
    is_big = big;
    if (!is_big) is_fire = false;
    updateHitboxSize();
    position.y -= hitbox.getSize().y - oldHeight;
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
    health = characterName == "Luigi" ? 4 : 3;
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
    entitySprite.setColor(sf::Color::White);

    horizontalControlsInverted = false;
    jumpHeld = false;
    running = false;
    crouching = false;
    airJumpsRemaining = maxAirJumps;
    isTransforming = false;
    transformationTimer = 0.f;
    shootTimer = 0.f;

    setPosition(spawnPosition);
}

bool PlayerManager::canShoot() const {
    // Group5 attaches its FireBuff as soon as the player becomes big.
    return (is_fire || is_big) && !dead && shootTimer <= 0.f;
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
    if (!val) entitySprite.setColor(sf::Color::White);
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
        // ponytail: use Group5's stable standing hitbox; add separate crouch
        // and jump shapes only if those animation states gain swept resizing.
        hitbox.setSize({CELL_SIZE, 122.f});
    } else {
        hitbox.setSize({CELL_SIZE, CELL_SIZE});
    }
    size = hitbox.getSize();
}

void PlayerManager::syncVisualToHitbox() {
    constexpr float playerScale = 4.f;
    const sf::Vector2i frameSize = entitySprite.getTextureRect().size;
    if (frameSize.x <= 0 || frameSize.y <= 0) {
        LivingEntity::syncVisualToHitbox();
        return;
    }

    const bool facingLeft = entitySprite.getScale().x < 0.f;
    entitySprite.setScale({facingLeft ? -playerScale : playerScale,
                           playerScale});
    entitySprite.setOrigin(
        facingLeft ? sf::Vector2f{static_cast<float>(frameSize.x), 0.f}
                   : sf::Vector2f{});

    const sf::Vector2f hitboxPosition = hitbox.getPosition();
    const sf::Vector2f hitboxSize = hitbox.getSize();
    entitySprite.setPosition({
        hitboxPosition.x +
            (hitboxSize.x - static_cast<float>(frameSize.x) * playerScale) /
                2.f,
        hitboxPosition.y + hitboxSize.y -
            static_cast<float>(frameSize.y) * playerScale});
    position = hitboxPosition;
}

bool PlayerManager::canJump() const noexcept {
    return !dead && (grounded || airJumpsRemaining > 0);
}

void PlayerManager::jump() {
    if (!movementComponent || !canJump()) return;

    movementComponent->setVelocity(
        movementComponent->getVelocity().x, jumpForce);
    if (grounded) airJumpsRemaining = maxAirJumps;
    else --airJumpsRemaining;
    grounded = false;
    jumping = true;
}

void PlayerManager::handleInput(float dt) {
    if (dead || isTransforming) return;

    const float leftDirection = horizontalControlsInverted ? 1.f : -1.f;
    const float rightDirection = -leftDirection;
    const bool leftPressed =
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
    const bool rightPressed =
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);
    const bool movingHorizontally = leftPressed != rightPressed;
    if (movingHorizontally)
        move(leftPressed ? leftDirection : rightDirection, 0.f, dt);

    running = movingHorizontally &&
              sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl);
    crouching = is_big && grounded && !movingHorizontally &&
                 (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
                  sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down));

    if (movementComponent) {
        movementComponent->setMaxVelocity(running ? 550.f : 400.f);
    }

    const bool jumpPressed =
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad1) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
    if (jumpPressed && !jumpHeld) jump();
    jumpHeld = jumpPressed;
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
        if (!dead) setImmortal(true, 2.0f);
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
            entitySprite.setColor(sf::Color::White);
        } else {
            constexpr float flashInterval = 0.1f;
            const bool visible =
                static_cast<int>(flashTimer / flashInterval) % 2 == 0;
            entitySprite.setColor(
                sf::Color(255, 255, 255, visible ? 255 : 0));
        }
    }

    Character::update(dt);
}
