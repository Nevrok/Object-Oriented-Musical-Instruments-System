#include "Localization.h"

Lang Locale::current = Lang::RU;
TextResources Locale::RU_Texts;
TextResources Locale::EN_Texts;

void Locale::init() {
    // ===== RUSSIAN =====
    auto& r = RU_Texts;
    r.frame_hint = "Для каждого кадра: <ESC> — назад; <TAB> — сменить язык.";
    r.splash_faculty = "Факультет ИСП";
    r.splash_city_year = "Донецк 2026";
    r.splash_manual = "Руководство оператора: <s>";
    r.splash_role_sup = "Руководитель:";
    r.splash_sup_title = "Асс. каф. ПИ";
    r.splash_sup_name1 = "Бердюкова С.С.";
    r.splash_sup_name2 = "Грищенко Д.А.";
    r.splash_role_perf = "Выполнил:";
    r.splash_perf_title = "ст. гр. ПИ—25В";
    r.splash_perf_name = "Рыбалко С.С.";
    r.splash_uni = "Донецкий Национальный Технический Университет";
    r.splash_dept = "Кафедра Программной инженерии";
    r.splash_coursework = "Курсовой проект";
    r.splash_discipline = "Объектно—ориентированное программирование";
    r.splash_topic = "Магазин музыкальных инструментов";
    r.splash_discipline_line = "по дисциплине: \"Объектно—ориентированное программирование\"";
    r.splash_topic_line = "на тему: \"Магазин музыкальных инструментов\"";
    r.splash_author = "Рыбалко С.С.";
    r.splash_checked = "Щедрин С.В.";
    r.splash_start = "Начать: <ENTER>";
    r.splash_change_lang = "Сменить язык: <TAB>";
    r.splash_exit = "Выйти: <ESC>";
    r.splash_about = "Об авторе: <a>";
    r.splash_program = "О программе: <d>";

    r.menu_title = "ГЛАВНОЕ МЕНЮ";
    r.menu_task = "1. Описание задачи";
    r.menu_demo = "2. Демонстрация объектов";
    r.menu_help = "3. Справка";
    r.menu_exit = "4. Выход";

    r.demo_title = "=== МАГАЗИН ГИТАР ===";
    r.demo_add = "1. Добавить инструмент";
    r.demo_list = "2. Список инструментов";
    r.demo_sell_all = "3. Продать все инструменты";
    r.demo_sell_one = "4. Продать инструмент по индексу";
    r.demo_save = "5. Сохранить базу в файл";
    r.demo_load = "6. Загрузить базу из файла";
    r.demo_count = "7. Количество объектов";
    r.demo_search = "8. Поиск по марке";
    r.demo_customer = "9. Действия покупателя";
    r.demo_interact = "0. Управление инструментом";
    r.demo_back = "ESC — Назад";

    r.add_title = "——— Добавить инструмент ———";
    r.add_acoustic = "1. Акустическая гитара";
    r.add_electric = "2. Электрогитара";
    r.add_bass = "3. Бас—гитара";

    r.act_title = "——— Управление инструментом ———";
    r.act_play = "1. Играть (Play)";
    r.act_setup = "2. Настроить (SetUp)";
    r.act_charge = "3. Зарядить (Charge)";
    r.act_info = "4. Подробная информация (Print)";
    r.act_tune = "5. Подкрутить струны (Tune)";
    r.act_set_brand = "6. Сменить марку";
    r.act_set_strings = "7. Изменить струны";
    r.act_use_power = "8. Работа с электроникой";
    r.act_back = "ESC — Назад";

    r.cust_title = "——— Покупатель ———";
    r.cust_buy = "1. Купить инструмент (по индексу)";
    r.cust_view = "2. Просмотреть инструмент (по индексу)";
    r.cust_info = "3. Информация о покупателе";
    r.cust_info_change = "4. Изменить информацию о покупателе";
    r.change_customer = "Выберите что хотите изменить:\n"
        "1.Изменить имя покупателя\n"
        "2.Изменить возраст покупателя\n";
    r.add_customer_name = "Введите новое имя покупателя: ";
    r.add_customer_age = "Введите новый возраст покупателя: ";

    r.lbl_brand = "Марка:      ";
    r.lbl_count = "Кол—во:     ";
    r.lbl_group = "Группа:     ";
    r.lbl_material = "Материал:   ";
    r.lbl_rim_color = "Цвет канта: ";
    r.lbl_body_size = "Размер:     ";
    r.lbl_body_color = "Цвет деки:  ";
    r.lbl_neck_mat = "Гриф (мат): ";
    r.lbl_string_mat = "Струны(мат):";
    r.lbl_strings = "Струны:     ";
    r.lbl_pickups = "Звукосниматели:    ";
    r.lbl_power = "Мощность:   ";
    r.lbl_battery = "Батарея:    ";
    r.lbl_type = "Тип:        ";
    r.lbl_store_count = "Инструментов в магазине: ";
    r.lbl_objects = "Объектов создано (статич.): ";
    r.lbl_index = "Введите индекс (1—N): ";
    r.lbl_name = "Имя покупателя: ";
    r.lbl_age = "Возраст: ";

    r.type_acoustic = "Акустическая гитара";
    r.type_electric = "Электрогитара";
    r.type_bass = "Бас—гитара";

    r.msg_added = "Инструмент добавлен в магазин.";
    r.msg_sold_all = "Все инструменты проданы.";
    r.msg_sold_one = "Инструмент удален из магазина.";
    r.msg_saved = "База сохранена в файл store.txt";
    r.msg_loaded = "База загружена из файла store.txt";
    r.msg_store_empty = "Магазин пуст.";
    r.msg_store_full = "Магазин переполнен!";
    r.msg_not_found = "Инструмент не найден.";
    r.msg_charged = "Устройство заряжено до 100%.";
    r.msg_playing = "Исполнение...";
    r.msg_setup_ok = "Настройка завершена успешно.";
    r.msg_needs_charge = "Требуется зарядка! Батарея разряжена.";
    r.msg_enter_brand = "Введите марку: ";
    r.msg_enter_index = "Введите номер инструмента: ";
    r.msg_enter_strings = "Введите кол—во струн: ";
    r.msg_customer_buy = " покупает инструмент: ";
    r.msg_customer_view = " рассматривает инструмент: ";
    r.msg_invalid = "Неверный ввод!";
    r.msg_power_used = "Использовано 10% заряда.";
    r.msg_tuned = "Гитара подстроена.";
    r.msg_about_author = "Автор: Рыбалко С.С. Группа: ПИ—25В";
    r.msg_about_program = "Программная система \"Guitar Store\" v2.0\n";

    r.msg_no_strings = " (Нет струн!)";
    r.msg_press_any_key = "\n(Нажмите любую клавишу для остановки...)";
    r.msg_string_broke = "\n*ДЗЫНЬ* Порвалась струна! Осталось: ";
    r.msg_cannot_play = "Невозможно играть: не осталось струн.\n";
    r.msg_finished_playing = "Игра завершена.\n";
    r.msg_playing_battery = "Играем... Заряд: ";
    r.msg_battery_died = "\nБатарея села! Игра остановлена.\n";
    r.msg_finished_battery = "Игра завершена. Текущий заряд: ";
    r.act_add_remove_string = "1. Натянуть струну (+), 2. Порвать струну (—)\n> ";
    r.act_str_operation = "\nОперация выполнена. Текущие струны: ";
    r.act_electronics_menu = "\n1. Зарядить на % (+)\n2. Использовать 10% (——)\n3. Изменить мощность (W)\n4. Сравнить мощность инструментов (==)\n> ";
    r.act_percent_add = "\nПроцент для зарядки: ";
    r.act_charge_added = "Заряд пополнен!\n";
    r.act_spent_10 = "\nПотрачено 10%. Текущий заряд: ";
    r.act_enter_power = "\nВведите новую мощность (W): ";
    r.act_power_set = "Мощность установлена: ";
    r.act_add_modes_title = "1. По шаблону (авто—подстановка)\n2. Вручную (ввод всех свойств)\n> ";
    r.act_add_modes_short = "1. По шаблону\n2. Вручную\n> ";
    r.act_enter_size = "Размер (дюймы): ";
    r.act_enter_material = "Материал: ";
    r.act_enter_power_w = "Мощность (W): ";

    r.menu_fill_from_data_file = "F. Автозаполнение из файла данных";
    r.msg_only_digits = "Ошибка: Введите только цифры!";
    r.msg_only_letters = "Ошибка: Введите только буквы!";
    r.msg_loaded_instruments = "Загружено инструментов: ";
    r.msg_data_file_missing = "Файл данных не найден или пуст!";

    r.prompt_group = "Группа инструмента: ";
    r.prompt_rim_color = "Цвет канта: ";
    r.prompt_body_color = "Цвет корпуса: ";
    r.prompt_neck_material = "Материал грифа: ";
    r.prompt_string_material = "Материал струн: ";
    r.prompt_initial_charge = "Начальный заряд (0—100): ";
    r.prompt_pickup_count = "Количество звукоснимателей: ";
    r.prompt_quantity_count = "Количество (Count): ";
    r.lbl_inventory = "Инвентарь: ";
    r.cust_my_inventory = "5. Инвентарь покупателя";

    r.msg_range_error_prefix = "Ошибка: Значение должно быть в диапазоне от ";
    r.msg_error_save_prefix = "Ошибка при сохранении: ";
    r.msg_error_load_prefix = "Ошибка при загрузке: ";

    r.msg_under_one_string = "Ошибка: минимум 1 струна!";
    r.masg_over_text_limit = "Ошибка: Слишком длинная строка (максимум 100 символов)!";
    r.msg_input_error_prefix = "Ошибка при вводе: ";
    r.msg_input_number_error_prefix = "Ошибка при вводе числа: ";
    r.msg_press_exclaim_to_break = "Нажмите '!' чтобы порвать струну, или любую другую клавишу для выхода.";
    r.msg_inventory_instruments_suffix = " инструментов";
    r.msg_index_out_of_range = "Индекс вне диапазона!";

    r.msg_power_equal = "Мощность совпадает (==).";
    r.msg_power_not_equal = "Мощность разная (!=).";
    r.msg_not_electronic = "У инструмента нет электроники!";
    r.input_idx_to_equal = "Введите индекс инструмента для сравнения мощности: ";
    r.msg_have_more_pow = " имеет больше мощности на ";
    r.msg_compare = " против ";

    r.task_text = "Задача: Реализовать ОО—структуры для магазина гитар.\n"
        "Классы: StringInstruments, AcousticGuitar,\n"
        "        ElectricGuitar, BassGuitar,\n"
        "        ElectronicDevice, InstrumentStore, Customer.\n"
        "Отношения: наследование, агрегация, ассоциация.\n"
        "Операторы: ++, ——, !, +, ==.\n"
        "Статический счётчик объектов.\n"
        "Локализация: RU / EN.";

    r.help_text = "Навигация:\n"
        "  ESC — назад / выход\n"
        "  TAB — смена языка\n"
        "  1—9 / 0 — выбор пункта меню\n\n"
        "Инструменты:\n"
        "  Акустическая гитара — не требует питания\n"
        "  Электрогитара — требует зарядки\n"
        "  Бас—гитара — многострунный электроинструмент\n\n"
        "Файл store.txt — база данных магазина.";

    r.any_key = "Для продолжения нажмите любую клавишу . . . ";
    r.separator = "————————————————————————————————————————————————————————————";
    r.instr_header = "——— Инструмент #";
    r.demo_auto_fill = "";
    r.lbl_strings_count = "Струны: ";
    r.lbl_pickups_count = "Звукосниматели: ";

    // ===== ENGLISH =====
    auto& e = EN_Texts;
    e.frame_hint = "For each frame: <ESC> — back; <TAB> — change language.";
    e.splash_faculty = "Faculty of IS&P";
    e.splash_city_year = "Donetsk 2026";
    e.splash_manual = "Operator manual: <s>";
    e.splash_role_sup = "Supervisor:";
    e.splash_sup_title = "Ass. dept. SE";
    e.splash_sup_name1 = "Berdyukova S.S";
    e.splash_sup_name2 = "Grischenko D.A.";
    e.splash_role_perf = "Performed by:";
    e.splash_perf_title = "st. gr. PI—25B";
    e.splash_perf_name = "Rubalko S.S.";
    e.splash_uni = "Donetsk National Technical University";
    e.splash_dept = "Software Engineering Department";
    e.splash_coursework = "Coursework project";
    e.splash_discipline = "Object—Oriented Programming";
    e.splash_topic = "Guitar Instrument Store";
    e.splash_discipline_line = "discipline: \"Object—Oriented Programming\"";
    e.splash_topic_line = "topic: \"Guitar Instrument Store\"";
    e.splash_author = "Rubalko S.S.";
    e.splash_checked = "Shchedrin S.V.";
    e.splash_start = "Start: <ENTER>";
    e.splash_change_lang = "Change lang: <TAB>";
    e.splash_exit = "Exit: <ESC>";
    e.splash_about = "About author: <a>";
    e.splash_program = "About program: <d>";

    e.menu_title = "MAIN MENU";
    e.menu_task = "1. Task Description";
    e.menu_demo = "2. Object Demonstration";
    e.menu_help = "3. Help";
    e.menu_exit = "4. Exit";

    e.demo_title = "=== GUITAR STORE ===";
    e.demo_add = "1. Add instrument";
    e.demo_list = "2. List all instruments";
    e.demo_sell_all = "3. Sell all instruments";
    e.demo_sell_one = "4. Sell instrument by index";
    e.demo_save = "5. Save database a file";
    e.demo_load = "6. Load database from file";
    e.demo_count = "7. Object count";
    e.demo_search = "8. Search by brand";
    e.demo_customer = "9. Customer actions";
    e.demo_interact = "0. Manage instrument";
    e.demo_back = "ESC — Back";

    e.add_title = "——— Add instrument ———";
    e.add_acoustic = "1. Acoustic guitar";
    e.add_electric = "2. Electric guitar";
    e.add_bass = "3. Bass guitar";

    e.act_title = "——— Manage instrument ———";
    e.act_play = "1. Play (Play)";
    e.act_setup = "2. Set up (SetUp)";
    e.act_charge = "3. Charge (Charge)";
    e.act_info = "4. Detailed info (Print)";
    e.act_tune = "5. Tune strings (Tune)";
    e.act_set_brand = "6. Change brand";
    e.act_set_strings = "7. Change strings";
    e.act_use_power = "8. Working with electricity";
    e.act_back = "ESC — Back";

    e.cust_title = "——— Customer ———";
    e.cust_buy = "1. Buy instrument (by index)";
    e.cust_view = "2. View instrument (by index)";
    e.cust_info = "3. Customer information";
    e.cust_info_change = "4. Change info about customer";
    e.change_customer = "Choose what do you want to change:\n"
        "1.Change customer name\n"
        "2.Change customer age\n";
    e.add_customer_name = "Enter new customer name: ";
    e.add_customer_age = "Enter new customer age: ";

    e.lbl_brand = "Brand:      ";
    e.lbl_count = "Count:      ";
    e.lbl_group = "Group:      ";
    e.lbl_material = "Material:   ";
    e.lbl_rim_color = "Rim Color:  ";
    e.lbl_body_size = "Body Size:  ";
    e.lbl_body_color = "Body Color: ";
    e.lbl_neck_mat = "Neck (mat): ";
    e.lbl_string_mat = "Str. (mat): ";
    e.lbl_strings = "Strings:     ";
    e.lbl_pickups = "Pickups:    ";
    e.lbl_power = "Power:      ";
    e.lbl_battery = "Battery:    ";
    e.lbl_type = "Type:       ";
    e.lbl_store_count = "Instruments in store: ";
    e.lbl_objects = "Objects created (static): ";
    e.lbl_index = "Enter index (1—N): ";
    e.lbl_name = "Customer name: ";
    e.lbl_age = "Age: ";

    e.type_acoustic = "Acoustic Guitar";
    e.type_electric = "Electric Guitar";
    e.type_bass = "Bass Guitar";

    e.msg_added = "Instrument added to the store.";
    e.msg_sold_all = "All instruments sold.";
    e.msg_sold_one = "Instrument removed from store.";
    e.msg_saved = "Database saved to store.txt";
    e.msg_loaded = "Database loaded from store.txt";
    e.msg_store_empty = "Store is empty.";
    e.msg_store_full = "Store is full!";
    e.msg_not_found = "Instrument not found.";
    e.msg_charged = "Device charged to 100%.";
    e.msg_playing = "Playing...";
    e.msg_setup_ok = "Setup completed successfully.";
    e.msg_needs_charge = "Needs charging! Battery is dead.";
    e.msg_enter_brand = "Enter brand: ";
    e.msg_enter_index = "Enter instrument number: ";
    e.msg_enter_strings = "Enter string count: ";
    e.msg_customer_buy = " is buying: ";
    e.msg_customer_view = " is inspecting: ";
    e.msg_invalid = "Invalid input!";
    e.msg_power_used = "10% power consumed.";
    e.msg_tuned = "Guitar tuned.";
    e.msg_about_author = "Author: Rubalko S.S. Group: PI—25B";
    e.msg_about_program = "Software system \"Guitar Store\" v2.0\nBuilt for Visual Studio 2026";

    e.msg_no_strings = " (No strings!)";
    e.msg_press_any_key = "\n(Press any key to stop...)";
    e.msg_string_broke = "\n*SNAP* A string broke! Remaining: ";
    e.msg_cannot_play = "Cannot play anymore, no strings left.\n";
    e.msg_finished_playing = "Finished playing.\n";
    e.msg_playing_battery = "Playing... Battery: ";
    e.msg_battery_died = "\nBattery died! Stopped playing.\n";
    e.msg_finished_battery = "Finished playing. Current battery: ";
    e.act_add_remove_string = "1. Add string (+), 2. Remove string (—)\n> ";
    e.act_str_operation = "\nOperation applied. Current strings: ";
    e.act_electronics_menu = "\n1. Add Charge (+)\n2. Use 10% Charge (——)\n3. Set Power (W)\n4. Compare instruments power (==)\n> ";
    e.act_percent_add = "\nPercent to add: ";
    e.act_charge_added = "Charge added!\n";
    e.act_spent_10 = "\nUsed 10%. Current charge: ";
    e.act_enter_power = "\nEnter new power (W): ";
    e.act_power_set = "Power set to: ";
    e.act_add_modes_title = "1. By Template (auto base properties)\n2. By Manual Input (enter all props)\n> ";
    e.act_add_modes_short = "1. By Template\n2. By Manual Input\n> ";
    e.act_enter_size = "Size (inches): ";
    e.act_enter_material = "Material: ";
    e.act_enter_power_w = "Power (W): ";

    e.menu_fill_from_data_file = "F. Auto—fill from data file";
    e.msg_only_digits = "Error: Enter digits only!";
    e.msg_only_letters = "Error: Enter letters only!";
    e.msg_loaded_instruments = "Loaded instruments: ";
    e.msg_data_file_missing = "Data file not found or empty!";

    e.prompt_group = "Instrument group: ";
    e.prompt_rim_color = "Rim color: ";
    e.prompt_body_color = "Body color: ";
    e.prompt_neck_material = "Neck material: ";
    e.prompt_string_material = "String material: ";
    e.prompt_initial_charge = "Initial charge (0—100): ";
    e.prompt_pickup_count = "Pickup count: ";
    e.prompt_quantity_count = "Quantity (Count): ";
    e.lbl_inventory = "Inventory: ";
    e.cust_my_inventory = "5. Customer inventory";

    e.msg_range_error_prefix = "Error: Value must be in range from ";
    e.msg_error_save_prefix = "Save error: ";
    e.msg_error_load_prefix = "Load error: ";

    e.msg_under_one_string = "Error: minimum 1 string!";
    e.masg_over_text_limit = "Error: Too long string (max 100 characters)!";
    e.msg_input_error_prefix = "Input error: ";
    e.msg_input_number_error_prefix = "Number input error: ";
    e.msg_press_exclaim_to_break = "Press '!' to break a string, or any other key to exit.";
    e.msg_inventory_instruments_suffix = " instruments";
    e.msg_index_out_of_range = "Index out of range!";

    e.task_text = "Task: Implement OOP structures for a Guitar Store.\n"
        "Classes: StringInstruments, AcousticGuitar,\n"
        "         ElectricGuitar, BassGuitar,\n"
        "         ElectronicDevice, InstrumentStore, Customer.\n"
        "Relations: inheritance, aggregation, association.\n"
        "Operators: ++, ——, !, +, ==.\n"
        "Static object counter.\n"
        "Localization: RU / EN.";

    e.help_text = "Navigation:\n"
        "  ESC — back / exit\n"
        "  TAB — switch language\n"
        "  1—9 / 0 — select menu item\n\n"
        "Instruments:\n"
        "  Acoustic Guitar — no power needed\n"
        "  Electric Guitar — requires charging\n"
        "  Bass Guitar     — multi—string electric\n\n"
        "File store.txt — store database.";

    e.any_key = "Press any key to continue . . . ";
    e.separator = "————————————————————————————————————————————————————————————";
    e.instr_header = "——— Instrument #";
    e.demo_auto_fill = "";
    e.lbl_strings_count = "Strings: ";
    e.lbl_pickups_count = "Pickups: ";

    e.msg_power_equal = "Power is equal (==).";
    e.msg_power_not_equal = "Power is different (!=).";
    e.msg_not_electronic = "This instrument has no electronics!";
    e.input_idx_to_equal = "Enter index of instrument to compare power: ";
    r.msg_have_more_pow = " have more power by ";
    r.msg_compare = " compare ";
}
