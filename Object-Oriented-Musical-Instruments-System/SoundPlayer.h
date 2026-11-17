#pragma once

class SoundPlayer {
    bool opened = false;

public:
    SoundPlayer() = default;
    ~SoundPlayer();
    SoundPlayer(const SoundPlayer&) = delete;
    SoundPlayer& operator=(const SoundPlayer&) = delete;

    bool Play(const wchar_t* filename);
    void Stop();
};
