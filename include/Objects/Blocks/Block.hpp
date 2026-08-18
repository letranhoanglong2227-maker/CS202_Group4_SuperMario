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
        bool loadOk = blocksTexture.loadFromFile("../assets/textures/Items_Blocks.png");
        if(loadOk){
            std::cout << "File loaded!...\n";
        } else
            std::cout << "File not found!...\n";
        return loadOk;
    }

    static sf::Texture& getBlocksTexture(){
        return blocksTexture;
    }
};