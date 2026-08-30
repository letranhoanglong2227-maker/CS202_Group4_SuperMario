#pragma once

#include "Core/GameObject.hpp"
#include "Components/AnimationComponent.hpp"
#include <string>
#include <memory>
#include "Entities/Players/PlayerManager.hpp"
#include "Objects/Items/ItemCollectionResult.hpp"

class Enemy;
class Block;

class PowerUpObject : public GameObject {
protected:
    std::string name;
    bool exist{ true };
    std::unique_ptr<AnimationComponent> animationComponent;
    bool popped{ false };
    float popTimer{ 0.f };
    float popDuration{ 0.5f };
    sf::Vector2f startPosition;

public:
    PowerUpObject(const std::string& name = "PowerUp");
    virtual ~PowerUpObject() = default;

    virtual ItemCollectionResult collect(PlayerManager& player, bool canGrow = true);

    const std::string& getName() const;
    bool exists() const;
    void setExist(bool e);
    bool isPopped() const;
    void setPopped(bool p);

    virtual void reactToCollision();
    virtual void reactToEnemyCollision(Enemy* enemy);
    virtual void reactToBlockCollision(Block* block);

    void update(float dt) override;
    void render(sf::RenderTarget* target) override;
};

class TextureItemManager {
private:
    inline static sf::Texture itemTexture;

public:
    static bool setupTexture() {
        if (!itemTexture.loadFromFile("assets/textures/Items_Blocks.png")) {
            if (!itemTexture.loadFromFile("../assets/textures/Items_Blocks.png")) {
                if (!itemTexture.loadFromFile("../../assets/textures/Items_Blocks.png")) {
                    std::cout << "Item Texture not found!...\n";
                    return false;
                }
            }
        }
        std::cout << "Item Texture loaded!...\n";
        return true;
    }

    static sf::Texture& getItemTexture() {
        return itemTexture;
    }
};
