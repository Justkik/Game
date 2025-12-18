#include "Game.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

// English strings
const char* englishStrings[] = {
    "COSMIC DINOSAUR",
    "Physics Adventure",
    "PLAY",
    "INVENTORY",
    "EXIT",
    "High Score: ",
    "CHOOSE A PLANET",
    "EARTH (g=9.8)",
    "MARS (g=3.7)",
    "JUPITER (g=24.8)",
    "BACK",
    "Planet: ",
    "CHOOSE DIFFICULTY",
    "EASY",
    "MEDIUM",
    "HARD",
    "START GAME",
    "Score: ",
    "Planet: ",
    "Difficulty: ",
    "SPACE - jump",
    "Double SPACE - double jump",
    "ESC - pause",
    "NEW PHYSICS FACT DISCOVERED!",
    "Check inventory in pause menu",
    "START!",
    "PAUSE",
    "RESUME",
    "INVENTORY",
    "TO MENU",
    "INVENTORY - PHYSICS FACTS",
    "Fact ",
    "No facts discovered yet!",
    "Earn 500 points in game to unlock a fact", // Изменено с 50 на 500
    "BACK",
    "GAME OVER!",
    "Your score: ",
    "NEW RECORD!",
    "Press ENTER to play again",
    "Press ESC to return to menu",
    "Earth",
    "Mars",
    "Jupiter",
    "Easy",
    "Medium",
    "Hard"
};

// Facts in English
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

// ===== DINOSAUR IMPLEMENTATION =====
Game::Dinosaur::Dinosaur() {
    position = Vector2{ 100, 500 };
    velocity = Vector2{ 0, 0 };
    collider = Rectangle{ 100, 500, 60, 100 };
    isJumping = false;
    canDoubleJump = true;
    jumpCount = 0;
    jumpForce = 18.0f;
}

void Game::Dinosaur::Update(float gravity) {
    // Apply gravity
    velocity.y += gravity;
    position.y += velocity.y;

    // Update collider
    collider.x = position.x + 10;
    collider.y = position.y + 10;

    // Ground check
    if (position.y >= 500) {
        position.y = 500;
        velocity.y = 0;
        isJumping = false;
        jumpCount = 0;
        canDoubleJump = true;
    }
}

void Game::Dinosaur::Jump(float force) {
    if (!isJumping) {
        velocity.y = -force;
        isJumping = true;
        jumpCount = 1;
    }
}

void Game::Dinosaur::DoubleJump(float force) {
    if (isJumping && canDoubleJump && jumpCount < 2) {
        velocity.y = -force * 0.9f;
        jumpCount = 2;
        canDoubleJump = false;
    }
}

void Game::Dinosaur::Reset() {
    position = Vector2{ 100, 500 };
    velocity = Vector2{ 0, 0 };
    collider = Rectangle{ 100, 500, 60, 100 };
    isJumping = false;
    canDoubleJump = true;
    jumpCount = 0;
}

// ===== OBSTACLE IMPLEMENTATION =====
Game::Obstacle::Obstacle() {
    position = Vector2{ 0, 0 };
    collider = Rectangle{ 0, 0, 50, 60 };
    speed = 0;
    active = false;
    color = RED;
}

void Game::Obstacle::Update() {
    if (active) {
        position.x -= speed;
        collider.x = position.x;

        if (position.x < -100) {
            active = false;
        }
    }
}

void Game::Obstacle::Spawn(float x, float y, float obstacleSpeed) {
    position = Vector2{ x, y };
    collider = Rectangle{ x, y, 50, 60 };
    speed = obstacleSpeed;
    active = true;
    color = Color{
        (unsigned char)GetRandomValue(100, 200),
        (unsigned char)GetRandomValue(100, 200),
        (unsigned char)GetRandomValue(100, 200),
        255
    };
}

void Game::Obstacle::Reset() {
    active = false;
    position = Vector2{ 0, 0 };
}

// ===== GAME IMPLEMENTATION =====

