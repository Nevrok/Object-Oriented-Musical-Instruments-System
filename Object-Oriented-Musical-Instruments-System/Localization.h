#pragma once
#include <string>
using namespace std;

enum class Lang { RU, EN };

struct TextResources {
    string frame_hint;       //Для каждого кадра: <ESC> — назад; <TAB> — язык
    string splash_faculty;
    string splash_city_year;
    string splash_manual;
    string splash_role_sup;
    string splash_sup_title;
    string splash_sup_name1;
    string splash_sup_name2;
    string splash_role_perf;
    string splash_perf_title;
    string splash_perf_name;
    // Заставка
    string splash_uni;
    string splash_dept;
    string splash_coursework;
    string splash_discipline;
    string splash_topic;
    string splash_author;
    string splash_checked;
    string splash_start;
    string splash_change_lang;
    string splash_exit;
    string splash_about;
    string splash_program;
    string splash_discipline_line;
    string splash_topic_line;

    // Главное меню
    string menu_title;
    string menu_task;
    string menu_demo;
    string menu_help;
    string menu_exit;

    // Меню магазина
    string demo_title;
    string demo_add;
    string demo_list;
    string demo_sell_all;
    string demo_sell_one;
    string demo_save;
    string demo_load;
    string demo_count;
    string demo_back;
    string demo_search;
    string demo_customer;
    string demo_interact;

    // Подменю добавления
    string add_title;
    string add_acoustic;
    string add_electric;
    string add_bass;

    // Действия с инструментом
    string act_title;
    string act_play;
    string act_setup;
    string act_charge;
    string act_info;
    string act_tune;
    string act_set_brand;
    string act_set_strings;
    string act_use_power;
    string act_back;

    // Меню покупателя
    string cust_title;
    string cust_buy;
    string cust_view;
    string cust_info;
    string cust_info_change;
    string change_customer;
    string add_customer_name;
    string add_customer_age;

    // Подписи
    string lbl_brand;
    string lbl_count;
    string lbl_group;
    string lbl_material;
    string lbl_rim_color;
    string lbl_body_size;
    string lbl_body_color;
    string lbl_neck_mat;
    string lbl_string_mat;
    string lbl_strings;
    string lbl_pickups;
    string lbl_power;
    string lbl_battery;
    string lbl_type;
    string lbl_store_count;
    string lbl_objects;
    string lbl_index;
    string lbl_name;
    string lbl_age;

    // Названия типов
    string type_acoustic;
    string type_electric;
    string type_bass;

    // Сообщения
    string msg_added;
    string msg_sold_all;
    string msg_sold_one;
    string msg_saved;
    string msg_loaded;
    string msg_store_empty;
    string msg_store_full;
    string msg_not_found;
    string msg_charged;
    string msg_playing;
    string msg_setup_ok;
    string msg_needs_charge;
    string msg_enter_brand;
    string msg_enter_index;
    string msg_enter_strings;
    string msg_customer_buy;
    string msg_customer_view;
    string msg_invalid;
    string msg_power_used;
    string msg_tuned;
    string msg_about_author;
    string msg_about_program;
    string msg_no_strings;
    string msg_press_any_key;
    string msg_string_broke;
    string msg_cannot_play;
    string msg_finished_playing;
    string msg_playing_battery;
    string msg_battery_died;
    string msg_finished_battery;
    string act_add_remove_string;
    string act_str_operation;
    string act_electronics_menu;
    string act_percent_add;
    string act_charge_added;
    string act_spent_10;
    string act_enter_power;
    string act_power_set;
    string act_add_modes_title;
    string act_add_modes_short;
    string act_enter_size;
    string act_enter_material;
    string act_enter_power_w;
    string demo_auto_fill;
    string lbl_strings_count;
    string lbl_pickups_count;
    string msg_not_electronic;
    string input_idx_to_equal;
    string msg_have_more_pow;
    string msg_compare;

    string menu_fill_from_data_file;
    string msg_only_digits;
    string msg_only_letters;
    string msg_loaded_instruments;
    string msg_data_file_missing;

    // Приглашения ручного ввода
    string prompt_group;
    string prompt_rim_color;
    string prompt_body_color;
    string prompt_neck_material;
    string prompt_string_material;
    string prompt_initial_charge;
    string prompt_pickup_count;
    string prompt_quantity_count;
    string lbl_inventory;
    string cust_my_inventory;

    // Общий текст ошибок / диапазоны
    string msg_range_error_prefix;
    string msg_error_save_prefix;
    string msg_error_load_prefix;
    string msg_under_one_string;
    string masg_over_text_limit;
    string msg_input_error_prefix;
    string msg_input_number_error_prefix;
    string msg_press_exclaim_to_break;
    string msg_inventory_instruments_suffix;
    string msg_index_out_of_range;

    // Текст задания
    string task_text;

    // Текст справки
    string help_text;

    // Разное
    string any_key;
    string separator;
    string instr_header;

    // Дополнительные сообщения о мощности
    string msg_power_equal;     // Сообщение о равенстве мощности
    string msg_power_not_equal; // Сообщение о различии мощности
};

class Locale {
public:
    static Lang current;
    static TextResources RU_Texts;
    static TextResources EN_Texts;
    static void init();
    static const TextResources& get() { return (current == Lang::RU) ? RU_Texts : EN_Texts; }
    static void toggle() { current = (current == Lang::RU) ? Lang::EN : Lang::RU; }
    static string getLangName() { return (current == Lang::RU) ? "RU" : "EN"; }
};
