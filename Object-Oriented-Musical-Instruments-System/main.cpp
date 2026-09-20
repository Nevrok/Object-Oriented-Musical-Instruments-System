#define NOMINMAX    
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <stdexcept>
#include <cctype>

#include "Localization.h"
#include "StringInstruments.h"
#include "AcousticGuitar.h"
#include "BassGuitar.h"
#include "ElectricGuitar.h"
#include "ElectronicDevice.h"
#include "InstrumentStore.h"
#include "Customer.h"

using namespace std;

InstrumentStore globalStore(20);
Customer        currentCustomer("Иванов И.И.", 25);

static void cls() { system("cls"); }

static void pause() {
    cout << Locale::get().any_key;
    _getch();
}

// Первая строка каждого кадра
static void frameHint() {
    cout << Locale::get().frame_hint << "\n\n";
}

static bool isBackspaceKey(int ch) {
    return ch == 8;
}

static string getExeDir() {
    char buf[MAX_PATH] = { 0 };
    DWORD len = GetModuleFileNameA(nullptr, buf, MAX_PATH);
    if (len == 0 || len >= MAX_PATH) return "";
    string full(buf, len);
    size_t pos = full.find_last_of("\\/");
    if (pos == string::npos) return "";
    return full.substr(0, pos + 1);
}

// Проверка: строка содержит только буквы, пробелы и дефисы
static bool isAlphaOnly(const string& s) {
    if (s.empty()) return false;
    for (unsigned char c : s) {
        // Пропускаем UTF—8 продолжающие байты, чтобы кириллица не ломала проверку по байтам
        if ((c & 0xC0) == 0x80) continue;
        if (c >= 0x80) continue;
        if (!isalpha(c) && !isdigit(c) && c != ' ' && c != '—' && c != '.') return false;
    }
    return true;
}