Game::Game() {
    // Window initialization
    InitWindow(1024, 768, "Cosmic Dinosaur");
    SetTargetFPS(60);

    // Audio initialization
    InitAudioDevice();

    // Load font
    const char* fontPaths[] = {
        "fonts/arial.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/tahoma.ttf",
        NULL
    };

    bool fontLoaded = false;
    for (int i = 0; fontPaths[i] != NULL; i++) {
        if (FileExists(fontPaths[i])) {
            russianFont = LoadFontEx(fontPaths[i], 32, 0, 250);
            fontLoaded = true;
            std::cout << "Font loaded: " << fontPaths[i] << std::endl;
            break;
        }
    }

    if (!fontLoaded) {
        russianFont = GetFontDefault();
        std::cout << "Using default font" << std::endl;
    }

    // Load resources
    LoadResources();

    // Initialize variables
    currentState = MENU_MAIN;
    currentPlanet = EARTH;
    currentDifficulty = MEDIUM;
    score = 0;
    highScore = 0;
    countdownTimer = 3.0f;
    gameTime = 0.0f;
    spawnTimer = 0.0f;
    notificationTimer = 0.0f;
    showNotification = false;

    // Initialize objects
    obstacles.resize(6);

    // Initialize clouds
    clouds.resize(3);
    for (int i = 0; i < 3; i++) {
        clouds[i].position = Vector2{ (float)GetRandomValue(0, 800), (float)GetRandomValue(50, 200) };
        clouds[i].speed = (float)GetRandomValue(1, 3);
    }

    // Initialize facts
    InitFacts();
}

Game::~Game() {
    // Unload font
    if (russianFont.texture.id != 0 && russianFont.texture.id != GetFontDefault().texture.id) {
        UnloadFont(russianFont);
    }

    UnloadResources();
    CloseAudioDevice();
    CloseWindow();
}

void Game::LoadResources() {
    // Загрузка PNG текстур вместо генерации
    if (FileExists("images/glav.png")) {
        menuBackground = LoadTexture("images/glav.png");
    } else {
        // Fallback если файл не найден
        Image menuImg = GenImageColor(1024, 768, DARKBLUE);
        menuBackground = LoadTextureFromImage(menuImg);
        UnloadImage(menuImg);
    }

    if (FileExists("images/gamefon.png")) {
        backgroundTexture = LoadTexture("images/gamefon.png");
    } else {
        // Fallback если файл не найден
        Image bgImg = GenImageColor(1024, 768, BLACK);
        // Add stars
        for (int i = 0; i < 200; i++) {
            int x = GetRandomValue(0, 1023);
            int y = GetRandomValue(0, 767);
            ImageDrawPixel(&bgImg, x, y, WHITE);
        }
        backgroundTexture = LoadTextureFromImage(bgImg);
        UnloadImage(bgImg);
    }

    // СОЗДАЕМ ДИНОЗАВРА ИЗ ЗЕЛЕНЫХ БЛОКОВ
    Image dinoImg = GenImageColor(80, 120, BLANK);
    
    // Тело (туловище)
    for (int y = 30; y < 70; y++) {
        for (int x = 25; x < 55; x++) {
            ImageDrawPixel(&dinoImg, x, y, GREEN);
        }
    }
    
    // Голова
    for (int y = 10; y < 30; y++) {
        for (int x = 30; x < 50; x++) {
            ImageDrawPixel(&dinoImg, x, y, GREEN);
        }
    }
    
    // Глаза
    ImageDrawPixel(&dinoImg, 35, 15, WHITE);
    ImageDrawPixel(&dinoImg, 45, 15, WHITE);
    
    // Ноги
    for (int y = 70; y < 90; y++) {
        for (int x = 30; x < 35; x++) {
            ImageDrawPixel(&dinoImg, x, y, GREEN);
        }
        for (int x = 45; x < 50; x++) {
            ImageDrawPixel(&dinoImg, x, y, GREEN);
        }
    }
    
    // Руки
    for (int y = 40; y < 50; y++) {
        for (int x = 20; x < 25; x++) {
            ImageDrawPixel(&dinoImg, x, y, GREEN);
        }
        for (int x = 55; x < 60; x++) {
            ImageDrawPixel(&dinoImg, x, y, GREEN);
        }
    }
    
    // Детали
    for (int x = 35; x < 45; x++) {
        ImageDrawPixel(&dinoImg, x, 25, RED);
    }
    
    ImageDrawPixel(&dinoImg, 36, 16, BLACK);
    ImageDrawPixel(&dinoImg, 46, 16, BLACK);
    
    dinoTexture = LoadTextureFromImage(dinoImg);

    // ПРЕПЯТСТВИЯ
    Image obstacleImg = GenImageColor(50, 60, BLANK);
    
    for (int y = 0; y < 60; y++) {
        for (int x = 0; x < 50; x++) {
            if (x > 5 && x < 45 && y > 5 && y < 55) {
                Color rockColor = {
                    (unsigned char)GetRandomValue(120, 180),
                    (unsigned char)GetRandomValue(80, 140),
                    (unsigned char)GetRandomValue(60, 120),
                    255
                };
                ImageDrawPixel(&obstacleImg, x, y, rockColor);
            }
        }
    }
    
    obstacleTexture = LoadTextureFromImage(obstacleImg);

    // Облака
    Image cloudImg = GenImageColor(200, 100, WHITE);
    cloudTexture = LoadTextureFromImage(cloudImg);

    // Освобождаем изображения
    UnloadImage(dinoImg);
    UnloadImage(obstacleImg);
    UnloadImage(cloudImg);
}

