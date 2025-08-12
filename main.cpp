#include "Game.h"
#include <iostream>
#include <windows.h>
#include <locale>
#include <string>

// Основная функция: вывод меню и запуск игры
int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::wcout.imbue(std::locale("ru_RU.UTF-8"));

    // Список конфигурационных файлов и их названий
    std::vector<std::pair<std::string, std::wstring>> configFiles = {
        {"config.txt", L"Гексагон"},
        {"config1.txt", L"Звезда"},
        {"config2.txt", L"Квадрат"},
        {"config3.txt", L"Треугольник с веткой"},
        {"config4.txt", L"Цепочка"},
        {"config5.txt", L"Квадрат с крестом"},
        {"config6.txt", L"Квадрат с центром"},
        {"config7.txt", L"Кольцо с центром"},
        {"config8.txt", L"Пентагон с крылом"},
        {"config9.txt", L"Пентагон усиленный"}
    };

    // Вывод меню выбора шаблона
    std::wcout << L"Выберите шаблон игры (введите номер от 0 до 9):\n";
    for (size_t i = 0; i < configFiles.size(); ++i) {
        std::wcout << i << L": " << configFiles[i].second << L"\n";
    }
    std::wcout << L"Ваш выбор: ";

    int choice;
    while (!(std::cin >> choice) || choice < 0 || choice >= static_cast<int>(configFiles.size())) {
        std::wcout << L"Ошибка: введите число от 0 до " << configFiles.size() - 1 << L": ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string selectedConfig = configFiles[choice].first;
    std::wstring selectedName = configFiles[choice].second;
    std::wcout << L"Выбран шаблон: " << selectedName << L" (" << selectedConfig.c_str() << L")\n";

    Game game;
    game.loadConfig(selectedConfig);
    game.run();
    return 0;
}