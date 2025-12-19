#include "raylib.h"
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

// ==================== КОНСТАНТЫ И ЦВЕТА ====================
// Постельные пастельные тона
#define COLOR_BACKGROUND CLITERAL(Color){ 250, 245, 240, 255 }    // Кремовый фон
#define COLOR_BUTTON_NORMAL CLITERAL(Color){ 180, 210, 225, 255 } // Голубой
#define COLOR_BUTTON_HOVER CLITERAL(Color){ 200, 230, 245, 255 }  // Светло-голубой
#define COLOR_BUTTON_CLICK CLITERAL(Color){ 160, 190, 205, 255 }  // Темно-голубой
#define COLOR_TEXT CLITERAL(Color){ 60, 70, 80, 255 }            // Темно-серый
#define COLOR_ACCENT CLITERAL(Color){ 255, 180, 180, 255 }       // Розовый акцент
#define COLOR_SUCCESS CLITERAL(Color){ 180, 230, 180, 255 }      // Зеленый успех
#define COLOR_TARGET CLITERAL(Color){ 200, 180, 220, 150 }       // Сиреневый цель
#define COLOR_BALL_1 CLITERAL(Color){ 255, 200, 200, 255 }       // Розовый шар
#define COLOR_BALL_2 CLITERAL(Color){ 200, 220, 255, 255 }       // Голубой шар
#define COLOR_BALL_3 CLITERAL(Color){ 220, 255, 220, 255 }       // Мятный шар
#define COLOR_SHADOW CLITERAL(Color){ 0, 0, 0, 50 }              // Тень

// ==================== КЛАССЫ ====================
class Ball {
public:
    Vector2 position;
    Vector2 velocity;
    float radius;
    Color color;
    bool isDragging;
    Vector2 originalPosition;

    Ball(float x, float y, float r, Color c) {
        position = { x, y };
        originalPosition = { x, y };
        velocity = { 0, 0 };
        radius = r;
        color = c;
        isDragging = false;
    }

    void Update(float deltaTime) {
        if (!isDragging) {
            // Гравитация
            velocity.y += 800.0f * deltaTime;

            // Движение
            position.x += velocity.x * deltaTime;
            position.y += velocity.y * deltaTime;

            // Затухание
            velocity.x *= 0.99f;
            velocity.y *= 0.99f;

            // Отскок от границ
            if (position.x - radius < 0) {
                position.x = radius;
                velocity.x = -velocity.x * 0.8f;
            }
            if (position.x + radius > 800) {
                position.x = 800 - radius;
                velocity.x = -velocity.x * 0.8f;
            }
            if (position.y - radius < 0) {
                position.y = radius;
                velocity.y = -velocity.y * 0.8f;
            }
            if (position.y + radius > 600) {
                position.y = 600 - radius;
                velocity.y = -velocity.y * 0.8f;
            }
        }
    }

    void Draw() {
        // Тень
        DrawCircle(position.x + 3, position.y + 3, radius, COLOR_SHADOW);
        // Шар
        DrawCircleV(position, radius, color);
        // Обводка
        DrawCircleLines(position.x, position.y, radius, Fade(WHITE, 0.3f));
        // Блики
        DrawCircle(position.x - radius / 3, position.y - radius / 3, radius / 4, Fade(WHITE, 0.3f));
    }

    void Reset() {
        position = originalPosition;
        velocity = { 0, 0 };
        isDragging = false;
    }

    bool Contains(Vector2 point) {
        float dx = point.x - position.x;
        float dy = point.y - position.y;
        return (dx * dx + dy * dy) <= (radius * radius);
    }
};

class Target {
public:
    Rectangle rect;
    Color color;
    bool isActive;

    Target(float x, float y, float w, float h) {
        rect = { x, y, w, h };
        color = COLOR_TARGET;
        isActive = false;
    }

    void Update(Ball& ball) {
        isActive = CheckCollisionCircleRec(ball.position, ball.radius, rect);
        color = isActive ? COLOR_SUCCESS : COLOR_TARGET;
    }

    void Draw() {
        // Тень
        DrawRectangleRec({ rect.x + 3, rect.y + 3, rect.width, rect.height }, COLOR_SHADOW);
        // Тело цели
        DrawRectangleRec(rect, color);
        // Обводка
        DrawRectangleLinesEx(rect, 3, Fade(WHITE, 0.5f));
        // Значок цели
        DrawText("o", rect.x + rect.width / 2 - 5, rect.y + rect.height / 2 - 10, 20, WHITE);
    }
};

