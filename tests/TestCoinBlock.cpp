#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include "Objects/Blocks/CoinBlock.hpp"
#include "Objects/Items/Coin.hpp"
#include "Objects/Blocks/Block.hpp"

// Setup a dummy callback handler
std::vector<std::unique_ptr<GameObject>> activeEntities;

void spawnCallback(GameObject* newObj) {
    if (newObj) {
        std::cout << "Spawn callback triggered! Added a new Coin.\n";
        activeEntities.push_back(std::unique_ptr<GameObject>(newObj));
    }
}

int main() {
    // 1. Setup Window
    sf::RenderWindow window(sf::VideoMode(800, 600), "CoinBlock Test");
    window.setFramerateLimit(60);

    // 2. Load Textures
    if (!TextureBlockManager::setupTexture()) {
        std::cerr << "Failed to load block textures. Make sure you are running from the build directory or the paths are correct.\n";
        return -1;
    }
    
    // We also need Coin textures if Coin doesn't load its own.
    // Assuming Coin handles its own texture loading or we can just see a white box if it fails.

    // 3. Create CoinBlock
    CoinBlock myBlock(3, spawnCallback);
    
    // Set position and let it initialize originalY
    myBlock.setPosition({400.f, 300.f});
    // In our actual implementation, update() initializes originalY on first frame if it's -1.

    // View to zoom in a bit since sprites are 16x16
    sf::View view(sf::FloatRect(0, 0, 800, 600));
    view.setCenter(400.f, 300.f);
    view.zoom(0.2f);
    window.setView(view);

    sf::Clock clock;

    // 4. Game Loop
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // Event polling
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Press SPACE to simulate hitting the block from below
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                std::cout << "Space pressed! Simulating collision...\n";
                myBlock.reactToCollision(COLLISION_BOTTOM);
            }
        }

        // Update logic
        myBlock.update(dt);
        
        // Update all spawned entities
        for (auto& entity : activeEntities) {
            // Note: Since Coin inherits from GameObject and might have update/render,
            // we should technically call update/render. 
            // In our system, maybe Coin is an Entity? Let's check.
            // If Coin is a GameObject, we might not have a pure virtual update() in GameObject.
            // But we will try to just draw it.
            
            // We can cast to Coin and call update if it has one.
            if (Coin* coin = dynamic_cast<Coin*>(entity.get())) {
                coin->update(dt);
            }
        }

        // Render
        window.clear(sf::Color(100, 149, 237)); // Mario sky blue
        
        myBlock.render(window);
        
        for (auto& entity : activeEntities) {
            entity->render(window);
        }

        window.display();
    }

    return 0;
}
