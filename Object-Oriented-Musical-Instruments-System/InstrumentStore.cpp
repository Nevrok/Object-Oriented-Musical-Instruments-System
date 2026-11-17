#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include "InstrumentStore.h"
#include "Localization.h"
#include "AcousticGuitar.h"
#include "BassGuitar.h"
#include "ElectricGuitar.h"
#include <charconv>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <limits>
#include <locale>
#include <stdexcept>
#include <string_view>
#include <system_error>
#include <typeinfo>
#include <windows.h>

namespace {

bool IsBlank(const string& value) {
    return value.find_first_not_of(" \t\r\n") == string::npos;
}

template<class Number>
Number ParseNumber(const string& text) {
    Number value{};
    const auto end = text.data() + text.size();
    const auto result = from_chars(text.data(), end, value);
    if (result.ec != errc{} || result.ptr != end || !isfinite(value) || value < 0) {
        throw invalid_argument(Locale::get().msg_invalid_data);
    }
    return value;
}

size_t FieldCount(string_view tag) {
    if (tag == "ACOUSTIC") return 10;
    if (tag == "BASS" || tag == "ELECTRIC") return 8;
    if (tag == "GENERIC") return 5;
    throw invalid_argument(Locale::get().msg_invalid_data);
}

unique_ptr<StringInstruments> ParseInstrument(const vector<string>& fields) {
    const auto& tag = fields.at(0);
    if (fields.size() != FieldCount(tag) ||
        IsBlank(fields[1]) || IsBlank(fields[3]) || IsBlank(fields[4])) {
        throw invalid_argument(Locale::get().msg_invalid_data);
    }
    const int count = ParseNumber<int>(fields[2]);
    if (tag == "ACOUSTIC") {
        const float size = ParseNumber<float>(fields[6]);
        if (size <= 0 || IsBlank(fields[5]) || IsBlank(fields[7]) ||
            IsBlank(fields[8]) || IsBlank(fields[9])) {
            throw invalid_argument(Locale::get().msg_invalid_data);
        }
        return make_unique<AcousticGuitar>(fields[1], count, fields[3], fields[4],
            fields[5], size, fields[7], fields[8], fields[9]);
    }
    if (tag == "GENERIC") {
        return make_unique<StringInstruments>(fields[1], count, fields[3], fields[4]);
    }

    const float power = ParseNumber<float>(fields[5]);
    const float battery = ParseNumber<float>(fields[6]);
    const int extra = ParseNumber<int>(fields[7]);
    if (power <= 0 || battery > 100 || (tag == "BASS" && extra == 0)) {
        throw invalid_argument(Locale::get().msg_invalid_data);
    }
    if (tag == "BASS") {
        return make_unique<BassGuitar>(fields[1], count, fields[3], fields[4], power, battery, extra);
    }
    return make_unique<ElectricGuitar>(fields[1], count, fields[3], fields[4], power, battery, extra);
}

void StripBom(string& line) {
    if (line.starts_with("\xEF\xBB\xBF")) line.erase(0, 3);
}

string ReadField(istream& in) {
    string value;
    if (!getline(in, value)) throw runtime_error(Locale::get().msg_invalid_data);
    return value;
}

void WriteInstrument(ostream& out, const StringInstruments& instr) {
    auto* acoustic = dynamic_cast<const AcousticGuitar*>(&instr);
    auto* bass = dynamic_cast<const BassGuitar*>(&instr);
    auto* electric = dynamic_cast<const ElectricGuitar*>(&instr);
    const char* tag = acoustic ? "ACOUSTIC" : bass ? "BASS" : electric ? "ELECTRIC" : "GENERIC";

    out << tag << '\n' << instr.GetBrend() << '\n' << instr.GetCount() << '\n'
        << instr.GetGroup() << '\n' << instr.GetMaterial() << '\n';
    if (acoustic) {
        out << acoustic->GetColorKant() << '\n' << acoustic->GetSize() << '\n'
            << acoustic->GetColorKorpus() << '\n' << acoustic->GetMatGrif() << '\n'
            << acoustic->GetMatString() << '\n';
    } else if (bass) {
        out << bass->GetPower() << '\n' << bass->GetBattery() << '\n'
            << bass->GetStringCount() << '\n';
    } else if (electric) {
        out << electric->GetPower() << '\n' << electric->GetBattery() << '\n'
            << electric->GetPickupCount() << '\n';
    }
}

} // namespace

InstrumentStore::InstrumentStore(int capacity) : mCapacity(capacity) {}

bool InstrumentStore::HasDuplicate(const StringInstruments& instr, const string& brand) const {
    for (const auto& existing : mInstruments) {
        const auto& item = *existing;
        if (&item != &instr && typeid(item) == typeid(instr) && item.GetBrend() == brand) {
            return true;
        }
    }
    return false;
}

bool InstrumentStore::AddInstrument(unique_ptr<StringInstruments> instr) {
    const auto& t = Locale::get();
    if (!instr) return false;
    if (GetCount() >= mCapacity) {
        cout << t.msg_store_full << '\n';
        return false;
    }
    if (HasDuplicate(*instr, instr->GetBrend())) {
        cout << t.msg_duplicate << '\n';
        return false;
    }
    mInstruments.push_back(move(instr));
    return true;
}

