#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>

// Константы для графики и анимации
const float RADIUS = 20.0f; // Радиус вершины
const float ANIMATION_DURATION = 0.2f; // Длительность анимации на одно ребро
const float EDGE_OFFSET = 4.0f; // Расстояние между граничными линиями ребра

// Структура для хранения координат вершины
struct Pos {
    float x, y;
    Pos(float _x = 0, float _y = 0) : x(_x), y(_y) {}
    bool operator==(const Pos& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Pos& other) const { return !(*this == other); }
};

// Функция для генерации цветов фишек
sf::Color getTileColor(int number, bool selected = false, bool movable = false);

class Game {
private:
    std::vector<Pos> vertices; // Координаты вершин
    std::vector<std::vector<int>> adjList; // Список смежности
    std::vector<int> chipNumbers; // Номера фишек
    std::vector<int> chipPositions; // Текущие позиции (индексы вершин)
    std::vector<int> targetPositions; // Целевые позиции
    std::vector<sf::Vector2f> currentChipPositions; // Текущие позиции фишек для анимации
    sf::RenderWindow window;
    int selectedChip; // Индекс выбранной фишки (-1, если не выбрана)
    std::vector<int> movablePositions; // Возможные позиции для перемещения
    bool isAnimating; // Флаг анимации
    int animatingChip; // Индекс анимируемой фишки
    int targetVertex; // Целевая вершина для анимации
    float animationProgress; // Прогресс анимации (0.0 - 1.0)
    sf::Clock animationClock; // Таймер анимации
    sf::Clock deltaClock; // Таймер для дельта-времени
    sf::Font font; // Шрифт для текста
    std::vector<int> animationPath; // Путь для анимации

    // Проверка, занята ли вершина
    bool isOccupied(int vertex) const;

    // Поиск пути с BFS
    std::vector<int> findPath(int start, int end);

    // Отрисовка графа и фишек
    void render();

public:
    Game();

    // Загрузка конфигурации из файла
    bool loadConfig(const std::string& filename);

    // Обновление доступных позиций для перемещения
    void updateMovablePositions();

    // Начало анимации перемещения фишки
    void startAnimation(int chipIndex, int target);

    // Обновление анимации
    void updateAnimation();

    // Обработка клика мыши
    void handleClick(float x, float y);

    // Проверка условия победы
    bool isWin() const;

    // Основной цикл игры
    void run();
};