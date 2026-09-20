#pragma once
#include "StringInstruments.h"
using namespace std;

class AcousticGuitar : public StringInstruments {
private:
    string mColorKant;      // Цвет канта
    float  mSize;           // Размер корпуса в дюймах
    string mColorKorpus;    // Цвет корпуса
    string mMatGrif;        // Материал грифа
    string mMatString;      // Материал струн

public:
    AcousticGuitar();
    AcousticGuitar(string brend, int count, string group, string material,
        string cKant, float size, string cKorpus,
        string mGrif, string mString);

    // Геттеры
    string GetColorKant()   const { return mColorKant; }
    float  GetSize()        const { return mSize; }
    string GetColorKorpus() const { return mColorKorpus; }
    string GetMatGrif()     const { return mMatGrif; }
    string GetMatString()   const { return mMatString; }

    // Сеттеры
    void SetColorKant(string v) { mColorKant = v; }
    void SetSize(float v) { mSize = v; }
    void SetColorKorpus(string v) { mColorKorpus = v; }
    void SetMatGrif(string v) { mMatGrif = v; }
    void SetMatString(string v) { mMatString = v; }

    // Переопределенные методы базового класса
    void   Play()         override;
    bool   SetUp()        override;
    void   Tune()         override;
    void   Print()        override;
    string GetTypeName()  const override;

    // Унарный оператор ! для уменьшения количества струн
    AcousticGuitar& operator!();
};

