#include "Objects/Items/Item.hpp"

Item::Item(const std::string& name, int val)
    : PowerUpObject(name), value(val) {
}

int Item::getValue() const {
    return value;
}

void Item::setValue(int val) {
    value = val;
}
