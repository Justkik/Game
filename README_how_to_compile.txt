HOW TO COMPILE AND RUN SANYA GAME:

1. REQUIREMENTS:
   - MinGW (g++) or Visual Studio C++ compiler
   - Raylib library installed

2. COMPILE COMMAND:
   g++ main.cpp Game.cpp Locale.cpp -o game.exe -lraylib

3. OR USE build_and_run.bat:
   Just double-click build_and_run.bat

4. FILES NEEDED:
   - main.cpp    (game entry point)
   - Game.cpp    (main game logic)
   - Game.h      (header)
   - Locale.cpp  (localization)
   - Locale.h    (locale header)