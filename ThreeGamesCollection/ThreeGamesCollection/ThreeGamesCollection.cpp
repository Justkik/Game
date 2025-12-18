#define _CRT_SECURE_NO_WARNINGS
#include "raylib.h"
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <cstring>

// ==================== ОБЩИЕ НАСТРОЙКИ ====================
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

// ==================== ГЛАВНОЕ МЕНЮ ====================
class MainMenu {
private:
    struct GameOption {
        const char* name;
        const char* author;
        Color color;
        int id;
        Rectangle rect;
    };

    std::vector<GameOption> games;
    int selectedGame = 0;
    float hoverAnimation = 0.0f;

public:
    MainMenu() {
        // Настройка кнопок для игр
        games = {
            {"Physics Game", "Nikita", Color{180, 210, 225, 255}, 1, Rectangle{262, 200, 500, 100}},
            {"Magnetic Labyrinth", "Sergey", Color{200, 180, 220, 255}, 2, Rectangle{262, 320, 500, 100}},
            {"Cosmic Dinosaur", "Alexander", Color{180, 230, 180, 255}, 3, Rectangle{262, 440, 500, 100}},
            {"Exit", "Close Game", GRAY, 0, Rectangle{262, 560, 500, 60}}
        };
    }

    int Update() {
        Vector2 mousePos = GetMousePosition();
        hoverAnimation += GetFrameTime() * 3.0f;

        // Проверка кликов мыши
        for (size_t i = 0; i < games.size(); i++) {
            if (CheckCollisionPointRec(mousePos, games[i].rect)) {
                selectedGame = i;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    return games[i].id;
                }
            }
        }

        // Управление клавиатурой
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
            selectedGame = (selectedGame + 1) % games.size();
        }
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            selectedGame = (selectedGame - 1 + games.size()) % games.size();
        }
        if (IsKeyPressed(KEY_ENTER)) {
            return games[selectedGame].id;
        }

        return -1; // Ничего не выбрано
    }

    void Draw() {
        // Градиентный фон
        for (int i = 0; i < SCREEN_HEIGHT; i++) {
            float ratio = (float)i / SCREEN_HEIGHT;
            Color gradColor = Color{
                (unsigned char)(10 + ratio * 30),
                (unsigned char)(20 + ratio * 40),
                (unsigned char)(40 + ratio * 50),
                255
            };
            DrawRectangle(0, i, SCREEN_WIDTH, 1, gradColor);
        }

        // Звезды на фоне
        float time = (float)GetTime();
        for (int i = 0; i < 100; i++) {
            float x = fmod(i * 37.0f + time * 50.0f, SCREEN_WIDTH);
            float y = fmod(i * 73.0f, SCREEN_HEIGHT);
            float alpha = 0.5f + 0.5f * sin(time + i);
            DrawPixel(x, y, Color{ 255, 255, 255, (unsigned char)(alpha * 255) });
        }

        // Заголовок
        const char* title = "THREE GAMES COLLECTION";
        int titleWidth = MeasureText(title, 50);
        DrawText(title, SCREEN_WIDTH / 2 - titleWidth / 2, 80, 50, Color{ 255, 255, 200, 255 });

        const char* subtitle = "Select a game to play";
        int subWidth = MeasureText(subtitle, 24);
        DrawText(subtitle, SCREEN_WIDTH / 2 - subWidth / 2, 140, 24, Color{ 200, 220, 255, 255 });

        // Кнопки игр
        for (size_t i = 0; i < games.size(); i++) {
            GameOption& game = games[i];
            bool isSelected = (i == selectedGame);

            // Анимация при наведении
            float pulse = isSelected ? (0.95f + 0.05f * sin(hoverAnimation * 5.0f)) : 1.0f;
            Color btnColor = game.color;

            if (isSelected) {
                btnColor.r = (unsigned char)(btnColor.r * 1.2f);
                btnColor.g = (unsigned char)(btnColor.g * 1.2f);
                btnColor.b = (unsigned char)(btnColor.b * 1.2f);
            }

            // Тень
            DrawRectangleRounded(
                Rectangle{ game.rect.x + 5, game.rect.y + 5, game.rect.width, game.rect.height },
                0.2f, 10, Color{ 0, 0, 0, 100 }
            );

            // Кнопка
            Rectangle scaledRect = {
                game.rect.x - (game.rect.width * (pulse - 1.0f)) / 2,
                game.rect.y - (game.rect.height * (pulse - 1.0f)) / 2,
                game.rect.width * pulse,
                game.rect.height * pulse
            };

            DrawRectangleRounded(scaledRect, 0.2f, 10, btnColor);
            DrawRectangleRoundedLines(scaledRect, 0.2f, 10, Color{ 255, 255, 255, 100 });

            // Текст
            int nameWidth = MeasureText(game.name, 32);
            int authorWidth = MeasureText(game.author, 20);

            DrawText(game.name,
                SCREEN_WIDTH / 2 - nameWidth / 2,
                game.rect.y + 25,
                32,
                isSelected ? WHITE : Color{ 50, 50, 50, 255 });

            DrawText(game.author,
                SCREEN_WIDTH / 2 - authorWidth / 2,
                game.rect.y + 65,
                20,
                isSelected ? Color{ 255, 255, 200, 255 } : Color{ 100, 100, 100, 255 });
        }

        // Инструкция
        DrawText("Use MOUSE, ARROWS or W/S to select, ENTER or CLICK to play",
            SCREEN_WIDTH / 2 - MeasureText("Use MOUSE, ARROWS or W/S to select, ENTER or CLICK to play", 18) / 2,
            SCREEN_HEIGHT - 40, 18, Color{ 200, 200, 200, 200 });
    }
};

// ==================== ИГРА НИКИТЫ (PHYSICS GAME) ====================
class PhysicsGame_Nikita {
private:
    // Константы и цвета
#define COLOR_BG CLITERAL(Color){ 250, 245, 240, 255 }
#define COLOR_BTN_NORMAL CLITERAL(Color){ 180, 210, 225, 255 }
#define COLOR_BTN_HOVER CLITERAL(Color){ 200, 230, 245, 255 }
#define COLOR_BTN_CLICK CLITERAL(Color){ 160, 190, 205, 255 }
#define COLOR_TEXT CLITERAL(Color){ 60, 70, 80, 255 }
#define COLOR_ACCENT CLITERAL(Color){ 255, 180, 180, 255 }
#define COLOR_SUCCESS CLITERAL(Color){ 180, 230, 180, 255 }
#define COLOR_TARGET CLITERAL(Color){ 200, 180, 220, 150 }
#define COLOR_BALL_1 CLITERAL(Color){ 255, 200, 200, 255 }
#define COLOR_BALL_2 CLITERAL(Color){ 200, 220, 255, 255 }
#define COLOR_BALL_3 CLITERAL(Color){ 220, 255, 220, 255 }
#define COLOR_SHADOW CLITERAL(Color){ 0, 0, 0, 50 }

    enum GameState { MAIN_MENU, LEVEL_SELECT, GAME, LEVEL_COMPLETE };

    class Ball {
    public:
        Vector2 position;
        Vector2 velocity;
        float radius;
        Color color;
        bool isDragging;
        Vector2 originalPosition;

        Ball(float x, float y, float r, Color c) :
            position{ x, y }, originalPosition{ x, y }, velocity{ 0, 0 },
            radius(r), color(c), isDragging(false) {
        }