void Game::UnloadResources() {
    UnloadTexture(dinoTexture);
    UnloadTexture(obstacleTexture);
    UnloadTexture(backgroundTexture);
    UnloadTexture(menuBackground);
    UnloadTexture(cloudTexture);
}

void Game::InitFacts() {
    facts.resize(10);
    for (int i = 0; i < 10; i++) {
        facts[i].id = i;
        facts[i].unlocked = false;
    }
}

float Game::GetPlanetGravity() {
    switch (currentPlanet) {
    case EARTH: return 0.5f;
    case MARS: return 0.3f;
    case JUPITER: return 0.8f;
    default: return 0.5f;
    }
}

float Game::GetObstacleSpeed() {
    switch (currentDifficulty) {
    case EASY: return 5.0f;
    case MEDIUM: return 7.0f;
    case HARD: return 10.0f;
    default: return 7.0f;
    }
}

float Game::GetSpawnRate() {
    switch (currentDifficulty) {
    case EASY: return 2.5f;
    case MEDIUM: return 2.0f;
    case HARD: return 1.5f;
    default: return 2.0f;
    }
}

void Game::DrawRussianText(const char* text, int posX, int posY, int fontSize, Color color) {
    if (russianFont.texture.id != 0) {
        DrawTextEx(russianFont, text, Vector2{ (float)posX, (float)posY }, (float)fontSize, 1, color);
    }
    else {
        DrawText(text, posX, posY, fontSize, color);
    }
}

int Game::MeasureRussianText(const char* text, int fontSize) {
    if (russianFont.texture.id != 0) {
        return MeasureTextEx(russianFont, text, (float)fontSize, 1).x;
    }
    else {
        return MeasureText(text, fontSize);
    }
}

