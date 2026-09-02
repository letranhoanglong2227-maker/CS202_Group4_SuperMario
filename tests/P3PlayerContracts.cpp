#include "Entities/Players/Mario.hpp"
#include "Entities/Players/Luigi.hpp"
#include "Components/MovementComponent.hpp"
#include <iostream>
#include <string>

int P3_test_passed = 0;
int P3_test_failed = 0;

class InspectableMario final : public Mario {
public:
    unsigned int visualAlpha() const { return entitySprite.getColor().a; }
    sf::FloatRect visualBounds() const { return entitySprite.getGlobalBounds(); }
    sf::Vector2f visualScale() const { return entitySprite.getScale(); }
    sf::IntRect visualFrame() const { return entitySprite.getTextureRect(); }
    void syncVisual() { syncVisualToHitbox(); }
    void setPresentationState(bool run, bool crouch) {
        running = run;
        crouching = crouch;
    }
};

void checkP3(bool condition, const std::string& testName) {
    if (condition) {
        std::cout << "[PASS] " << testName << "\n";
        P3_test_passed++;
    } else {
        std::cout << "[FAIL] " << testName << "\n";
        P3_test_failed++;
    }
}

int main() {
    std::cout << "--- P3 Player Contracts ---\n";
    Mario mario;
    Luigi luigi;
    checkP3(mario.getHealth() == 3 && luigi.getHealth() == 4,
            "Mario and Luigi start with Group5 health values");
    checkP3(TexturePlayerManager::setupTexture() &&
                TexturePlayerManager::getPlayerTexture().getSize().x > 0,
            "Player texture provider is ready for production rendering");
    mario.setPosition({100.f, 200.f}); // Bottom is 200 + 64 = 264
    
    // 1. State logic
    checkP3(mario.setBig(true, false) == false, "Growth rejected if canGrow=false");
    
    mario.setBig(true, true);
    checkP3(mario.isBig() == true, "Mario is Big");
    checkP3(mario.canShoot() == true,
            "Big Mario inherits Group5 shooting capability");
    // Feet anchor check
    checkP3(mario.hitbox.getSize() == sf::Vector2f(64.f, 122.f) &&
                mario.getPosition().y + mario.hitbox.getSize().y == 264.f,
            "Big Mario uses Group5 standing height with feet anchored");

    InspectableMario visualMario;
    visualMario.setPosition({100.f, 200.f});
    visualMario.setBig(true);
    visualMario.setGrounded(true);
    visualMario.update(0.f);
    visualMario.syncVisual();
    const sf::FloatRect bigIdleBounds = visualMario.visualBounds();
    checkP3(visualMario.visualScale() == sf::Vector2f(4.f, 4.f) &&
                bigIdleBounds.size == sf::Vector2f(56.f, 120.f) &&
                bigIdleBounds.position == sf::Vector2f(104.f, 144.f) &&
                bigIdleBounds.position.y + bigIdleBounds.size.y == 264.f,
            "Big player frames keep uniform 4x scale and bottom-center alignment");

    visualMario.getMovementComponent()->setVelocity({500.f, 0.f});
    visualMario.setPresentationState(true, false);
    visualMario.updateAnimation(0.f);
    checkP3(visualMario.visualFrame() == sf::IntRect({307, 2}, {16, 32}),
            "Ctrl-running Big Mario uses Group5's run frame");
    visualMario.getMovementComponent()->setVelocity({0.f, 0.f});
    visualMario.setPresentationState(false, true);
    visualMario.updateAnimation(0.f);
    checkP3(visualMario.visualFrame() == sf::IntRect({433, 3}, {16, 30}),
            "stationary Big Mario uses Group5's crouch frame");
    
    mario.setFire(true, true);
    checkP3(mario.isFire() == true && mario.isBig() == true, "Fire form implies Big form");
    checkP3(mario.canShoot() == true, "Fire Mario can shoot");
    
    auto req = mario.shoot(1.f);
    checkP3(req.has_value() && req->type == ProjectileKind::Fireball, "Shoot returns Fireball request");
    checkP3(mario.canShoot() == false, "Cannot shoot again immediately (cooldown)");
    
    // 2. Damage flow
    mario.takeDamage(1);
    checkP3(mario.isFire() == false && mario.isBig() == true, "Damage in Fire form drops to Big form");
    checkP3(mario.isImmortal() == true, "Damage grants immortality");
    
    mario.setImmortal(false);
    mario.takeDamage(1);
    checkP3(mario.isBig() == false && mario.isDead() == false, "Damage in Big form drops to Small form");
    checkP3(mario.getPosition().y == 200.f,
            "Feet anchor preserved when shrinking to 64px");
    
    mario.setImmortal(false);
    mario.setHealth(1);
    mario.takeDamage(1);
    checkP3(mario.isDead() == true,
            "Small player dies when the Group5 health pool is exhausted");
    
    // 3. Reset logic
    mario.resetForRespawn({50.f, 50.f});
    checkP3(mario.isDead() == false && mario.isBig() == false &&
                mario.getHealth() == 3 && mario.getPosition().x == 50.f,
            "resetForRespawn restores Mario's Group5 health and position");
    checkP3(mario.isImmortal() == false, "resetForRespawn clears immortality");
    checkP3(mario.isFire() == false, "resetForRespawn clears fire form");

    MovementComponent movement(200.f, 1000.f, 800.f);
    movement.move(1.f, 0.f, 0.1f);
    movement.update(0.1f);
    checkP3(movement.getVelocity().x == 100.f,
            "Held movement input is not cancelled by friction");
    movement.update(0.1f);
    checkP3(movement.getVelocity().x == 20.f,
            "Released movement input decelerates predictably");

    Mario jumpingMario;
    jumpingMario.setGrounded(true);
    jumpingMario.jump();
    const float firstJumpVelocity =
        jumpingMario.getMovementComponent()->getVelocity().y;
    jumpingMario.getMovementComponent()->setVelocity(0.f, 100.f);
    jumpingMario.jump();
    const float secondJumpVelocity =
        jumpingMario.getMovementComponent()->getVelocity().y;
    jumpingMario.getMovementComponent()->setVelocity(0.f, 100.f);
    jumpingMario.jump();
    checkP3(firstJumpVelocity == -900.f && secondJumpVelocity == -900.f &&
                jumpingMario.getMovementComponent()->getVelocity().y == 100.f &&
                !jumpingMario.canJump(),
            "Mario gets exactly Group5's two jumps");

    Luigi jumpingLuigi;
    jumpingLuigi.setGrounded(true);
    jumpingLuigi.jump();
    const float luigiJumpVelocity =
        jumpingLuigi.getMovementComponent()->getVelocity().y;
    jumpingLuigi.getMovementComponent()->setVelocity(0.f, 100.f);
    jumpingLuigi.jump();
    checkP3(luigiJumpVelocity == -1450.f &&
                jumpingLuigi.getMovementComponent()->getVelocity().y == 100.f &&
                !jumpingLuigi.canJump(),
            "Luigi matches Group5's single higher jump");

    InspectableMario flashingMario;
    flashingMario.setImmortal(true, 0.25f);
    flashingMario.update(0.11f);
    checkP3(flashingMario.visualAlpha() == 0,
            "temporary damage immunity visibly flashes the player");
    flashingMario.update(0.15f);
    checkP3(!flashingMario.isImmortal() &&
                flashingMario.visualAlpha() == 255,
            "player visibility is restored when immunity ends");
    
    std::cout << "P3 Player Results: " << P3_test_passed << " PASS, " << P3_test_failed << " FAIL\n";
    return P3_test_failed > 0 ? 1 : 0;
}
