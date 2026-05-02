/*
    student  : hafza naz
    roll no  : 25i-2086
    course   : cs-1004 oop (se department)
    leaderboard.h stores, sorts, saves, loads, and displays scores
    also acts as filemanager as required by the rubric
*/
#pragma once
#ifndef LEADERBOARD_H
#define LEADERBOARD_H
#include <SFML/Graphics.hpp>
#include "common.h"
class Leaderboard
{
private:
    sf::Texture bgTexture;
    sf::Sprite* bgSprite;
    PlayerRecord records[MAX_RECORDS]; // fixed array 
    int          count;
    const char* fileName;
    sf::Font font;
    FontData fontData;
    bool     fontLoaded;
    // 0=all  1=tictactoe  2=snake  3=pong
    int filterMode;
    bool loadFont();
    // bubble sort highest first
    void sortRecords();
    void drawCentered(sf::RenderWindow& window, const sf::String& str,
        int size, sf::Color color, float x, float y);
public:
    explicit Leaderboard(const char* file);
    ~Leaderboard();
    // bubble sort highest first
    void         loadFromFile();
    void         saveToFile();
    void         addEntry(const char* name, const char* gameName, int score);
    void         cycleFilter();
    void         handleClick(sf::Vector2f mPos);
    int          getCount()       const;
    PlayerRecord getRecord(int i) const;
    void         draw(sf::RenderWindow& window);
};
#endif