#include "Objects/Blocks/Block.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

// We need a concrete subclass to instantiate Block since Block is abstract
class DummyBlock : public Block {
public:
    void initSpritesSheet() override {}
    void reactToCollision(int) override {}
    void update(float) override {}
    
    // Helper to expose sprite for testing
    sf::Sprite& getSprite() { return entitySprite; }
};

bool check(bool condition, const std::string& name) {
    if (condition) {
        std::cout << "[PASS] " << name << "\n";
        return true;
    } else {
        std::cerr << "[FAIL] " << name << "\n";
        return false;
    }
}

int main() {
    bool passed = true;

    {
        DummyBlock block;
        
        // Setup initial texture rect to simulate a 16x16 frame
        block.getSprite().setTextureRect(sf::IntRect({0, 0}, {16, 16}));

        // Test setSize(Vector2f)
        block.setSize(sf::Vector2f(64.f, 64.f));
        passed &= check(block.getSize() == sf::Vector2f(64.f, 64.f), "GameObject size updated via setSize(Vector2f)");
        passed &= check(block.getSprite().getScale() == sf::Vector2f(4.f, 4.f), "Sprite scaled correctly (64/16 = 4)");

        // Test setSize(float, float)
        block.setSize(32.f, 32.f);
        passed &= check(block.getSize() == sf::Vector2f(32.f, 32.f), "GameObject size updated via setSize(float, float)");
        passed &= check(block.getSprite().getScale() == sf::Vector2f(2.f, 2.f), "Sprite scaled correctly (32/16 = 2)");
        
        // Test setSizeBlock (backward compatibility)
        block.setSizeBlock(sf::Vector2f(16.f, 16.f));
        passed &= check(block.getSize() == sf::Vector2f(16.f, 16.f), "GameObject size updated via setSizeBlock");
        passed &= check(block.getSprite().getScale() == sf::Vector2f(1.f, 1.f), "Sprite scaled correctly via setSizeBlock (16/16 = 1)");
    }

    {
        // Test Zero/Empty Rect policy
        DummyBlock block;
        // SFML default rect size is 0x0
        block.getSprite().setTextureRect(sf::IntRect({0, 0}, {0, 0}));
        block.setSize(32.f, 32.f);
        // The policy is to assume 16x16 if frame size is <= 0
        passed &= check(block.getSprite().getScale() == sf::Vector2f(2.f, 2.f), "Zero Rect safely treated as 16x16");
    }
    
    {
        // Test Repeated scaling idempotence
        DummyBlock block;
        block.getSprite().setTextureRect(sf::IntRect({0, 0}, {16, 16}));
        block.setSize(64.f, 64.f);
        block.setSize(64.f, 64.f); // Repeated call
        passed &= check(block.getSprite().getScale() == sf::Vector2f(4.f, 4.f), "Repeated setSize does not double-scale");
    }

    return passed ? 0 : 1;
}
