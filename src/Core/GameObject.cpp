#include "Core/GameObject.hpp"

GameObject::GameObject() : entitySprite(entityTexture)
{
    std::cout << "Inform GameObject Constructor...\n";
    // entityTexture.loadFromFile("assets/textures/Items_Blocks.png");
    // entitySprite.setTexture(entityTexture);
    //entitySprite set up?
}