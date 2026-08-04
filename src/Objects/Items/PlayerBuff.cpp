#include "Objects/Items/PlayerBuff.hpp"

PlayerBuff::PlayerBuff(const std::string& type, float dur)
    : buffType(type), duration(dur) {
}

const std::string& PlayerBuff::getType() const {
    return buffType;
}

bool PlayerBuff::isExpired() const {
    return expired;
}

void PlayerBuff::apply(PlayerManager* player) {
    (void)player;
}

void PlayerBuff::update(float dt) {
    if (expired) return;
    if (duration > 0.f) {
        timer += dt;
        if (timer >= duration) {
            expired = true;
        }
    }
}

void PlayerBuff::remove(PlayerManager* player) {
    (void)player;
}
