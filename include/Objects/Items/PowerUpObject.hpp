#pragma once

#include "Core/GameObject.hpp"
#include <string>

class Enemy;
class Block;

class PowerUpObject : public GameObject {
protected:
    std::string name;
    bool exist{ true };

public:
    PowerUpObject(const std::string& name = "PowerUp");
    virtual ~PowerUpObject() = default;

    const std::string& getName() const;
    bool exists() const;
    void setExist(bool e);

    virtual void reactToCollision();
    virtual void reactToEnemyCollision(Enemy* enemy);
    virtual void reactToBlockCollision(Block* block);

    void update(float dt) override;
    void render(sf::RenderTarget* target) override;
};
