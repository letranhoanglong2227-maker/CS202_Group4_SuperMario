#include "Components/AnimationComponent.hpp"

#include <SFML/Graphics.hpp>

#include <cassert>
#include <iostream>

int main() {
    sf::Texture texture;
    sf::Sprite sprite(texture);
    AnimationComponent animation(sprite, texture, 0.1f);

    assert(animation.addAnimation("walk", {
        {{0, 0}, {16, 16}}, {{16, 0}, {16, 16}}, {{32, 0}, {16, 16}}
    }));
    assert(sprite.getTextureRect() == sf::IntRect({0, 0}, {16, 16}));
    assert(!animation.addAnimation("walk", {{{48, 0}, {16, 16}}}));
    assert(!animation.addAnimation("empty", {}));
    assert(!animation.play("missing", 0.016f));

    assert(animation.play("walk", 0.25f));
    assert(sprite.getTextureRect() == sf::IntRect({32, 0}, {16, 16}));
    assert(animation.play("walk", 0.05f));
    assert(sprite.getTextureRect() == sf::IntRect({0, 0}, {16, 16}));

    assert(animation.addAnimation("jump", {{{64, 0}, {16, 16}}}));
    assert(animation.play("jump", 0.f));
    assert(sprite.getTextureRect() == sf::IntRect({64, 0}, {16, 16}));

    AnimationComponent zeroInterval(sprite, texture, 0.f);
    assert(zeroInterval.addAnimation("safe", {
        {{0, 0}, {16, 16}}, {{16, 0}, {16, 16}}
    }));
    assert(zeroInterval.play("safe", 0.001f));
    assert(sprite.getTextureRect() == sf::IntRect({16, 0}, {16, 16}));

    std::cout << "Person 4 animation contracts passed\n";
    return 0;
}