class Button {
public:
    Rectangle rect;
    const char* text;  // Изменено на const char*
    Color color;
    bool isHovered;
    bool isClicked;
    float scale;

    Button(float x, float y, float w, float h, const char* t) {  // Изменено на const char*
        rect = { x, y, w, h };
        text = t;
        color = COLOR_BUTTON_NORMAL;
        isHovered = false;
        isClicked = false;
        scale = 1.0f;
    }

    virtual void Update(Vector2 mousePos) {
        isHovered = CheckCollisionPointRec(mousePos, rect);
        isClicked = isHovered && IsMouseButtonDown(MOUSE_LEFT_BUTTON);

        if (isClicked) {
            color = COLOR_BUTTON_CLICK;
            scale = 0.95f;
        }
        else if (isHovered) {
            color = COLOR_BUTTON_HOVER;
            scale = 1.05f;
        }
        else {
            color = COLOR_BUTTON_NORMAL;
            scale = 1.0f;
        }
    }

    bool IsPressed() {
        return isHovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
    }

    virtual void Draw() {
        // Тень
        DrawRectangleRounded({ rect.x + 5, rect.y + 5, rect.width, rect.height }, 0.3f, 10, COLOR_SHADOW);

        // Тело кнопки
        Rectangle scaledRect = {
            rect.x - (rect.width * (scale - 1.0f)) / 2,
            rect.y - (rect.height * (scale - 1.0f)) / 2,
            rect.width * scale,
            rect.height * scale
        };
        DrawRectangleRounded(scaledRect, 0.3f, 10, color);

        // Обводка
        Color borderColor = Fade(WHITE, 0.5f);
        DrawRectangleRoundedLines(scaledRect, 0.3f, 10, borderColor);

        // Текст
        int fontSize = 24;
        int textWidth = MeasureText(text, fontSize);
        int textX = rect.x + rect.width / 2 - textWidth / 2;
        int textY = rect.y + rect.height / 2 - fontSize / 2;

        DrawText(text, textX, textY, fontSize, COLOR_TEXT);
    }
};

class LevelButton : public Button {
public:
    int level;
    bool isLocked;

    LevelButton(float x, float y, int lvl) : Button(x, y, 180, 80, TextFormat("Level %d", lvl)) {
        level = lvl;
        isLocked = (lvl > 1); // Первый уровень открыт, остальные закрыты
    }

    virtual void Update(Vector2 mousePos) override {
        Button::Update(mousePos);
    }

    virtual void Draw() override {
        // Рисуем базовую кнопку
        Rectangle scaledRect = {
            rect.x - (rect.width * (scale - 1.0f)) / 2,
            rect.y - (rect.height * (scale - 1.0f)) / 2,
            rect.width * scale,
            rect.height * scale
        };

        // Тень
        DrawRectangleRounded({ rect.x + 5, rect.y + 5, rect.width, rect.height }, 0.3f, 10, COLOR_SHADOW);
        // Тело кнопки
        DrawRectangleRounded(scaledRect, 0.3f, 10, color);
        // Обводка
        Color borderColor = Fade(WHITE, 0.5f);
        DrawRectangleRoundedLines(scaledRect, 0.3f, 10, borderColor);

        // Текст
        int fontSize = 24;
        int textWidth = MeasureText(text, fontSize);
        int textX = rect.x + rect.width / 2 - textWidth / 2;
        int textY = rect.y + rect.height / 2 - fontSize / 2;

        DrawText(text, textX, textY, fontSize, COLOR_TEXT);

        if (isLocked) {
            DrawRectangleRec(scaledRect, Fade(BLACK, 0.5f));
            DrawText("?", rect.x + rect.width / 2 - 5, rect.y + rect.height / 2 - 15, 30, WHITE);
        }
        else {
            // Значок уровня
            const char* icon;
            switch (level) {
            case 1: icon = "1"; break;
            case 2: icon = "2"; break;
            case 3: icon = "3"; break;
            default: icon = "*";
            }
            DrawText(icon, rect.x + rect.width - 40, rect.y + rect.height / 2 - 15, 30, COLOR_ACCENT);
        }
    }
};

// ==================== ГЛАВНЫЙ КЛАСС ИГРЫ ====================
class PhysicsGame {
private:
    // Состояния игры
    enum GameState {
        MAIN_MENU,
        LEVEL_SELECT,
        GAME,
        LEVEL_COMPLETE
    };

    GameState currentState;

    // Игровые объекты
    std::vector<Ball> balls;
    std::vector<Target> targets;
    Ball* draggedBall;
    Vector2 dragOffset;

