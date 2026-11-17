#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <windows.h>

inline std::string toUtf8(std::wstring_view text) {
    if (text.empty()) return {};

    int length = WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()),
        nullptr, 0, nullptr, nullptr);
    std::string result(length, '\0');
    if (length > 0) {
        WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()),
            result.data(), length, nullptr, nullptr);
    }
    return result;
}

inline std::filesystem::path findBundledFile(const std::filesystem::path& name) {
    std::wstring executable(32768, L'\0');
    DWORD length = GetModuleFileNameW(nullptr, executable.data(), static_cast<DWORD>(executable.size()));
    if (length > 0 && length < executable.size()) {
        executable.resize(length);
        auto bundled = std::filesystem::path(executable).parent_path() / name;
        std::error_code error;
        if (std::filesystem::is_regular_file(bundled, error)) return bundled;
    }
    return name;
}
