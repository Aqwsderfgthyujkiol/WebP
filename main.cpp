#include "Game.h"
#include <iostream>
#include <windows.h>
#include <locale>
#include <string>

// �������� �������: ����� ���� � ������ ����
int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::wcout.imbue(std::locale("ru_RU.UTF-8"));

    // ������ ���������������� ������ � �� ��������
    std::vector<std::pair<std::string, std::wstring>> configFiles = {
        {"config.txt", L"��������"},
        {"config1.txt", L"������"},
        {"config2.txt", L"�������"},
        {"config3.txt", L"����������� � ������"},
        {"config4.txt", L"�������"},
        {"config5.txt", L"������� � �������"},
        {"config6.txt", L"������� � �������"},
        {"config7.txt", L"������ � �������"},
        {"config8.txt", L"�������� � ������"},
        {"config9.txt", L"�������� ���������"}
    };

    // ����� ���� ������ �������
    std::wcout << L"�������� ������ ���� (������� ����� �� 0 �� 9):\n";
    for (size_t i = 0; i < configFiles.size(); ++i) {
        std::wcout << i << L": " << configFiles[i].second << L"\n";
    }
    std::wcout << L"��� �����: ";

    int choice;
    while (!(std::cin >> choice) || choice < 0 || choice >= static_cast<int>(configFiles.size())) {
        std::wcout << L"������: ������� ����� �� 0 �� " << configFiles.size() - 1 << L": ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string selectedConfig = configFiles[choice].first;
    std::wstring selectedName = configFiles[choice].second;
    std::wcout << L"������ ������: " << selectedName << L" (" << selectedConfig.c_str() << L")\n";

    Game game;
    game.loadConfig(selectedConfig);
    game.run();
    return 0;
}