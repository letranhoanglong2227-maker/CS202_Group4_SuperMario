#include "Objects/Blocks/Block.hpp"
#include "Objects/Blocks/MushroomBlock.hpp"
#include "Objects/Blocks/CoinBlock.hpp"
#include "Objects/Blocks/Brick.hpp"
#include "Objects/Blocks/SolidBlock.hpp"
#include "Objects/Environment/Trampoline.hpp"
#include "Entities/Base/Character.hpp"
#include "Entities/Base/Enemy.hpp"
#include "Entities/Players/Mario.hpp"
#include "Entities/Players/Luigi.hpp"
#include "Entities/Enemies/Goomba.hpp"
#include "Entities/Enemies/Koopa.hpp"
#include "Entities/Enemies/FlyingKoopa.hpp"
#include "Entities/Enemies/Heriss.hpp"
#include "Entities/Enemies/Bowser.hpp"
#include "Entities/Enemies/PeteyPiranha.hpp"
#include "Objects/Items/FireFlower.hpp"
#include "Objects/Items/Fireball.hpp"
#include "Objects/Items/PowerUpObject.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>

std::vector<std::unique_ptr<GameObject>> activeEntities;

void spawnCallback(GameObject* newObj) {
    if (newObj) {
        std::cout << "Spawned new object!\n";
        activeEntities.push_back(std::unique_ptr<GameObject>(newObj));
    }
}