        void Update(float deltaTime) {
            if (!isDragging) {
                velocity.y += 800.0f * deltaTime;
                position.x += velocity.x * deltaTime;
                position.y += velocity.y * deltaTime;
                velocity.x *= 0.99f;
                velocity.y *= 0.99f;

                // Границы
                if (position.x - radius < 0) { position.x = radius; velocity.x = -velocity.x * 0.8f; }
                if (position.x + radius > 800) { position.x = 800 - radius; velocity.x = -velocity.x * 0.8f; }
                if (position.y - radius < 0) { position.y = radius; velocity.y = -velocity.y * 0.8f; }
                if (position.y + radius > 600) { position.y = 600 - radius; velocity.y = -velocity.y * 0.8f; }
            }
        }

        void Draw() {
            DrawCircle(position.x + 3, position.y + 3, radius, COLOR_SHADOW);
            DrawCircleV(position, radius, color);
            DrawCircleLines(position.x, position.y, radius, Fade(WHITE, 0.3f));
            DrawCircle(position.x - radius / 3, position.y - radius / 3, radius / 4, Fade(WHITE, 0.3f));
        }

        void Reset() { position = originalPosition; velocity = { 0,0 }; isDragging = false; }
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

        Target(float x, float y, float w, float h) :
            rect{ x, y, w, h }, color(COLOR_TARGET), isActive(false) {
        }

        void Update(Ball& ball) {
            isActive = CheckCollisionCircleRec(ball.position, ball.radius, rect);
            color = isActive ? COLOR_SUCCESS : COLOR_TARGET;
        }

        void Draw() {
            DrawRectangleRec({ rect.x + 3, rect.y + 3, rect.width, rect.height }, COLOR_SHADOW);
            DrawRectangleRec(rect, color);
            DrawRectangleLinesEx(rect, 3, Fade(WHITE, 0.5f));
            DrawText("o", rect.x + rect.width / 2 - 5, rect.y + rect.height / 2 - 10, 20, WHITE);
        }
    };

    class Button {
    public:
        Rectangle rect;
        const char* text;
        Color color;
        bool isHovered;
        bool isClicked;
        float scale;

        Button(float x, float y, float w, float h, const char* t) :
            rect{ x, y, w, h }, text(t), color(COLOR_BTN_NORMAL),
            isHovered(false), isClicked(false), scale(1.0f) {
        }

        virtual void Update(Vector2 mousePos) {
            isHovered = CheckCollisionPointRec(mousePos, rect);
            isClicked = isHovered && IsMouseButtonDown(MOUSE_LEFT_BUTTON);

            if (isClicked) { color = COLOR_BTN_CLICK; scale = 0.95f; }
            else if (isHovered) { color = COLOR_BTN_HOVER; scale = 1.05f; }
            else { color = COLOR_BTN_NORMAL; scale = 1.0f; }
        }

        bool IsPressed() { return isHovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON); }

        virtual void Draw() {
            DrawRectangleRounded({ rect.x + 5, rect.y + 5, rect.width, rect.height }, 0.3f, 10, COLOR_SHADOW);
            Rectangle scaledRect = {
                rect.x - (rect.width * (scale - 1.0f)) / 2,
                rect.y - (rect.height * (scale - 1.0f)) / 2,
                rect.width * scale,
                rect.height * scale
            };
            DrawRectangleRounded(scaledRect, 0.3f, 10, color);
            DrawRectangleRoundedLines(scaledRect, 0.3f, 10, Fade(WHITE, 0.5f));

            int fontSize = 24;
            int textWidth = MeasureText(text, fontSize);
            DrawText(text, rect.x + rect.width / 2 - textWidth / 2, rect.y + rect.height / 2 - fontSize / 2, fontSize, COLOR_TEXT);
        }
    };

    GameState currentState;
    std::vector<Ball> balls;
    std::vector<Target> targets;
    Ball* draggedBall;
    Vector2 dragOffset;

    Button startButton;
    Button exitButton;
    Button backButton;
    Button restartButton;
    Button nextButton;

    int score;
    int ballsInTarget;
    int currentLevel;
    bool levelCompleted;
    std::vector<bool> levelsUnlocked;
    float titleAnimation;

