#pragma once
#include "StringInstruments.h"
#include "ElectronicDevice.h"
using namespace std;

class BassGuitar : public StringInstruments, public ElectronicDevice {
private:
    int mStringCount;  // Количество струн бас—гитары

public:
    BassGuitar();
    BassGuitar(string brend, int count, string group, string material,
        float power, float battery, int strings);

    // Геттеры и сеттеры
    int GetStringCount() const { return mStringCount; }
    void SetStringCount(int v) { mStringCount = v; }

    void DecreaseStringCount();

    // Переопределенные методы базового класса
    void   Play()        override;
    bool   SetUp()       override;
    void   Tune()        override;
    void   Print()       override;
    string GetTypeName() const override;
};
