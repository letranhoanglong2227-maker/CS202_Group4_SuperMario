#include "Objects/Blocks/CloudBlock.hpp"

#include "Core/Constants.hpp"
#include <algorithm>

CloudBlock::CloudBlock(sf::Vector2f pos, int widthInTiles, float initialTime)
    : timer(std::max(0.f, initialTime)) {
    name = "CloudBlock";
    initSpritesSheet();
    animationComponent.addAnimation("Idle", {spritesSheet["Cloud"]});
    setPosition(pos);
    setSizeBlock({static_cast<float>(std::max(1, widthInTiles)) * CELL_SIZE,
                  CELL_SIZE});
}

void CloudBlock::initSpritesSheet() {
    spritesSheet["Cloud"] = {{1, 215}, {64, 16}};
}

void CloudBlock::reactToCollision(int) {}

void CloudBlock::update(float dt) {
    if (dt <= 0.f) return;
    animationComponent.play("Idle", dt);
    timer += dt;
    const float duration = exist ? visibleDuration : hiddenDuration;
    if (timer >= duration) {
        timer = 0.f;
        exist = !exist;
    }
}

void CloudBlock::render(sf::RenderTarget* target) {
    if (target && exist) target->draw(entitySprite);
}