public:
    PhysicsGame_Nikita() :
        startButton(300, 300, 200, 60, "START GAME"),
        exitButton(300, 380, 200, 60, "EXIT"),
        backButton(50, 500, 150, 40, "BACK"),
        restartButton(500, 500, 150, 40, "RESTART"),
        nextButton(300, 400, 200, 60, "NEXT LEVEL"),
        currentState(MAIN_MENU), draggedBall(nullptr), score(0), ballsInTarget(0),
        currentLevel(1), levelCompleted(false), titleAnimation(0)
    {
        InitWindow(800, 600, "Physics Game by Nikita");
        SetTargetFPS(60);
        SetExitKey(0);
        levelsUnlocked = { true, false, false };
        LoadLevel(1);
    }

    ~PhysicsGame_Nikita() { CloseWindow(); }

    void LoadLevel(int level) {
        balls.clear();
        targets.clear();
        draggedBall = nullptr;
        ballsInTarget = 0;
        score = 0;
        levelCompleted = false;
        currentLevel = level;

        switch (level) {
        case 1:
            balls.push_back(Ball(100, 100, 30, COLOR_BALL_1));
            balls.push_back(Ball(300, 150, 25, COLOR_BALL_2));
            targets.push_back(Target(600, 300, 100, 100));
            break;
        case 2:
            balls.push_back(Ball(150, 150, 35, COLOR_BALL_1));
            balls.push_back(Ball(400, 200, 30, COLOR_BALL_2));
            balls.push_back(Ball(600, 150, 25, COLOR_BALL_3));
            targets.push_back(Target(200, 400, 120, 120));
            targets.push_back(Target(500, 400, 120, 120));
            break;
        case 3:
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
        titleAnimation += deltaTime * 2.0f;

        switch (currentState) {
        case MAIN_MENU:
            startButton.Update(mousePos);
            exitButton.Update(mousePos);
            if (startButton.IsPressed()) currentState = LEVEL_SELECT;
            if (exitButton.IsPressed() || IsKeyPressed(KEY_ESCAPE)) { CloseWindow(); return; }
            break;

        case LEVEL_SELECT:
            backButton.Update(mousePos);
            if (backButton.IsPressed() || IsKeyPressed(KEY_ESCAPE)) currentState = MAIN_MENU;
            break;

        case GAME:
            UpdateGame(deltaTime, mousePos);
            break;

        case LEVEL_COMPLETE:
            nextButton.Update(mousePos);
            backButton.Update(mousePos);
            if (nextButton.IsPressed()) {
                if (currentLevel < 3) { LoadLevel(currentLevel + 1); currentState = GAME; }
                else currentState = LEVEL_SELECT;
            }
            if (backButton.IsPressed() || IsKeyPressed(KEY_ESCAPE)) currentState = LEVEL_SELECT;
            break;
        }
    }

    void UpdateGame(float deltaTime, Vector2 mousePos) {
        backButton.Update(mousePos);
        restartButton.Update(mousePos);

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

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && draggedBall) {
            draggedBall->isDragging = false;
            draggedBall = nullptr;
        }

        ballsInTarget = 0;
        for (auto& ball : balls) {
            ball.Update(deltaTime);
            for (auto& target : targets) {
                target.Update(ball);
                if (target.isActive) ballsInTarget++;
            }
        }

        if (draggedBall && draggedBall->isDragging) {
            draggedBall->position.x = mousePos.x - dragOffset.x;
            draggedBall->position.y = mousePos.y - dragOffset.y;
        }

        score = ballsInTarget * 100;

        if (ballsInTarget == balls.size() && !levelCompleted) {
            levelCompleted = true;
            if (currentLevel < 3 && !levelsUnlocked[currentLevel]) {
                levelsUnlocked[currentLevel] = true;
            }
            currentState = LEVEL_COMPLETE;
        }

        if (backButton.IsPressed() || IsKeyPressed(KEY_ESCAPE)) currentState = LEVEL_SELECT;
        if (restartButton.IsPressed() || IsKeyPressed(KEY_R)) LoadLevel(currentLevel);
    }

    void Draw() {
        BeginDrawing();
        ClearBackground(COLOR_BG);

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

    void DrawMainMenu() {
        float titleY = 100 + sin(titleAnimation) * 5;
        DrawText("PHYSICS GAME", 250, titleY, 48, COLOR_TEXT);
        DrawText("Educational Physics Simulator", 200, titleY + 60, 24, COLOR_ACCENT);

        float pulse = 0.8f + sin(titleAnimation * 3) * 0.2f;
        DrawText("Drag and drop balls to learn physics!",
            400 - MeasureText("Drag and drop balls to learn physics!", 24) / 2,
            200, 24, Fade(COLOR_TEXT, 0.7f + sin(titleAnimation * 3) * 0.3f));

        startButton.Draw();
        exitButton.Draw();

        DrawText("Click START to begin playing",
            400 - MeasureText("Click START to begin playing", 20) / 2,
            500, 20, Fade(COLOR_TEXT, 0.6f));
    }

    void DrawLevelSelect() {
        DrawText("SELECT LEVEL", 250, 80, 36, COLOR_TEXT);
        backButton.Draw();

        // Простые кнопки уровней
        for (int i = 0; i < 3; i++) {
            Rectangle levelRect = { 100 + i * 220, 200, 180, 80 };
            bool isHover = CheckCollisionPointRec(GetMousePosition(), levelRect);
            bool isLocked = (i > 0) && !levelsUnlocked[i];

            Color color = isLocked ? GRAY : (isHover ? COLOR_BTN_HOVER : COLOR_BTN_NORMAL);
            DrawRectangleRounded(levelRect, 0.3f, 10, color);
            DrawRectangleRoundedLines(levelRect, 0.3f, 10, Fade(WHITE, 0.5f));

            char levelText[20];
            sprintf(levelText, "Level %d", i + 1);
            DrawText(levelText, levelRect.x + levelRect.width / 2 - MeasureText(levelText, 24) / 2,
                levelRect.y + 25, 24, COLOR_TEXT);

            if (isLocked) {
                DrawRectangleRec(levelRect, Fade(BLACK, 0.5f));
                DrawText("?", levelRect.x + levelRect.width / 2 - 5, levelRect.y + 25, 30, WHITE);
            }

            if (!isLocked && isHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                LoadLevel(i + 1);
                currentState = GAME;
            }
        }
    }

    void DrawGame() {
        DrawRectangleGradientV(0, 0, 800, 600, COLOR_BG, Fade(COLOR_BTN_NORMAL, 0.1f));

        for (auto& target : targets) target.Draw();
        for (auto& ball : balls) ball.Draw();

        DrawRectangle(0, 0, 800, 70, Fade(COLOR_BG, 0.9f));
        DrawLine(0, 70, 800, 70, Fade(COLOR_BTN_NORMAL, 0.3f));

        DrawText(TextFormat("Level: %d", currentLevel), 20, 20, 24, COLOR_TEXT);
        DrawText(TextFormat("Balls: %d/%d", ballsInTarget, (int)balls.size()), 200, 20, 24,
            ballsInTarget == balls.size() ? COLOR_SUCCESS : COLOR_TEXT);
        DrawText(TextFormat("Score: %d", score), 400, 20, 24, COLOR_TEXT);

        DrawText("Controls: LMB - drag, R - restart, ESC - menu", 20, 50, 18, Fade(COLOR_TEXT, 0.6f));

        backButton.Draw();
        restartButton.Draw();
    }

    void DrawLevelComplete() {
        DrawRectangle(0, 0, 800, 600, Fade(BLACK, 0.3f));

        Rectangle card = { 150, 150, 500, 300 };
        DrawRectangleRounded(card, 0.1f, 10, COLOR_BG);
        DrawRectangleRoundedLines(card, 0.1f, 10, COLOR_ACCENT);

        DrawText("LEVEL COMPLETED!", 400 - MeasureText("LEVEL COMPLETED!", 36) / 2, 180, 36, COLOR_ACCENT);
        DrawText(TextFormat("Level: %d", currentLevel), 300, 240, 24, COLOR_TEXT);
        DrawText(TextFormat("Score: %d", score), 300, 270, 24, COLOR_TEXT);
        DrawText(TextFormat("Accuracy: %.0f%%", (balls.size() > 0 ? (float)ballsInTarget / balls.size() * 100 : 0)),
            300, 300, 24, COLOR_TEXT);

        const char* congrats = "Great job!";
        DrawText(congrats, 400 - MeasureText(congrats, 28) / 2, 340, 28, COLOR_TEXT);

        nextButton.Draw();
        backButton.Draw();
    }

    void Run() {
        while (!WindowShouldClose()) {
            Update();
            Draw();
        }
    }
};

// ==================== ИГРА СЕРГЕЯ (MAGNETIC GAME) ====================
class MagneticGame_Sergey {
private:
    const int screenWidth = 1500;
    const int screenHeight = 1200;

    struct MetalObject {
        Vector2 position;
        float radius;
        Color color;
        Vector2 velocity;
    };

    struct MagneticField {
        Vector2 position;
        float radius;
        float strength;
        Color color;
        bool isRepelling;
        float waveStrength;
    };

    struct MovingObstacle {
        Vector2 position;
        float radius;
        Color color;
        Vector2 velocity;
    };

    MetalObject metalObject;
    std::vector<MagneticField> magneticFields;
    std::vector<MovingObstacle> obstacles;

    int currentLevel = 0;
    bool levelCompleted = false;
    bool inMainMenu = true;
    bool inLevelSelection = false;
    float waveTime = 0.0f;

    Vector2 NormalizeVector(Vector2 v) {
        float len = sqrtf(v.x * v.x + v.y * v.y);
        if (len > 0) { v.x /= len; v.y /= len; }
        return v;
    }

    bool CheckCollision(MetalObject& obj, MovingObstacle& obs) {
        float dx = obj.position.x - obs.position.x;
        float dy = obj.position.y - obs.position.y;
        float distance = sqrtf(dx * dx + dy * dy);
        return distance < (obj.radius + obs.radius);
    }

