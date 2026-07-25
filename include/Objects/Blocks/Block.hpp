#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>

#include "Core/GameObject.hpp"
#include "Components/AnimationComponent.hpp"

class Block : public GameObject {
protected:
    std::string name;
    bool exist{ true };
    std::unordered_map<std::string, sf::IntRect> spriteSheets;

public:
    std::unique_ptr<AnimationComponent> animationComponent;

    Block();
    virtual ~Block() = default;

    bool exists() const;
    void setExist(bool value);

    const std::string& getName() const;

    virtual void initSpritesSheet() = 0;
    virtual void reactToCollison(int collidedSide) = 0;

    void update(float dt) override;
    void render(sf::RenderTarget* target) override;
};

