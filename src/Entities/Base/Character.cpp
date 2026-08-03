#include "Entities/Base/Character.hpp"

Character::Character(const std::string& name)
    : LivingEntity(), characterName(name) {
}

const std::string& Character::getCharacterName() const {
    return characterName;
}

void Character::setCharacterName(const std::string& name) {
    characterName = name;
}

bool Character::isJumping() const {
    return jumping;
}

void Character::setJumping(bool j) {
    jumping = j;
}

bool Character::isGrounded() const {
    return grounded;
}

void Character::setGrounded(bool g) {
    grounded = g;
    if (grounded) {
        jumping = false;
    }
}

void Character::jump() {
    if (grounded && movementComponent) {
        movementComponent->setVelocity(movementComponent->getVelocity().x, jumpForce);
        grounded = false;
        jumping = true;
    }
}
