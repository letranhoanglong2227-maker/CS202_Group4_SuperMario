#include "Core/AssetLocator.hpp"

#include <array>
#include <system_error>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#endif

namespace {
std::filesystem::path normalizedExisting(
    const std::filesystem::path& candidate) {
    std::error_code error;
    if (!std::filesystem::exists(candidate, error) || error) return {};
    const auto normalized = std::filesystem::weakly_canonical(candidate, error);
    return error ? candidate.lexically_normal() : normalized;
}
}

std::filesystem::path AssetLocator::executableDirectory() {
#ifdef _WIN32
    std::vector<wchar_t> buffer(512);
    for (;;) {
        const DWORD length = GetModuleFileNameW(
            nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
        if (length == 0) break;
        if (length < buffer.size() - 1) {
            return std::filesystem::path(
                       std::wstring_view(buffer.data(), length))
                .parent_path();
        }
        buffer.resize(buffer.size() * 2);
    }
#elif defined(__linux__)
    std::array<char, 4096> buffer{};
    const ssize_t length = readlink(
        "/proc/self/exe", buffer.data(), buffer.size() - 1);
    if (length > 0) {
        return std::filesystem::path(
                   std::string_view(buffer.data(), static_cast<std::size_t>(length)))
            .parent_path();
    }
#endif
    std::error_code error;
    const auto current = std::filesystem::current_path(error);
    return error ? std::filesystem::path{} : current;
}

std::optional<std::filesystem::path> AssetLocator::find(
    const std::filesystem::path& relativePath) {
    if (relativePath.empty()) return std::nullopt;
    if (relativePath.is_absolute()) {
        const auto resolved = normalizedExisting(relativePath);
        return resolved.empty() ? std::nullopt
                                : std::optional<std::filesystem::path>(resolved);
    }

    std::error_code error;
    const auto current = std::filesystem::current_path(error);
    const auto executable = executableDirectory();
    const std::array roots{
        executable,
        executable.parent_path(),
        error ? std::filesystem::path{} : current,
    };
    for (const auto& root : roots) {
        if (root.empty()) continue;
        const auto resolved = normalizedExisting(root / relativePath);
        if (!resolved.empty()) return resolved;
    }
    return std::nullopt;
}

std::string AssetLocator::missingMessage(
    const std::filesystem::path& relativePath) {
    return "Missing asset '" + relativePath.generic_string() +
           "' beside the executable or in the working directory";
}