// Проверка: строка содержит только цифры (для числовых полей)
static bool isDigitsOnly(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

static string inputLineEsc(const string& prompt, bool allowEmpty = false) {
    cout << prompt;
    string s;
    while (true) {
        int ch = _getch();

        if (ch == 27) {
            return "";
        }

        if (ch == '\r') {
            cout << "\n";
            if (!allowEmpty && s.empty()) {
                cout << Locale::get().msg_invalid << "\n";
                cout << prompt;
                continue;
            }
            if (s.length() > 100) {
                cout << Locale::get().masg_over_text_limit << "\n";
                s.clear();
                cout << prompt;
                continue;
            }
            return s;
        }

        if (isBackspaceKey(ch)) {
            if (!s.empty()) {
                s.pop_back();
                cout << "\b \b";
            }
            continue;
        }

        if (isprint(static_cast<unsigned char>(ch))) {
            s.push_back(static_cast<char>(ch));
            cout << static_cast<char>(ch);
        }
    }
}

static bool readIntFromLine(const string& s, int& out) {
    try {
        size_t pos = 0;
        long long v = stoll(s, &pos);
        if (pos != s.size()) return false;
        out = static_cast<int>(v);
        return true;
    }
    catch (...) {
        return false;
    }
}

static int inputIntEsc(const string& prompt, int minVal = 0, int maxVal = 1000) {
    while (true) {
        string s = inputLineEsc(prompt, true);
        if (s.empty()) return -1;

        if (!isDigitsOnly(s)) {
            cout << Locale::get().msg_only_digits << "\n";
            continue;
        }

        int v = 0;
        if (!readIntFromLine(s, v)) {
            cout << Locale::get().msg_invalid << "\n";
            continue;
        }
        if (v < minVal || v > maxVal) {
            cout << Locale::get().msg_range_error_prefix << minVal << " до " << maxVal << "!\n";
            continue;
        }
        return v;
    }
}

// Ввод строки с проверкой на буквы (для названий, брендов, материалов)
static string inputAlphaEsc(const string& prompt) {
    while (true) {
        string s = inputLineEsc(prompt, true);
        if (s.empty()) return "";

        if (!isAlphaOnly(s)) {
            cout << Locale::get().msg_only_letters << "\n";
            continue;
        }
        return s;
    }
}

// Ввод материала
static string inputMaterialEsc(const string& prompt) {
    while (true) {
        string s = inputAlphaEsc(prompt);
        if (s.empty()) return "";
        for (unsigned char c : s) {
            if (isdigit(c)) {
                cout << Locale::get().msg_only_letters << "\n";
                s.clear();
                break;
            }
        }
        if (!s.empty()) return s;
    }
}


// Ввод целого числа с проверкой на диапазон
static int inputInt(const string& prompt, int minVal = 0, int maxVal = 1000) {
    cout << prompt;
    int v = -1;
    try {
        cin >> v;
        if (cin.fail()) {
            cin.clear();
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            cout << Locale::get().msg_invalid << "\n";
            return -1;
        }
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
        // Проверка на отрицательное значение
        if (v < 0) {
            cout << Locale::get().msg_invalid << "\n";
            return -1;
        }
        // Проверка на диапазон
        if (v < minVal || v > maxVal) {
            cout << Locale::get().msg_range_error_prefix << minVal << " до " << maxVal << "!\n";
            return -1;
        }
    }
    catch (const exception& e) {
        cout << Locale::get().msg_input_number_error_prefix << e.what() << "\n";
        cin.clear();
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
        return -1;
    }
    return v;
}

static int visualLen(const string& s) {
    int len = 0;
    for (unsigned char c : s) {
        if ((c & 0xC0) != 0x80) len++;
    }
    return len;
}

static string centre(const string& s, int width) {
    int len = visualLen(s);
    int pad = (width - len) / 2;
    if (pad < 0) pad = 0;
    return string(pad, ' ') + s;
}

static void twoCol(const string& left, const string& right, int width = 100) {
    int l = visualLen(left);
    int r = visualLen(right);

    int sp = width - l - r;
    if (sp < 1) sp = 1;

    cout << left << string(sp, ' ') << right << "\n";
}

// Подсчёт типов гитар в магазине
static void countByType(int& ac, int& el, int& bs) {
    ac = el = bs = 0;
    for (int i = 0; i < globalStore.GetCount(); i++) {
        auto* p = globalStore.GetAt(i);
        if (!p) continue; // Пропускаем nullptr
        if (dynamic_cast<AcousticGuitar*>(p)) ac++;
        else if (dynamic_cast<ElectricGuitar*>(p)) el++;
        else if (dynamic_cast<BassGuitar*>(p)) bs++;
    }
}

// Разбиение строки по разделителю
static vector<string> split(const string& s, char delim) {
    vector<string> tokens;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}

// Загрузка инструментов из файла данных
static int loadFromFile(const string& filename) {
    string exeDir = getExeDir();
    ifstream file(exeDir + filename);
    if (!file.is_open()) file.open(filename);
    if (!file.is_open()) return 0;

    int count = 0;
    string line;

    // Формат строк: Type;... (Type = A/E/B), остальное зависит от типа.
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        vector<string> parts = split(line, ';');
        if (parts.empty()) continue;

        char type = parts[0][0];

        try {
            if (type == 'A' && parts.size() >= 10) {
                // Acoustic: A;бренд;струны;группа;материал;цвет канта;размер;цвет корпуса;материал грифа;материал струн
                globalStore.AddInstrument(new AcousticGuitar(
                    parts[1],
                    stoi(parts[2]),
                    parts[3],
                    parts[4],
                    parts[5],
                    stof(parts[6]),
                    parts[7],
                    parts[8],
                    parts[9]));
                count++;
            }
            else if (type == 'E' && parts.size() >= 8) {
                // Electric: E;бренд;струны;группа;материал;мощность;батарея;звукосниматели
                globalStore.AddInstrument(new ElectricGuitar(
                    parts[1],
                    stoi(parts[2]),
                    parts[3],
                    parts[4],
                    stof(parts[5]),
                    stof(parts[6]),
                    stoi(parts[7])));
                count++;
            }
            else if (type == 'B' && parts.size() >= 8) {
                // Bass: B;бренд;count;группа;материал;мощность;батарея;струны
                globalStore.AddInstrument(new BassGuitar(
                    parts[1],
                    stoi(parts[2]),
                    parts[3],
                    parts[4],
                    stof(parts[5]),
                    stof(parts[6]),
                    stoi(parts[7])));
                count++;
            }
        }
        catch (...) {
            // Пропускаем строки с ошибками
            continue;
        }
    }

    file.close();
    return count;
}

