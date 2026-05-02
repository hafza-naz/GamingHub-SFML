/*
    student  : Hafza Naz
    roll no  : 25I-2086
    common.h has shared constants, structs, helpers, sound manager
    everything used across all classes lives here
*/
#pragma once
#ifndef COMMON_H
#define COMMON_H
#include <cstdio>
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int MAX_RECORDS = 10;
const int NAME_LIMIT = 50;
// which screen is active
enum GameState
{
    STATE_MAIN_MENU,
    STATE_ENTER_P1_NAME,
    STATE_ENTER_P2_NAME,
    STATE_TICTACTOE,
    STATE_SNAKE,
    STATE_PONG,
    STATE_LEADERBOARD,
    STATE_EXIT
};
// which game button was clicked
enum SelectedGame
{
    GAME_NONE,
    GAME_TICTACTOE,
    GAME_SNAKE,
    GAME_PONG
};
// one row in the leaderboard file
struct PlayerRecord
{
    char name[NAME_LIMIT];
    char gameName[NAME_LIMIT];
    int  score;
};
// string helpers  no <cstring> allowed
inline int myStrLen(const char* str)
{
    int l = 0;
    while (str[l]) l++;
    return l;
}
inline void myStrCopy(char* dest, const char* src)
{
    int i = 0;
    while (src[i]) { dest[i] = src[i]; i++; }
    dest[i] = '\0';
}
inline int myStrCmp(const char* s1, const char* s2)
{
    int i = 0;
    while (s1[i] && s2[i])
    {
        if (s1[i] != s2[i]) return s1[i] - s2[i];
        i++;
    }
    return s1[i] - s2[i];
}
// converts int to char array  no sprintf allowed
inline void myIntToStr(int n, char* str)
{
    if (n == 0) { str[0] = '0'; str[1] = '\0'; return; }
    bool neg = (n < 0);
    if (neg) n = -n;
    int i = 0;
    while (n > 0) { str[i++] = (char)((n % 10) + '0'); n /= 10; }
    if (neg) str[i++] = '-';
    str[i] = '\0';
    for (int j = 0; j < i / 2; j++)
    {
        char t = str[j]; str[j] = str[i - j - 1]; str[i - j - 1] = t;
    }
}
// loads a font file into a heap buffer 
struct FontData
{
    char* buf;
    int   size;

    FontData() : buf(nullptr), size(0) {}
    ~FontData() { freeMem(); }

    void freeMem() { delete[] buf; buf = nullptr; size = 0; }

    bool load(const char* path)
    {
        FILE* f = nullptr;
#ifdef _WIN32
        fopen_s(&f, path, "rb");
#else
        f = fopen(path, "rb");
#endif
        if (!f) return false;
        fseek(f, 0, SEEK_END);
        long sz = ftell(f);
        rewind(f);
        if (sz <= 0) { fclose(f); return false; }
        freeMem();
        buf = new char[(int)sz];
        size = (int)sz;
        fread(buf, 1, sz, f);
        fclose(f);
        return true;
    }
};
// sound effects pointer used because sfml3 removed default sf::Sound ctor
#include <SFML/Audio.hpp>
class SoundManager
{
private:
    sf::SoundBuffer bufClick, bufEat, bufBounce, bufGameOver, bufWin;
    sf::Sound* sndClick = nullptr;
    sf::Sound* sndEat = nullptr;
    sf::Sound* sndBounce = nullptr;
    sf::Sound* sndGameOver = nullptr;
    sf::Sound* sndWin = nullptr;
    void tryLoad(sf::SoundBuffer& b, sf::Sound*& s, const char* p)
    {
        if (b.loadFromFile(p)) s = new sf::Sound(b);
    }
public:
    SoundManager()
    {
        tryLoad(bufClick, sndClick, "sounds/click.wav");
        tryLoad(bufEat, sndEat, "sounds/eat.wav");
        tryLoad(bufBounce, sndBounce, "sounds/bounce.wav");
        tryLoad(bufGameOver, sndGameOver, "sounds/gameover.wav");
        tryLoad(bufWin, sndWin, "sounds/win.wav");
    }
    ~SoundManager()
    {
        delete sndClick; delete sndEat; delete sndBounce;
        delete sndGameOver; delete sndWin;
    }
    void playClick() { if (sndClick)    sndClick->play(); }
    void playEat() { if (sndEat)      sndEat->play(); }
    void playBounce() { if (sndBounce)   sndBounce->play(); }
    void playGameOver() { if (sndGameOver) sndGameOver->play(); }
    void playWin() { if (sndWin)      sndWin->play(); }
};
// global singleton  one instance shared by all classes
inline SoundManager& getSounds()
{
    static SoundManager sm;
    return sm;
}
// shared font loader called by every class, avoids repeating 6 times
#include <SFML/Graphics.hpp>
inline bool loadFontFromSystem(sf::Font& font, FontData& fontData)
{
    if (fontData.load("C:\\Windows\\Fonts\\arial.ttf"))
        return font.openFromMemory(fontData.buf, fontData.size);
    if (fontData.load("C:\\Windows\\Fonts\\DejaVuSans.ttf"))
        return font.openFromMemory(fontData.buf, fontData.size);
    return false;
}
#endif