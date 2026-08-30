#pragma once

enum class PlayerEnemyContactKind {
    Stomp,   // Player hits top of enemy
    Side     // Player hits side/bottom of enemy
};

enum class EnemyContactResult {
    None,
    EnemyStomped,
    PlayerDamaged,
    PlayerKilled,
    ShellStopped,
    ShellKicked,
    HazardousShell
};

struct EnemyContactOutcome {
    EnemyContactResult result{ EnemyContactResult::None };
    int scoreDelta{ 0 };
    float bounceVelocity{ 0.f };
    bool enemyKilled{ false };
};
