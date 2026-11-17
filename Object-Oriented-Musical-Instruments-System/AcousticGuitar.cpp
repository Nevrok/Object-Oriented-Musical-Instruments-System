#include "AcousticGuitar.h"
#include "Localization.h"
#include "SoundPlayer.h"
#include <algorithm>
#include <chrono>
#include <conio.h>
#include <iomanip>
#include <thread>

using namespace std;
using namespace chrono;
using namespace this_thread;

// Конструктор по умолчанию
AcousticGuitar::AcousticGuitar()
    : StringInstruments("Yamaha F310", 6, "Acoustic", "Spruce"),
    mColorKant("Ivory"), mSize(41.0f), mColorKorpus("Natural"),
    mMatGrif("Maple"), mMatString("Nylon") {}

// Параметризованный конструктор
AcousticGuitar::AcousticGuitar(string brend, int count, string group, string material,
    string cKant, float size, string cKorpus,
    string mGrif, string mString)
    : StringInstruments(brend, count, group, material),
    mColorKant(cKant), mSize(size), mColorKorpus(cKorpus),
    mMatGrif(mGrif), mMatString(mString) {}

// Метод Play() — воспроизведение звука акустической гитары
void AcousticGuitar::Play() {
    const auto& t = Locale::get();
    if (mCount <= 0) {
        cout << t.msg_invalid << t.msg_no_strings << "\n";
        return;
    }
    cout << t.msg_playing << " [" << mBrend << "] — "
        << t.type_acoustic << t.msg_press_any_key << endl;
    cout << t.msg_press_exclaim_to_break << endl;

    // Начинаем воспроизведение звука акустической гитары
    SoundPlayer sound;
    sound.Play(L"acousticguitarsound.mp3");

    auto lastPing = steady_clock::now();
    while (true) {
        if (_kbhit()) {
            int key = _getch();
            if (key == '!') {
                // Рвем струну по нажатию '!'
                if (mCount > 0) {
                    // Останавливаем основной звук и воспроизводим звук разрыва струны
                    sound.Stop();
                    bool played = sound.Play(L"brokestringsound.mp3");
                    !(*this);
                    cout << t.msg_string_broke << mCount << "\n";
                    if (played) {
                        const auto end = steady_clock::now() + seconds(4);
                        while (steady_clock::now() < end && !_kbhit()) {
                            sleep_for(milliseconds(50));
                        }
                        if (_kbhit()) {
                            _getch();
                            break;
                        }
                    }

                    if (mCount <= 0) {
                        cout << t.msg_cannot_play;
                        sound.Stop();
                        return;
                    }
                    sound.Stop();
                    // Начинаем игру заново
                    sound.Play(L"acousticguitarsound.mp3");
                }
            }
            else {
                // Любая другая клавиша — выход
                break;
            }
        }
        // Периодически выводим, что акустическая гитара играет (раз в 2 секунды)
        auto now = steady_clock::now();
        if (duration_cast<seconds>(now - lastPing).count() >= 2) {
            cout << t.msg_playing << " [" << mBrend << "]" << endl;
            lastPing = now;
        }
        sleep_for(milliseconds(50));
    }
    // Останавливаем воспроизведение при выходе
    sound.Stop();
    cout << t.msg_finished_playing;
}
// Метод SetUp() — настройка акустической гитары
bool AcousticGuitar::SetUp() {
    const auto& t = Locale::get();
    cout << t.msg_setup_ok << " [" << mBrend << "] "
        << t.type_acoustic << endl;
    return true;
}
// Метод Tune() — настройка струн акустической гитары
void AcousticGuitar::Tune() {
    const auto& t = Locale::get();
    cout << t.msg_tuned << " [" << mBrend << "] "
        << mCount << " " << t.lbl_strings << endl;
}
// Метод Print() — вывод информации об акустической гитаре
void AcousticGuitar::Print() {
    const auto& t = Locale::get();
    StringInstruments::Print();
    cout << t.lbl_rim_color << mColorKant << endl;
    cout << t.lbl_body_size << mSize << "\"" << endl;
    cout << t.lbl_body_color << mColorKorpus << endl;
    cout << t.lbl_neck_mat << mMatGrif << endl;
    cout << t.lbl_string_mat << mMatString << endl;
}
// Метод GetTypeName() — получение типа инструмента
string AcousticGuitar::GetTypeName() const {
    return Locale::get().type_acoustic;
}
AcousticGuitar& AcousticGuitar::operator!() {
    if (mCount > 0) {
        mCount--;
    }
    return *this;
}


