#pragma once
#include<iostream>
#include<string>
#include<unordered_map>
#include<SFML/Graphics.hpp>
#include"Components/AnimationComponent.hpp"
#include"Core/AssetLocator.hpp"
#include"Core/GameObject.hpp"

class Block : public GameObject{
protected:
    std::string name;
    bool exist;
    std::unordered_map<std::string, sf::IntRect> spritesSheet;
    AnimationComponent animationComponent;
public:
    Block();
    
    bool isExist() const { return exist; }
    virtual bool canBeRemoved() const { return !exist; }
    virtual bool isBumpingUpward() const noexcept { return false; }
    void setSizeBlock(const sf::Vector2f& sz);

    virtual void initSpritesSheet() = 0;
    virtual void reactToCollision(int collidedSide) = 0;
    virtual void update(float dt) = 0;

    virtual ~Block();
};

class TextureBlockManager{
private:
    inline static sf::Texture blocksTexture;
public:
    static bool setupTexture(){
        const auto path = AssetLocator::find(
            "assets/textures/Tileset.png");
        const bool loadOk = path && blocksTexture.loadFromFile(*path);
        if (!loadOk)
            std::cerr << AssetLocator::missingMessage(
                             "assets/textures/Tileset.png")
                      << '\n';
        return loadOk;
    }

    static sf::Texture& getBlocksTexture(){
        return blocksTexture;
    }
};