void Game::Run() {
    while (!WindowShouldClose()) {
        // Update state
        switch (currentState) {
        case MENU_MAIN: UpdateMainMenu(); break;
        case MENU_PLANET: UpdatePlanetMenu(); break;
        case MENU_DIFFICULTY: UpdateDifficultyMenu(); break;
        case GAME_COUNTDOWN: UpdateCountdown(); break;
        case GAME_PLAYING: UpdatePlaying(); break;
        case GAME_PAUSED: UpdatePaused(); break;
        case MENU_INVENTORY: UpdateInventory(); break;
        case GAME_OVER: UpdateGameOver(); break;
        }

        // Drawing
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

// ===== UPDATE METHODS =====

void Game::UpdateMainMenu() {
    Vector2 mousePos = GetMousePosition();

    // PLAY button
    if (CheckCollisionPointRec(mousePos, Rectangle{ 412, 250, 200, 60 }) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        currentState = MENU_PLANET; // ПЕРЕХОДИМ В ВЫБОР ПЛАНЕТЫ
    }

    // INVENTORY button
    if (CheckCollisionPointRec(mousePos, Rectangle{ 412, 320, 200, 60 }) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        currentState = MENU_INVENTORY;
    }

    // EXIT button
    if (CheckCollisionPointRec(mousePos, Rectangle{ 412, 390, 200, 60 }) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        CloseWindow();
    }
}

void Game::UpdatePlanetMenu() {
    Vector2 mousePos = GetMousePosition();

    // EARTH button
    if (CheckCollisionPointRec(mousePos, Rectangle{ 362, 200, 300, 80 }) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        currentPlanet = EARTH;
        currentState = MENU_DIFFICULTY; // ПЕРЕХОДИМ В ВЫБОР СЛОЖНОСТИ
    }

    // MARS button
    if (CheckCollisionPointRec(mousePos, Rectangle{ 362, 300, 300, 80 }) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        currentPlanet = MARS;
        currentState = MENU_DIFFICULTY;
    }

    // JUPITER button
    if (CheckCollisionPointRec(mousePos, Rectangle{ 362, 400, 300, 80 }) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        currentPlanet = JUPITER;
        currentState = MENU_DIFFICULTY;
    }

    // BACK button
    if (CheckCollisionPointRec(mousePos, Rectangle{ 362, 500, 300, 60 }) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        currentState = MENU_MAIN; // ВОЗВРАТ В ГЛАВНОЕ МЕНЮ
    }

    // ESC to exit
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentState = MENU_MAIN;
    }
}

void Game::UpdateDifficultyMenu() {
    Vector2 mousePos = GetMousePosition();

    // EASY button
    if (CheckCollisionPointRec(mousePos, Rectangle{ 362, 250, 300, 80 }) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        currentDifficulty = EASY;
    }

    // MEDIUM button
    if (CheckCollisionPointRec(mousePos, Rectangle{ 362, 350, 300, 80 }) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        currentDifficulty = MEDIUM;
    }

    // HARD button
    if (CheckCollisionPointRec(mousePos, Rectangle{ 362, 450, 300, 80 }) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        currentDifficulty = HARD;
    }

    // START GAME button
    if (CheckCollisionPointRec(mousePos, Rectangle{ 362, 550, 300, 60 }) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        StartGame(); // НАЧАЛО ИГРЫ
    }

    // BACK button
    if (CheckCollisionPointRec(mousePos, Rectangle{ 362, 620, 300, 50 }) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        currentState = MENU_PLANET; // ВОЗВРАТ К ВЫБОРУ ПЛАНЕТЫ
    }

    // ESC to exit
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentState = MENU_PLANET;
    }
}

void Game::UpdateCountdown() {
    countdownTimer -= GetFrameTime();

    if (countdownTimer <= 0) {
        currentState = GAME_PLAYING;
        countdownTimer = 3.0f;
    }
}

void Game::UpdatePlaying() {
    // Pause
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentState = GAME_PAUSED;
        return;
    }

    // Dinosaur physics
    float gravity = GetPlanetGravity();
    dino.Update(gravity);

    // Jumping
    if (IsKeyPressed(KEY_SPACE)) {
        if (!dino.isJumping) {
            dino.Jump(dino.jumpForce);
        }
        else if (dino.canDoubleJump && dino.jumpCount < 2) {
            dino.DoubleJump(dino.jumpForce);
        }
    }

    // Update obstacles
    float obstacleSpeed = GetObstacleSpeed();
    float spawnRate = GetSpawnRate();

    spawnTimer += GetFrameTime();
    if (spawnTimer >= spawnRate) {
        SpawnObstacle();
        spawnTimer = 0;
    }

    for (auto& obstacle : obstacles) {
        obstacle.Update();

        // Score for passed obstacles
        if (!obstacle.active && obstacle.position.x < -100) {
            score++;

            // ИЗМЕНЕНО: Новый факт каждые 500 очков вместо 50
            if (score > 0 && score % 500 == 0) {
                UnlockRandomFact();
            }

            // Update high score
            if (score > highScore) {
                highScore = score;
            }
        }
    }

    // Update clouds
    for (auto& cloud : clouds) {
        cloud.position.x -= cloud.speed;
        if (cloud.position.x < -200) {
            cloud.position.x = 1100;
            cloud.position.y = (float)GetRandomValue(50, 300);
        }
    }

    // Check collisions
    CheckCollisions();

    // Update notifications
    if (showNotification) {
        notificationTimer -= GetFrameTime();
        if (notificationTimer <= 0) {
            showNotification = false;
        }
    }
}

