#include "Game.h"
#include <fstream>
#include <sstream>

// Функция для генерации цветов фишек
sf::Color getTileColor(int number, bool selected, bool movable) {
    if (number == 0) { // Для пустых вершин
        if (movable) {
            return sf::Color(50, 50, 50, 255); // Тёмный и непрозрачный для доступных позиций
        }
        return sf::Color(200, 200, 200); // Обычная вершина
    }
    // Более тёмные цвета для фишек
    std::vector<sf::Color> chipColors = {
        sf::Color(204, 40, 40),   // Тёмно-красный
        sf::Color(40, 204, 40),   // Тёмно-зелёный
        sf::Color(40, 40, 204),   // Тёмно-синий
        sf::Color(204, 204, 40),  // Тёмно-жёлтый
        sf::Color(204, 40, 204)   // Тёмно-фиолетовый
    };
    sf::Color baseColor = chipColors[(number - 1) % chipColors.size()];
    if (selected) {
        // Для выбранной фишки увеличиваем яркость
        return sf::Color(
            std::min(255, baseColor.r + 100),
            std::min(255, baseColor.g + 100),
            std::min(255, baseColor.b + 100)
        );
    }
    return baseColor;
}

Game::Game() : window(sf::VideoMode(400, 400), "Cupboards"), selectedChip(-1),
isAnimating(false), animatingChip(-1), targetVertex(-1), animationProgress(0.0f) {
    window.setVerticalSyncEnabled(true); // Включаем VSync для плавности
    font.loadFromFile("arial.ttf");
}

bool Game::isOccupied(int vertex) const {
    for (size_t i = 0; i < chipPositions.size(); ++i) {
        if (chipPositions[i] == vertex && static_cast<int>(i) != selectedChip) return true;
    }
    return false;
}

std::vector<int> Game::findPath(int start, int end) {
    std::vector<bool> visited(vertices.size(), false);
    std::vector<int> parent(vertices.size(), -1);
    std::queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        if (current == end) {
            std::vector<int> path;
            int p = end;
            while (p != -1) {
                path.push_back(p);
                p = parent[p];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }
        for (int next : adjList[current]) {
            if (!visited[next] && !isOccupied(next)) {
                visited[next] = true;
                parent[next] = current;
                q.push(next);
            }
        }
    }
    return {};
}

void Game::render() {
    window.clear(sf::Color::White);

    // Отрисовка рёбер как двойных линий
    for (size_t i = 0; i < adjList.size(); ++i) {
        for (int j : adjList[i]) {
            if (i < static_cast<size_t>(j)) { // Избегаем дублирования рёбер
                sf::Vector2f start(vertices[i].x, vertices[i].y);
                sf::Vector2f end(vertices[j].x, vertices[j].y);
                sf::Vector2f direction = end - start;
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if (length > 0) {
                    sf::Vector2f unitDir = direction / length;
                    sf::Vector2f normal(-unitDir.y, unitDir.x);
                    sf::Vector2f offset = normal * (EDGE_OFFSET / 2.0f);

                    // Первая линия
                    sf::Vertex line1[] = {
                        sf::Vertex(start + offset, sf::Color::Black),
                        sf::Vertex(end + offset, sf::Color::Black)
                    };
                    // Вторая линия
                    sf::Vertex line2[] = {
                        sf::Vertex(start - offset, sf::Color::Black),
                        sf::Vertex(end - offset, sf::Color::Black)
                    };
                    window.draw(line1, 2, sf::Lines);
                    window.draw(line2, 2, sf::Lines);
                }
            }
        }
    }

    // Отрисовка вершин
    sf::CircleShape vertexShape(RADIUS);
    vertexShape.setOrigin(RADIUS, RADIUS);
    for (size_t i = 0; i < vertices.size(); ++i) {
        bool isMovable = std::find(movablePositions.begin(), movablePositions.end(), i) != movablePositions.end();
        vertexShape.setPosition(vertices[i].x, vertices[i].y);
        vertexShape.setFillColor(getTileColor(0, false, isMovable));
        window.draw(vertexShape);
    }

    // Отрисовка фишек без номеров
    sf::CircleShape chipShape(RADIUS * 0.8f);
    chipShape.setOrigin(RADIUS * 0.8f, RADIUS * 0.8f);
    sf::CircleShape glowShape(RADIUS * 1.2f); // Круг для подсветки
    glowShape.setOrigin(RADIUS * 1.2f, RADIUS * 1.2f);
    glowShape.setFillColor(sf::Color(255, 255, 200, 50)); // Светлый полупрозрачный цвет подсветки
    for (size_t i = 0; i < chipPositions.size(); ++i) {
        bool isSelected = (static_cast<int>(i) == selectedChip);
        chipShape.setPosition(currentChipPositions[i]);
        chipShape.setFillColor(getTileColor(chipNumbers[i], isSelected));
        if (isSelected) {
            glowShape.setPosition(currentChipPositions[i]);
            window.draw(glowShape); // Отрисовка подсветки для выбранной фишки
        }
        window.draw(chipShape);
    }

    // Проверка победы и отображение текста
    if (isWin()) {
        sf::Text winText;
        winText.setFont(font);
        winText.setString(L"Победа!"); // Используем std::wstring для кириллицы
        winText.setCharacterSize(30);
        winText.setFillColor(sf::Color::Green);
        winText.setPosition(150, 150);
        window.draw(winText);
    }

    window.display();
}

