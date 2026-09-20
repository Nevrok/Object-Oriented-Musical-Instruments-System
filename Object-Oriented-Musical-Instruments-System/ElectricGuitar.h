#pragma once
#include "StringInstruments.h"
#include "ElectronicDevice.h"
using namespace std;

class ElectricGuitar : public StringInstruments, public ElectronicDevice {
private:
    int mPickupCount;  // Количество звукоснимателей

public:
    ElectricGuitar();
    ElectricGuitar(string brend, int count, string group, string material,
        float power, float battery, int pickups);

    // Геттеры и сеттеры
    int GetPickupCount() const { return mPickupCount; }
    void SetPickupCount(int v) { mPickupCount = v; }

    // Переопределенные методы базового класса
    void   Play()        override;
    bool   SetUp()       override;
    void   Tune()        override;
    void   Print()       override;
    string GetTypeName() const override;


    // Дружественный унарный оператор —— для уменьшения количества струн
    friend ElectricGuitar& operator--(ElectricGuitar&);
};