void Game::UpdatePaused() {
    Vector2 mousePos = GetMousePosition();

    // RESUME button
    if (CheckCollisionPointRec(mousePos, Rectangle{ 412, 300, 200, 60 }) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        currentState = GAME_PLAYING;
    }

    // INVENTORY button
    if (CheckCollisionPointRec(mousePos, Rectangle{ 412, 380, 200, 60 }) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        currentState = MENU_INVENTORY;
    }

    // TO MENU button
    if (CheckCollisionPointRec(mousePos, Rectangle{ 412, 460, 200, 60 }) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        ResetGame();
        currentState = MENU_MAIN;
    }

    // ESC to resume
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentState = GAME_PLAYING;
    }
}

void Game::UpdateInventory() {
    Vector2 mousePos = GetMousePosition();

    // BACK button
    if (CheckCollisionPointRec(mousePos, Rectangle{ 412, 650, 200, 50 }) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        currentState = GAME_PAUSED;
    }

    // ESC to exit
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentState = GAME_PAUSED;
    }
}

void Game::UpdateGameOver() {
    // New game on ENTER
    if (IsKeyPressed(KEY_ENTER)) {
        ResetGame();
        currentState = MENU_MAIN;
    }

    // ESC to exit to menu
    if (IsKeyPressed(KEY_ESCAPE)) {
        ResetGame();
        currentState = MENU_MAIN;
    }
}

// ===== DRAW METHODS =====

void Game::DrawMainMenu() {
    // Background with loaded PNG
    DrawTexture(menuBackground, 0, 0, WHITE);
    
    // Dark overlay for better text visibility
    DrawRectangle(0, 0, 1024, 768, Color{ 0, 0, 0, 100 });

    // Title
    DrawRussianText(englishStrings[0], 250, 100, 40, RED);
    DrawRussianText(englishStrings[1], 300, 150, 25, WHITE);

    // Buttons
    Rectangle buttons[3];
    buttons[0] = Rectangle{ 412, 250, 200, 60 };
    buttons[1] = Rectangle{ 412, 320, 200, 60 };
    buttons[2] = Rectangle{ 412, 390, 200, 60 };

    const char* buttonTexts[3] = { englishStrings[2], englishStrings[3], englishStrings[4] };

    for (int i = 0; i < 3; i++) {
        bool hover = CheckCollisionPointRec(GetMousePosition(), buttons[i]);

        // Red border
        DrawRectangleLinesEx(buttons[i], 3, RED);

        // Background on hover
        if (hover) {
            DrawRectangleRec(buttons[i], Color{ 255, 0, 0, 100 });
        } else {
            DrawRectangleRec(buttons[i], Color{ 0, 0, 0, 150 });
        }

        // Text
        int textWidth = MeasureRussianText(buttonTexts[i], 25);
        DrawRussianText(buttonTexts[i],
            buttons[i].x + buttons[i].width / 2 - textWidth / 2,
            buttons[i].y + buttons[i].height / 2 - 12,
            25, hover ? YELLOW : WHITE);
    }

    // High Score
    char hsText[50];
    sprintf(hsText, "%s%d", englishStrings[5], highScore);
    DrawRussianText(hsText, 20, 20, 25, YELLOW);
}

void Game::DrawPlanetMenu() {
    // Darkened background with menu image
    DrawTexture(menuBackground, 0, 0, Color{ 100, 100, 100, 255 });

    DrawRussianText(englishStrings[6], 350, 100, 40, RED);

    // Planet buttons
    Rectangle buttons[4];
    buttons[0] = Rectangle{ 362, 200, 300, 80 };
    buttons[1] = Rectangle{ 362, 300, 300, 80 };
    buttons[2] = Rectangle{ 362, 400, 300, 80 };
    buttons[3] = Rectangle{ 362, 500, 300, 60 };

    const char* planetTexts[4] = { englishStrings[7], englishStrings[8], englishStrings[9], englishStrings[10] };

    for (int i = 0; i < 4; i++) {
        bool hover = CheckCollisionPointRec(GetMousePosition(), buttons[i]);

        // Red border
        DrawRectangleLinesEx(buttons[i], 3, RED);

        // Background on hover
        if (hover) {
            DrawRectangleRec(buttons[i], Color{ 255, 0, 0, 100 });
        } else {
            DrawRectangleRec(buttons[i], Color{ 0, 0, 0, 150 });
        }

        // Text
        int textWidth = MeasureRussianText(planetTexts[i], 20);
        DrawRussianText(planetTexts[i],
            buttons[i].x + buttons[i].width / 2 - textWidth / 2,
            buttons[i].y + buttons[i].height / 2 - 10,
            20, hover ? YELLOW : WHITE);
    }
}

