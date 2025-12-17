#include "raylib.h"
#include <cmath>
#include <vector>

// Размеры экрана
const int screenWidth = 1500;
const int screenHeight = 1200;

struct MetalObject {
    Vector2 position;
    float radius;
    Color color;
    Vector2 velocity; // Для добавления скорости (простая физика)
};

struct MagneticField {
    Vector2 position;
    float radius;
    float strength;  // Сила магнитного поля
    Color color;
    bool isRepelling;  // Новый флаг для противоположных магнитных полей (отталкивающие)
    float waveStrength; // Сила волны (эффект магнитного поля)
};

struct MovingObstacle {
    Vector2 position;
    float radius;
    Color color;
    Vector2 velocity;
};

// Уровни
int currentLevel = 0;
bool levelCompleted = false;
bool inMainMenu = true;  // Индикатор для главного меню
bool inLevelSelection = false;  // Индикатор для выбора уровня
float waveTime = 0.0f; // Время для волны магнитного поля

// Функция для нормализации вектора вручную
Vector2 NormalizeVector(Vector2 vector) {
    float length = sqrtf(vector.x * vector.x + vector.y * vector.y);
    if (length != 0) {
        vector.x /= length;
        vector.y /= length;
    }
    return vector;
}

// Функция для проверки коллизий между металлическим объектом и препятствием
bool CheckCollision(MetalObject& metalObject, MovingObstacle& obstacle) {
    float distance = sqrtf(powf(metalObject.position.x - obstacle.position.x, 2) + powf(metalObject.position.y - obstacle.position.y, 2));
    return distance < (metalObject.radius + obstacle.radius);
}

// Обновление игры
void UpdateGame(MetalObject& metalObject, std::vector<MagneticField>& magneticFields, std::vector<MovingObstacle>& obstacles) {
    // Движение металлического объекта с помощью клавиш
    float speed = 5.0f;
    if (IsKeyDown(KEY_RIGHT)) metalObject.position.x += speed;
    if (IsKeyDown(KEY_LEFT)) metalObject.position.x -= speed;
    if (IsKeyDown(KEY_DOWN)) metalObject.position.y += speed;
    if (IsKeyDown(KEY_UP)) metalObject.position.y -= speed;

    // Применение магнитных полей
    for (auto& magneticField : magneticFields) {
        Vector2 direction = { magneticField.position.x - metalObject.position.x, magneticField.position.y - metalObject.position.y };
        float distance = sqrtf(direction.x * direction.x + direction.y * direction.y);

        // Если объект в пределах радиуса магнитного поля
        if (distance < magneticField.radius) {
            float force = magneticField.strength / (distance + 1); // Сила магнитного поля уменьшается с увеличением расстояния
            direction = NormalizeVector(direction);  // Нормализуем вектор для правильной силы

            // Если магнитное поле отталкивающее, применяем силу в противоположном направлении
            if (magneticField.isRepelling) {
                direction.x = -direction.x;
                direction.y = -direction.y;
            }

            // Применение силы магнитного поля к металлическому объекту
            metalObject.velocity.x += direction.x * force * 0.1f;
            metalObject.velocity.y += direction.y * force * 0.1f;

            // Добавляем волновое воздействие от магнитного поля
            if (distance < magneticField.radius / 2) {
                waveTime = fmodf(waveTime + 0.05f, 1.0f);
                float waveForce = magneticField.waveStrength * sinf(waveTime * 3.14f);
                metalObject.velocity.x += direction.x * waveForce * 0.1f;
                metalObject.velocity.y += direction.y * waveForce * 0.1f;
            }
        }
    }

    // Применение скорости к металлическому объекту
    metalObject.position.x += metalObject.velocity.x;
    metalObject.position.y += metalObject.velocity.y;

    // Простая физика для торможения объекта
    metalObject.velocity.x *= 0.98f;
    metalObject.velocity.y *= 0.98f;

    // Ограничиваем движение металлического объекта, чтобы он не выходил за границы экрана
    if (metalObject.position.x - metalObject.radius < 0) metalObject.position.x = metalObject.radius;
    if (metalObject.position.x + metalObject.radius > screenWidth) metalObject.position.x = screenWidth - metalObject.radius;
    if (metalObject.position.y - metalObject.radius < 0) metalObject.position.y = metalObject.radius;
    if (metalObject.position.y + metalObject.radius > screenHeight) metalObject.position.y = screenHeight - metalObject.radius;

    // Обновление движущихся препятствий
    for (auto& obstacle : obstacles) {
        obstacle.position.x += obstacle.velocity.x;
        obstacle.position.y += obstacle.velocity.y;

        // Проверка коллизий между металлическим объектом и препятствием
        if (CheckCollision(metalObject, obstacle)) {
            metalObject.velocity.x *= -1;  // Отражаем скорость металлического объекта
            metalObject.velocity.y *= -1;
        }

        // Отскок препятствий от краев экрана
        if (obstacle.position.x - obstacle.radius < 0 || obstacle.position.x + obstacle.radius > screenWidth) {
            obstacle.velocity.x *= -1;
        }
        if (obstacle.position.y - obstacle.radius < 0 || obstacle.position.y + obstacle.radius > screenHeight) {
            obstacle.velocity.y *= -1;
        }
    }
}

