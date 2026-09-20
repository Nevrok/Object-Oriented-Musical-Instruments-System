#pragma once
#include <string>
#include <vector>
#include "StringInstruments.h"
using namespace std;

class Customer {
private:
    string mName;
    int    mAge;
    vector<StringInstruments*> mInventory;  // Инвентарь покупателя

public:
    Customer();
    Customer(string name, int age);
    ~Customer();

    string GetName() const { return mName; }
    int    GetAge()  const { return mAge; }
    int    GetInventoryCount() const { return (int)mInventory.size(); }

    void SetName(const string& v);
    void SetAge(int v);

    void BuyInstrument(StringInstruments* instr);  // Добавить в инвентарь
    StringInstruments* GetInventoryItem(int index) const;  // Получить инструмент из инвентаря
    void PrintInventory() const;  // Показать весь инвентарь
    void ShowInstrumentInfo(const StringInstruments& instr) const;
    void PrintInfo() const;
};
