#include "ElectricGuitar.h"
#include "Localization.h"
#include "SoundPlayer.h"
#include <algorithm>
#include <chrono>
#include <conio.h>
#include <iomanip>
#include <thread>

using namespace std;
using namespace chrono;
using namespace this_thread;

// Конструктор по умолчанию
ElectricGuitar::ElectricGuitar()
    : StringInstruments("Gibson SG", 6, "Electric", "Mahogany"),
    ElectronicDevice(100.0f, 100.0f), mPickupCount(2) {}

// Параметризованный конструктор
ElectricGuitar::ElectricGuitar(string brend, int count, string group, string material,
    float power, float battery, int pickups)
    : StringInstruments(brend, count, group, material),
    ElectronicDevice(power, battery), mPickupCount(pickups) {}

// Метод Play() — воспроизведение звука электрогитары
void ElectricGuitar::Play() {
    const auto& t = Locale::get();
    if (mBatteryLevel <= 0) {
        cout << t.msg_needs_charge << endl;
        return;
    }
    cout << t.msg_playing << " [" << mBrend << "] — "
        << t.type_electric << t.msg_press_any_key << endl;

    // Начинаем воспроизведение звука электрогитары
    SoundPlayer sound;
    sound.Play(L"electricguitarsound.mp3");

    auto start = steady_clock::now();
    float initialBattery = mBatteryLevel;
    int lastSeconds = 0;

    while (!_kbhit()) {
        sleep_for(milliseconds(50));
        auto current = steady_clock::now();
        float elapsed = duration<float>(current - start).count();
        float drain = (mPower / 10.0f) * elapsed;
        mBatteryLevel = max(0.0f, initialBattery - drain);

        int sec = static_cast<int>(elapsed);
        if (sec > lastSeconds) {
            lastSeconds = sec;
            cout << t.msg_playing_battery << fixed << setprecision(1) << mBatteryLevel << "%\n";
        }

        if (mBatteryLevel <= 0.0f) {
            cout << t.msg_battery_died;
            sound.Stop();
            return;
        }
    }
    _getch(); // очистка буфера

    // Останавливаем воспроизведение при выходе
    sound.Stop();
    cout << t.msg_finished_battery << fixed << setprecision(1) << mBatteryLevel << "%.\n";
}

// Метод SetUp() — настройка электрогитары
bool ElectricGuitar::SetUp() {
    const auto& t = Locale::get();
    if (IsWorking()) {
        cout << t.msg_setup_ok << " [" << mBrend << "] " << t.type_electric << endl;
        UsePower(2.0f);
        return true;
    }
    cout << t.msg_needs_charge << endl;
    return false;
}

// Метод Tune() — настройка струн электрогитары
void ElectricGuitar::Tune() {
    const auto& t = Locale::get();
    cout << t.msg_tuned << " [" << mBrend << "] "
        << mCount << " " << t.lbl_strings
        << " | " << mPickupCount << " " << t.lbl_pickups << endl;
}

// Метод Print() — вывод информации об электрогитаре
void ElectricGuitar::Print() {
    const auto& t = Locale::get();
    StringInstruments::Print();
    cout << t.lbl_pickups << mPickupCount << endl;
    cout << t.lbl_power << mPower << " W" << endl;
    cout << t.lbl_battery << fixed << setprecision(1) << mBatteryLevel << "%" << endl;
}

// Метод GetTypeName() — получение типа инструмента
string ElectricGuitar::GetTypeName() const {
    return Locale::get().type_electric;
}


// Унарный оператор —— для уменьшения количества струн
// Уменьшает количество струн, но не менее 1
ElectricGuitar& operator--(ElectricGuitar& obj) {
    if (obj.mCount > 1) {
        obj.mCount--;
        cout << Locale::get().msg_string_broke << obj.mCount << "\n";
    }
    else {
        cout << Locale::get().msg_under_one_string << "\n";
    }
    return obj;
}
