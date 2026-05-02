/*
    student  : hafza naz
    roll no  : 25i-2086
    ponggame.cpp HAS ball bounces off walls and paddles, angle changes based
    on where it hits the paddle, trail animation behind the ball
*/
#include "PongGame.h"
bool PongGame::loadFont()
{
    return loadFontFromSystem(font, fontData);
}
void PongGame::resetBall()
{
    for (int i = 0; i < TRAIL_LEN; i++)
    {
        trailX[i] = WINDOW_WIDTH / 2.f; trailY[i] = WINDOW_HEIGHT / 2.f;
    }
    ballX = WINDOW_WIDTH / 2.f;
    ballY = WINDOW_HEIGHT / 2.f;
    // alternate direction
    ballDx = (score1 + score2) % 2 == 0 ? 1.f : -1.f;
    ballDy = 0.7f;
    ballSpeed = 280.f;
}
void PongGame::drawCentered(sf::RenderWindow& window,
    const sf::String& str, int size, sf::Color color, float x, float y)
{
    if (!fontLoaded) return;
    sf::Text t(font);
    t.setString(str); t.setCharacterSize(size); t.setFillColor(color);
    sf::FloatRect b = t.getLocalBounds();
    t.setOrigin({ b.position.x + b.size.x / 2.f,
                  b.position.y + b.size.y / 2.f });
    t.setPosition({ x, y });
    window.draw(t);
}
PongGame::PongGame()
    : bgSprite(nullptr),
    p1Y(WINDOW_HEIGHT / 2.f - PADDLE_H / 2.f),
    p2Y(WINDOW_HEIGHT / 2.f - PADDLE_H / 2.f),
    paddleSpeed(350.f),
    ballX(0.f), ballY(0.f), ballDx(1.f), ballDy(0.7f), ballSpeed(280.f),
    score1(0), score2(0),
    gameOver(false), started(false), winSoundPlayed(false),
    fontLoaded(false)
{
    myStrCopy(p1Name, "Player 1");
    myStrCopy(p2Name, "Player 2");
    if (bgTexture.loadFromFile("bg_pong.png"))
        bgSprite = new sf::Sprite(bgTexture);
    fontLoaded = loadFont();
    paddle1.setSize({ (float)PADDLE_W, (float)PADDLE_H });
    paddle1.setFillColor(sf::Color::Cyan);
    paddle2.setSize({ (float)PADDLE_W, (float)PADDLE_H });
    paddle2.setFillColor(sf::Color(255, 120, 120));
    ball.setRadius(9.f);
    ball.setFillColor(sf::Color::White);
    ball.setOrigin({ 9.f, 9.f });
    for (int i = 0; i < TRAIL_LEN; i++)
    {
        trailX[i] = WINDOW_WIDTH / 2.f; trailY[i] = WINDOW_HEIGHT / 2.f;
    }
    resetBall();
}
PongGame::~PongGame()
{
    delete bgSprite;
}
void PongGame::setPlayerNames(const char* name1, const char* name2)
{
    myStrCopy(p1Name, name1);
    myStrCopy(p2Name, name2);
}
bool        PongGame::isOver()             const { return gameOver; }
int         PongGame::getScore1()          const { return score1; }
int         PongGame::getScore2()          const { return score2; }
int         PongGame::getWinner()          const { return (score1 >= WIN_SCORE) ? 1 : (score2 >= WIN_SCORE) ? 2 : 0; }
int         PongGame::getFinalScore(int p) const { return (p == 1) ? score1 : score2; }
const char* PongGame::getPlayerName(int p) const { return (p == 1) ? p1Name : p2Name; }
void PongGame::reset()
{
    score1 = 0; score2 = 0;
    p1Y = WINDOW_HEIGHT / 2.f - PADDLE_H / 2.f;
    p2Y = WINDOW_HEIGHT / 2.f - PADDLE_H / 2.f;
    gameOver = false; started = false; winSoundPlayed = false;
    resetBall();
}
void PongGame::handleKey(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Key::R) reset();
}
void PongGame::update(float dt)
{
    if (gameOver)
    {
        if (!winSoundPlayed) { getSounds().playWin(); winSoundPlayed = true; }
        return;
    }
    // paddle movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && p1Y > 0)
        p1Y -= paddleSpeed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && p1Y + PADDLE_H < WINDOW_HEIGHT)
        p1Y += paddleSpeed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && p2Y > 0)
        p2Y -= paddleSpeed * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && p2Y + PADDLE_H < WINDOW_HEIGHT)
        p2Y += paddleSpeed * dt;
    // game starts on first paddle move
    if (!started)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            started = true;
        return;
    }
    // shift trail before moving ball
    for (int i = TRAIL_LEN - 1; i > 0; i--)
    {
        trailX[i] = trailX[i - 1]; trailY[i] = trailY[i - 1];
    }
    trailX[0] = ballX; trailY[0] = ballY;
    ballX += ballDx * ballSpeed * dt;
    ballY += ballDy * ballSpeed * dt;
    // top/bottom wall bounce
    if (ballY <= 0) { ballY = 0;             ballDy = -ballDy; getSounds().playBounce(); }
    if (ballY >= WINDOW_HEIGHT) { ballY = WINDOW_HEIGHT; ballDy = -ballDy; getSounds().playBounce(); }
    // paddle 1 hit -- angle based on where on paddle it lands
    if (ballX - 9 <= 30 + PADDLE_W && ballY >= p1Y && ballY <= p1Y + PADDLE_H && ballDx < 0)
    {
        ballDx = -ballDx;
        ballDy = ((ballY - (p1Y + PADDLE_H / 2.f)) / (PADDLE_H / 2.f)) * 1.2f;
        ballSpeed += 10.f;
        getSounds().playBounce();
    }
    // paddle 2 hit
    if (ballX + 9 >= WINDOW_WIDTH - 30 - PADDLE_W && ballY >= p2Y && ballY <= p2Y + PADDLE_H && ballDx > 0)
    {
        ballDx = -ballDx;
        ballDy = ((ballY - (p2Y + PADDLE_H / 2.f)) / (PADDLE_H / 2.f)) * 1.2f;
        ballSpeed += 10.f;
        getSounds().playBounce();
    }
    // scoring
    if (ballX < 0)
    {
        score2++; getSounds().playClick(); started = false; resetBall(); if (score2 >= WIN_SCORE) gameOver = true;
    }
    if (ballX > WINDOW_WIDTH)
    {
        score1++; getSounds().playClick(); started = false; resetBall(); if (score1 >= WIN_SCORE) gameOver = true;
    }
}
void PongGame::draw(sf::RenderWindow& window)
{
    if (bgSprite) window.draw(*bgSprite);

    sf::RectangleShape overlay({ (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT });
    overlay.setFillColor(sf::Color(0, 0, 0, 60));
    window.draw(overlay);
    // fading ball trail -- older = smaller and more transparent
    for (int i = TRAIL_LEN - 1; i >= 0; i--)
    {
        float alpha = (float)(TRAIL_LEN - i) / TRAIL_LEN * 120.f;
        float radius = 9.f * (float)(TRAIL_LEN - i) / TRAIL_LEN;
        sf::CircleShape ghost(radius);
        ghost.setFillColor(sf::Color(200, 200, 255, (unsigned char)alpha));
        ghost.setOrigin({ radius, radius });
        ghost.setPosition({ trailX[i], trailY[i] });
        window.draw(ghost);
    }
    // player names above scores
    if (fontLoaded)
    {
        sf::Text n1(font); n1.setString(p1Name);
        n1.setCharacterSize(16); n1.setFillColor(sf::Color::Cyan);
        sf::FloatRect b1 = n1.getLocalBounds();
        n1.setOrigin({ b1.position.x + b1.size.x / 2.f, b1.position.y + b1.size.y / 2.f });
        n1.setPosition({ WINDOW_WIDTH / 2.f - 90.f, 5.f });
        window.draw(n1);
        sf::Text n2(font); n2.setString(p2Name);
        n2.setCharacterSize(16); n2.setFillColor(sf::Color(255, 120, 120));
        sf::FloatRect b2 = n2.getLocalBounds();
        n2.setOrigin({ b2.position.x + b2.size.x / 2.f, b2.position.y + b2.size.y / 2.f });
        n2.setPosition({ WINDOW_WIDTH / 2.f + 90.f, 5.f });
        window.draw(n2);
    }
    char s1[10], s2[10];
    myIntToStr(score1, s1); myIntToStr(score2, s2);
    drawCentered(window, s1, 52, sf::Color::Cyan, WINDOW_WIDTH / 2.f - 90.f, 28.f);
    drawCentered(window, s2, 52, sf::Color(255, 120, 120), WINDOW_WIDTH / 2.f + 90.f, 28.f);
    drawCentered(window, "P1: W / S", 18, sf::Color(100, 220, 220), 70.f, WINDOW_HEIGHT - 22.f);
    drawCentered(window, "P2: UP / DOWN", 18, sf::Color(220, 110, 110), WINDOW_WIDTH - 90.f, WINDOW_HEIGHT - 22.f);
    paddle1.setPosition({ 30.f, p1Y });
    paddle2.setPosition({ WINDOW_WIDTH - 30.f - PADDLE_W, p2Y });
    window.draw(paddle1); window.draw(paddle2);
    ball.setPosition({ ballX, ballY });
    window.draw(ball);
    if (!started && !gameOver)
        drawCentered(window, "Move a paddle to start!", 22,
            sf::Color::Yellow, WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
    if (gameOver)
    {
        sf::RectangleShape pill({ 480.f, 100.f });
        pill.setFillColor(sf::Color(0, 0, 0, 180));
        pill.setOrigin({ 240.f, 50.f });
        pill.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
        window.draw(pill);

        sf::String msg = (score1 >= WIN_SCORE)
            ? sf::String(p1Name) + " Wins!"
            : sf::String(p2Name) + " Wins!";
        drawCentered(window, msg, 36, sf::Color::Yellow,
            WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 22.f);
        drawCentered(window, "R = Restart   ESC = Menu", 22,
            sf::Color::Green, WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 28.f);
    }
}