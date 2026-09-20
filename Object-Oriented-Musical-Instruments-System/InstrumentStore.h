#pragma once
#include <vector>
#include <string>
#include "StringInstruments.h"
using namespace std;

class InstrumentStore {
private:
    vector<StringInstruments*> mInstruments; // Коллекция инструментов
    int mCapacity;                           // Максимальная вместимость

public:
    InstrumentStore(int capacity = 10);
    ~InstrumentStore();

    // CRUD (Создание, чтение, обновление, удаление)
    void AddInstrument(StringInstruments* instr);    // Добавить инструмент
    void RemoveInstrument(int index);                // Удалить по индексу
    void Clear();                                    // Очистить всё

    // Запросы
    void              PrintAll()   const;             // Вывести все инструменты
    int               GetCount()   const { return (int)mInstruments.size(); } // Текущее количество
    int               GetCapacity()const { return mCapacity; }               // Вместимость
    StringInstruments* GetAt(int index) const;        // Получить по индексу
    StringInstruments* FindByBrend(const string& brand) const; // Найти по бренду

    // Сохранение/загрузка
    void SaveToFile(const string& filename) const;   // Сохранить в файл
    void LoadFromFile(const string& filename);       // Загрузить из файла
};