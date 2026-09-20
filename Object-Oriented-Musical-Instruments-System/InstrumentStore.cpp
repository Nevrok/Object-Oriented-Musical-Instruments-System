#include "InstrumentStore.h"
#include "Localization.h"
#include "AcousticGuitar.h"
#include "BassGuitar.h"
#include "ElectricGuitar.h"
#include <iostream>
#include <fstream>
using namespace std;

// Статические константы для идентификации типов при сохранении/загрузке
static const string TAG_ACOUSTIC = "ACOUSTIC";
static const string TAG_ELECTRIC = "ELECTRIC";
static const string TAG_BASS = "BASS";

// Конструктор с указанием вместимости
InstrumentStore::InstrumentStore(int capacity) : mCapacity(capacity) {}

// Деструктор — очищает все инструменты
InstrumentStore::~InstrumentStore() {
    Clear();
}

// Добавление инструмента в магазин
void InstrumentStore::AddInstrument(StringInstruments* instr) {
    const auto& t = Locale::get();
    if (!instr) {
        cout << t.msg_invalid << " nullptr instrument\n";
        return;
    }

    if ((int)mInstruments.size() < mCapacity) {
        // Проверка на дубликаты по бренду и типу
        for (auto* existing : mInstruments) {
            if (existing && existing->GetBrend() == instr->GetBrend() &&
                existing->GetTypeName() == instr->GetTypeName()) {
                cout << t.msg_invalid << " " << t.lbl_brand << " " << instr->GetBrend()
                    << " (" << instr->GetTypeName() << ") " << "already exists\n";
                delete instr;
                return;
            }
        }
        mInstruments.push_back(instr);
    }
    else {
        cout << t.msg_store_full << endl;
        delete instr; // Удаляем объект, если не удалось добавить
    }
}

// Удаление инструмента по индексу
void InstrumentStore::RemoveInstrument(int index) {
    if (index >= 0 && index < (int)mInstruments.size()) {
        delete mInstruments[index];
        mInstruments.erase(mInstruments.begin() + index);
    }
}

// Получение инструмента по индексу
StringInstruments* InstrumentStore::GetAt(int index) const {
    if (index >= 0 && index < (int)mInstruments.size())
        return mInstruments[index];
    return nullptr;
}

// Вывод всех инструментов в консоль
void InstrumentStore::PrintAll() const {
    const auto& t = Locale::get();
    if (mInstruments.empty()) {
        cout << t.msg_store_empty << endl;
        return;
    }
    for (int i = 0; i < (int)mInstruments.size(); ++i) {
        cout << t.instr_header << (i + 1) << " ———" << endl;
        mInstruments[i]->Print();
        cout << t.separator << endl;
    }
}

// Сохранение всех инструментов в файл
void InstrumentStore::SaveToFile(const string& filename) const {
    ofstream out(filename);
    if (!out) return;

    out << mInstruments.size() << "\n";
    for (auto* instr : mInstruments) {
        string tag;
        if (dynamic_cast<AcousticGuitar*>(instr)) tag = TAG_ACOUSTIC;
        else if (dynamic_cast<BassGuitar*>    (instr)) tag = TAG_BASS;
        else if (dynamic_cast<ElectricGuitar*>(instr)) tag = TAG_ELECTRIC;
        else                                            tag = "GENERIC";

        out << tag << "\n";
        out << instr->GetBrend() << "\n";
        out << instr->GetCount() << "\n";
        out << instr->GetGroup() << "\n";
        out << instr->GetMaterial() << "\n";

        // Поля, специфичные для подклассов
        if (auto* ag = dynamic_cast<AcousticGuitar*>(instr)) {
            out << ag->GetColorKant() << "\n";
            out << ag->GetSize() << "\n";
            out << ag->GetColorKorpus() << "\n";
            out << ag->GetMatGrif() << "\n";
            out << ag->GetMatString() << "\n";
        }
        else if (auto* bg = dynamic_cast<BassGuitar*>(instr)) {
            out << bg->GetPower() << "\n";
            out << bg->GetBattery() << "\n";
            out << bg->GetStringCount() << "\n";
        }
        else if (auto* eg = dynamic_cast<ElectricGuitar*>(instr)) {
            out << eg->GetPower() << "\n";
            out << eg->GetBattery() << "\n";
            out << eg->GetPickupCount() << "\n";
        }
    }
    out.close();
}

// Загрузка инструментов из файла
void InstrumentStore::LoadFromFile(const string& filename) {
    ifstream in(filename);
    if (!in) return;

    Clear();
    size_t total;
    in >> total;
    in.ignore();

    for (size_t i = 0; i < total; ++i) {
        string tag, brend, group, material;
        int count;

        getline(in, tag);
        getline(in, brend);
        in >> count; in.ignore();
        getline(in, group);
        getline(in, material);

        StringInstruments* obj = nullptr;

        if (tag == TAG_ACOUSTIC) {
            string cKant, cKorpus, mGrif, mStr;
            float size;
            getline(in, cKant);
            in >> size; in.ignore();
            getline(in, cKorpus);
            getline(in, mGrif);
            getline(in, mStr);
            obj = new AcousticGuitar(brend, count, group, material,
                cKant, size, cKorpus, mGrif, mStr);
        }
        else if (tag == TAG_BASS) {
            float power, battery;
            int strings;
            in >> power >> battery >> strings; in.ignore();
            obj = new BassGuitar(brend, count, group, material,
                power, battery, strings);
        }
        else if (tag == TAG_ELECTRIC) {
            float power, battery;
            int pickups;
            in >> power >> battery >> pickups; in.ignore();
            obj = new ElectricGuitar(brend, count, group, material,
                power, battery, pickups);
        }
        else {
            obj = new StringInstruments(brend, count, group, material);
        }

        if (obj) AddInstrument(obj);
    }
}

// Поиск инструмента по бренду
StringInstruments* InstrumentStore::FindByBrend(const string& brand) const {
    for (auto* instr : mInstruments)
        if (instr->GetBrend() == brand) return instr;
    return nullptr;
}

// Очистка всех инструментов с освобождением памяти
void InstrumentStore::Clear() {
    for (auto* instr : mInstruments) delete instr;
    mInstruments.clear();
}
