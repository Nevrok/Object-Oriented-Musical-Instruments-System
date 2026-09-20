#define NOMINMAX    
#define WIN32_LEAN_AND_MEAN
#include "BassGuitar.h"
#include "ElectricGuitar.h"
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
BassGuitar::BassGuitar()
    : StringInstruments("Fender Precision", 1, "Bass", "Alder"),
    ElectronicDevice(50.0f, 100.0f), mStringCount(4) {}

// Параметризованный конструктор
BassGuitar::BassGuitar(string brend, int count, string group, string material,
    float power, float battery, int strings)
    : StringInstruments(brend, count, group, material),
    ElectronicDevice(power, battery), mStringCount(strings) {}

// Метод Play() — воспроизведение звука бас—гитары
void BassGuitar::Play() {
    const auto& t = Locale::get();
    if (mBatteryLevel <= 0) {
        cout << t.msg_needs_charge << endl;
        return;
    }
    cout << t.msg_playing << " [" << mBrend << "] — "
        << t.type_bass << t.msg_press_any_key << endl;

    // Начинаем воспроизведение звука бас—гитары
    playSound(L"bassguitarsound.mp3");

    auto start = steady_clock::now();
    float initialBattery = mBatteryLevel;
    int lastSeconds = 0;

    while (!_kbhit()) {
        sleep_for(milliseconds(50));
        auto current = steady_clock::now();
        float ms = duration_cast<milliseconds>(current - start).count();

        float drain = (mPower / 10.0f) * (ms / 1000.0f);
        mBatteryLevel = initialBattery - drain;

        if (mBatteryLevel < 0.0f) mBatteryLevel = 0.0f;
        int sec = static_cast<int>(ms / 1000.0f);
        if (sec > lastSeconds) {
            lastSeconds = sec;
            cout << t.msg_playing_battery << fixed << setprecision(1) << mBatteryLevel << "%\n";
        }

        if (mBatteryLevel <= 0.0f) {
            cout << t.msg_battery_died;
            stopSound();
            return;
        }
    }
    _getch(); // очистка буфера

    // Останавливаем воспроизведение при выходе
    stopSound();
    cout << t.msg_finished_battery << fixed << setprecision(1) << mBatteryLevel << "%.\n";
}

// Метод SetUp() — настройка бас—гитары
bool BassGuitar::SetUp() {
    const auto& t = Locale::get();
    if (IsWorking()) {
        cout << t.msg_setup_ok << " [" << mBrend << "] " << t.type_bass << endl;
        UsePower(2.0f);
        return true;
    }
    cout << t.msg_needs_charge << endl;
    return false;
}

// Метод Tune() — настройка струн бас—гитары
void BassGuitar::Tune() {
    const auto& t = Locale::get();
    cout << t.msg_tuned << " [" << mBrend << "] "
        << mStringCount << " " << t.lbl_strings
        << " | " << t.lbl_battery << fixed << setprecision(1) << mBatteryLevel << "%" << endl;
}

// Метод Print() — вывод информации о бас—гитаре
void BassGuitar::Print() {
    const auto& t = Locale::get();
    cout << t.lbl_type << GetTypeName() << endl;
    cout << t.lbl_brand << mBrend << endl;
    cout << t.lbl_group << mGroup << endl;
    cout << t.lbl_material << mMaterial << endl;
    cout << t.lbl_strings << mStringCount << endl;
    cout << t.lbl_power << mPower << " W" << endl;
    cout << t.lbl_battery << fixed << setprecision(1) << mBatteryLevel << "%" << endl;
}

// Метод GetTypeName() — получение типа инструмента
string BassGuitar::GetTypeName() const {
    return Locale::get().type_bass;
}

void BassGuitar::DecreaseStringCount() {
    if (mStringCount > 1) {
        mStringCount--;
    }

    if (mStringCount > 1) {
        cout << Locale::get().msg_string_broke << mStringCount << "\n";
        return;
    }

    cout << Locale::get().msg_under_one_string << "\n";
}
