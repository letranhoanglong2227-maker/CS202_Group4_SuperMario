#pragma once

#include <memory>
#include <string>

#include "Base/Character.hpp"
#include "../Objects/Items/Item.hpp"

class EntityFactory {
public:
    static std::unique_ptr<Character> createEnemy(const std::string& type);
    static std::unique_ptr<Item> createItem(const std::string& type);
};
