#include "Entities/Players/Mario.hpp"

Mario::Mario()
    : PlayerManager(1, "Mario") {
    speed = 100.f;
    jumpForce = -500.f;
    movementComponent = std::make_unique<MovementComponent>(200.f, 1000.f, 800.f);
}

void Mario::updateAnimation(float dt) {
    (void)dt;
    if (animationComponent) {
        animationComponent->play(is_big ? "big_idle" : "idle", dt);
    }
}
