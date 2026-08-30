#include "Objects/Blocks/Block.hpp"
#include "Entities/Base/Character.hpp"
#include "Entities/Base/Enemy.hpp"
#include "Entities/Players/Mario.hpp"
#include "Entities/Players/Luigi.hpp"
#include "Entities/Enemies/Goomba.hpp"
#include "Entities/Enemies/Koopa.hpp"
#include "Entities/Enemies/Bowser.hpp"
#include "Objects/Items/FireFlower.hpp"
#include "Objects/Items/Fireball.hpp"
#include "Objects/Items/PowerUpObject.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>

int main() {
    std::cout << "==================================================\n";
    std::cout << "  SUPER MARIO BROS - PERSON 3 ANIMATION SHOWCASE  \n";
    std::cout << "==================================================\n";
    std::cout << "Controls:\n";
    std::cout << "  Mario (P1): [A]/[D] to Move, [W] or [J] to Jump\n";
    std::cout << "  Luigi (P2): [Left]/[Right] to Move, [Up] to Jump\n";
    std::cout << "  [B] Key: Toggle Mario Big/Small form\n";
    std::cout << "  [F] Key: Mario shoots Fireball\n";
    std::cout << "  [K] Key: Stomp/Squish Goomba and Koopa\n";
    std::cout << "==================================================\n";

    // 1. Setup global textures
    TextureBlockManager::setupTexture();
    TexturePlayerManager::setupTexture();
    TextureEnemyManager::setupTexture();
    TextureItemManager::setupTexture();

    // 2. Setup SFML Window
    sf::RenderWindow window(sf::VideoMode({960, 540}), "Super Mario Bros - Person 3 Animation Showcase");
    window.setFramerateLimit(60);

    // 3. Instantiate Entities
    Mario mario;
    mario.setPosition({100.f, 350.f});
    
    Luigi luigi;
    luigi.setPosition({250.f, 350.f});

    Goomba goomba({400.f, 350.f});
    Koopa koopa({550.f, 330.f});
    Bowser bowser({750.f, 310.f});

    std::vector<std::unique_ptr<Fireball>> fireballs;
    FireFlower flower({250.f, 350.f});

    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // 4. Input Handling for Showcase
        // Mario Controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            mario.move(-1.f, 0.f, dt);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            mario.move(1.f, 0.f, dt);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J)) {
            mario.jump();
        }

        // Luigi Controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            luigi.move(-1.f, 0.f, dt);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            luigi.move(1.f, 0.f, dt);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            luigi.jump();
        }

        while (std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPress->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
                // Toggle Big Mario
                if (keyPress->code == sf::Keyboard::Key::B) {
                    mario.setBig(!mario.isBig());
                    std::cout << "Toggled Mario Big: " << (mario.isBig() ? "ON" : "OFF") << "\n";
                }
                // Mario shoots fireball
                if (keyPress->code == sf::Keyboard::Key::F) {
                    mario.setFire(true);
                    std::cout << "Mario is now FIRE MARIO! Pew pew!\n";
                    float dir = mario.getMovementComponent()->getVelocity().x >= 0.f ? 1.f : -1.f;
                    fireballs.push_back(std::make_unique<Fireball>(mario.getPosition(), dir));
                }
                // Stomp test
                if (keyPress->code == sf::Keyboard::Key::K) {
                    goomba.onStomped();
                    koopa.onStomped();
                    std::cout << "Triggered squish/shell animations!\n";
                }
            }
        }

        // Update entities
        mario.update(dt);
        luigi.update(dt);
        goomba.update(dt);
        koopa.update(dt);
        bowser.update(dt);
        flower.update(dt);

        for (auto& fb : fireballs) {
            if (fb) fb->update(dt);
        }

        // Clean up dead fireballs
        fireballs.erase(std::remove_if(fireballs.begin(), fireballs.end(),
            [](const std::unique_ptr<Fireball>& fb) { return fb->isDead(); }),
            fireballs.end());

        // Simple Physics Integration for Showcase
        const float gravity = 1200.f;
        const float groundY = 390.f;

        if (mario.getMovementComponent()) {
            mario.getMovementComponent()->update(dt);
            mario.getMovementComponent()->setVelocity(
                mario.getMovementComponent()->getVelocity().x,
                mario.getMovementComponent()->getVelocity().y + gravity * dt
            );
            sf::Vector2f pos = mario.getPosition();
            pos += mario.getMovementComponent()->getVelocity() * dt;
            if (pos.y >= groundY) {
                pos.y = groundY;
                mario.setGrounded(true);
            }
            mario.setPosition(pos);
        }

        if (luigi.getMovementComponent()) {
            luigi.getMovementComponent()->update(dt);
            luigi.getMovementComponent()->setVelocity(
                luigi.getMovementComponent()->getVelocity().x,
                luigi.getMovementComponent()->getVelocity().y + gravity * dt
            );
            sf::Vector2f pos = luigi.getPosition();
            pos += luigi.getMovementComponent()->getVelocity() * dt;
            if (pos.y >= groundY) {
                pos.y = groundY;
                luigi.setGrounded(true);
            }
            luigi.setPosition(pos);
        }

        // Fireball Physics
        for (auto& fb : fireballs) {
            if (fb && fb->getPosition().y >= groundY) {
                sf::Vector2f pos = fb->getPosition();
                pos.y = groundY;
                fb->setPosition(pos);
                fb->reactToBlockCollision(); // Bounce
            }
        }

        // Render
        window.clear(sf::Color(100, 149, 237)); // Sky blue

        // Draw simple ground
        sf::RectangleShape ground(sf::Vector2f(960.f, 150.f));
        ground.setPosition({0.f, 390.f});
        ground.setFillColor(sf::Color(140, 70, 20));
        window.draw(ground);

        // Render all entities
        flower.render(&window);
        mario.render(&window);
        luigi.render(&window);
        goomba.render(&window);
        koopa.render(&window);
        bowser.render(&window);
        for (auto& fb : fireballs) {
            if (fb) fb->render(&window);
        }

        window.display();
    }

    return 0;
}
