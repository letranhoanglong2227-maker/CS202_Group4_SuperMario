#include "Entities/Players/Mario.hpp"

Mario::Mario()
    : PlayerManager(1, "Mario") {
    speed = 400.f;
    jumpForce = -900.f;
    movementComponent = std::make_unique<MovementComponent>(400.f, 3000.f, 1000.f);

    // Initial scale
    entitySprite.setScale({2.0f, 2.0f});

    // Add normal animations
    animationComponent->addAnimation("idle", { sf::IntRect({1, 17}, {16, 16}) });
    animationComponent->addAnimation("walk", { sf::IntRect({17, 17}, {16, 16}), sf::IntRect({35, 17}, {16, 16}), sf::IntRect({17, 17}, {16, 16}) });
    animationComponent->addAnimation("run", { sf::IntRect({69, 17}, {16, 16}), sf::IntRect({17, 17}, {16, 16}), sf::IntRect({35, 17}, {16, 16}), sf::IntRect({17, 17}, {16, 16}), sf::IntRect({1, 17}, {16, 16}) });
    animationComponent->addAnimation("jump", { sf::IntRect({86, 17}, {16, 16}) });
    animationComponent->addAnimation("dead", { sf::IntRect({52, 17}, {16, 16}) }); // STOP- used as dead for now

    // Add big animations
    animationComponent->addAnimation("big_idle", { sf::IntRect({241, 3}, {14, 30}) });
    animationComponent->addAnimation("big_walk", { sf::IntRect({256, 3}, {16, 30}), sf::IntRect({273, 3}, {16, 30}), sf::IntRect({256, 3}, {16, 30}) });
    animationComponent->addAnimation("big_run", { sf::IntRect({307, 2}, {16, 32}), sf::IntRect({256, 3}, {16, 30}), sf::IntRect({273, 3}, {16, 30}), sf::IntRect({256, 3}, {16, 30}), sf::IntRect({241, 3}, {14, 30}) });
    animationComponent->addAnimation("big_jump", { sf::IntRect({328, 3}, {20, 30}) });
    animationComponent->addAnimation("big_crouch", { sf::IntRect({433, 3}, {16, 30}) });
}

void Mario::updateAnimation(float dt) {
    (void)dt;
    if (animationComponent) {
        if (dead) {
            animationComponent->play("dead", dt);
        } else if (jumping || !grounded) {
            animationComponent->play(is_big ? "big_jump" : "jump", dt);
        } else if (crouching) {
            animationComponent->play("big_crouch", dt);
        } else if (movementComponent && std::abs(movementComponent->getVelocity().x) > 5.f) {
            animationComponent->play(
                running ? (is_big ? "big_run" : "run")
                        : (is_big ? "big_walk" : "walk"),
                dt);
        } else {
            animationComponent->play(is_big ? "big_idle" : "idle", dt);
        }

        // Handle facing direction
        if (movementComponent) {
            if (movementComponent->getVelocity().x > 0.1f) {
                entitySprite.setScale({2.0f, 2.0f});
                entitySprite.setOrigin({0.f, 0.f});
            } else if (movementComponent->getVelocity().x < -0.1f) {
                entitySprite.setScale({-2.0f, 2.0f});
                entitySprite.setOrigin({entitySprite.getLocalBounds().size.x, 0.f});
            }
        }
    }
}
