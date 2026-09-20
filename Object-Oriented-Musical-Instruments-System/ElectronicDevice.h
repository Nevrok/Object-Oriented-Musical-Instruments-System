#pragma once
class ElectronicDevice {
protected:
    float mPower;        // Мощность устройства в ваттах
    float mBatteryLevel; // Уровень заряда батареи в процентах

public:
    ElectronicDevice();
    ElectronicDevice(float power, float battery);
    virtual ~ElectronicDevice();

    // Геттеры и сеттеры
    float GetPower()   const { return mPower; }
    float GetBattery() const { return mBatteryLevel; }

    void SetPower(float p) { mPower = p; }
    void SetBattery(float b) { mBatteryLevel = b; }

    // Виртуальные методы
    virtual void Charge();
    void UsePower(float amount);
    bool IsWorking() const;

    // Дружественный унарный оператор —— для использования 10% заряда
    // Объявляется как friend non—member, реализация в .cpp
    friend ElectronicDevice& operator--(ElectronicDevice&);

    // Бинарный оператор == для сравнения мощности
    bool operator==(const ElectronicDevice& other) const;
};