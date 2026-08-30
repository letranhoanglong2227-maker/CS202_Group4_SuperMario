#include "Core/AssetResolver.hpp"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

std::filesystem::path AssetResolver::resolve(const std::string& relativePath) {
    std::filesystem::path target(relativePath);

    // If it's already an absolute path, just check if it exists
    if (target.is_absolute()) {
        if (std::filesystem::exists(target)) {
            return target;
        }
    }

    std::filesystem::path exeDir;
#ifdef _WIN32
    wchar_t path[MAX_PATH];
    if (GetModuleFileNameW(NULL, path, MAX_PATH)) {
        exeDir = std::filesystem::path(path).parent_path();
    }
#else
    // Fallback to current working directory for non-Windows if needed
    exeDir = std::filesystem::current_path();
#endif

    // 1. Try executable-adjacent first (e.g., build directory or install directory)
    std::filesystem::path candidate = exeDir / target;
    if (std::filesystem::exists(candidate)) {
        return candidate;
    }

    // 2. Try current working directory (useful for CTest with WORKING_DIRECTORY set to source root)
    candidate = std::filesystem::current_path() / target;
    if (std::filesystem::exists(candidate)) {
        return candidate;
    }

    throw std::runtime_error("Asset not found: " + relativePath + "\nLooked in executable directory and working directory.");
}
