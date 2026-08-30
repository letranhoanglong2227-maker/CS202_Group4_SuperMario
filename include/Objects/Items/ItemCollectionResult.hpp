#pragma once

enum class RequestedPlayerForm {
    None,
    Big,
    Fire,
    Invincible
};

struct ItemCollectionResult {
    bool consumed{ false };
    int scoreDelta{ 0 };
    int coinDelta{ 0 };
    int lifeDelta{ 0 };
    RequestedPlayerForm requestedForm{ RequestedPlayerForm::None };
};