//  Управление одним инструментом
static void manageInstrument(StringInstruments* instr) {
    if (!instr) {
        cout << Locale::get().msg_invalid << " " << Locale::get().msg_index_out_of_range << "\n";
        return;
    }
    ElectronicDevice* elec = dynamic_cast<ElectronicDevice*>(instr);


    // Для электроинструментов дополнительно открываем пункты меню,связанные с электричеством через dynamic_cast к ElectronicDevice.

    bool running = true;
    while (running) {
        cls();
        const auto& t = Locale::get();
        frameHint();

        cout << t.act_title << "\n\n";
        cout << t.lbl_brand << instr->GetBrend()
            << "  [" << instr->GetTypeName() << "]";
        if (elec) cout << "  " << t.lbl_battery << elec->GetBattery() << "%";
        cout << "\n\n";

        cout << t.act_play << "\n";
        cout << t.act_setup << "\n";
        cout << (elec ? t.act_charge : "3. ———") << "\n";
        cout << t.act_info << "\n";
        cout << t.act_tune << "\n";
        cout << t.act_set_brand << "\n";
        cout << t.act_set_strings << "\n";
        cout << (elec ? t.act_use_power : "8. ———") << "\n";
        cout << "\n";

        int key = _getch();
        if (key == 9) { Locale::toggle(); continue; }
        if (key == 27) { running = false; break; }

        cls(); frameHint();
        const auto& t2 = Locale::get();

        switch (key) {
        case '1': instr->Play();  break;
        case '2': {
            AcousticGuitar* ag = dynamic_cast<AcousticGuitar*>(instr);
            if (ag && ag->GetCount() <= 0) {
                cout << t2.msg_invalid << t2.msg_no_strings << "\n";
                break;
            }
            instr->SetUp();
            break;
        }
        case '3': if (elec) elec->Charge(); break;
        case '4': {
            AcousticGuitar* ag = dynamic_cast<AcousticGuitar*>(instr);
            if (ag && ag->GetCount() <= 0) {
                cout << t2.msg_invalid << t2.msg_no_strings << "\n";
                break;
            }
            if (!instr) {
                cout << Locale::get().msg_invalid << " nullptr\n";
                break;
            }
            instr->Print();
            cout << t2.separator << "\n";
            break;
        }
        case '5': {
            AcousticGuitar* ag = dynamic_cast<AcousticGuitar*>(instr);
            if (ag && ag->GetCount() <= 0) {
                cout << t2.msg_invalid << t2.msg_no_strings << "\n";
                break;
            }
            if (!instr) {
                cout << Locale::get().msg_invalid << " nullptr\n";
                break;
            }
            string nb = inputAlphaEsc(t2.msg_enter_brand);
            if (nb.empty()) break;
            instr->SetBrend(nb);
            cout << t2.lbl_brand << instr->GetBrend() << "\n";
            break;
        }
        case '7': {
            // В зависимости от типа инструмента логика изменения струн различается:
            // — Для AcousticGuitar используется оператор ! (порвать струну)
            // — Для ElectricGuitar используется ——
            // — Для BassGuitar свой счётчик струн (mStringCount)
            cout << t2.act_add_remove_string;
            int s = _getch();
            if (s == '1') {
                BassGuitar* bg = dynamic_cast<BassGuitar*>(instr);
                if (bg) { bg->SetStringCount(bg->GetStringCount() + 1); }
                else { ++(*instr); }
            }
            else if (s == '2') {
                AcousticGuitar* ag = dynamic_cast<AcousticGuitar*>(instr);
                ElectricGuitar* eg = dynamic_cast<ElectricGuitar*>(instr);
                BassGuitar* bg = dynamic_cast<BassGuitar*>(instr);
                if (ag) {
                    if (ag->GetCount() > 1) {
                        !(*ag);
                        cout << Locale::get().msg_string_broke << ag->GetCount() << "\n";
                    }
                    else {
                        cout << Locale::get().msg_under_one_string << "\n";
                    }
                }
                else if (eg) { --(*eg); }
                else if (bg) { bg->DecreaseStringCount(); }
            }
            BassGuitar* bg2 = dynamic_cast<BassGuitar*>(instr);
            if (bg2) cout << t2.act_str_operation << bg2->GetStringCount() << "\n";
            else cout << t2.act_str_operation << instr->GetCount() << "\n";
            break;
        }
        case '8':
            if (elec) {
                cout << t2.act_electronics_menu;
                int b = _getch();
                if (b == '1') {
                    ElectricGuitar* eg = dynamic_cast<ElectricGuitar*>(instr);
                    BassGuitar* bg = dynamic_cast<BassGuitar*>(instr);
                    if (eg) {
                        int pct = inputIntEsc(t2.act_percent_add, 1, 100);
                        if (pct < 0) break;
                        if (pct > 0) {
                            eg->SetBattery((min)(100.0f, eg->GetBattery() + static_cast<float>(pct)));
                            cout << t2.act_charge_added << eg->GetBattery() << "%\n";
                        }
                    }
                    else if (bg) {
                        elec->Charge();
                    }
                }
                else if (b == '2') {
                    --(*elec); // Использование виртуального оператора ElectronicDevice
                    cout << t2.act_spent_10 << elec->GetBattery() << "%\n";
                }
                else if (b == '3') {
                    int pw = inputIntEsc(t2.act_enter_power, 1, 1000); // Мощность от 1 до 1000
                    if (pw < 0) break;
                    if (pw > 0) { elec->SetPower(pw); cout << t2.act_power_set << pw << "\n"; }
                }
                else if (b == '4') {
                    // Новый пункт: сравнение мощности с другим инструментом
                    cout << t2.lbl_index;
                    int idx = inputIntEsc(Locale::get().input_idx_to_equal, 1, globalStore.GetCount());
                    if (idx < 1 || idx > globalStore.GetCount()) {
                        cout << Locale::get().msg_invalid << " " << Locale::get().msg_index_out_of_range << "\n";
                        break;
                    }
                    StringInstruments* other = globalStore.GetAt(idx - 1);
                    ElectronicDevice* otherElec = dynamic_cast<ElectronicDevice*>(other);
                    if (!otherElec) {
                        cout << Locale::get().msg_not_electronic << "\n";
                        break;
                    }
                    if (*elec == *otherElec) {
                        cout << Locale::get().msg_power_equal << "\n";
                    }
                    else {
                        float p1 = elec->GetPower();
                        float p2 = otherElec->GetPower();
                        cout << Locale::get().msg_power_not_equal << "\n";
                        if (p1 > p2) {
                            cout << instr->GetBrend() << " [" << instr->GetTypeName() << "]" << Locale::get().msg_have_more_pow
                                << (p1 - p2) << " (" << p1 << Locale::get().msg_compare << p2 << ")\n";
                        }
                        else {
                            cout << other->GetBrend() << " [" << other->GetTypeName() << "]" << Locale::get().msg_have_more_pow
                                << (p2 - p1) << " (" << p2 << Locale::get().msg_compare << p1 << ")\n";
                        }
                    }
                }
            }
            break;
        default:
            cout << t2.msg_invalid << "\n";
        }
        pause();
    }
}