bool InstrumentStore::RenameInstrument(StringInstruments& instr, const string& brand) {
    if (HasDuplicate(instr, brand)) {
        cout << Locale::get().msg_duplicate << '\n';
        return false;
    }
    instr.SetBrend(brand);
    return true;
}

unique_ptr<StringInstruments> InstrumentStore::TakeInstrument(int index) {
    if (index < 0 || index >= GetCount()) return {};
    auto position = mInstruments.begin() + index;
    auto instrument = move(*position);
    mInstruments.erase(position);
    return instrument;
}

void InstrumentStore::RemoveInstrument(int index) {
    TakeInstrument(index);
}

StringInstruments* InstrumentStore::GetAt(int index) const {
    return index >= 0 && index < GetCount() ? mInstruments[index].get() : nullptr;
}

void InstrumentStore::PrintAll() const {
    const auto& t = Locale::get();
    if (mInstruments.empty()) {
        cout << t.msg_store_empty << '\n';
        return;
    }
    for (int i = 0; i < GetCount(); ++i) {
        cout << t.instr_header << (i + 1) << " ———\n";
        mInstruments[i]->Print();
        cout << t.separator << '\n';
    }
}

void InstrumentStore::SaveToFile(const filesystem::path& filename) const {
    const auto target = filesystem::absolute(filename);
    wchar_t temporary[MAX_PATH]{};
    if (!GetTempFileNameW(target.parent_path().c_str(), L"gtr", 0, temporary)) {
        throw runtime_error(Locale::get().msg_file_write_failed);
    }
    try {
        ofstream out{filesystem::path(temporary)};
        out.imbue(locale::classic());
        out << setprecision(numeric_limits<float>::max_digits10);
        out << mInstruments.size() << '\n';
        for (const auto& instr : mInstruments) WriteInstrument(out, *instr);
        out.close();

        if (!out || !MoveFileExW(temporary, target.c_str(), MOVEFILE_REPLACE_EXISTING)) {
            throw runtime_error(Locale::get().msg_file_write_failed);
        }
    } catch (...) {
        DeleteFileW(temporary);
        throw;
    }
}

void InstrumentStore::LoadFromFile(const filesystem::path& filename) {
    ifstream in(filename);
    if (!in) throw runtime_error(Locale::get().msg_file_open_failed);

    string header = ReadField(in);
    StripBom(header);
    const int total = ParseNumber<int>(header);
    if (total > mCapacity) throw runtime_error(Locale::get().msg_store_full);

    InstrumentStore loaded(mCapacity);
    for (int i = 0; i < total; ++i) {
        vector<string> fields{ReadField(in)};
        const auto size = FieldCount(fields.front());
        while (fields.size() < size) fields.push_back(ReadField(in));
        // Старый интерфейс позволял создать повторяющиеся марки переименованием.
        // При восстановлении каталога сохраняем все ранее записанные позиции.
        loaded.mInstruments.push_back(ParseInstrument(fields));
    }
    string line;
    while (getline(in, line)) {
        if (!IsBlank(line)) throw runtime_error(Locale::get().msg_invalid_data);
    }
    if (!in.eof() || in.bad()) throw runtime_error(Locale::get().msg_invalid_data);
    mInstruments.swap(loaded.mInstruments);
}

int InstrumentStore::LoadDefaults(const filesystem::path& filename) {
    ifstream in(filename);
    if (!in) throw runtime_error(Locale::get().msg_file_open_failed);

    InstrumentStore loaded(mCapacity);
    string line;
    bool firstLine = true;
    while (loaded.GetCount() < mCapacity && getline(in, line)) {
        if (firstLine) StripBom(line);
        firstLine = false;
        if (line.empty() || line.front() == '#') continue;

        vector<string> fields;
        size_t start = 0;
        for (size_t separator = line.find(';'); separator != string::npos;
             separator = line.find(';', start)) {
            fields.push_back(line.substr(start, separator - start));
            start = separator + 1;
        }
        fields.push_back(line.substr(start));
        if (fields[0] == "A") fields[0] = "ACOUSTIC";
        else if (fields[0] == "B") fields[0] = "BASS";
        else if (fields[0] == "E") fields[0] = "ELECTRIC";
        else continue;

        try {
            auto instrument = ParseInstrument(fields);
            if (!loaded.HasDuplicate(*instrument, instrument->GetBrend())) {
                loaded.mInstruments.push_back(move(instrument));
            }
        } catch (const invalid_argument&) {
            // Файл автозаполнения может содержать отдельные ошибочные строки.
        }
    }
    if (in.bad() || loaded.mInstruments.empty()) {
        throw runtime_error(Locale::get().msg_invalid_data);
    }
    mInstruments.swap(loaded.mInstruments);
    return GetCount();
}

StringInstruments* InstrumentStore::FindByBrend(const string& brand) const {
    for (const auto& instr : mInstruments) {
        if (instr->GetBrend() == brand) return instr.get();
    }
    return nullptr;
}

void InstrumentStore::Clear() {
    mInstruments.clear();
}