void Game::DrawDifficultyMenu() {
    // Darkened background with menu image
    DrawTexture(menuBackground, 0, 0, Color{ 100, 100, 100, 255 });

    DrawRussianText(englishStrings[12], 350, 100, 40, RED);

    // Planet info
    const char* planetName = "";
    switch (currentPlanet) {
    case EARTH: planetName = englishStrings[40]; break;
    case MARS: planetName = englishStrings[41]; break;
    case JUPITER: planetName = englishStrings[42]; break;
    }

    char planetText[50];
    sprintf(planetText, "%s%s", englishStrings[11], planetName);
    DrawRussianText(planetText, 400, 160, 25, YELLOW);

    // Difficulty buttons
    Rectangle diffButtons[5];
    diffButtons[0] = Rectangle{ 362, 250, 300, 80 };
    diffButtons[1] = Rectangle{ 362, 350, 300, 80 };
    diffButtons[2] = Rectangle{ 362, 450, 300, 80 };
    diffButtons[3] = Rectangle{ 362, 550, 300, 60 };
    diffButtons[4] = Rectangle{ 362, 620, 300, 50 };

    const char* diffTexts[5] = { englishStrings[13], englishStrings[14], englishStrings[15], englishStrings[16], englishStrings[10] };

    // Determine selected difficulty
    bool isEasySelected = (currentDifficulty == EASY);
    bool isMediumSelected = (currentDifficulty == MEDIUM);
    bool isHardSelected = (currentDifficulty == HARD);

    for (int i = 0; i < 5; i++) {
        bool hover = CheckCollisionPointRec(GetMousePosition(), diffButtons[i]);

        // For "Start" button - no border
        if (i != 3) {
            DrawRectangleLinesEx(diffButtons[i], 3, RED);
        }

        // Background color
        Color bgColor = Color{ 0, 0, 0, 150 };
        if (i == 0 && isEasySelected) bgColor = Color{ 0, 100, 0, 200 };
        else if (i == 1 && isMediumSelected) bgColor = Color{ 200, 200, 0, 200 };
        else if (i == 2 && isHardSelected) bgColor = Color{ 200, 0, 0, 200 };
        else if (hover) bgColor = Color{ 255, 0, 0, 100 };

        if (i == 3) { // "Start" button - green
            bgColor = hover ? Color{ 0, 200, 0, 255 } : Color{ 0, 150, 0, 200 };
        }

        DrawRectangleRec(diffButtons[i], bgColor);

        // Text
        int textWidth = MeasureRussianText(diffTexts[i], (i == 3) ? 25 : 20);
        Color textColor = WHITE;

        if (i == 0 && isEasySelected) textColor = GREEN;
        else if (i == 1 && isMediumSelected) textColor = YELLOW;
        else if (i == 2 && isHardSelected) textColor = Color{ 255, 100, 100, 255 };
        else if (i == 3) textColor = WHITE;
        else if (hover) textColor = YELLOW;

        DrawRussianText(diffTexts[i],
            diffButtons[i].x + diffButtons[i].width / 2 - textWidth / 2,
            diffButtons[i].y + diffButtons[i].height / 2 - ((i == 3) ? 12 : 10),
            (i == 3) ? 25 : 20, textColor);
    }
}