//  Выбор инструмента —> управление
static void selectAndManage() {
    cls();
    const auto& t = Locale::get();
    frameHint();
    if (globalStore.GetCount() == 0) { cout << t.msg_store_empty << "\n"; pause(); return; }
    globalStore.PrintAll();
    int idx = inputIntEsc(t.lbl_index, 1, globalStore.GetCount());
    if (idx < 0) return;
    manageInstrument(globalStore.GetAt(idx - 1));
}

//  Добавить инструмент
static void addInstrumentMenu() {
    bool running = true;
    while (running) {
        cls();
        const auto& t = Locale::get();
        frameHint();
        cout << t.add_title << "\n\n";
        cout << t.add_acoustic << "\n";
        cout << t.add_electric << "\n";
        cout << t.add_bass << "\n\n";

        int key = _getch();
        if (key == 9) { Locale::toggle(); continue; }
        if (key == 27) break;

        cls(); frameHint();
        const auto& t2 = Locale::get();

        if (key == '1') {
            cout << t2.act_add_modes_title;
            int mode = _getch(); cout << "\n";
            string br = inputAlphaEsc(t2.msg_enter_brand);
            if (br.empty()) break;

            // В режиме "2" пользователь вводит все поля вручную
            // В режиме "1" подставляется шаблон

            if (mode == '2') {
                int strings = inputIntEsc(t2.msg_enter_strings, 0, 12);
                if (strings < 0) break;
                string group = inputAlphaEsc(t2.prompt_group);
                if (group.empty()) break;
                string material = inputMaterialEsc(t2.act_enter_material);
                if (material.empty()) break;
                string cKant = inputAlphaEsc(t2.prompt_rim_color);
                if (cKant.empty()) break;
                int sizeI = inputIntEsc(t2.act_enter_size, 1, 100);
                if (sizeI < 0) break;
                float size = static_cast<float>(sizeI);
                string cKorpus = inputAlphaEsc(t2.prompt_body_color);
                if (cKorpus.empty()) break;
                string mGrif = inputAlphaEsc(t2.prompt_neck_material);
                if (mGrif.empty()) break;
                string mString = inputAlphaEsc(t2.prompt_string_material);
                if (mString.empty()) break;

                globalStore.AddInstrument(new AcousticGuitar(
                    br, strings, group, material, cKant, size, cKorpus, mGrif, mString));
            }
            else {
                globalStore.AddInstrument(new AcousticGuitar(
                    br, 6, "Acoustic", "Spruce", "Ivory", 41.0f, "Natural", "Maple", "Nylon"));
            }

            cout << t2.msg_added << "\n"; running = false;
        }
        else if (key == '2') {
            cout << t2.act_add_modes_short;
            int mode = _getch(); cout << "\n";
            string br = inputAlphaEsc(t2.msg_enter_brand);
            if (br.empty()) break;

            if (mode == '2') {
                int strings = inputIntEsc(t2.msg_enter_strings, 1, 12);
                if (strings < 0) break;
                string group = inputAlphaEsc(t2.prompt_group);
                if (group.empty()) break;
                string material = inputMaterialEsc(t2.act_enter_material);
                if (material.empty()) break;
                int powerI = inputIntEsc(t2.act_enter_power_w, 1, 1000);
                if (powerI < 0) break;
                int batteryI = inputIntEsc(t2.prompt_initial_charge, 0, 100);
                if (batteryI < 0) break;
                int pickups = inputIntEsc(t2.prompt_pickup_count, 0, 10);
                if (pickups < 0) break;

                globalStore.AddInstrument(new ElectricGuitar(
                    br, strings, group, material,
                    static_cast<float>(powerI), static_cast<float>(batteryI), pickups));
            }
            else {
                globalStore.AddInstrument(new ElectricGuitar(
                    br, 6, "Electric", "Mahogany", 100.0f, 100.0f, 2));
            }

            cout << t2.msg_added << "\n"; running = false;
        }
        else if (key == '3') {
            cout << t2.act_add_modes_short;
            int mode = _getch(); cout << "\n";
            string br = inputAlphaEsc(t2.msg_enter_brand);
            if (br.empty()) break;

            if (mode == '2') {
                int count = inputIntEsc(t2.prompt_quantity_count, 0, 1000);
                if (count < 0) break;
                string group = inputAlphaEsc(t2.prompt_group);
                if (group.empty()) break;
                string material = inputMaterialEsc(t2.act_enter_material);
                if (material.empty()) break;
                int powerI = inputIntEsc(t2.act_enter_power_w, 1, 1000);
                if (powerI < 0) break;
                int batteryI = inputIntEsc(t2.prompt_initial_charge, 0, 100);
                if (batteryI < 0) break;
                int strings = inputIntEsc(t2.msg_enter_strings, 1, 12);
                if (strings < 0) break;

                globalStore.AddInstrument(new BassGuitar(
                    br, count, group, material,
                    static_cast<float>(powerI), static_cast<float>(batteryI), strings));
            }
            else {
                globalStore.AddInstrument(new BassGuitar(
                    br, 1, "Bass", "Alder", 50.0f, 100.0f, 4));
            }

            cout << t2.msg_added << "\n"; running = false;
        }
        else {
            cout << t2.msg_invalid << "\n";
        }
        pause();
    }
}

