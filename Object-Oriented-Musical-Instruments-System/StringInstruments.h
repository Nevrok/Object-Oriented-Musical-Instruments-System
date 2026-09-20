#pragma once
#include <string>
#include <iostream>
using namespace std;

class StringInstruments {
protected:
    string mBrend;        // Бренд инструмента
    int    mCount;        // Количество струн
    string mGroup;        // Группа инструмента
    string mMaterial;     // Материал изготовления
    static int mObjectCount; // Статический счетчик объектов

public:
    StringInstruments();
    StringInstruments(string brend, int count, string group, string material);
    virtual ~StringInstruments();

    // Геттеры
    string GetBrend()    const { return mBrend; }
    int    GetCount()    const { return mCount; }
    string GetGroup()    const { return mGroup; }
    string GetMaterial() const { return mMaterial; }

    // Сеттеры
    void SetBrend(string value) { mBrend = value; }
    void SetCount(int value) { mCount = (value >= 0) ? value : 0; }
    void SetGroup(string value) { mGroup = value; }
    void SetMaterial(string value) { mMaterial = value; }

    // Виртуальные методы
    virtual void Play();
    virtual bool SetUp();
    virtual void Tune();
    virtual void Print();
    virtual string GetTypeName() const;

    // Статический метод получения количества объектов
    static int GetObjectCount() { return mObjectCount; }

    // Унарный оператор ++ для увеличения количества струн
    StringInstruments& operator++();

    // Бинарный оператор == для сравнения базовых полей инструментов
    bool operator==(const StringInstruments& other) const;
};