    void InitLevel() {
        magneticFields.clear();
        obstacles.clear();

        switch (currentLevel) {
        case 0: // Level 1
            metalObject = { {screenWidth / 2.0f, screenHeight / 2.0f}, 50.0f, RED, {0.0f, 0.0f} };
            magneticFields.push_back({ {screenWidth / 4.0f, 600.0f}, 200.0f, 500.0f, BLUE, false, 0.0f });
            magneticFields.push_back({ {screenWidth * 3.0f / 4.0f, 600.0f}, 200.0f, 500.0f, BLUE, false, 0.0f });
            obstacles.push_back({ {screenWidth / 2.0f, screenHeight / 3.0f}, 50.0f, RED, {2.0f, 2.0f} });
            break;

        case 1: // Level 2
            metalObject = { {screenWidth / 2.0f, screenHeight / 2.0f}, 50.0f, RED, {0.0f, 0.0f} };
            magneticFields.push_back({ {screenWidth / 2.0f, 600.0f}, 200.0f, 500.0f, BLUE, false, 0.0f });
            magneticFields.push_back({ {screenWidth / 4.0f, 600.0f}, 200.0f, 500.0f, BLUE, false, 0.0f });
            magneticFields.push_back({ {screenWidth * 3.0f / 4.0f, 600.0f}, 200.0f, 500.0f, BLUE, false, 0.0f });
            obstacles.push_back({ {screenWidth / 4.0f, screenHeight / 3.0f}, 50.0f, GREEN, {-2.0f, -2.0f} });
            obstacles.push_back({ {screenWidth / 2.0f, screenHeight / 2.0f}, 50.0f, PURPLE, {2.0f, -2.0f} });
            break;

        case 2: // Level 3
            metalObject = { {screenWidth / 2.0f, screenHeight / 2.0f}, 50.0f, RED, {0.0f, 0.0f} };
            magneticFields.push_back({ {screenWidth / 4.0f, 600.0f}, 200.0f, 500.0f, BLUE, false, 0.2f });
            magneticFields.push_back({ {screenWidth * 3.0f / 4.0f, 600.0f}, 200.0f, 500.0f, BLUE, false, 0.2f });
            magneticFields.push_back({ {screenWidth / 2.0f, 800.0f}, 200.0f, 500.0f, RED, true, 0.3f });
            obstacles.push_back({ {screenWidth / 4.0f, screenHeight / 3.0f}, 60.0f, BLUE, {3.0f, 0.0f} });
            obstacles.push_back({ {screenWidth / 2.0f, screenHeight / 2.0f}, 60.0f, YELLOW, {0.0f, 3.0f} });
            break;
        }
    }

public:
    MagneticGame_Sergey() {
        InitWindow(screenWidth, screenHeight, "Magnetic Labyrinth by Sergey");
        SetTargetFPS(60);
        InitLevel();
    }

    ~MagneticGame_Sergey() { CloseWindow(); }

    void Update() {
        if (inMainMenu) {
            if (IsKeyPressed(KEY_ENTER)) { inMainMenu = false; }
            if (IsKeyPressed(KEY_ONE)) { inMainMenu = false; inLevelSelection = true; }
            if (IsKeyPressed(KEY_ESCAPE)) { CloseWindow(); return; }
            return;
        }

        if (inLevelSelection) {
            if (IsKeyPressed(KEY_ONE)) { currentLevel = 0; InitLevel(); inLevelSelection = false; }
            if (IsKeyPressed(KEY_TWO)) { currentLevel = 1; InitLevel(); inLevelSelection = false; }
            if (IsKeyPressed(KEY_THREE)) { currentLevel = 2; InitLevel(); inLevelSelection = false; }
            if (IsKeyPressed(KEY_ESCAPE)) { inLevelSelection = false; inMainMenu = true; }
            return;
        }

        // Gameplay
        float speed = 5.0f;
        if (IsKeyDown(KEY_RIGHT)) metalObject.position.x += speed;
        if (IsKeyDown(KEY_LEFT)) metalObject.position.x -= speed;
        if (IsKeyDown(KEY_DOWN)) metalObject.position.y += speed;
        if (IsKeyDown(KEY_UP)) metalObject.position.y -= speed;

        for (auto& field : magneticFields) {
            Vector2 dir = { field.position.x - metalObject.position.x, field.position.y - metalObject.position.y };
            float dist = sqrtf(dir.x * dir.x + dir.y * dir.y);

            if (dist < field.radius) {
                float force = field.strength / (dist + 1);
                dir = NormalizeVector(dir);

                if (field.isRepelling) { dir.x = -dir.x; dir.y = -dir.y; }

                metalObject.velocity.x += dir.x * force * 0.1f;
                metalObject.velocity.y += dir.y * force * 0.1f;

                if (dist < field.radius / 2) {
                    waveTime = fmodf(waveTime + 0.05f, 1.0f);
                    float waveForce = field.waveStrength * sinf(waveTime * 3.14f);
                    metalObject.velocity.x += dir.x * waveForce * 0.1f;
                    metalObject.velocity.y += dir.y * waveForce * 0.1f;
                }
            }
        }

        metalObject.position.x += metalObject.velocity.x;
        metalObject.position.y += metalObject.velocity.y;
        metalObject.velocity.x *= 0.98f;
        metalObject.velocity.y *= 0.98f;

        // Boundaries
        if (metalObject.position.x - metalObject.radius < 0) metalObject.position.x = metalObject.radius;
        if (metalObject.position.x + metalObject.radius > screenWidth) metalObject.position.x = screenWidth - metalObject.radius;
        if (metalObject.position.y - metalObject.radius < 0) metalObject.position.y = metalObject.radius;
        if (metalObject.position.y + metalObject.radius > screenHeight) metalObject.position.y = screenHeight - metalObject.radius;

        for (auto& obs : obstacles) {
            obs.position.x += obs.velocity.x;
            obs.position.y += obs.velocity.y;

            if (CheckCollision(metalObject, obs)) {
                metalObject.velocity.x *= -1;
                metalObject.velocity.y *= -1;
            }

            if (obs.position.x - obs.radius < 0 || obs.position.x + obs.radius > screenWidth) obs.velocity.x *= -1;
            if (obs.position.y - obs.radius < 0 || obs.position.y + obs.radius > screenHeight) obs.velocity.y *= -1;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            currentLevel = (currentLevel + 1) % 3;
            InitLevel();
        }
        if (IsKeyPressed(KEY_ESCAPE)) { inMainMenu = true; }
    }

    void Draw() {
        BeginDrawing();

        if (inMainMenu) {
            ClearBackground(RAYWHITE);
            Color start = Color{ 135, 206, 235, 255 };
            Color end = Color{ 70, 130, 180, 255 };
            DrawRectangleGradientV(0, 0, screenWidth, screenHeight, start, end);

            DrawText("MAGNETIC LABYRINTH", screenWidth / 2 - MeasureText("MAGNETIC LABYRINTH", 60) / 2, screenHeight / 3, 60, DARKBLUE);
            DrawText("Press ENTER to Start", screenWidth / 2 - MeasureText("Press ENTER to Start", 40) / 2, screenHeight / 2, 40, DARKGREEN);
            DrawText("Press 1 to Select Level", screenWidth / 2 - MeasureText("Press 1 to Select Level", 40) / 2, screenHeight / 2 + 50, 40, DARKGREEN);
            DrawText("Press ESC to Exit", screenWidth / 2 - MeasureText("Press ESC to Exit", 40) / 2, screenHeight / 2 + 100, 40, DARKGREEN);
        }
        else if (inLevelSelection) {
            ClearBackground(RAYWHITE);
            Color start = Color{ 135, 206, 235, 255 };
            Color end = Color{ 70, 130, 180, 255 };
            DrawRectangleGradientV(0, 0, screenWidth, screenHeight, start, end);

            DrawText("SELECT LEVEL", screenWidth / 2 - MeasureText("SELECT LEVEL", 60) / 2, screenHeight / 3, 60, DARKBLUE);
            DrawText("Press 1 for Level 1", screenWidth / 2 - MeasureText("Press 1 for Level 1", 40) / 2, screenHeight / 2, 40, DARKGREEN);
            DrawText("Press 2 for Level 2", screenWidth / 2 - MeasureText("Press 2 for Level 2", 40) / 2, screenHeight / 2 + 50, 40, DARKGREEN);
            DrawText("Press 3 for Level 3", screenWidth / 2 - MeasureText("Press 3 for Level 3", 40) / 2, screenHeight / 2 + 100, 40, DARKGREEN);
            DrawText("Press ESC to return", screenWidth / 2 - MeasureText("Press ESC to return", 40) / 2, screenHeight / 2 + 150, 40, DARKGREEN);
        }
        else {
            // Gameplay
            ClearBackground(RAYWHITE);
            Color gradStart = Color{ 30, 30, 30, 255 };
            Color gradEnd = Color{ 10, 10, 10, 255 };
            DrawRectangleGradientV(0, 0, screenWidth, screenHeight, gradStart, gradEnd);

            for (auto& field : magneticFields) {
                int numRings = 5;
                for (int i = 0; i < numRings; i++) {
                    float alpha = (1.0f - (float)i / numRings) * 0.5f;
                    Color ringColor = Color{ field.color.r, field.color.g, field.color.b, (unsigned char)(alpha * 255) };
                    DrawCircle(field.position.x, field.position.y, field.radius + i * 10, ringColor);
                }
            }

            DrawCircle(metalObject.position.x, metalObject.position.y, metalObject.radius, metalObject.color);
            DrawCircleLines(metalObject.position.x, metalObject.position.y, metalObject.radius + 5, GOLD);

            for (auto& obs : obstacles) {
                DrawCircle(obs.position.x, obs.position.y, obs.radius, obs.color);
            }

            DrawText(TextFormat("Level: %d", currentLevel + 1), 20, 20, 30, DARKBLUE);
            DrawText("Goal: Reach the top right corner!", 20, 60, 30, DARKGREEN);
            DrawText("Controls: ARROWS to move, ENTER next level, ESC menu",
                screenWidth / 2 - MeasureText("Controls: ARROWS to move, ENTER next level, ESC menu", 30) / 2,
                screenHeight - 50, 30, GRAY);
        }

        EndDrawing();
    }