bool Game::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    int numVertices;
    file >> numVertices;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    vertices.resize(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        std::getline(file, line);
        std::stringstream ss(line);
        std::string xStr, yStr;
        std::getline(ss, xStr, ',');
        std::getline(ss, yStr);
        vertices[i].x = std::stof(xStr);
        vertices[i].y = std::stof(yStr);
    }

    int numEdges;
    file >> numEdges;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    adjList.assign(numVertices, std::vector<int>());
    for (int i = 0; i < numEdges; ++i) {
        int v1, v2;
        file >> v1 >> v2;
        adjList[v1].push_back(v2);
        adjList[v2].push_back(v1); // Ненаправленный граф
    }

    int numChips;
    file >> numChips;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    chipNumbers.resize(numChips);
    chipPositions.resize(numChips);
    targetPositions.resize(numChips);
    currentChipPositions.resize(numChips, sf::Vector2f(0, 0));
    for (int i = 0; i < numChips; ++i) {
        file >> chipNumbers[i];
    }
    for (int i = 0; i < numChips; ++i) {
        file >> chipPositions[i];
        currentChipPositions[i] = sf::Vector2f(vertices[chipPositions[i]].x, vertices[chipPositions[i]].y);
    }
    for (int i = 0; i < numChips; ++i) {
        file >> targetPositions[i];
    }

    file.close();
    return true;
}

void Game::updateMovablePositions() {
    movablePositions.clear();
    if (selectedChip == -1 || selectedChip >= static_cast<int>(chipPositions.size())) {
        return;
    }
    int start = chipPositions[selectedChip];
    for (int i = 0; i < static_cast<int>(vertices.size()); ++i) {
        if (i == start) continue;
        std::vector<int> path = findPath(start, i);
        if (!path.empty()) {
            movablePositions.push_back(i);
        }
    }
}

void Game::startAnimation(int chipIndex, int target) {
    animatingChip = chipIndex;
    targetVertex = target;
    animationPath = findPath(chipPositions[chipIndex], target);
    if (animationPath.empty()) {
        return;
    }
    isAnimating = true;
    animationProgress = 0.0f;
    animationClock.restart();
    deltaClock.restart();
    currentChipPositions[chipIndex] = sf::Vector2f(vertices[animationPath[0]].x, vertices[animationPath[0]].y);
}

void Game::updateAnimation() {
    if (!isAnimating) {
        return;
    }

    if (animationPath.size() < 2) {
        isAnimating = false;
        if (animatingChip >= 0 && targetVertex >= 0) {
            currentChipPositions[animatingChip] = sf::Vector2f(vertices[targetVertex].x, vertices[targetVertex].y);
            chipPositions[animatingChip] = targetVertex;
        }
        animationPath.clear();
        animatingChip = -1;
        targetVertex = -1;
        return;
    }

    float dt = deltaClock.restart().asSeconds();
    float totalDuration = ANIMATION_DURATION * (animationPath.size() - 1);
    animationProgress += dt / totalDuration;
    if (animationProgress > 1.0f) animationProgress = 1.0f;

    float totalProgress = animationProgress * (animationPath.size() - 1);
    int currentSegment = static_cast<int>(totalProgress);
    float segmentProgress = totalProgress - currentSegment;

    if (currentSegment >= static_cast<int>(animationPath.size()) - 1) {
        currentSegment = animationPath.size() - 2;
        segmentProgress = 1.0f;
    }

    int startVertex = animationPath[currentSegment];
    int endVertex = animationPath[currentSegment + 1];
    sf::Vector2f startPos(vertices[startVertex].x, vertices[startVertex].y);
    sf::Vector2f endPos(vertices[endVertex].x, vertices[endVertex].y);
    currentChipPositions[animatingChip] = startPos + (endPos - startPos) * segmentProgress;

    if (animationProgress >= 1.0f) {
        currentChipPositions[animatingChip] = sf::Vector2f(vertices[targetVertex].x, vertices[targetVertex].y);
        chipPositions[animatingChip] = targetVertex;
        isAnimating = false;
        animatingChip = -1;
        targetVertex = -1;
        animationPath.clear();
    }
}

void Game::handleClick(float x, float y) {
    if (isAnimating) {
        return;
    }
    for (int i = 0; i < static_cast<int>(vertices.size()); ++i) {
        float dx = x - vertices[i].x;
        float dy = y - vertices[i].y;
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance <= RADIUS) {
            if (selectedChip == -1) {
                for (size_t j = 0; j < chipPositions.size(); ++j) {
                    if (chipPositions[j] == i) {
                        selectedChip = static_cast<int>(j);
                        updateMovablePositions();
                        return;
                    }
                }
            }
            else {
                for (size_t j = 0; j < chipPositions.size(); ++j) {
                    if (chipPositions[j] == i && static_cast<int>(j) == selectedChip) {
                        selectedChip = -1;
                        movablePositions.clear();
                        return;
                    }
                }
                bool isMovable = std::find(movablePositions.begin(), movablePositions.end(), i) != movablePositions.end();
                if (isMovable) {
                    startAnimation(selectedChip, i);
                    selectedChip = -1;
                    movablePositions.clear();
                    return;
                }
            }
            return;
        }
    }
}

bool Game::isWin() const {
    for (size_t i = 0; i < chipPositions.size(); ++i) {
        if (chipPositions[i] != targetPositions[i]) return false;
    }
    return true;
}

void Game::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                handleClick(mousePos.x, mousePos.y);
            }
        }

        updateAnimation();
        render();
    }
}