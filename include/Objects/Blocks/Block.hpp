#pragma once
#include<iostream>
#include<string>
#include<unordered_map>
#include<SFML/Graphics.hpp>
#include"Components/AnimationComponent.hpp"
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
    void setSizeBlock(const sf::Vector2f& sz);

    void setSize(const sf::Vector2f& sz) override;
    void setSize(float width, float height) override;

    virtual void initSpritesSheet() = 0;
    virtual void reactToCollision(int collidedSide) = 0;
    virtual void update(float dt) = 0;

    virtual ~Block();
};

#include "Core/AssetResolver.hpp"

class TextureBlockManager{
private:
    inline static sf::Texture blocksTexture;
public:
    static bool setupTexture(){
        try {
            auto path = AssetResolver::resolve("assets/textures/Items_Blocks.png");
            bool loadOk = blocksTexture.loadFromFile(path.string());
            if(loadOk){
                std::cout << "File loaded!...\n";
            } else {
                std::cout << "File not found!...\n";
            }
            return loadOk;
        } catch (const std::exception& e) {
            std::cerr << "Asset error: " << e.what() << "\n";
            return false;
        }
    }

    static sf::Texture& getBlocksTexture(){
        return blocksTexture;
    }
};
