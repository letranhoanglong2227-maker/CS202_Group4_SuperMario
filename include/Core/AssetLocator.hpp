#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace AssetLocator {

[[nodiscard]] std::filesystem::path executableDirectory();
[[nodiscard]] std::optional<std::filesystem::path> find(
    const std::filesystem::path& relativePath);
[[nodiscard]] std::string missingMessage(
    const std::filesystem::path& relativePath);

}