int main() {
    std::cout << "==================================================\n";
    std::cout << "  SUPER MARIO BROS - THE MEGA SHOWCASE            \n";
    std::cout << "==================================================\n";
    std::cout << "Controls:\n";
    std::cout << "  [A]/[D] Move Mario, [W] Jump Mario\n";
    std::cout << "  [B] Mario Toggle Big/Small\n";
    std::cout << "  [F] Mario Shoot Fireball\n";
    std::cout << "  [K] Trigger Squish on Goomba/Koopa\n";
    std::cout << "  [Space] Simulate hit on Blocks from below\n";
    std::cout << "==================================================\n";

    TextureBlockManager::setupTexture();
    TexturePlayerManager::setupTexture();
    TextureEnemyManager::setupTexture();
    TextureItemManager::setupTexture();

    sf::RenderWindow window(sf::VideoMode({1200, 600}), "Super Mario Bros - Mega Showcase");
    window.setFramerateLimit(60);

    // Characters
    auto mario = std::make_unique<Mario>();
    mario->setPosition({50.f, 350.f});
    
    // Enemies
    auto goomba = std::make_unique<Goomba>(sf::Vector2f{250.f, 350.f});
    auto koopa = std::make_unique<Koopa>(sf::Vector2f{350.f, 330.f});
    auto flyingKoopa = std::make_unique<FlyingKoopa>(sf::Vector2f{450.f, 200.f});
    auto heriss = std::make_unique<Heriss>(sf::Vector2f{550.f, 350.f});
    auto piranha = std::make_unique<PeteyPiranha>(sf::Vector2f{650.f, 350.f}); // Hoa an thit
    auto bowser = std::make_unique<Bowser>(sf::Vector2f{900.f, 300.f});

    // Items
    auto flower = std::make_unique<FireFlower>(sf::Vector2f{200.f, 250.f});
    
    // Blocks
    auto mushroomBlock = std::make_unique<MushroomBlock>(spawnCallback);
    mushroomBlock->setPosition({400.f, 200.f});
    
    auto coinBlock = std::make_unique<CoinBlock>(1, spawnCallback);
    coinBlock->setPosition({500.f, 200.f});
    
    auto brick = std::make_unique<Brick>(spawnCallback);
    brick->setPosition({600.f, 200.f});
    
    auto solidBlock = std::make_unique<SolidBlock>();
    solidBlock->setPosition({700.f, 200.f});

    // Trampoline
    auto trampoline = std::make_unique<Trampoline>(sf::Vector2f{800.f, 350.f}, -800.f);

    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // Mario Controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            mario->move(-1.f, 0.f, dt);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            mario->move(1.f, 0.f, dt);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            mario->jump();
        }

        while (std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPress->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
                if (keyPress->code == sf::Keyboard::Key::B) {
                    mario->setBig(!mario->isBig());
                }
                if (keyPress->code == sf::Keyboard::Key::F) {
                    mario->setFire(true);
                    float dir = mario->getMovementComponent()->getVelocity().x >= 0.f ? 1.f : -1.f;
                    auto fb = std::make_unique<Fireball>(mario->getPosition(), dir);
                    activeEntities.push_back(std::move(fb));
                }
                if (keyPress->code == sf::Keyboard::Key::K) {
                    goomba->onStomped();
                    koopa->onStomped();
                }
                if (keyPress->code == sf::Keyboard::Key::Space) {
                    mushroomBlock->reactToCollision(3); // 3 = COLLISION_BOTTOM
                    coinBlock->reactToCollision(3);
                    brick->reactToCollision(3);
                    trampoline->reactToCollision(1); // 1 = COLLISION_TOP
                }
            }
        }

        // Update entities
        mario->update(dt);
        goomba->update(dt);
        koopa->update(dt);
        flyingKoopa->update(dt);
        heriss->update(dt);
        piranha->update(dt);
        bowser->update(dt);
        flower->update(dt);
        
        mushroomBlock->update(dt);
        coinBlock->update(dt);
        brick->update(dt);
        solidBlock->update(dt);
        trampoline->update(dt);

        for (auto& ent : activeEntities) {
            if (ent) ent->update(dt);
        }

        // Simple Physics Integration for Mario
        const float gravity = 1200.f;
        const float groundY = 390.f;

        if (mario->getMovementComponent()) {
            mario->getMovementComponent()->update(dt);
            mario->getMovementComponent()->setVelocity(
                mario->getMovementComponent()->getVelocity().x,
                mario->getMovementComponent()->getVelocity().y + gravity * dt
            );
            sf::Vector2f pos = mario->getPosition();
            pos += mario->getMovementComponent()->getVelocity() * dt;
            if (pos.y >= groundY) {
                pos.y = groundY;
                mario->setGrounded(true);
            }
            mario->setPosition(pos);
        }

        for (auto& ent : activeEntities) {
            // Apply gravity to spawned items like Mushroom, Coin, Fireballs
            auto* pUp = dynamic_cast<PowerUpObject*>(ent.get());
            if (pUp) {
                // simple ground check for items
                if (pUp->getPosition().y >= groundY) {
                    sf::Vector2f p = pUp->getPosition();
                    p.y = groundY;
                    pUp->setPosition(p);
                    pUp->reactToBlockCollision(nullptr);
                }
            }
            auto* fb = dynamic_cast<Fireball*>(ent.get());
            if (fb && fb->getPosition().y >= groundY) {
                sf::Vector2f p = fb->getPosition();
                p.y = groundY;
                fb->setPosition(p);
                fb->reactToBlockCollision();
            }
        }

        // Clean dead entities (like broken bricks, dead fireballs)
        // ... (skipping for showcase simplicity)

        // Render
        window.clear(sf::Color(100, 149, 237)); // Sky blue

        // Draw simple ground
        sf::RectangleShape ground(sf::Vector2f(1200.f, 210.f));
        ground.setPosition({0.f, 390.f});
        ground.setFillColor(sf::Color(140, 70, 20));
        window.draw(ground);

        // Render all entities
        mario->render(&window);
        goomba->render(&window);
        koopa->render(&window);
        flyingKoopa->render(&window);
        heriss->render(&window);
        piranha->render(&window);
        bowser->render(&window);
        flower->render(&window);
        
        mushroomBlock->render(&window);
        coinBlock->render(&window);
        brick->render(&window);
        solidBlock->render(&window);
        trampoline->render(&window);

        for (auto& ent : activeEntities) {
            if (ent) ent->render(&window);
        }

        window.display();
    }

    return 0;
}