void Game::DrawCountdown() {
    // Dark overlay
    DrawRectangle(0, 0, 1024, 768, Color{ 0, 0, 0, 150 });

    int count = (int)countdownTimer + 1;
    char countText[10];

    if (count > 0) {
        sprintf(countText, "%d", count);
    }
    else {
        strcpy(countText, englishStrings[25]);
    }

    int fontSize = (count > 0) ? 100 : 60;
    Color color = (count > 0) ? RED : GREEN;

    int textWidth = MeasureRussianText(countText, fontSize);
    DrawRussianText(countText, 1024 / 2 - textWidth / 2, 768 / 2 - fontSize / 2, fontSize, color);
}

void Game::DrawPlaying() {
    // Background with loaded PNG
    DrawTexture(backgroundTexture, 0, 0, WHITE);

    // Ground
    DrawRectangle(0, 620, 1024, 148, Color{ 100, 70, 30, 255 });

    // Облака
    for (const auto& cloud : clouds) {
        DrawTexture(cloudTexture, (int)cloud.position.x, (int)cloud.position.y, Color{ 255, 255, 255, 180 });
    }

    // ПРЕПЯТСТВИЯ
    for (const auto& obstacle : obstacles) {
        if (obstacle.active) {
            DrawTexture(obstacleTexture, (int)obstacle.position.x, 560 - 60, WHITE);
        }
    }

    // Динозавр
    DrawTexture(dinoTexture, (int)dino.position.x, (int)dino.position.y, WHITE);

    // Интерфейс
    char scoreText[50];
    sprintf(scoreText, "%s%d", englishStrings[17], score);
    DrawRussianText(scoreText, 20, 20, 30, YELLOW);

    // Информация о планете и сложности
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
    DrawRussianText(infoText, 20, 60, 20, GREEN);

    // Уведомление о новом факте
    if (showNotification) {
        DrawRectangle(200, 300, 624, 100, Color{ 0, 100, 0, 200 });
        DrawRectangleLines(200, 300, 624, 100, GREEN);
        DrawRussianText(englishStrings[23],
            1024 / 2 - MeasureRussianText(englishStrings[23], 25) / 2,
            320, 25, YELLOW);
        DrawRussianText(englishStrings[24],
            1024 / 2 - MeasureRussianText(englishStrings[24], 20) / 2,
            360, 20, WHITE);
    }

    // Управление
    DrawRussianText(englishStrings[20], 600, 20, 18, WHITE);
    DrawRussianText(englishStrings[21], 600, 45, 18, WHITE);
    DrawRussianText(englishStrings[22], 600, 70, 18, WHITE);
}

void Game::DrawPaused() {
    // Игра на фоне
    DrawPlaying();

    // Затемнение
    DrawRectangle(0, 0, 1024, 768, Color{ 0, 0, 0, 150 });

    // Меню паузы
    DrawRussianText(englishStrings[26], 1024 / 2 - MeasureRussianText(englishStrings[26], 50) / 2, 200, 50, RED);

    Rectangle buttons[3];
    buttons[0] = Rectangle{ 412, 300, 200, 60 };
    buttons[1] = Rectangle{ 412, 380, 200, 60 };
    buttons[2] = Rectangle{ 412, 460, 200, 60 };

    const char* pauseTexts[3] = { englishStrings[27], englishStrings[28], englishStrings[29] };

    for (int i = 0; i < 3; i++) {
        bool hover = CheckCollisionPointRec(GetMousePosition(), buttons[i]);

        DrawRectangleLinesEx(buttons[i], 3, RED);

        if (hover) {
            DrawRectangleRec(buttons[i], Color{ 255, 0, 0, 100 });
        } else {
            DrawRectangleRec(buttons[i], Color{ 0, 0, 0, 150 });
        }

        int textWidth = MeasureRussianText(pauseTexts[i], 25);
        DrawRussianText(pauseTexts[i],
            buttons[i].x + buttons[i].width / 2 - textWidth / 2,
            buttons[i].y + buttons[i].height / 2 - 12,
            25, hover ? YELLOW : WHITE);
    }
}