    void Run() {
        while (!WindowShouldClose()) {
            Update();
            Draw();
        }
    }
};

// ==================== ИГРА АЛЕКСАНДРА (COSMIC DINO) ====================
class CosmicDino_Alexander {
private:
    enum Planet { EARTH, MARS, JUPITER };
    enum Difficulty { EASY, MEDIUM, HARD };
    enum GameState {
        MENU_MAIN, MENU_PLANET, MENU_DIFFICULTY,
        GAME_COUNTDOWN, GAME_PLAYING, GAME_PAUSED,
        MENU_INVENTORY, GAME_OVER
    };

    struct Dinosaur {
        Vector2 position = { 100, 500 };
        Vector2 velocity = { 0, 0 };
        Rectangle collider = { 100, 500, 60, 100 };
        bool isJumping = false;
        bool canDoubleJump = true;
        int jumpCount = 0;
        float jumpForce = 18.0f;

        void Update(float gravity) {
            velocity.y += gravity;
            position.y += velocity.y;
            collider.x = position.x + 10;
            collider.y = position.y + 10;

            if (position.y >= 500) {
                position.y = 500;
                velocity.y = 0;
                isJumping = false;
                jumpCount = 0;
                canDoubleJump = true;
            }
        }

        void Jump(float force) {
            if (!isJumping) {
                velocity.y = -force;
                isJumping = true;
                jumpCount = 1;
            }
        }

        void DoubleJump(float force) {
            if (isJumping && canDoubleJump && jumpCount < 2) {
                velocity.y = -force * 0.9f;
                jumpCount = 2;
                canDoubleJump = false;
            }
        }

        void Reset() {
            position = { 100, 500 };
            velocity = { 0, 0 };
            collider = { 100, 500, 60, 100 };
            isJumping = false;
            canDoubleJump = true;
            jumpCount = 0;
        }
    };

    struct Obstacle {
        Vector2 position = { 0, 0 };
        Rectangle collider = { 0, 0, 50, 60 };
        float speed = 0;
        bool active = false;
        Color color = RED;

        void Update() {
            if (active) {
                position.x -= speed;
                collider.x = position.x;
                if (position.x < -100) active = false;
            }
        }

        void Spawn(float x, float y, float spd) {
            position = { x, y };
            collider = { x, y, 50, 60 };
            speed = spd;
            active = true;
            color = Color{
                (unsigned char)GetRandomValue(100, 200),
                (unsigned char)GetRandomValue(100, 200),
                (unsigned char)GetRandomValue(100, 200),
                255
            };
        }

        void Reset() { active = false; position = { 0,0 }; }
    };

    struct Cloud {
        Vector2 position;
        float speed;
    };

    struct Fact {
        int id;
        bool unlocked;
    };

    const char* englishStrings[46] = {
        "COSMIC DINOSAUR", "Physics Adventure", "PLAY", "INVENTORY", "EXIT",
        "High Score: ", "CHOOSE A PLANET", "EARTH (g=9.8)", "MARS (g=3.7)",
        "JUPITER (g=24.8)", "BACK", "Planet: ", "CHOOSE DIFFICULTY", "EASY",
        "MEDIUM", "HARD", "START GAME", "Score: ", "Planet: ", "Difficulty: ",
        "SPACE - jump", "Double SPACE - double jump", "ESC - pause",
        "NEW PHYSICS FACT DISCOVERED!", "Check inventory in pause menu",
        "START!", "PAUSE", "RESUME", "INVENTORY", "TO MENU",
        "INVENTORY - PHYSICS FACTS", "Fact ", "No facts discovered yet!",
        "Earn 500 points in game to unlock a fact", "BACK", "GAME OVER!",
        "Your score: ", "NEW RECORD!", "Press ENTER to play again",
        "Press ESC to return to menu", "Earth", "Mars", "Jupiter",
        "Easy", "Medium", "Hard"
    };

    const char* factTexts[10] = {
        "1. On Jupiter you would weigh 2.5 times more!",
        "2. Gravity on Mars is only 38% of Earth's!",
        "3. Black holes have the strongest gravity!",
        "4. In weightlessness, flames become spherical!",
        "5. Time slows down in strong gravitational fields!",
        "6. On neutron stars gravity is billions of times stronger!",
        "7. The Moon moves away from Earth by 3.8 cm per year!",
        "8. There is no Archimedes' force in space!",
        "9. On Venus pressure is 92 times greater than on Earth!",
        "10. The Sun makes up 99.86% of Solar System mass!"
    };

    GameState currentState;
    Planet currentPlanet;
    Difficulty currentDifficulty;

    Dinosaur dino;
    std::vector<Obstacle> obstacles;
    std::vector<Cloud> clouds;
    std::vector<Fact> facts;

    int score;
    int highScore;
    float countdownTimer;
    float gameTime;
    float spawnTimer;
    float notificationTimer;
    bool showNotification;

    Texture2D dinoTexture;
    Texture2D obstacleTexture;
    Texture2D backgroundTexture;
    Texture2D menuBackground;
    Texture2D cloudTexture;
    Font gameFont;

public:
    CosmicDino_Alexander() :
        currentState(MENU_MAIN), currentPlanet(EARTH), currentDifficulty(MEDIUM),
        score(0), highScore(0), countdownTimer(3.0f), gameTime(0.0f),
        spawnTimer(0.0f), notificationTimer(0.0f), showNotification(false)
    {
        InitWindow(1024, 768, "Cosmic Dinosaur by Alexander");
        SetTargetFPS(60);
        InitAudioDevice();

        // Create textures
        CreateTextures();

        obstacles.resize(6);
        clouds.resize(3);
        for (int i = 0; i < 3; i++) {
            clouds[i] = { {(float)GetRandomValue(0, 800), (float)GetRandomValue(50, 200)}, (float)GetRandomValue(1, 3) };
        }

        facts.resize(10);
        for (int i = 0; i < 10; i++) {
            facts[i] = { i, false };
        }
    }