//  Покупатель
static void customerMenu() {
    bool running = true;
    while (running) {
        cls();
        const auto& t = Locale::get();
        frameHint();
        cout << t.cust_title << "\n\n";
        cout << t.lbl_name << currentCustomer.GetName()
            << "  " << t.lbl_age << currentCustomer.GetAge() << "\n";
        cout << t.lbl_inventory << currentCustomer.GetInventoryCount() << "\n\n";
        cout << t.cust_buy << "\n";
        cout << t.cust_view << "\n";
        cout << t.cust_info << "\n";
        cout << t.cust_info_change << "\n";
        cout << t.cust_my_inventory << "\n\n";

        int key = _getch();
        if (key == 9) { Locale::toggle(); continue; }
        if (key == 27) break;

        cls(); frameHint();
        const auto& t2 = Locale::get();

        if (globalStore.GetCount() == 0 && (key == '1' || key == '2')) {
            cout << t2.msg_store_empty << "\n"; pause(); continue;
        }

        if (key == '1') {
            // Покупка: инструмент переносится в инвентарь покупателя и удаляется из магазина.
            globalStore.PrintAll();
            int idx = inputIntEsc(t2.lbl_index, 1, globalStore.GetCount());
            if (idx < 0) { pause(); continue; }
            StringInstruments* item = globalStore.GetAt(idx - 1);
            if (item) {
                currentCustomer.BuyInstrument(item);
                globalStore.RemoveInstrument(idx - 1);
                cout << t2.msg_sold_one << "\n";
            }
        }
        else if (key == '2') {
            // Просмотр информации — без покупки, инструмент остаётся в магазине.
            globalStore.PrintAll();
            int idx = inputIntEsc(t2.lbl_index, 1, globalStore.GetCount());
            if (idx < 0) { pause(); continue; }
            StringInstruments* item = globalStore.GetAt(idx - 1);
            if (item) {
                currentCustomer.ShowInstrumentInfo(*item);
            }
            else {
                cout << Locale::get().msg_invalid << " " << Locale::get().msg_index_out_of_range << "\n";
            }
        }
        else if (key == '3') {
            currentCustomer.PrintInfo();
        }
        else if (key == '4') {
            cout << t.change_customer;
            int key2 = _getch();
            if (key2 == 9) { Locale::toggle(); continue; }
            if (key2 == 27) break;

            if (key2 == '1') {
                string name = inputAlphaEsc(t.add_customer_name);
                if (name.empty()) { pause(); continue; }
                currentCustomer.SetName(name);
            }
            else if (key2 == '2') {
                int age = inputIntEsc(t.add_customer_age, 1, 120); // Возраст от 1 до 120
                if (age < 0) { pause(); continue; }
                try {
                    currentCustomer.SetAge(age);
                }
                catch (const invalid_argument& e) {
                    cout << t2.msg_invalid << " " << e.what() << "\n";
                    pause();
                }
            }

        }
        else if (key == '5') {
            cout << "\n";
            currentCustomer.PrintInventory();
        }
        else {
            cout << t2.msg_invalid << "\n";
        }
        pause();
    }
}

