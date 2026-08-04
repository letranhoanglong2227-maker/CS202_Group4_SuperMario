#include "Objects/Items/FireBuff.hpp"
#include "Entities/Players/PlayerManager.hpp"

FireBuff::FireBuff(float dur)
    : PlayerBuff("FireBuff", dur) {
}

void FireBuff::apply(PlayerManager* player) {
    if (player) {
        player->setFire(true);
    }
}

void FireBuff::remove(PlayerManager* player) {
    if (player) {
        player->setFire(false);
    }
}