    // UI элементы - теперь с английским текстом
    Button startButton;
    Button exitButton;
    Button backButton;
    Button restartButton;
    Button nextButton;
    std::vector<LevelButton> levelButtons;

    // Счет и прогресс
    int score;
    int ballsInTarget;
    int currentLevel;
    bool levelCompleted;
    std::vector<bool> levelsUnlocked;

    // Анимации
    float titleAnimation;
    float buttonPulse;

    // Шрифт
    Font gameFont;

public:
    PhysicsGame() :
        // Английский текст для кнопок
        startButton(300, 300, 200, 60, "START GAME"),
        exitButton(300, 380, 200, 60, "EXIT"),
        backButton(50, 500, 150, 40, "BACK"),
        restartButton(500, 500, 150, 40, "RESTART"),
        nextButton(300, 400, 200, 60, "NEXT LEVEL")
    {
        // Инициализация окна с английским названием
        InitWindow(800, 600, "Physics Game");
        SetTargetFPS(60);
        SetExitKey(0); // Отключаем ESC для выхода

        currentState = MAIN_MENU;
        draggedBall = nullptr;
        score = 0;
        ballsInTarget = 0;
        currentLevel = 1;
        levelCompleted = false;
        titleAnimation = 0;
        buttonPulse = 0;

        // Создаем кнопки уровней
        for (int i = 0; i < 3; i++) {
            levelButtons.push_back(LevelButton(100 + i * 220, 200, i + 1));
        }
        levelsUnlocked = { true, false, false };

        // Загружаем уровень 1 по умолчанию
        LoadLevel(1);
    }

    ~PhysicsGame() {
        CloseWindow();
    }

    void LoadLevel(int level) {
        // Очищаем предыдущий уровень
        balls.clear();
        targets.clear();
        draggedBall = nullptr;
        ballsInTarget = 0;
        score = 0;
        levelCompleted = false;
        currentLevel = level;

        switch (level) {
        case 1: // Уровень 1: Гравитация
            balls.push_back(Ball(100, 100, 30, COLOR_BALL_1));
            balls.push_back(Ball(300, 150, 25, COLOR_BALL_2));
            targets.push_back(Target(600, 300, 100, 100));
            break;

        case 2: // Уровень 2: Столкновения
            balls.push_back(Ball(150, 150, 35, COLOR_BALL_1));
            balls.push_back(Ball(400, 200, 30, COLOR_BALL_2));
            balls.push_back(Ball(600, 150, 25, COLOR_BALL_3));
            targets.push_back(Target(200, 400, 120, 120));
            targets.push_back(Target(500, 400, 120, 120));
            break;

        case 3: // Уровень 3: Точность
            balls.push_back(Ball(100, 100, 20, COLOR_BALL_1));
            balls.push_back(Ball(300, 200, 25, COLOR_BALL_2));
            balls.push_back(Ball(500, 150, 20, COLOR_BALL_3));
            targets.push_back(Target(650, 450, 80, 80));
            targets.push_back(Target(350, 450, 80, 80));
            targets.push_back(Target(100, 450, 80, 80));
            break;
        }
    }

    void Update() {
        float deltaTime = GetFrameTime();
        Vector2 mousePos = GetMousePosition();

        // Обновление анимаций
        titleAnimation += deltaTime * 2.0f;
        buttonPulse += deltaTime * 3.0f;

        switch (currentState) {
        case MAIN_MENU:
            UpdateMainMenu(mousePos);
            break;

        case LEVEL_SELECT:
            UpdateLevelSelect(mousePos);
            break;

        case GAME:
            UpdateGame(deltaTime, mousePos);
            break;

        case LEVEL_COMPLETE:
            UpdateLevelComplete(mousePos);
            break;
        }
    }

    void UpdateMainMenu(Vector2 mousePos) {
        startButton.Update(mousePos);
        exitButton.Update(mousePos);

        if (startButton.IsPressed()) {
            currentState = LEVEL_SELECT;
        }

        if (exitButton.IsPressed()) {
            CloseWindow();
        }
    }

    void UpdateLevelSelect(Vector2 mousePos) {
        backButton.Update(mousePos);

        for (auto& btn : levelButtons) {
            btn.Update(mousePos);

            if (btn.IsPressed() && !btn.isLocked) {
                LoadLevel(btn.level);
                currentState = GAME;
            }
        }

        if (backButton.IsPressed()) {
            currentState = MAIN_MENU;
        }
    }

