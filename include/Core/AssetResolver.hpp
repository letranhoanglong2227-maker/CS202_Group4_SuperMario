#pragma once

#include <filesystem>
#include <string>
#include <stdexcept>

class AssetResolver {
public:
    /**
     * @brief Resolves the absolute path to a given asset.
     * @param relativePath The relative path of the asset (e.g., "assets/textures/Items_Blocks.png")
     * @return The absolute path to the asset.
     * @throws std::runtime_error if the asset cannot be found.
     */
    static std::filesystem::path resolve(const std::string& relativePath);
};
