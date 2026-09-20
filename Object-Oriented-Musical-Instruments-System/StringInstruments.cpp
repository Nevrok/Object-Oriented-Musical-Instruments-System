#include "StringInstruments.h"
#include "Localization.h"
#include <iomanip>

// Инициализация статического члена
int StringInstruments::mObjectCount = 0;

// Конструктор по умолчанию
StringInstruments::StringInstruments()
    : mBrend("Unknown"), mCount(6), mGroup("Strings"), mMaterial("Wood") {
    mObjectCount++;
}

// Параметризованный конструктор
StringInstruments::StringInstruments(string brend, int count, string group, string material)
    : mBrend(brend), mCount(count), mGroup(group), mMaterial(material) {
    mObjectCount++;
}

// Деструктор
StringInstruments::~StringInstruments() {
    mObjectCount--;
}

// Метод Play() — воспроизведение звука инструмента
void StringInstruments::Play() {
    const auto& t = Locale::get();
    cout << t.msg_playing << " [" << mBrend << "]" << endl;
}

// Метод SetUp() — настройка инструмента
bool StringInstruments::SetUp() {
    const auto& t = Locale::get();
    cout << t.msg_setup_ok << " [" << mBrend << "]" << endl;
    return true;
}

// Метод Tune() — настройка струн
void StringInstruments::Tune() {
    const auto& t = Locale::get();
    cout << t.msg_tuned << " [" << mBrend << "]" << endl;
}

// Метод Print() — вывод информации об инструменте
void StringInstruments::Print() {
    const auto& t = Locale::get();
    cout << t.lbl_type << GetTypeName() << endl;
    cout << t.lbl_brand << mBrend << endl;
    cout << t.lbl_strings << mCount << endl;
    cout << t.lbl_group << mGroup << endl;
    cout << t.lbl_material << mMaterial << endl;
}

// Метод GetTypeName() — получение типа инструмента
string StringInstruments::GetTypeName() const {
    return "StringInstruments";
}

// Унарный оператор ++ для увеличения количества струн
// Увеличивает количество струн на 1
StringInstruments& StringInstruments::operator++() {
    mCount++;
    return *this;
}

// Бинарный оператор == для сравнения базовых полей инструментов
bool StringInstruments::operator==(const StringInstruments& other) const {
    return mBrend == other.mBrend &&
        mCount == other.mCount &&
        mGroup == other.mGroup &&
        mMaterial == other.mMaterial;
}