    ~CosmicDino_Alexander() {
        UnloadTexture(dinoTexture);
        UnloadTexture(obstacleTexture);
        UnloadTexture(backgroundTexture);
        UnloadTexture(menuBackground);
        UnloadTexture(cloudTexture);
        if (gameFont.texture.id != 0 && gameFont.texture.id != GetFontDefault().texture.id) {
            UnloadFont(gameFont);
        }
        CloseAudioDevice();
        CloseWindow();
    }

    void CreateTextures() {
        // Dino texture
        Image dinoImg = GenImageColor(80, 120, BLANK);
        for (int y = 30; y < 70; y++) {
            for (int x = 25; x < 55; x++) ImageDrawPixel(&dinoImg, x, y, GREEN);
        }
        for (int y = 10; y < 30; y++) {
            for (int x = 30; x < 50; x++) ImageDrawPixel(&dinoImg, x, y, GREEN);
        }
        ImageDrawPixel(&dinoImg, 35, 15, WHITE);
        ImageDrawPixel(&dinoImg, 45, 15, WHITE);
        for (int x = 35; x < 45; x++) ImageDrawPixel(&dinoImg, x, 25, RED);
        dinoTexture = LoadTextureFromImage(dinoImg);
        UnloadImage(dinoImg);

        // Obstacle texture
        Image obsImg = GenImageColor(50, 60, BLANK);
        for (int y = 0; y < 60; y++) {
            for (int x = 0; x < 50; x++) {
                if (x > 5 && x < 45 && y > 5 && y < 55) {
                    Color rockColor = {
                        (unsigned char)GetRandomValue(120, 180),
                        (unsigned char)GetRandomValue(80, 140),
                        (unsigned char)GetRandomValue(60, 120),
                        255
                    };
                    ImageDrawPixel(&obsImg, x, y, rockColor);
                }
            }
        }
        obstacleTexture = LoadTextureFromImage(obsImg);
        UnloadImage(obsImg);

        // Backgrounds
        Image bgImg = GenImageColor(1024, 768, BLACK);
        for (int i = 0; i < 200; i++) {
            ImageDrawPixel(&bgImg, GetRandomValue(0, 1023), GetRandomValue(0, 767), WHITE);
        }
        backgroundTexture = LoadTextureFromImage(bgImg);
        UnloadImage(bgImg);

        menuBackground = LoadTextureFromImage(GenImageColor(1024, 768, DARKBLUE));
        cloudTexture = LoadTextureFromImage(GenImageColor(200, 100, Color{ 255, 255, 255, 180 }));

        gameFont = GetFontDefault();
    }

    float GetPlanetGravity() {
        switch (currentPlanet) {
        case EARTH: return 0.5f;
        case MARS: return 0.3f;
        case JUPITER: return 0.8f;
        default: return 0.5f;
        }
    }

    float GetObstacleSpeed() {
        switch (currentDifficulty) {
        case EASY: return 5.0f;
        case MEDIUM: return 7.0f;
        case HARD: return 10.0f;
        default: return 7.0f;
        }
    }

    float GetSpawnRate() {
        switch (currentDifficulty) {
        case EASY: return 2.5f;
        case MEDIUM: return 2.0f;
        case HARD: return 1.5f;
        default: return 2.0f;
        }
    }

    void Run() {
        while (!WindowShouldClose()) {
            float deltaTime = GetFrameTime();
            Vector2 mousePos = GetMousePosition();

            switch (currentState) {
            case MENU_MAIN: UpdateMainMenu(mousePos); break;
            case MENU_PLANET: UpdatePlanetMenu(mousePos); break;
            case MENU_DIFFICULTY: UpdateDifficultyMenu(mousePos); break;
            case GAME_COUNTDOWN: UpdateCountdown(deltaTime); break;
            case GAME_PLAYING: UpdatePlaying(deltaTime); break;
            case GAME_PAUSED: UpdatePaused(mousePos); break;
            case MENU_INVENTORY: UpdateInventory(mousePos); break;
            case GAME_OVER: UpdateGameOver(); break;
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);

            switch (currentState) {
            case MENU_MAIN: DrawMainMenu(); break;
            case MENU_PLANET: DrawPlanetMenu(); break;
            case MENU_DIFFICULTY: DrawDifficultyMenu(); break;
            case GAME_COUNTDOWN: DrawCountdown(); break;
            case GAME_PLAYING: DrawPlaying(); break;
            case GAME_PAUSED: DrawPaused(); break;
            case MENU_INVENTORY: DrawInventory(); break;
            case GAME_OVER: DrawGameOver(); break;
            }

            EndDrawing();
        }
    }

