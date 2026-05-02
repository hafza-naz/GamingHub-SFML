/*
    student  : hafza naz
    roll no  : 25i-2086
    mainhub.h has central controller, owns all game objects on heap
    flow: click game -> ask p1 name -> ask p2 name -> launch -> auto-save on end
*/

#pragma once
#ifndef MAINHUB_H
#define MAINHUB_H
#include <SFML/Graphics.hpp>
#include "common.h"
#include "MainMenu.h"
#include "TicTacToe.h"
#include "SnakeGame.h"
#include "PongGame.h"
#include "Leaderboard.h"
#include "NameInput.h"
class MainHub
{
private:
    sf::RenderWindow window;
    GameState        currentState;
    SelectedGame     selectedGame;
    MainMenu* menu;
    TicTacToe* ttt;
    SnakeGame* snake;
    PongGame* pong;
    Leaderboard* leaderboard;
    NameInput* nameInput;
    sf::Clock clock;
    bool      scoreSaved;
    // holds p1 name while waiting for p2
    char p1NameBuffer[NAME_LIMIT];
    // called after both names collected
    void launchSelectedGame();
    // auto-saves when game ends
    void saveScores();
    void handleEvents();
    void update(float dt);
    void render();
public:
    MainHub();
    ~MainHub();
    void run();
};
#endif