    void UpdateGame(float deltaTime, Vector2 mousePos) {
        backButton.Update(mousePos);
        restartButton.Update(mousePos);

        // Перетаскивание шариков
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (auto& ball : balls) {
                if (ball.Contains(mousePos)) {
                    ball.isDragging = true;
                    draggedBall = &ball;
                    dragOffset = { mousePos.x - ball.position.x, mousePos.y - ball.position.y };
                    break;
                }
            }
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            if (draggedBall) {
                draggedBall->isDragging = false;
                draggedBall = nullptr;
            }
        }

        // Обновление шариков
        ballsInTarget = 0;
        for (auto& ball : balls) {
            ball.Update(deltaTime);

            // Проверка целей
            for (auto& target : targets) {
                target.Update(ball);
                if (target.isActive) {
                    ballsInTarget++;
                }
            }
        }

        // Перетаскивание
        if (draggedBall && draggedBall->isDragging) {
            draggedBall->position.x = mousePos.x - dragOffset.x;
            draggedBall->position.y = mousePos.y - dragOffset.y;
        }

        // Обновление счета
        score = ballsInTarget * 100;

        // Проверка завершения уровня
        if (ballsInTarget == balls.size() && !levelCompleted) {
            levelCompleted = true;

            // Разблокируем следующий уровень
            if (currentLevel < 3 && !levelsUnlocked[currentLevel]) {
                levelsUnlocked[currentLevel] = true;
                levelButtons[currentLevel].isLocked = false;
            }

            // Переходим к экрану завершения
            currentState = LEVEL_COMPLETE;
        }

        // Кнопки управления
        if (backButton.IsPressed()) {
            currentState = LEVEL_SELECT;
        }

        if (restartButton.IsPressed()) {
            LoadLevel(currentLevel);
        }

