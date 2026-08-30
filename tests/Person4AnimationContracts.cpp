#include "Components/AnimationComponent.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

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

    // Dummy texture and sprite for testing
    sf::Texture texture;
    // texture.create is removed in SFML3 or changed. Not strictly needed for logic testing.
    sf::Sprite sprite(texture);

    // Test_Zero_Interval
    {
        AnimationComponent anim(sprite, texture, 0.f);
        std::vector<sf::IntRect> frames = { sf::IntRect({0,0}, {16,16}), sf::IntRect({16,0}, {16,16}) };
        anim.addAnimation("idle", frames);
        anim.play("idle", 0.f); // start
        anim.play("idle", 0.0015f); // dt > 0.001f, should advance 1 frame
        passed &= check(sprite.getTextureRect().position.x == 16, "Zero interval is safely corrected");
    }

    {
        AnimationComponent anim(sprite, texture, 0.1f);
        std::vector<sf::IntRect> validFrames = { sf::IntRect({0,0}, {16,16}), sf::IntRect({16,0}, {16,16}), sf::IntRect({32,0}, {16,16}) };
        
        // Test_Valid_Registration
        bool added = anim.addAnimation("walk", validFrames);
        passed &= check(added, "Register valid key returns true");

        // Test_Duplicate_Key
        added = anim.addAnimation("walk", validFrames);
        passed &= check(!added, "Register duplicate key returns false");

        // Test_Empty_Frames
        added = anim.addAnimation("empty", {});
        passed &= check(!added, "Register empty frames returns false");

        // Test_Missing_Key
        bool played = anim.play("missing_key", 0.016f);
        passed &= check(!played, "Play missing key returns false without crash");

        // Test_Immediate_Frame_Zero
        anim.play("walk", 0.f); // Initial play, dt = 0
        passed &= check(sprite.getTextureRect().position.x == 0, "Play new key sets frame 0 immediately");

        // Test_Exact_Interval
        // dt exactly equals interval (0.1f)
        anim.play("walk", 0.1f);
        passed &= check(sprite.getTextureRect().position.x == 16, "dt equals interval exactly advances 1 frame");

        // Test_Deterministic_Time
        // Current frame index is 1 (left=16). Interval is 0.1f. 
        // We pass dt = 0.25f (2.5 intervals). Should advance 2 frames, remainder 0.05f.
        anim.play("walk", 0.25f);
        // Frame index was 1 -> 1+2 = 3. frames.size() is 3, so 3 % 3 = 0. (position.x=0)
        passed &= check(sprite.getTextureRect().position.x == 0, "Large dt advances deterministic frames");

        // Now pass 0.05f. The remainder was 0.05f. 0.05 + 0.05 = 0.1f. Should advance 1 frame.
        anim.play("walk", 0.05f);
        passed &= check(sprite.getTextureRect().position.x == 16, "Excess time remainder is kept and used correctly");

        // Test_Key_Transition A -> B -> A
        std::vector<sf::IntRect> jumpFrames = { sf::IntRect({64,0}, {16,16}) };
        anim.addAnimation("jump", jumpFrames);

        anim.play("jump", 0.1f);
        passed &= check(sprite.getTextureRect().position.x == 64, "Transition A -> B works");
        
        anim.play("walk", 0.f); // Switch back with 0 dt to verify it resets immediately to frame 0
        passed &= check(sprite.getTextureRect().position.x == 0, "Transition B -> A resets frame to 0 immediately");
    }

    return passed ? 0 : 1;
}
