#ifndef LOCALE_H
#define LOCALE_H

#include <string>
#include <unordered_map>
#include "raylib.h"

enum Language {
    LANG_RUSSIAN,
    LANG_ENGLISH
};

class LocaleSystem {
private:
    std::unordered_map<std::string, std::string> russianStrings;
    std::unordered_map<std::string, std::string> englishStrings;
    Language currentLanguage;
    Font russianFont;
    
public:
    LocaleSystem();

    void SetLanguage(Language lang);
    Language GetCurrentLanguage() const;
    Font GetFont() const;

    // Получение строки по ключу
    const char* GetText(const std::string& key);

    // Рисование текста
    void DrawRussianText(const char* text, int posX, int posY, int fontSize, Color color);
    int MeasureRussianText(const char* text, int fontSize);
};

// Глобальный экземпляр
extern LocaleSystem locale;

// Макросы для удобства
#define L(key) locale.GetText(key)

#endif