// Отрисовка объектов
void DrawGame(MetalObject& metalObject, std::vector<MagneticField>& magneticFields, std::vector<MovingObstacle>& obstacles) {
    // Отображение фонового цвета
    ClearBackground(RAYWHITE);

    // Создание градиентного фона
    Color gradientStart = Color{ 30, 30, 30, 255 }; // Темный цвет
    Color gradientEnd = Color{ 10, 10, 10, 255 };   // Еще темнее
    DrawRectangleGradientV(0, 0, screenWidth, screenHeight, gradientStart, gradientEnd);

    // Отображаем магнитные поля в виде колец
    for (auto& magneticField : magneticFields) {
        int numRings = 5;  // Количество колец для эффекта магнитного поля
        for (int i = 0; i < numRings; i++) {
            float alpha = (1.0f - (float)i / numRings) * 0.5f; // Уменьшаем прозрачность колец
            Color ringColor = Color{ magneticField.color.r, magneticField.color.g, magneticField.color.b, (unsigned char)(alpha * 255) };
            DrawCircle(magneticField.position.x, magneticField.position.y, magneticField.radius + i * 10, ringColor);
        }
    }

    // Отображаем металлический объект с эффектом свечения
    DrawCircle(metalObject.position.x, metalObject.position.y, metalObject.radius, metalObject.color);
    DrawCircleLines(metalObject.position.x, metalObject.position.y, metalObject.radius + 5, GOLD);  // Свечение

    // Отображаем движущиеся препятствия
    for (auto& obstacle : obstacles) {
        DrawCircle(obstacle.position.x, obstacle.position.y, obstacle.radius, obstacle.color);
    }

    // Отображаем текущий уровень и цель
    DrawText(TextFormat("Level: %d", currentLevel + 1), 20, 20, 30, DARKBLUE);
    DrawText("Goal: Reach the top right corner!", 20, 60, 30, DARKGREEN);

    // Отображаем кнопку перехода на следующий уровень
    DrawText("Press ENTER to proceed to the next level", screenWidth / 2 - MeasureText("Press ENTER to proceed to the next level", 40) / 2, screenHeight - 100, 40, Color{ 139, 0, 0, 255 }); // Темно-красный
}