    void UpdateMainMenu(Vector2 mousePos) {
        if (CheckCollisionPointRec(mousePos, { 412, 250, 200, 60 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentState = MENU_PLANET;
        }
        if (CheckCollisionPointRec(mousePos, { 412, 320, 200, 60 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentState = MENU_INVENTORY;
        }
        if (CheckCollisionPointRec(mousePos, { 412, 390, 200, 60 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            CloseWindow();
        }
        if (IsKeyPressed(KEY_ESCAPE)) CloseWindow();
    }

    void UpdatePlanetMenu(Vector2 mousePos) {
        if (CheckCollisionPointRec(mousePos, { 362, 200, 300, 80 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentPlanet = EARTH; currentState = MENU_DIFFICULTY;
        }
        if (CheckCollisionPointRec(mousePos, { 362, 300, 300, 80 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentPlanet = MARS; currentState = MENU_DIFFICULTY;
        }
        if (CheckCollisionPointRec(mousePos, { 362, 400, 300, 80 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentPlanet = JUPITER; currentState = MENU_DIFFICULTY;
        }
        if (CheckCollisionPointRec(mousePos, { 362, 500, 300, 60 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentState = MENU_MAIN;
        }
        if (IsKeyPressed(KEY_ESCAPE)) currentState = MENU_MAIN;
    }

    void UpdateDifficultyMenu(Vector2 mousePos) {
        if (CheckCollisionPointRec(mousePos, { 362, 250, 300, 80 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentDifficulty = EASY;
        }
        if (CheckCollisionPointRec(mousePos, { 362, 350, 300, 80 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentDifficulty = MEDIUM;
        }
        if (CheckCollisionPointRec(mousePos, { 362, 450, 300, 80 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentDifficulty = HARD;
        }
        if (CheckCollisionPointRec(mousePos, { 362, 550, 300, 60 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            StartGame();
        }
        if (CheckCollisionPointRec(mousePos, { 362, 620, 300, 50 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentState = MENU_PLANET;
        }
        if (IsKeyPressed(KEY_ESCAPE)) currentState = MENU_PLANET;
    }

    void UpdateCountdown(float deltaTime) {
        countdownTimer -= deltaTime;
        if (countdownTimer <= 0) {
            currentState = GAME_PLAYING;
            countdownTimer = 3.0f;
        }
    }

    void UpdatePlaying(float deltaTime) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            currentState = GAME_PAUSED;
            return;
        }

        float gravity = GetPlanetGravity();
        dino.Update(gravity);

        if (IsKeyPressed(KEY_SPACE)) {
            if (!dino.isJumping) dino.Jump(dino.jumpForce);
            else if (dino.canDoubleJump && dino.jumpCount < 2) dino.DoubleJump(dino.jumpForce);
        }

        float spawnRate = GetSpawnRate();
        spawnTimer += deltaTime;
        if (spawnTimer >= spawnRate) {
            for (auto& obs : obstacles) {
                if (!obs.active) {
                    obs.Spawn(1100, 560, GetObstacleSpeed());
                    break;
                }
            }
            spawnTimer = 0;
        }

        for (auto& obs : obstacles) {
            obs.Update();
            if (!obs.active && obs.position.x < -100) {
                score++;
                if (score > 0 && score % 500 == 0) UnlockRandomFact();
                if (score > highScore) highScore = score;
            }
        }

        for (auto& cloud : clouds) {
            cloud.position.x -= cloud.speed;
            if (cloud.position.x < -200) {
                cloud.position.x = 1100;
                cloud.position.y = (float)GetRandomValue(50, 300);
            }
        }

        // Check collisions
        for (const auto& obs : obstacles) {
            if (obs.active && CheckCollisionRecs(dino.collider, obs.collider)) {
                currentState = GAME_OVER;
                return;
            }
        }

        if (showNotification) {
            notificationTimer -= deltaTime;
            if (notificationTimer <= 0) showNotification = false;
        }
    }

    void UpdatePaused(Vector2 mousePos) {
        if (CheckCollisionPointRec(mousePos, { 412, 300, 200, 60 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentState = GAME_PLAYING;
        }
        if (CheckCollisionPointRec(mousePos, { 412, 380, 200, 60 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentState = MENU_INVENTORY;
        }
        if (CheckCollisionPointRec(mousePos, { 412, 460, 200, 60 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            ResetGame(); currentState = MENU_MAIN;
        }
        if (IsKeyPressed(KEY_ESCAPE)) currentState = GAME_PLAYING;
    }

    void UpdateInventory(Vector2 mousePos) {
        if (CheckCollisionPointRec(mousePos, { 412, 650, 200, 50 }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentState = GAME_PAUSED;
        }
        if (IsKeyPressed(KEY_ESCAPE)) currentState = GAME_PAUSED;
    }

    void UpdateGameOver() {
        if (IsKeyPressed(KEY_ENTER)) { ResetGame(); currentState = MENU_MAIN; }
        if (IsKeyPressed(KEY_ESCAPE)) { ResetGame(); currentState = MENU_MAIN; }
    }

    void DrawMainMenu() {
        DrawTexture(menuBackground, 0, 0, WHITE);
        DrawRectangle(0, 0, 1024, 768, Color{ 0,0,0,100 });

        DrawText(englishStrings[0], 250, 100, 40, RED);
        DrawText(englishStrings[1], 300, 150, 25, WHITE);

        Rectangle buttons[3] = { {412,250,200,60}, {412,320,200,60}, {412,390,200,60} };
        const char* texts[3] = { englishStrings[2], englishStrings[3], englishStrings[4] };

        for (int i = 0; i < 3; i++) {
            bool hover = CheckCollisionPointRec(GetMousePosition(), buttons[i]);
            DrawRectangleLinesEx(buttons[i], 3, RED);
            DrawRectangleRec(buttons[i], hover ? Color{ 255,0,0,100 } : Color{ 0,0,0,150 });
            int w = MeasureText(texts[i], 25);
            DrawText(texts[i], buttons[i].x + buttons[i].width / 2 - w / 2, buttons[i].y + buttons[i].height / 2 - 12, 25, hover ? YELLOW : WHITE);
        }

        char hs[50];
        sprintf(hs, "%s%d", englishStrings[5], highScore);
        DrawText(hs, 20, 20, 25, YELLOW);
    }

    void DrawPlanetMenu() {
        DrawTexture(menuBackground, 0, 0, Color{ 100,100,100,255 });
        DrawText(englishStrings[6], 350, 100, 40, RED);

        Rectangle buttons[4] = { {362,200,300,80}, {362,300,300,80}, {362,400,300,80}, {362,500,300,60} };
        const char* texts[4] = { englishStrings[7], englishStrings[8], englishStrings[9], englishStrings[10] };

        for (int i = 0; i < 4; i++) {
            bool hover = CheckCollisionPointRec(GetMousePosition(), buttons[i]);
            DrawRectangleLinesEx(buttons[i], 3, RED);
            DrawRectangleRec(buttons[i], hover ? Color{ 255,0,0,100 } : Color{ 0,0,0,150 });
            int w = MeasureText(texts[i], 20);
            DrawText(texts[i], buttons[i].x + buttons[i].width / 2 - w / 2, buttons[i].y + buttons[i].height / 2 - 10, 20, hover ? YELLOW : WHITE);
        }
    }

    void DrawDifficultyMenu() {
        DrawTexture(menuBackground, 0, 0, Color{ 100,100,100,255 });
        DrawText(englishStrings[12], 350, 100, 40, RED);

        const char* planetName = "";
        switch (currentPlanet) {
        case EARTH: planetName = englishStrings[40]; break;
        case MARS: planetName = englishStrings[41]; break;
        case JUPITER: planetName = englishStrings[42]; break;
        }
        char planetText[50];
        sprintf(planetText, "%s%s", englishStrings[11], planetName);
        DrawText(planetText, 400, 160, 25, YELLOW);

        Rectangle diffButtons[5] = { {362,250,300,80}, {362,350,300,80}, {362,450,300,80}, {362,550,300,60}, {362,620,300,50} };
        const char* diffTexts[5] = { englishStrings[13], englishStrings[14], englishStrings[15], englishStrings[16], englishStrings[10] };

        for (int i = 0; i < 5; i++) {
            bool hover = CheckCollisionPointRec(GetMousePosition(), diffButtons[i]);
            if (i != 3) DrawRectangleLinesEx(diffButtons[i], 3, RED);

            Color bgColor = Color{ 0,0,0,150 };
            if (i == 0 && currentDifficulty == EASY) bgColor = Color{ 0,100,0,200 };
            else if (i == 1 && currentDifficulty == MEDIUM) bgColor = Color{ 200,200,0,200 };
            else if (i == 2 && currentDifficulty == HARD) bgColor = Color{ 200,0,0,200 };
            else if (hover) bgColor = Color{ 255,0,0,100 };

            if (i == 3) bgColor = hover ? Color{ 0,200,0,255 } : Color{ 0,150,0,200 };

            DrawRectangleRec(diffButtons[i], bgColor);

            Color textColor = WHITE;
            if (i == 0 && currentDifficulty == EASY) textColor = GREEN;
            else if (i == 1 && currentDifficulty == MEDIUM) textColor = YELLOW;
            else if (i == 2 && currentDifficulty == HARD) textColor = Color{ 255,100,100,255 };
            else if (hover) textColor = YELLOW;

            int w = MeasureText(diffTexts[i], (i == 3) ? 25 : 20);
            DrawText(diffTexts[i], diffButtons[i].x + diffButtons[i].width / 2 - w / 2,
                diffButtons[i].y + diffButtons[i].height / 2 - ((i == 3) ? 12 : 10),
                (i == 3) ? 25 : 20, textColor);
        }
    }

    void DrawCountdown() {
        DrawRectangle(0, 0, 1024, 768, Color{ 0,0,0,150 });
        int count = (int)countdownTimer + 1;
        char countText[10];
        if (count > 0) sprintf(countText, "%d", count);
        else strcpy(countText, englishStrings[25]);
        int size = (count > 0) ? 100 : 60;
        Color color = (count > 0) ? RED : GREEN;
        int w = MeasureText(countText, size);
        DrawText(countText, 1024 / 2 - w / 2, 768 / 2 - size / 2, size, color);
    }

    void DrawPlaying() {
        DrawTexture(backgroundTexture, 0, 0, WHITE);
        DrawRectangle(0, 620, 1024, 148, Color{ 100,70,30,255 });

        for (const auto& cloud : clouds) {
            DrawTexture(cloudTexture, (int)cloud.position.x, (int)cloud.position.y, Color{ 255,255,255,180 });
        }

        for (const auto& obs : obstacles) {
            if (obs.active) DrawTexture(obstacleTexture, (int)obs.position.x, 560 - 60, WHITE);
        }

        DrawTexture(dinoTexture, (int)dino.position.x, (int)dino.position.y, WHITE);

        char scoreText[50];
        sprintf(scoreText, "%s%d", englishStrings[17], score);
        DrawText(scoreText, 20, 20, 30, YELLOW);

        const char* planetName = "";
        switch (currentPlanet) {
        case EARTH: planetName = englishStrings[40]; break;
        case MARS: planetName = englishStrings[41]; break;
        case JUPITER: planetName = englishStrings[42]; break;
        }
        const char* diffName = "";
        switch (currentDifficulty) {
        case EASY: diffName = englishStrings[43]; break;
        case MEDIUM: diffName = englishStrings[44]; break;
        case HARD: diffName = englishStrings[45]; break;
        }
        char infoText[100];
        sprintf(infoText, "%s%s | %s%s", englishStrings[18], planetName, englishStrings[19], diffName);
        DrawText(infoText, 20, 60, 20, GREEN);

        if (showNotification) {
            DrawRectangle(200, 300, 624, 100, Color{ 0,100,0,200 });
            DrawRectangleLines(200, 300, 624, 100, GREEN);
            DrawText(englishStrings[23], 1024 / 2 - MeasureText(englishStrings[23], 25) / 2, 320, 25, YELLOW);
            DrawText(englishStrings[24], 1024 / 2 - MeasureText(englishStrings[24], 20) / 2, 360, 20, WHITE);
        }

        DrawText(englishStrings[20], 600, 20, 18, WHITE);
        DrawText(englishStrings[21], 600, 45, 18, WHITE);
        DrawText(englishStrings[22], 600, 70, 18, WHITE);
    }

    void DrawPaused() {
        DrawPlaying();
        DrawRectangle(0, 0, 1024, 768, Color{ 0,0,0,150 });
        DrawText(englishStrings[26], 1024 / 2 - MeasureText(englishStrings[26], 50) / 2, 200, 50, RED);

        Rectangle buttons[3] = { {412,300,200,60}, {412,380,200,60}, {412,460,200,60} };
        const char* texts[3] = { englishStrings[27], englishStrings[28], englishStrings[29] };

        for (int i = 0; i < 3; i++) {
            bool hover = CheckCollisionPointRec(GetMousePosition(), buttons[i]);
            DrawRectangleLinesEx(buttons[i], 3, RED);
            DrawRectangleRec(buttons[i], hover ? Color{ 255,0,0,100 } : Color{ 0,0,0,150 });
            int w = MeasureText(texts[i], 25);
            DrawText(texts[i], buttons[i].x + buttons[i].width / 2 - w / 2, buttons[i].y + buttons[i].height / 2 - 12, 25, hover ? YELLOW : WHITE);
        }
    }

    void DrawInventory() {
        DrawRectangle(0, 0, 1024, 768, Color{ 30,30,30,255 });
        DrawText(englishStrings[30], 1024 / 2 - MeasureText(englishStrings[30], 30) / 2, 50, 30, GREEN);

        int yPos = 120;
        int factsShown = 0;
        for (size_t i = 0; i < facts.size(); i++) {
            if (facts[i].unlocked) {
                DrawText(factTexts[i], 50, yPos, 20, WHITE);
                yPos += 40;
                factsShown++;
            }
        }

        if (factsShown == 0) {
            DrawText(englishStrings[32], 1024 / 2 - MeasureText(englishStrings[32], 25) / 2, 300, 25, YELLOW);
            DrawText(englishStrings[33], 1024 / 2 - MeasureText(englishStrings[33], 20) / 2, 350, 20, GRAY);
        }

        Rectangle backBtn = { 412, 650, 200, 50 };
        bool hover = CheckCollisionPointRec(GetMousePosition(), backBtn);
        DrawRectangleLinesEx(backBtn, 3, RED);
        DrawRectangleRec(backBtn, hover ? Color{ 255,0,0,100 } : Color{ 0,0,0,150 });
        int w = MeasureText(englishStrings[34], 25);
        DrawText(englishStrings[34], backBtn.x + backBtn.width / 2 - w / 2, backBtn.y + backBtn.height / 2 - 12, 25, hover ? YELLOW : WHITE);
    }

    void DrawGameOver() {
        DrawRectangle(0, 0, 1024, 768, Color{ 0,0,0,200 });
        DrawText(englishStrings[35], 1024 / 2 - MeasureText(englishStrings[35], 50) / 2, 200, 50, RED);

        char scoreText[50];
        sprintf(scoreText, "%s%d", englishStrings[36], score);
        DrawText(scoreText, 1024 / 2 - MeasureText(scoreText, 40) / 2, 280, 40, YELLOW);

        if (score > highScore) {
            DrawText(englishStrings[37], 1024 / 2 - MeasureText(englishStrings[37], 35) / 2, 340, 35, GREEN);
        }

        DrawText(englishStrings[38], 1024 / 2 - MeasureText(englishStrings[38], 25) / 2, 450, 25, WHITE);
        DrawText(englishStrings[39], 1024 / 2 - MeasureText(englishStrings[39], 25) / 2, 500, 25, WHITE);
    }

    void StartGame() {
        ResetGame();
        currentState = GAME_COUNTDOWN;
    }

    void ResetGame() {
        score = 0;
        gameTime = 0.0f;
        spawnTimer = 0.0f;
        countdownTimer = 3.0f;
        dino.Reset();
        for (auto& obs : obstacles) obs.Reset();
    }

    void UnlockRandomFact() {
        std::vector<int> lockedFacts;
        for (size_t i = 0; i < facts.size(); i++) {
            if (!facts[i].unlocked) lockedFacts.push_back((int)i);
        }

        if (!lockedFacts.empty()) {
            int randomIndex = GetRandomValue(0, (int)lockedFacts.size() - 1);
            int factId = lockedFacts[randomIndex];
            facts[factId].unlocked = true;
            showNotification = true;
            notificationTimer = 3.0f;
        }
    }
};

// ==================== ГЛАВНАЯ ФУНКЦИЯ ====================
// ==================== ГЛАВНАЯ ФУНКЦИЯ ====================
int main() {
    // Инициализация главного меню
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Three Games Collection");
    SetTargetFPS(60);

    MainMenu mainMenu;

    // Главный цикл меню
    while (!WindowShouldClose()) {
        int selectedGame = mainMenu.Update();

        // Запуск выбранной игры
        if (selectedGame > 0) {
            CloseWindow();  // Закрываем главное меню

            if (selectedGame == 1) {
                // Запуск игры Никиты
                PhysicsGame_Nikita game;
                game.Run();
            }
            else if (selectedGame == 2) {
                // Запуск игры Сергея
                MagneticGame_Sergey game;
                game.Run();
            }
            else if (selectedGame == 3) {
                // Запуск игры Александра
                CosmicDino_Alexander game;
                game.Run();
            }

            // После выхода из игры - воссоздаем главное меню
            InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Three Games Collection");
            SetTargetFPS(60);
        }
        else if (selectedGame == 0) {
            break;  // Выход из программы
        }

        // Отрисовка главного меню
        BeginDrawing();
        ClearBackground(BLACK);
        mainMenu.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}