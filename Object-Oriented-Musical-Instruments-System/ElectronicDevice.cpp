#include "ElectronicDevice.h"
#include "Localization.h"
#include <iostream>
using namespace std;

// Конструктор по умолчанию
ElectronicDevice::ElectronicDevice()
    : mPower(10.0f), mBatteryLevel(100.0f) {}

// Параметризованный конструктор
ElectronicDevice::ElectronicDevice(float power, float battery)
    : mPower(power), mBatteryLevel(battery) {}

// Деструктор
ElectronicDevice::~ElectronicDevice() {}

// Метод Charge() — полная зарядка устройства
void ElectronicDevice::Charge() {
    mBatteryLevel = 100.0f;
    cout << Locale::get().msg_charged << endl;
}

// Метод UsePower() — использование энергии
void ElectronicDevice::UsePower(float amount) {
    mBatteryLevel -= amount;
    if (mBatteryLevel < 0.0f) mBatteryLevel = 0.0f;
}

// Метод IsWorking() — проверка работоспособности
bool ElectronicDevice::IsWorking() const {
    return mBatteryLevel > 0.0f;
}

// Унарный оператор —— для использования 10% заряда
// Уменьшает уровень заряда на 10%
ElectronicDevice& operator--(ElectronicDevice& obj) {
    obj.UsePower(10.0f);
    return obj;
}

// Бинарный оператор == для сравнения мощности
bool ElectronicDevice::operator==(const ElectronicDevice& other) const {
    return mPower == other.mPower;
}