        // Горячие клавиши
        if (IsKeyPressed(KEY_R)) {
            LoadLevel(currentLevel);
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            currentState = LEVEL_SELECT;
        }
    }

    void UpdateLevelComplete(Vector2 mousePos) {
        nextButton.Update(mousePos);
        backButton.Update(mousePos);

        if (nextButton.IsPressed()) {
            if (currentLevel < 3) {
                LoadLevel(currentLevel + 1);
                currentState = GAME;
            }
            else {
                currentState = LEVEL_SELECT;
            }
        }

        if (backButton.IsPressed()) {
            currentState = LEVEL_SELECT;
        }
    }

    void Draw() {
        BeginDrawing();

        // Фон
        ClearBackground(COLOR_BACKGROUND);

        // Декоративные элементы
        DrawDecoration();

        switch (currentState) {
        case MAIN_MENU:
            DrawMainMenu();
            break;

        case LEVEL_SELECT:
            DrawLevelSelect();
            break;

        case GAME:
            DrawGame();
            break;

        case LEVEL_COMPLETE:
            DrawLevelComplete();
            break;
        }

        EndDrawing();
    }

    void DrawDecoration() {
        // Пузырьки на заднем плане
        for (int i = 0; i < 20; i++) {
            float x = sin(titleAnimation + i * 0.5f) * 50 + i * 40;
            float y = cos(titleAnimation + i * 0.3f) * 30 + i * 30;
            float radius = 10 + sin(titleAnimation + i) * 5;
            DrawCircle(x, y, radius, Fade(COLOR_BUTTON_NORMAL, 0.1f));
        }

        // Волны внизу
        for (int i = 0; i < 800; i += 50) {
            float waveHeight = sin(titleAnimation + i * 0.01f) * 10;
            DrawRectangle(i, 580, 50, 20 + waveHeight, Fade(COLOR_BUTTON_NORMAL, 0.2f));
        }
    }

    void DrawMainMenu() {
        // Анимированный заголовок
        float titleY = 100 + sin(titleAnimation) * 5;

        // Основной заголовок (английский)
        DrawText("PHYSICS GAME", 250, titleY, 48, COLOR_TEXT);
        DrawText("Educational Physics Simulator", 200, titleY + 60, 24, COLOR_ACCENT);

        // Подзаголовок с анимацией
        float pulse = 0.8f + sin(buttonPulse) * 0.2f;
        Color subtitleColor = Fade(COLOR_TEXT, 0.7f + sin(buttonPulse) * 0.3f);
        DrawText("Drag and drop balls to learn physics!",
            400 - MeasureText("Drag and drop balls to learn physics!", 24) / 2,
            200, 24, subtitleColor);

        // Кнопки
        startButton.Draw();
        exitButton.Draw();

        // Инструкция
        DrawText("Click START to begin playing",
            400 - MeasureText("Click START to begin playing", 20) / 2,
            500, 20, Fade(COLOR_TEXT, 0.6f));
    }

    void DrawLevelSelect() {
        DrawText("SELECT LEVEL", 250, 80, 36, COLOR_TEXT);

        // Рисуем кнопки уровней
        for (auto& btn : levelButtons) {
            btn.Draw();
        }

        backButton.Draw();

        // Описание уровней (английский)
        DrawText("Level 1: Learn Gravity", 100, 320, 20, COLOR_TEXT);
        DrawText("Level 2: Ball Collisions", 100, 350, 20, COLOR_TEXT);
        DrawText("Level 3: Precision Target", 100, 380, 20, COLOR_TEXT);

        // Прогресс
        DrawText("Progress:", 550, 320, 24, COLOR_TEXT);
        for (int i = 0; i < 3; i++) {
            if (levelsUnlocked[i]) {
                DrawText("V", 550 + i * 30, 360, 30, COLOR_SUCCESS);
            }
            else {
                DrawText("O", 550 + i * 30, 360, 30, Fade(COLOR_TEXT, 0.3f));
            }
        }
    }

    void DrawGame() {
        // Фон уровня
        DrawRectangleGradientV(0, 0, 800, 600,
            COLOR_BACKGROUND,
            Fade(COLOR_BUTTON_NORMAL, 0.1f));

        // Цели
        for (auto& target : targets) {
            target.Draw();
        }

        // Шарики
        for (auto& ball : balls) {
            ball.Draw();
        }

        // UI панель
        DrawRectangle(0, 0, 800, 70, Fade(COLOR_BACKGROUND, 0.9f));
        DrawLine(0, 70, 800, 70, Fade(COLOR_BUTTON_NORMAL, 0.3f));

        // Информация об уровне
        DrawText(TextFormat("Level: %d", currentLevel), 20, 20, 24, COLOR_TEXT);
        DrawText(TextFormat("Balls: %d/%d", ballsInTarget, (int)balls.size()), 200, 20, 24,
            ballsInTarget == balls.size() ? COLOR_SUCCESS : COLOR_TEXT);
        DrawText(TextFormat("Score: %d", score), 400, 20, 24, COLOR_TEXT);

        // Инструкция
        DrawText("Controls: LMB - drag, R - restart, ESC - menu",
            20, 50, 18, Fade(COLOR_TEXT, 0.6f));

        // Кнопки
        backButton.Draw();
        restartButton.Draw();

        // Физическая информация
        if (!balls.empty() && draggedBall) {
            DrawText(TextFormat("Speed: %.1f px/s",
                sqrt(draggedBall->velocity.x * draggedBall->velocity.x +
                    draggedBall->velocity.y * draggedBall->velocity.y)),
                500, 500, 18, COLOR_TEXT);
        }
    }

    void DrawLevelComplete() {
        // Полупрозрачный фон
        DrawRectangle(0, 0, 800, 600, Fade(BLACK, 0.3f));

        // Карточка победы
        Rectangle card = { 150, 150, 500, 300 };
        DrawRectangleRounded(card, 0.1f, 10, COLOR_BACKGROUND);
        Color borderColor = Fade(COLOR_ACCENT, 1.0f);
        DrawRectangleRoundedLines(card, 0.1f, 10, borderColor);

        // Заголовок
        DrawText("LEVEL COMPLETED!", 400 - MeasureText("LEVEL COMPLETED!", 36) / 2, 180, 36, COLOR_ACCENT);

        // Статистика
        DrawText(TextFormat("Level: %d", currentLevel), 300, 240, 24, COLOR_TEXT);
        DrawText(TextFormat("Score: %d", score), 300, 270, 24, COLOR_TEXT);
        DrawText(TextFormat("Accuracy: %.0f%%", (balls.size() > 0 ? (float)ballsInTarget / balls.size() * 100 : 0)),
            300, 300, 24, COLOR_TEXT);

        // Поздравление
        const char* congrats[] = {
            "Great job!",
            "Excellent work!",
            "Perfect score!",
            "Physics mastered!"
        };
        int congratsIndex = currentLevel - 1;
        if (congratsIndex < 0) congratsIndex = 0;
        if (congratsIndex > 3) congratsIndex = 3;
        DrawText(congrats[congratsIndex], 400 - MeasureText(congrats[congratsIndex], 28) / 2, 340, 28, COLOR_TEXT);

        // Кнопки
        nextButton.Draw();
        backButton.Draw();
    }

    void GameLoop() {
        while (!WindowShouldClose()) {
            Update();
            Draw();
        }
    }
};

// ==================== ТОЧКА ВХОДА ====================
int main() {
    // Настройка сглаживания
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    PhysicsGame game;
    game.GameLoop();

    return 0;
}