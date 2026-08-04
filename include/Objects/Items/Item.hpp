#pragma once

#include "Objects/Items/PowerUpObject.hpp"

class Item : public PowerUpObject {
protected:
    int value{ 0 };

public:
    Item(const std::string& name = "Item", int value = 0);
    virtual ~Item() = default;

    int getValue() const;
    void setValue(int val);
};