//  Поиск по марке
static void searchMenu() {
    cls();
    const auto& t = Locale::get();
    frameHint();

    // Поиск выполняется по точному совпадению марки
    // Ввод ограничен "текстовыми" символами через inputAlphaEsc()
    string br = inputAlphaEsc(t.msg_enter_brand);
    if (br.empty()) return;
    StringInstruments* found = globalStore.FindByBrend(br);
    if (found) {
        cout << t.separator << "\n";
        found->Print();
        cout << t.separator << "\n";
        AcousticGuitar* ag = dynamic_cast<AcousticGuitar*>(found);
        if (ag)
            cout << "operator==() : " << ((*ag == *ag) ? "true (одинаковые)" : "false (разные)") << "\n";
    }
    else {
        cout << t.msg_not_found << "\n";
    }
    pause();
}

//  Количество объектов
static void countScreen() {
    cout << "\n";
    const auto& t = Locale::get();
    int ac = 0, el = 0, bs = 0;

    // Подсчитываем количество инструментов каждого типа в текущем магазине.
    countByType(ac, el, bs);
    cout << t.type_acoustic << ": " << ac << "\n";
    cout << t.type_electric << ": " << el << "\n";
    cout << t.type_bass << ": " << bs << "\n\n";
    cout << t.lbl_store_count << globalStore.GetCount()
        << " / " << globalStore.GetCapacity() << "\n";
    cout << t.lbl_objects << StringInstruments::GetObjectCount() << "\n";
    pause();
}

