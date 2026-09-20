#define NOMINMAX    
#define WIN32_LEAN_AND_MEAN
#include "AcousticGuitar.h"
#include "Localization.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>
#include <io.h>
#include <fcntl.h>
#include <cstdio>
#include <errno.h>
#pragma comment(lib, "winmm.lib")
using namespace std;
using namespace chrono;
using namespace this_thread;

// Воспроизведение MP3 файла
static void playSound(const wchar_t* filename) {
    // Проверяем существование файла
    FILE* file = nullptr;
    errno_t ferr = _wfopen_s(&file, filename, L"rb");
    if (ferr != 0 || !file) {
        wprintf(L"Warning: Sound file not found: %s\n", filename);
        return;
    }
    fclose(file);

    wchar_t command[512];

    swprintf(
        command,
        sizeof(command) / sizeof(wchar_t),
        L"open \"%s\" type mpegvideo alias sound",
        filename
    );

    MCIERROR error = mciSendStringW(command, NULL, 0, NULL);
    if (error) {
        wchar_t errorText[256];
        mciGetErrorStringW(error, errorText, 256);
        wprintf(L"MCI Error: %s\n", errorText);
        return;
    }


    error = mciSendStringW(L"play sound notify", NULL, 0, NULL);
    if (error) {
        wprintf(L"MCI Play Error: %d\n", error);
    }
}

// Остановка воспроизведения
static void stopSound() {
    mciSendStringW(L"stop sound", NULL, 0, NULL);
    mciSendStringW(L"close sound", NULL, 0, NULL);
}

// Конструктор по умолчанию
AcousticGuitar::AcousticGuitar()
    : StringInstruments("Yamaha F310", 6, "Acoustic", "Spruce"),
    mColorKant("Ivory"), mSize(41.0f), mColorKorpus("Natural"),
    mMatGrif("Maple"), mMatString("Nylon") {}

// Параметризованный конструктор
AcousticGuitar::AcousticGuitar(string brend, int count, string group, string material,
    string cKant, float size, string cKorpus,
    string mGrif, string mString)
    : StringInstruments(brend, count, group, material),
    mColorKant(cKant), mSize(size), mColorKorpus(cKorpus),
    mMatGrif(mGrif), mMatString(mString) {}

// Метод Play() — воспроизведение звука акустической гитары
void AcousticGuitar::Play() {
    const auto& t = Locale::get();
    if (mCount <= 0) {
        cout << t.msg_invalid << t.msg_no_strings << "\n";
        return;
    }
    cout << t.msg_playing << " [" << mBrend << "] — "
        << t.type_acoustic << t.msg_press_any_key << endl;
    cout << t.msg_press_exclaim_to_break << endl;

    // Начинаем воспроизведение звука акустической гитары
    playSound(L"acousticguitarsound.mp3");

    auto lastPing = steady_clock::now();
    while (true) {
        if (_kbhit()) {
            int key = _getch();
            if (key == '!') {
                // Рвем струну по нажатию '!'
                if (mCount > 0) {
                    // Останавливаем основной звук и воспроизводим звук разрыва струны
                    stopSound();
                    playSound(L"brokestringsound.mp3");
                    mCount--;
                    cout << t.msg_string_broke << mCount << "\n";
                    sleep_for(milliseconds(4000)); // Даем звук разрыва проиграться

                    if (mCount <= 0) {
                        cout << t.msg_cannot_play;
                        stopSound();
                        return;
                    }
                    stopSound();
                    // Начинаем игру заново
                    playSound(L"acousticguitarsound.mp3");
                }
            }
            else {
                // Любая другая клавиша — выход
                break;
            }
        }
        // Периодически выводим, что акустическая гитара играет (раз в 2 секунды)
        auto now = steady_clock::now();
        if (duration_cast<seconds>(now - lastPing).count() >= 2) {
            cout << t.msg_playing << " [" << mBrend << "]" << endl;
            lastPing = now;
        }
        sleep_for(milliseconds(50));
    }
    // Останавливаем воспроизведение при выходе
    stopSound();
    cout << t.msg_finished_playing;
}
// Метод SetUp() — настройка акустической гитары
bool AcousticGuitar::SetUp() {
    const auto& t = Locale::get();
    cout << t.msg_setup_ok << " [" << mBrend << "] "
        << t.type_acoustic << endl;
    return true;
}
// Метод Tune() — настройка струн акустической гитары
void AcousticGuitar::Tune() {
    const auto& t = Locale::get();
    cout << t.msg_tuned << " [" << mBrend << "] "
        << mCount << " " << t.lbl_strings << endl;
}
// Метод Print() — вывод информации об акустической гитаре
void AcousticGuitar::Print() {
    const auto& t = Locale::get();
    StringInstruments::Print();
    cout << t.lbl_rim_color << mColorKant << endl;
    cout << t.lbl_body_size << mSize << "\"" << endl;
    cout << t.lbl_body_color << mColorKorpus << endl;
    cout << t.lbl_neck_mat << mMatGrif << endl;
    cout << t.lbl_string_mat << mMatString << endl;
}
// Метод GetTypeName() — получение типа инструмента
string AcousticGuitar::GetTypeName() const {
    return Locale::get().type_acoustic;
}
AcousticGuitar& AcousticGuitar::operator!() {
    if (mCount > 0) {
        mCount--;
    }
    return *this;
}


