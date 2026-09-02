#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include "Objects/Blocks/MushroomBlock.hpp"
#include "Objects/Blocks/Block.hpp"

// Setup a dummy callback handler
void spawnCallback(std::unique_ptr<GameObject> newObj) {
    std::cout << "Spawn callback triggered!\n";
    if (newObj) {
        std::cout << "Added a new Mushroom.\n";
    }
}

int main() {
    // 1. Setup Window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "MushroomBlock Test");
    window.setFramerateLimit(60);

    // 2. Load Textures
    if (!TextureBlockManager::setupTexture()) {
        std::cerr << "Failed to load block textures. Make sure you are running from the build directory or the paths are correct.\n";
        return -1;
    }

    // 3. Create MushroomBlock
    MushroomBlock myBlock(spawnCallback);
    
    // Set position and let it initialize originalY
    myBlock.setPosition({400.f, 300.f});

    // View to zoom in a bit since sprites are 16x16
    sf::View view(sf::FloatRect({0.f, 0.f}, {800.f, 600.f}));
    view.setCenter({400.f, 300.f});
    view.zoom(0.2f);
    window.setView(view);

    sf::Clock clock;

    // 4. Game Loop
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // Event polling
        while (std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Press SPACE to simulate hitting the block from below
            if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPress->code == sf::Keyboard::Key::Space) {
                    std::cout << "Space pressed! Simulating collision...\n";
                    myBlock.reactToCollision(COLLISION_BOTTOM);
                }
            }
        }

        // Update logic
        myBlock.update(dt);

        // Render
        window.clear(sf::Color(100, 149, 237)); // Mario sky blue
        
        myBlock.render(&window);
        
        window.display();
    }

    return 0;
}
