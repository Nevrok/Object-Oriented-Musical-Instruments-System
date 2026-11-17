#pragma once
#include <filesystem>
#include <memory>
#include <string>
#include <vector>
#include "StringInstruments.h"

class InstrumentStore {
    std::vector<std::unique_ptr<StringInstruments>> mInstruments;
    int mCapacity;

    bool HasDuplicate(const StringInstruments& instr, const std::string& brand) const;

public:
    InstrumentStore(int capacity = 10);

    bool AddInstrument(std::unique_ptr<StringInstruments> instr);
    bool RenameInstrument(StringInstruments& instr, const std::string& brand);
    std::unique_ptr<StringInstruments> TakeInstrument(int index);
    void RemoveInstrument(int index);
    void Clear();

    void PrintAll() const;
    int GetCount() const { return static_cast<int>(mInstruments.size()); }
    int GetCapacity() const { return mCapacity; }
    StringInstruments* GetAt(int index) const;
    StringInstruments* FindByBrend(const std::string& brand) const;

    void SaveToFile(const std::filesystem::path& filename) const;
    void LoadFromFile(const std::filesystem::path& filename);
    int LoadDefaults(const std::filesystem::path& filename);
};
