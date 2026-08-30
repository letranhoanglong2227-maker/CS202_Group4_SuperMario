#include "Objects/Items/PowerUpObject.hpp"

PowerUpObject::PowerUpObject(const std::string& name) : GameObject(), name(name) {
    animationComponent = std::make_unique<AnimationComponent>(
        entitySprite, 
        TextureItemManager::getItemTexture(), 
        0.1f
    );
}

ItemCollectionResult PowerUpObject::collect(PlayerManager& player, bool canGrow) {
    (void)player;
    return ItemCollectionResult{false, 0, 0, 0, RequestedPlayerForm::None};
}

const std::string& PowerUpObject::getName() const {
    return name;
}

bool PowerUpObject::exists() const {
    return exist;
}

void PowerUpObject::setExist(bool e) {
    exist = e;
}

void PowerUpObject::reactToCollision() {
    exist = false;
}

void PowerUpObject::reactToEnemyCollision(Enemy* enemy) {
    (void)enemy;
}

void PowerUpObject::reactToBlockCollision(Block* block) {
    (void)block;
}

void PowerUpObject::update(float dt) {
    (void)dt;
    entitySprite.setPosition(hitbox.getPosition());
    position = hitbox.getPosition();
}

void PowerUpObject::render(sf::RenderTarget* target) {
    if (target && exist) {
        target->draw(entitySprite);
    }
}