// Инициализация уровня
void InitLevel(MetalObject& metalObject, std::vector<MagneticField>& magneticFields, std::vector<MovingObstacle>& obstacles) {
    // Очистка предыдущего уровня
    magneticFields.clear();
    obstacles.clear();

    // Уровень 1
    if (currentLevel == 0) {
        metalObject.position = { screenWidth / 2, screenHeight / 2 };
        metalObject.radius = 50;  // Увеличиваем металлический шарик
        metalObject.color = Color{ 255, 0, 0, 255 }; // Ярко красный
        metalObject.velocity = { 0.0f, 0.0f };

        // Расставляем два магнитных поля на первом уровне
        magneticFields.push_back({ {screenWidth / 4, 600}, 200, 500, BLUE, false, 0.0f });  // Притягивающее поле
        magneticFields.push_back({ {screenWidth - screenWidth / 4, 600}, 200, 500, BLUE, false, 0.0f });  // Притягивающее поле

        // Добавляем движущиеся препятствия
        obstacles.push_back({ {screenWidth / 2, screenHeight / 3}, 50, RED, {2.0f, 2.0f} });
    }

    // Уровень 2
    if (currentLevel == 1) {
        metalObject.position = { screenWidth / 2, screenHeight / 2 };
        metalObject.radius = 50;  // Увеличиваем металлический шарик
        metalObject.color = Color{ 255, 0, 0, 255 }; // Ярко красный
        metalObject.velocity = { 0.0f, 0.0f };

        // Три магнитных поля на втором уровне
        magneticFields.push_back({ {screenWidth / 2, 600}, 200, 500, BLUE, false, 0.0f });
        magneticFields.push_back({ {screenWidth / 4, 600}, 200, 500, BLUE, false, 0.0f });
        magneticFields.push_back({ {screenWidth - screenWidth / 4, 600}, 200, 500, BLUE, false, 0.0f });

        // Добавляем движущиеся препятствия
        obstacles.push_back({ {screenWidth / 4, screenHeight / 3}, 50, GREEN, {-2.0f, -2.0f} });
        obstacles.push_back({ {screenWidth / 2, screenHeight / 2}, 50, PURPLE, {2.0f, -2.0f} });
    }

    // Уровень 3
    if (currentLevel == 2) {
        metalObject.position = { screenWidth / 2, screenHeight / 2 };
        metalObject.radius = 50;  // Увеличиваем металлический шарик
        metalObject.color = Color{ 255, 0, 0, 255 }; // Ярко красный
        metalObject.velocity = { 0.0f, 0.0f };

        // Магнитные поля на третьем уровне
        magneticFields.push_back({ {screenWidth / 4, 600}, 200, 500, BLUE, false, 0.2f });  // Притягивающее поле
        magneticFields.push_back({ {screenWidth - screenWidth / 4, 600}, 200, 500, BLUE, false, 0.2f });  // Притягивающее поле
        magneticFields.push_back({ {screenWidth / 2, 800}, 200, 500, RED, true, 0.3f });  // Отталкивающее поле

        // Добавляем движущиеся препятствия
        obstacles.push_back({ {screenWidth / 4, screenHeight / 3}, 60, BLUE, {3.0f, 0.0f} });
        obstacles.push_back({ {screenWidth / 2, screenHeight / 2}, 60, YELLOW, {0.0f, 3.0f} });
    }
}

// Главное меню
void DrawMainMenu() {
    ClearBackground(RAYWHITE);

    // Создание градиентного фона для главного меню
    Color gradientStart = Color{ 135, 206, 235, 255 }; // Светло-голубой
    Color gradientEnd = Color{ 70, 130, 180, 255 };   // Синий
    DrawRectangleGradientV(0, 0, screenWidth, screenHeight, gradientStart, gradientEnd);

    // Название игры по центру
    DrawText("SGayGame", screenWidth / 2 - MeasureText("SGayGame", 60) / 2, screenHeight / 3, 60, DARKBLUE);

    // Кнопки по центру
    DrawText("Press ENTER to Start", screenWidth / 2 - MeasureText("Press ENTER to Start", 40) / 2, screenHeight / 2, 40, DARKGREEN);
    DrawText("Press 1 to Select Level", screenWidth / 2 - MeasureText("Press 1 to Select Level", 40) / 2, screenHeight / 2 + 50, 40, DARKGREEN);
    DrawText("Press ESC to Exit", screenWidth / 2 - MeasureText("Press ESC to Exit", 40) / 2, screenHeight / 2 + 100, 40, DARKGREEN);

    // Описание игры внизу экрана
    DrawText("A physics-based game with magnetic fields.", screenWidth / 2 - MeasureText("A physics-based game with magnetic fields.", 30) / 2, screenHeight - 60, 30, DARKGRAY);
    DrawText("Use magnets to move and solve puzzles!", screenWidth / 2 - MeasureText("Use magnets to move and solve puzzles!", 30) / 2, screenHeight - 30, 30, DARKGRAY);
}

