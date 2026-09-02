#include "Entities/Players/Luigi.hpp"

Luigi::Luigi()
    : PlayerManager(2, "Luigi") {
    speed = 400.f;
    jumpForce = -1450.f; // Group5 gives Luigi the higher jump.
    movementComponent = std::make_unique<MovementComponent>(400.f, 3000.f, 1000.f);

    // Initial scale
    entitySprite.setScale({2.0f, 2.0f});

    // Add normal animations (Luigi's base Y is 50 instead of 17)
    animationComponent->addAnimation("idle", { sf::IntRect({1, 50}, {16, 16}) });
    animationComponent->addAnimation("walk", { sf::IntRect({17, 50}, {16, 16}), sf::IntRect({35, 50}, {16, 16}), sf::IntRect({17, 50}, {16, 16}) });
    animationComponent->addAnimation("run", { sf::IntRect({69, 50}, {16, 16}), sf::IntRect({17, 50}, {16, 16}), sf::IntRect({35, 50}, {16, 16}), sf::IntRect({17, 50}, {16, 16}), sf::IntRect({1, 50}, {16, 16}) });
    animationComponent->addAnimation("jump", { sf::IntRect({86, 50}, {16, 16}) });
    animationComponent->addAnimation("dead", { sf::IntRect({52, 50}, {16, 16}) });

    // Add big animations (Luigi's big Y is 35 instead of 3)
    animationComponent->addAnimation("big_idle", { sf::IntRect({241, 35}, {14, 30}) });
    animationComponent->addAnimation("big_walk", { sf::IntRect({256, 35}, {16, 30}), sf::IntRect({273, 35}, {16, 30}), sf::IntRect({256, 35}, {16, 30}) });
    animationComponent->addAnimation("big_run", { sf::IntRect({307, 35}, {16, 32}), sf::IntRect({256, 35}, {16, 30}), sf::IntRect({273, 35}, {16, 30}), sf::IntRect({256, 35}, {16, 30}), sf::IntRect({241, 35}, {14, 30}) });
    animationComponent->addAnimation("big_jump", { sf::IntRect({328, 35}, {20, 30}) });
    animationComponent->addAnimation("big_crouch", { sf::IntRect({433, 35}, {16, 30}) });
}

void Luigi::updateAnimation(float dt) {
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
