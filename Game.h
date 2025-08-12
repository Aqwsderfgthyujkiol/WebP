#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>

// ��������� ��� ������� � ��������
const float RADIUS = 20.0f; // ������ �������
const float ANIMATION_DURATION = 0.2f; // ������������ �������� �� ���� �����
const float EDGE_OFFSET = 4.0f; // ���������� ����� ���������� ������� �����

// ��������� ��� �������� ��������� �������
struct Pos {
    float x, y;
    Pos(float _x = 0, float _y = 0) : x(_x), y(_y) {}
    bool operator==(const Pos& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Pos& other) const { return !(*this == other); }
};

// ������� ��� ��������� ������ �����
sf::Color getTileColor(int number, bool selected = false, bool movable = false);

class Game {
private:
    std::vector<Pos> vertices; // ���������� ������
    std::vector<std::vector<int>> adjList; // ������ ���������
    std::vector<int> chipNumbers; // ������ �����
    std::vector<int> chipPositions; // ������� ������� (������� ������)
    std::vector<int> targetPositions; // ������� �������
    std::vector<sf::Vector2f> currentChipPositions; // ������� ������� ����� ��� ��������
    sf::RenderWindow window;
    int selectedChip; // ������ ��������� ����� (-1, ���� �� �������)
    std::vector<int> movablePositions; // ��������� ������� ��� �����������
    bool isAnimating; // ���� ��������
    int animatingChip; // ������ ����������� �����
    int targetVertex; // ������� ������� ��� ��������
    float animationProgress; // �������� �������� (0.0 - 1.0)
    sf::Clock animationClock; // ������ ��������
    sf::Clock deltaClock; // ������ ��� ������-�������
    sf::Font font; // ����� ��� ������
    std::vector<int> animationPath; // ���� ��� ��������

    // ��������, ������ �� �������
    bool isOccupied(int vertex) const;

    // ����� ���� � BFS
    std::vector<int> findPath(int start, int end);

    // ��������� ����� � �����
    void render();

public:
    Game();

    // �������� ������������ �� �����
    bool loadConfig(const std::string& filename);

    // ���������� ��������� ������� ��� �����������
    void updateMovablePositions();

    // ������ �������� ����������� �����
    void startAnimation(int chipIndex, int target);

    // ���������� ��������
    void updateAnimation();

    // ��������� ����� ����
    void handleClick(float x, float y);

    // �������� ������� ������
    bool isWin() const;

    // �������� ���� ����
    void run();
};