//  Меню демонстрации
static void storeMenu() {
    bool running = true;
    while (running) {
        cls();
        const auto& t = Locale::get();
        frameHint();

        cout << t.demo_add << "\n";
        cout << t.demo_list << "\n";
        cout << t.demo_sell_all << "\n";
        cout << t.demo_sell_one << "\n";
        cout << t.demo_save << "\n";
        cout << t.demo_load << "\n";
        cout << t.demo_count << "\n";
        cout << t.demo_search << "\n";
        cout << t.demo_customer << "\n";
        cout << t.demo_interact << "\n";
        cout << t.menu_fill_from_data_file << "\n\n";

        int key = _getch();
        if (key == 9) { Locale::toggle(); continue; }
        if (key == 27) { running = false; break; }

        const auto& t2 = Locale::get();

        switch (key) {
        case '1': addInstrumentMenu(); break;
        case '2':
            cout << "\n"; globalStore.PrintAll(); pause(); break;
        case '3':
            cout << "\n"; globalStore.Clear();
            cout << t2.msg_sold_all << "\n"; pause(); break;
        case '4': {
            cout << "\n";
            try {
                if (globalStore.GetCount() == 0) { cout << t2.msg_store_empty << "\n"; pause(); break; }
                globalStore.PrintAll();
                int idx = inputInt(t2.lbl_index);
                if (idx < 1 || idx > globalStore.GetCount()) {
                    throw out_of_range(Locale::get().msg_index_out_of_range);
                }
                globalStore.RemoveInstrument(idx - 1);
                cout << t2.msg_sold_one << "\n";
            }
            catch (const out_of_range& e) {
                cout << t2.msg_invalid << " " << e.what() << "\n";
            }
            catch (const exception& e) {
                cout << t2.msg_invalid << " " << e.what() << "\n";
            }
            pause(); break;
        }
        case '5':
            cout << "\n";
            try {
                globalStore.SaveToFile("store.txt");
                cout << t2.msg_saved << "\n";
            }
            catch (const exception& e) {
                cout << Locale::get().msg_error_save_prefix << e.what() << "\n";
            }
            pause(); break;
        case '6':
            cout << "\n";
            try {
                globalStore.LoadFromFile("store.txt");
                cout << t2.msg_loaded << "\n";
            }
            catch (const exception& e) {
                cout << Locale::get().msg_error_load_prefix << e.what() << "\n";
            }
            pause(); break;
        case '7': countScreen();      break;
        case '8': searchMenu();       break;
        case '9': customerMenu();     break;
        case '0': selectAndManage();  break;
        case 'f':
        case 'F':
            // Автозаполнение из файла данных
            cout << "\n";
            try {
                globalStore.Clear();
                int loaded = loadFromFile("instruments_data.txt");
                if (loaded > 0) {
                    cout << Locale::get().msg_loaded_instruments << loaded << "\n";
                }
                else {
                    cout << Locale::get().msg_data_file_missing << "\n";
                }
            }
            catch (const exception& e) {
                cout << Locale::get().msg_error_load_prefix << e.what() << "\n";
            }
            pause(); break;
        default: break;
        }
    }
}

