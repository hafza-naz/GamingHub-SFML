/*
    student  : hafza naz
    roll no  : 25i-2086
    mainmenu.h has graphical menu with 5 buttons and animated title
*/
#pragma once
#ifndef MAINMENU_H
#define MAINMENU_H
#include <SFML/Graphics.hpp>
#include "common.h"
class MainMenu
{
private:
    sf::Texture bgTexture;
    sf::Sprite* bgSprite;
    sf::Font font;
    FontData fontData;
    bool     fontLoaded;
    static const int BTN_COUNT = 5;
    sf::RectangleShape buttons[BTN_COUNT];
    // heap text for each button
    sf::Text* labels[BTN_COUNT];
    sf::Color normalColor;
    sf::Color hoverColor;
    sf::Color outlineNormal;
    sf::Color outlineHover;
    const char* names[BTN_COUNT];
    // for pulsing title
    sf::Clock animClock;
    float     titleScale;
    bool loadFont();
public:
    MainMenu();
    ~MainMenu();
    void update(sf::Vector2f mousePos);
    int  getClickedItem(sf::Vector2f mPos);
    void draw(sf::RenderWindow& window);
};
#endif