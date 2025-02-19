# ВИДЕО ИГРЫ: Геннадия Горина
переписанные с его видео на C++ и SDL

### В наличии
- УГАДАЙ ТОПОР
- УГАДАЙ БАБУШКУ
- остальные будут позже

### Сборка
Убедись в наличии у себя libSDL2_image и libSDL2_ttf
```cpp
g++ -Iinclude src/main.cpp -L/usr/lib -lSDL2 -lSDL2_image -lSDL2_ttf
```