void Game::DrawInventory() {
    // Темный фон
    DrawRectangle(0, 0, 1024, 768, Color{ 30, 30, 30, 255 });

    DrawRussianText(englishStrings[30],
        1024 / 2 - MeasureRussianText(englishStrings[30], 30) / 2,
        50, 30, GREEN);

    // Отображение фактов - ТОЛЬКО ОТКРЫТЫХ!
    int yPos = 120;
    int factsShown = 0;

    for (size_t i = 0; i < facts.size(); i++) {
        if (facts[i].unlocked) {
            DrawRussianText(factTexts[i], 50, yPos, 20, WHITE);
            yPos += 40;
            factsShown++;
        }
    }

    if (factsShown == 0) {
        DrawRussianText(englishStrings[32],
            1024 / 2 - MeasureRussianText(englishStrings[32], 25) / 2,
            300, 25, YELLOW);
        DrawRussianText(englishStrings[33],
            1024 / 2 - MeasureRussianText(englishStrings[33], 20) / 2,
            350, 20, GRAY);
    }

    // Кнопка назад
    Rectangle backBtn = Rectangle{ 412, 650, 200, 50 };
    bool hover = CheckCollisionPointRec(GetMousePosition(), backBtn);

    DrawRectangleLinesEx(backBtn, 3, RED);
    if (hover) {
        DrawRectangleRec(backBtn, Color{ 255, 0, 0, 100 });
    } else {
        DrawRectangleRec(backBtn, Color{ 0, 0, 0, 150 });
    }

    int textWidth = MeasureRussianText(englishStrings[34], 25);
    DrawRussianText(englishStrings[34],
        backBtn.x + backBtn.width / 2 - textWidth / 2,
        backBtn.y + backBtn.height / 2 - 12,
        25, hover ? YELLOW : WHITE);
}

void Game::DrawGameOver() {
    DrawRectangle(0, 0, 1024, 768, Color{ 0, 0, 0, 200 });

    DrawRussianText(englishStrings[35], 1024 / 2 - MeasureRussianText(englishStrings[35], 50) / 2, 200, 50, RED);

    char scoreText[50];
    sprintf(scoreText, "%s%d", englishStrings[36], score);
    DrawRussianText(scoreText, 1024 / 2 - MeasureRussianText(scoreText, 40) / 2, 280, 40, YELLOW);

    if (score > highScore) {
        DrawRussianText(englishStrings[37], 1024 / 2 - MeasureRussianText(englishStrings[37], 35) / 2, 340, 35, GREEN);
    }

    DrawRussianText(englishStrings[38],
        1024 / 2 - MeasureRussianText(englishStrings[38], 25) / 2,
        450, 25, WHITE);
    DrawRussianText(englishStrings[39],
        1024 / 2 - MeasureRussianText(englishStrings[39], 25) / 2,
        500, 25, WHITE);
}

// ===== HELPER METHODS =====

void Game::StartGame() {
    ResetGame();
    currentState = GAME_COUNTDOWN;
}

void Game::ResetGame() {
    score = 0;
    gameTime = 0.0f;
    spawnTimer = 0.0f;
    countdownTimer = 3.0f;

    dino.Reset();

    for (auto& obstacle : obstacles) {
        obstacle.Reset();
    }
}

void Game::SpawnObstacle() {
    float obstacleSpeed = GetObstacleSpeed();

    for (auto& obstacle : obstacles) {
        if (!obstacle.active) {
            obstacle.Spawn(1100, 560, obstacleSpeed);
            break;
        }
    }
}

void Game::CheckCollisions() {
    for (const auto& obstacle : obstacles) {
        if (obstacle.active && CheckCollisionRecs(dino.collider, obstacle.collider)) {
            currentState = GAME_OVER;
            return;
        }
    }
}

void Game::UnlockRandomFact() {
    // Find locked facts
    std::vector<int> lockedFacts;
    for (size_t i = 0; i < facts.size(); i++) {
        if (!facts[i].unlocked) {
            lockedFacts.push_back((int)i);
        }
    }

    if (!lockedFacts.empty()) {
        int randomIndex = GetRandomValue(0, (int)lockedFacts.size() - 1);
        int factId = lockedFacts[randomIndex];
        facts[factId].unlocked = true;

        // Show notification
        showNotification = true;
        notificationTimer = 3.0f;

        std::cout << "Opened fact: " << factId + 1 << std::endl;
    }
    else {
        std::cout << "All facts already opened!" << std::endl;
    }
}
