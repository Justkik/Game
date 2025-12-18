#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

// Состояния игры
enum GameState {
    MENU_MAIN,
    MENU_PLANET,
    MENU_DIFFICULTY,
    GAME_COUNTDOWN,
    GAME_PLAYING,
    GAME_PAUSED,
    MENU_INVENTORY,
    GAME_OVER
};

// Планеты
enum Planet {
    EARTH,
    MARS,
    JUPITER
};

// Сложность
enum Difficulty {
    EASY,
    MEDIUM,
    HARD
};

// Физический факт
struct PhysicsFact {
    int id;
    bool unlocked;
};

// Класс игры
class Game {
private:
    // Состояние игры
    GameState currentState;

    // Шрифт для русского текста
    Font russianFont;

    // Структура динозавра
    struct Dinosaur {
        Vector2 position;
        Vector2 velocity;
        Rectangle collider;
        bool isJumping;
        bool canDoubleJump;
        int jumpCount;
        float jumpForce;

        Dinosaur();
        void Update(float gravity);
        void Jump(float force);
        void DoubleJump(float force);
        void Reset();
    };

    // Структура препятствия
    struct Obstacle {
        Vector2 position;
        Rectangle collider;
        float speed;
        bool active;
        Color color;

        Obstacle();
        void Update();
        void Spawn(float x, float y, float obstacleSpeed);
        void Reset();
    };

    // Структура облака
    struct Cloud {
        Vector2 position;
        float speed;
    };

    // Игровые объекты
    Dinosaur dino;
    std::vector<Obstacle> obstacles;
    std::vector<Cloud> clouds;
    std::vector<PhysicsFact> facts;

    // Ресурсы
    Texture2D dinoTexture;
    Texture2D obstacleTexture;
    Texture2D backgroundTexture;
    Texture2D menuBackground;
    Texture2D cloudTexture;

    // Настройки
    Planet currentPlanet;
    Difficulty currentDifficulty;

    // Игровые переменные
    int score;
    int highScore;
    float countdownTimer;
    float gameTime;
    float spawnTimer;
    float notificationTimer;

    // Флаги
    bool showNotification;

    // Факты (10 физических фактов)
    const char* factTexts[10] = {
        "1. На Юпитере вы бы весили в 2.5 раза больше!",
        "2. Сила притяжения на Марсе всего 38% от земной!",
        "3. Черные дыры обладают самой сильной гравитацией!",
        "4. В невесомости пламя становится сферическим!",
        "5. Время замедляется в сильном гравитационном поле!",
        "6. На нейтронных звездах гравитация в миллиарды раз сильнее!",
        "7. Луна удаляется от Земли на 3.8 см в год!",
        "8. В космосе отсутствует сила Архимеда!",
        "9. На Венере давление в 92 раза больше земного!",
        "10. Солнце составляет 99.86% массы Солнечной системы!"
    };

public:
    Game();
    ~Game();

    void Run();

private:
    // Методы инициализации
    void LoadResources();
    void UnloadResources();
    void InitFacts();

    // Методы обновления
    void UpdateMainMenu();
    void UpdatePlanetMenu();
    void UpdateDifficultyMenu();
    void UpdateCountdown();
    void UpdatePlaying();
    void UpdatePaused();
    void UpdateInventory();
    void UpdateGameOver();

    // Методы отрисовки
    void DrawMainMenu();
    void DrawPlanetMenu();
    void DrawDifficultyMenu();
    void DrawCountdown();
    void DrawPlaying();
    void DrawPaused();
    void DrawInventory();
    void DrawGameOver();

    // Методы для русского текста
    void DrawRussianText(const char* text, int posX, int posY, int fontSize, Color color);
    int MeasureRussianText(const char* text, int fontSize);

    // Вспомогательные методы
    void StartGame();
    void ResetGame();
    void SpawnObstacle();
    void CheckCollisions();
    void UnlockRandomFact();

    // Настройки планет и сложности
    float GetPlanetGravity();
    float GetObstacleSpeed();
    float GetSpawnRate();
};

#endif