//  Заставка
static void showSplash() {
    while (true) {
        cls();
        const auto& t = Locale::get();
        const int W = 100;

        // Название университета / Факультет / кафедра — как на скрине 1
        cout << centre(t.splash_uni, W) << "\n";
        cout << t.splash_faculty << "\n";
        cout << t.splash_dept << "\n\n\n\n";

        // Тип работы, дисциплина, тема — по центру
        cout << centre(t.splash_coursework, W) << "\n";
        cout << centre(t.splash_discipline_line, W) << "\n";
        cout << centre(t.splash_topic_line, W) << "\n\n\n\n";

        // Руководитель (лево) | Выполнил (право)
        twoCol(t.splash_role_sup, t.splash_role_perf, W);
        twoCol(t.splash_sup_title, t.splash_perf_title, W);
        twoCol(t.splash_sup_name1, t.splash_perf_name, W);
        cout << t.splash_sup_name2 << "\n\n\n\n";

        // Город и год — по центру
        cout << centre(t.splash_city_year, W) << "\n\n\n";

        // Подсказки (лево | право)
        twoCol(t.splash_start, t.splash_change_lang, W);
        twoCol(t.splash_exit, t.splash_about, W);
        twoCol(t.splash_manual, t.splash_program, W);

        int key = _getch();
        if (key == 13)             return;
        if (key == 9)              Locale::toggle();
        if (key == 27)             exit(0);
        if (key == 'a' || key == 'A') {
            cls(); cout << "\n" << Locale::get().msg_about_author << "\n"; pause();
        }
        if (key == 'd' || key == 'D') {
            cls(); cout << "\n" << Locale::get().msg_about_program << "\n"; pause();
        }
        if (key == 's' || key == 'S') {
            cls(); cout << "\n" << Locale::get().help_text << "\n"; pause();
        }
    }
}

//  Главное меню
static void mainMenu() {
    while (true) {
        cls();
        const auto& t = Locale::get();
        frameHint();
        cout << t.menu_task << "\n";
        cout << t.menu_demo << "\n";
        cout << t.menu_help << "\n";
        cout << t.menu_exit << "\n\n";

        int key = _getch();
        if (key == 9) { Locale::toggle(); continue; }
        if (key == 27 || key == '4') break;

        const auto& t2 = Locale::get();
        if (key == '1') {
            cls(); frameHint(); cout << t2.task_text << "\n"; pause();
        }
        else if (key == '2') {
            storeMenu();
        }
        else if (key == '3') {
            cls(); frameHint(); cout << t2.help_text << "\n"; pause();
        }
    }
}

//  Точка входа
int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Locale::init();

    showSplash();
    mainMenu();

    return 0;
}