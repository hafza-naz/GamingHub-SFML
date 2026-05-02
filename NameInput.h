/*
    student  : hafza naz
    roll no  : 25i-2086
    nameinput.h has text entry screen shown before each game starts
    player types their name then presses enter
*/
#pragma once
#ifndef NAMEINPUT_H
#define NAMEINPUT_H
#include <SFML/Graphics.hpp>
#include "common.h"
class NameInput
{
private:
    // what the player has typed
    char buffer[NAME_LIMIT];
    int  len;
    // true when enter is pressed
    bool done;
    sf::Font font;
    FontData fontData;
    bool     fontLoaded;
    sf::Clock cursorClock;
    // flips every 0.5s for blink effect
    bool      cursorVisible;
    // question shown above the box
    char prompt[100];
    // game name shown at top of screen
    char gameTitle[60];
    bool loadFont();
public:
    NameInput();
    void        reset(const char* promptText, const char* title = "");
    bool        isDone()  const;
    const char* getName() const;
    void handleEvent(const sf::Event& event);
    void update();
    void draw(sf::RenderWindow& window);
};
#endif