// Меню выбора уровня
void DrawLevelSelectionMenu() {
    ClearBackground(RAYWHITE);

    // Создание градиентного фона для выбора уровня
    Color gradientStart = Color{ 135, 206, 235, 255 }; // Светло-голубой
    Color gradientEnd = Color{ 70, 130, 180, 255 };   // Синий
    DrawRectangleGradientV(0, 0, screenWidth, screenHeight, gradientStart, gradientEnd);

    // Заголовок меню выбора уровня
    DrawText("Select Level", screenWidth / 2 - MeasureText("Select Level", 60) / 2, screenHeight / 3, 60, DARKBLUE);

    // Кнопки для выбора уровней
    DrawText("Press 1 for Level 1", screenWidth / 2 - MeasureText("Press 1 for Level 1", 40) / 2, screenHeight / 2, 40, DARKGREEN);
    DrawText("Press 2 for Level 2", screenWidth / 2 - MeasureText("Press 2 for Level 2", 40) / 2, screenHeight / 2 + 50, 40, DARKGREEN);
    DrawText("Press 3 for Level 3", screenWidth / 2 - MeasureText("Press 3 for Level 3", 40) / 2, screenHeight / 2 + 100, 40, DARKGREEN);
    DrawText("Press ESC to return to Main Menu", screenWidth / 2 - MeasureText("Press ESC to return to Main Menu", 40) / 2, screenHeight / 2 + 150, 40, DARKGREEN);
}

// Главный игровой цикл
int main() {
    // Инициализация Raylib
    InitWindow(screenWidth, screenHeight, "Magnetic Labyrinth");
    SetTargetFPS(60);

    // Создаем объекты
    MetalObject metalObject = { {screenWidth / 2, screenHeight / 2}, 50, Color{255, 0, 0, 255} };  // Увеличили размер металлического шарика
    std::vector<MagneticField> magneticFields;
    std::vector<MovingObstacle> obstacles;

    // Основное меню
    while (inMainMenu) {
        if (IsKeyPressed(KEY_ENTER)) {
            inMainMenu = false;  // Переход к игре
            InitLevel(metalObject, magneticFields, obstacles); // Инициализация первого уровня
        }

        if (IsKeyPressed(KEY_ONE)) {
            inMainMenu = false;  // Переход в выбор уровня
            inLevelSelection = true;  // Включаем выбор уровня
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            CloseWindow(); // Закрытие окна
        }

        BeginDrawing();
        DrawMainMenu();
        EndDrawing();
    }

    // Меню выбора уровня
    while (inLevelSelection) {
        if (IsKeyPressed(KEY_ONE)) {
            currentLevel = 0;
            InitLevel(metalObject, magneticFields, obstacles); // Инициализация уровня 1
            inLevelSelection = false;
        }

        if (IsKeyPressed(KEY_TWO)) {
            currentLevel = 1;
            InitLevel(metalObject, magneticFields, obstacles); // Инициализация уровня 2
            inLevelSelection = false;
        }

        if (IsKeyPressed(KEY_THREE)) {
            currentLevel = 2;
            InitLevel(metalObject, magneticFields, obstacles); // Инициализация уровня 3
            inLevelSelection = false;
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            inLevelSelection = false;
            inMainMenu = true;  // Возврат в главное меню
        }

        BeginDrawing();
        DrawLevelSelectionMenu();
        EndDrawing();
    }

    // Игровой цикл
    while (!WindowShouldClose()) {
        // Обновляем состояние игры
        UpdateGame(metalObject, magneticFields, obstacles);

        // Переход на следующий уровень через ENTER
        if (IsKeyPressed(KEY_ENTER)) {
            levelCompleted = false; // Сброс состояния завершения уровня
            currentLevel++;
            if (currentLevel > 2) {
                currentLevel = 0; // Сброс до первого уровня
            }
            InitLevel(metalObject, magneticFields, obstacles);
        }

        // Отображаем все на экране
        BeginDrawing();
        DrawGame(metalObject, magneticFields, obstacles);
        EndDrawing();
    }

    // Закрытие игры
    CloseWindow();

    return 0;
}
