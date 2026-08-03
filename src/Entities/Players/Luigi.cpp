#include "Entities/Players/Luigi.hpp"

Luigi::Luigi()
    : PlayerManager(2, "Luigi") {
    speed = 110.f;
    jumpForce = -560.f; // Luigi jumps higher than Mario
    movementComponent = std::make_unique<MovementComponent>(220.f, 1000.f, 500.f); // Slippery movement
}

void Luigi::updateAnimation(float dt) {
    (void)dt;
    if (animationComponent) {
        animationComponent->play(is_big ? "big_idle" : "idle", dt);
    }
}
