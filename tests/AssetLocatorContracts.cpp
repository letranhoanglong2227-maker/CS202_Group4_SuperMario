#include "Core/AssetLocator.hpp"

#include <cassert>
#include <filesystem>
#include <iostream>

int main() {
    const auto atlas = AssetLocator::find("assets/textures/Enemies.png");
    assert(atlas);
    assert(std::filesystem::is_regular_file(*atlas));
    assert(!AssetLocator::find("assets/definitely-missing.asset"));
    assert(AssetLocator::missingMessage("assets/definitely-missing.asset")
               .find("definitely-missing") != std::string::npos);
    std::cout << "Asset locator contracts passed\n";
}
