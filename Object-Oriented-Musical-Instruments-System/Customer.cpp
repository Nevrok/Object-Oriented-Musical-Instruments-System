#include "Customer.h"
#include "InstrumentStore.h"
#include "Localization.h"
#include <iostream>
#include <stdexcept>
using namespace std;

Customer::Customer() : mName("Guest"), mAge(18) {}

Customer::Customer(string name, int age) : mName(name), mAge(age) {}

void Customer::SetName(const string& v) {
    try {
        if (v.empty()) {
            throw invalid_argument(Locale::get().msg_invalid);
        }
        mName = v;
    }
    catch (const invalid_argument& e) {
        cout << Locale::get().msg_invalid << " " << e.what() << "\n";
    }
}

void Customer::SetAge(int v) {
    try {
        if (v <= 0 || v > 150) {
            throw invalid_argument(Locale::get().msg_invalid);
        }
        mAge = v;
    }
    catch (const invalid_argument& e) {
        cout << Locale::get().msg_invalid << " " << e.what() << "\n";
    }
}
bool Customer::BuyInstrument(InstrumentStore& store, int index) {
    if (!store.GetAt(index)) return false;
    // Выделяем место до передачи владения, чтобы покупка не терялась при ошибке.
    mInventory.emplace_back();
    mInventory.back() = store.TakeInstrument(index);

    const auto& instr = *mInventory.back();
    cout << mName << Locale::get().msg_customer_buy
        << "[" << instr.GetBrend() << "] " << instr.GetTypeName() << endl;
    return true;
}

StringInstruments* Customer::GetInventoryItem(int index) const {
    if (index >= 0 && index < (int)mInventory.size())
        return mInventory[index].get();
    return nullptr;
}

void Customer::PrintInventory() const {
    const auto& t = Locale::get();
    if (mInventory.empty()) {
        cout << t.msg_store_empty << endl;
        return;
    }
    cout << "\n=== " << mName << t.msg_customer_view << " ===\n";
    for (int i = 0; i < (int)mInventory.size(); ++i) {
        cout << t.instr_header << (i + 1) << " ———" << endl;
        mInventory[i]->Print();
        cout << t.separator << endl;
    }
}
void Customer::ShowInstrumentInfo(const StringInstruments& instr) const {
    const auto& t = Locale::get();
    cout << mName << t.msg_customer_view
        << "[" << instr.GetBrend() << "] "
        << instr.GetTypeName() << endl;
    const_cast<StringInstruments&>(instr).Print();
}

void Customer::PrintInfo() const {
    const auto& t = Locale::get();
    cout << t.lbl_name << mName << endl;
    cout << t.lbl_age << mAge << endl;
    cout << t.lbl_inventory << mInventory.size() << t.msg_inventory_instruments_suffix << "\n";
}
