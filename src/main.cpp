// Nhân: Code test chức năng của SolidBlock - Block

#include<iostream>
#include"../include/Objects/Blocks/SolidBlock.hpp"

int main() {
    TextureBlockManager::setupTexture();
    
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Title");
    
    Block* pBlock = new SolidBlock();
    pBlock->setupTexture();
    
    while(window.isOpen()){
        // Handle event
        while(std::optional event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }
        }

        // Draw
        window.clear(sf::Color(10, 10, 10));
        pBlock->render(&window);

        // Display
        window.display();
    }
    return 0;
}
