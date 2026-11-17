#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include "SoundPlayer.h"
#include "RuntimeFiles.h"
#include "Localization.h"
#include <iostream>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

SoundPlayer::~SoundPlayer() {
    Stop();
}

bool SoundPlayer::Play(const wchar_t* filename) {
    Stop();
    auto path = findBundledFile(filename);
    std::error_code error;
    if (!std::filesystem::is_regular_file(path, error)) {
        std::cout << Locale::get().msg_sound_missing << toUtf8(path.wstring()) << '\n';
        return false;
    }

    std::wstring command = L"open \"" + path.wstring() + L"\" type mpegvideo alias sound";
    MCIERROR status = mciSendStringW(command.c_str(), nullptr, 0, nullptr);
    if (!status) {
        opened = true;
        status = mciSendStringW(L"play sound", nullptr, 0, nullptr);
    }
    if (!status) return true;

    wchar_t message[256]{};
    if (mciGetErrorStringW(status, message, static_cast<UINT>(std::size(message)))) {
        std::cout << "MCI: " << toUtf8(message) << '\n';
    } else {
        std::cout << "MCI: " << status << '\n';
    }
    Stop();
    return false;
}

void SoundPlayer::Stop() {
    if (opened) {
        mciSendStringW(L"stop sound", nullptr, 0, nullptr);
        mciSendStringW(L"close sound", nullptr, 0, nullptr);
        opened = false;
    }
}
