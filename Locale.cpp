#include "Locale.h"
#include <iostream>
#include "raylib.h"

LocaleSystem locale;

LocaleSystem::LocaleSystem() : currentLanguage(LANG_RUSSIAN) {
    // Загрузка шрифта
    if (FileExists("fonts/arial.ttf")) {
        russianFont = LoadFont("fonts/arial.ttf");
    }
    else {
        russianFont = GetFontDefault();
        std::cout << "Используется стандартный шрифт" << std::endl;
    }

    // Инициализация русских строк
    russianStrings = {
        // Главное меню
        {"menu_title", "КОСМИЧЕСКИЙ ДИНОЗАВР"},
        {"menu_subtitle", "Физическое приключение"},
        {"menu_play", "ИГРАТЬ"},
        {"menu_inventory", "ИНВЕНТАРЬ"},
        {"menu_exit", "ВЫХОД"},
        {"menu_highscore", "Рекорд: "},

        // Выбор планеты
        {"planet_title", "ВЫБЕРИТЕ ПЛАНЕТУ"},
        {"planet_earth", "ЗЕМЛЯ (гравитация: 9.8)"},
        {"planet_mars", "МАРС (гравитация: 3.7)"},
        {"planet_jupiter", "ЮПИТЕР (гравитация: 24.8)"},
        {"planet_back", "НАЗАД"},
        {"planet_selected", "Планета: "},

        // Выбор сложности
        {"difficulty_title", "ВЫБЕРИТЕ СЛОЖНОСТЬ"},
        {"difficulty_easy", "ЛЕГКО"},
        {"difficulty_medium", "СРЕДНЕ"},
        {"difficulty_hard", "СЛОЖНО"},
        {"difficulty_start", "НАЧАТЬ ИГРУ"},

        // Игра
        {"game_score", "Очки: "},
        {"game_planet", "Планета: "},
        {"game_difficulty", "Сложность: "},
        {"game_controls_jump", "ПРОБЕЛ - прыжок"},
        {"game_controls_double", "Дважды ПРОБЕЛ - двойной прыжок"},
        {"game_controls_pause", "ESC - пауза"},
        {"game_new_fact", "НОВЫЙ ФИЗИЧЕСКИЙ ФАКТ ОТКРЫТ!"},
        {"game_check_inventory", "Загляните в инвентарь"},
        {"game_start", "СТАРТ!"},

        // Пауза
        {"pause_title", "ПАУЗА"},
        {"pause_resume", "ПРОДОЛЖИТЬ"},
        {"pause_inventory", "ИНВЕНТАРЬ"},
        {"pause_menu", "В МЕНЮ"},

        // Инвентарь
        {"inventory_title", "ИНВЕНТАРЬ - ФИЗИЧЕСКИЕ ФАКТЫ"},
        {"inventory_fact", "Факт "},
        {"inventory_empty", "Еще нет открытых фактов!"},
        {"inventory_hint", "Наберите 50 очков в игре"},
        {"inventory_back", "НАЗАД"},

        // Конец игры
        {"gameover_title", "ИГРА ОКОНЧЕНА!"},
        {"gameover_score", "Ваш счет: "},
        {"gameover_newrecord", "НОВЫЙ РЕКОРД!"},
        {"gameover_restart", "ENTER - новая игра"},
        {"gameover_menu", "ESC - в меню"},

        // Названия планет
        {"name_earth", "Земля"},
        {"name_mars", "Марс"},
        {"name_jupiter", "Юпитер"},

        // Названия сложностей
        {"name_easy", "Легко"},
        {"name_medium", "Средне"},
        {"name_hard", "Сложно"},

        // Факты
        {"fact1", "На Юпитере вы весили бы в 2.5 раза больше!"},
        {"fact2", "Сила притяжения на Марсе - 38% от земной!"},
        {"fact3", "Черные дыры обладают самой сильной гравитацией!"},
        {"fact4", "В невесомости пламя становится сферическим!"},
        {"fact5", "Время замедляется в сильном гравитационном поле!"},
        {"fact6", "На нейтронных звездах гравитация в миллиарды раз сильнее!"},
        {"fact7", "Луна удаляется от Земли на 3.8 см в год!"},
        {"fact8", "В космосе отсутствует сила Архимеда!"},
        {"fact9", "На Венере давление в 92 раза больше земного!"},
        {"fact10", "Солнце - 99.86% массы Солнечной системы!"}
    };

    // Английские строки
    englishStrings = {
        {"menu_title", "COSMIC DINOSAUR"},
        {"menu_play", "PLAY"},
        // ... можно добавить позже
    };
}

void LocaleSystem::SetLanguage(Language lang) {
    currentLanguage = lang;
}

Language LocaleSystem::GetCurrentLanguage() const {
    return currentLanguage;
}

Font LocaleSystem::GetFont() const {
    return russianFont;
}

const char* LocaleSystem::GetText(const std::string& key) {
    if (currentLanguage == LANG_RUSSIAN) {
        auto it = russianStrings.find(key);
        if (it != russianStrings.end()) {
            return it->second.c_str();
        }
    }
    else {
        auto it = englishStrings.find(key);
        if (it != englishStrings.end()) {
            return it->second.c_str();
        }
    }

    return key.c_str();
}

void LocaleSystem::DrawRussianText(const char* text, int posX, int posY, int fontSize, Color color) {
    DrawText(text, posX, posY, fontSize, color);
}

int LocaleSystem::MeasureRussianText(const char* text, int fontSize) {
    return MeasureText(text, fontSize);
}
