/*
    student  : hafza naz
    roll no  : 25i-2086
    ponggame.h pvp pong, p1=w/s p2=up/down, first to 5 goals wins
    ball leaves fading trail behind it as animation
*/
#pragma once
#ifndef PONGGAME_H
#define PONGGAME_H
#include <SFML/Graphics.hpp>
#include "common.h"
class PongGame
{
private:
    sf::Texture bgTexture;
    sf::Sprite* bgSprite;
    sf::RectangleShape paddle1, paddle2;
    float p1Y, p2Y, paddleSpeed;
    static const int PADDLE_W = 14;
    static const int PADDLE_H = 85;
    sf::CircleShape ball;
    float ballX, ballY, ballDx, ballDy, ballSpeed;
    // fading ghost circles behind ball
    static const int TRAIL_LEN = 6;
    float trailX[TRAIL_LEN];
    float trailY[TRAIL_LEN];
    int score1, score2;
    static const int WIN_SCORE = 5;
    bool gameOver, started, winSoundPlayed;
    char p1Name[NAME_LIMIT];
    char p2Name[NAME_LIMIT];
    sf::Font font;
    FontData fontData;
    bool     fontLoaded;
    bool loadFont();
    void resetBall();
    void drawCentered(sf::RenderWindow& window, const sf::String& str,
        int size, sf::Color color, float x, float y);
public:
    PongGame();
    ~PongGame();
    void setPlayerNames(const char* name1, const char* name2);
    void reset();
    void handleKey(sf::Keyboard::Key key);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    bool        isOver()              const;
    int         getWinner()           const;
    int         getScore1()           const;
    int         getScore2()           const;
    int         getFinalScore(int p)  const;
    const char* getPlayerName(int p)  const;
};
#endif