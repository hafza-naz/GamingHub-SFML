/*
    student  : hafza naz
    roll no  : 25i-2086
    snakegame.cpp HAS moves on timer, wall/self collision, grows on food
    speed increases every eat, food placed on random empty cell
*/
#include "SnakeGame.h"
#include <cstdlib>
#include <ctime>
bool SnakeGame::loadFont()
{
    return loadFontFromSystem(font, fontData);
}
// collects all empty cells then picks one at random
void SnakeGame::spawnFood()
{
    Pos empty[COLS * ROWS];
    int ec = 0;
    for (int x = 0; x < COLS; x++)
        for (int y = 0; y < ROWS; y++)
        {
            bool on = false;
            for (int i = 0; i < snakeLen; i++)
                if (snake[i].x == x && snake[i].y == y)
                {
                    on = true; break;
                }
            if (!on)
                empty[ec++] = { x, y };
        }
    if (ec > 0) food = empty[rand() % ec];
}
void SnakeGame::drawCentered(sf::RenderWindow& window,
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
SnakeGame::SnakeGame()
    : bgSprite(nullptr), snakeLen(0), dx(1), dy(0), nextDx(1), nextDy(0),
    score(0), gameOver(false), started(false), fontLoaded(false),
    moveTimer(0.f), moveInterval(0.15f), gameOverSoundPlayed(false)
{
    srand((unsigned int)time(nullptr));
    myStrCopy(playerName, "Player");

    if (bgTexture.loadFromFile("bg_snake.png"))
        bgSprite = new sf::Sprite(bgTexture);

    fontLoaded = loadFont();
    offsetX = (WINDOW_WIDTH - COLS * CELL) / 2.f;
    offsetY = (WINDOW_HEIGHT - ROWS * CELL) / 2.f + 10.f;
    food = { 0, 0 };
    reset();
}
SnakeGame::~SnakeGame() { delete bgSprite; }
void SnakeGame::setPlayerName(const char* name) { myStrCopy(playerName, name); }
int         SnakeGame::getScore()      const { return score; }
bool        SnakeGame::isOver()        const { return gameOver; }
const char* SnakeGame::getPlayerName() const { return playerName; }
void SnakeGame::reset()
{
    snakeLen = 3;
    snake[0] = { COLS / 2, ROWS / 2 };
    snake[1] = { COLS / 2 - 1, ROWS / 2 };
    snake[2] = { COLS / 2 - 2, ROWS / 2 };
    dx = 1; dy = 0; nextDx = 1; nextDy = 0;
    score = 0; gameOver = false; started = false;
    moveTimer = 0.f; moveInterval = 0.15f;
    gameOverSoundPlayed = false;
    spawnFood();
}
void SnakeGame::handleKey(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Key::R) { reset(); return; }
    if (gameOver) return;
    started = true;
    if (key == sf::Keyboard::Key::Up && dy == 0) { nextDx = 0; nextDy = -1; }
    if (key == sf::Keyboard::Key::Down && dy == 0) { nextDx = 0; nextDy = 1; }
    if (key == sf::Keyboard::Key::Left && dx == 0) { nextDx = -1; nextDy = 0; }
    if (key == sf::Keyboard::Key::Right && dx == 0) { nextDx = 1; nextDy = 0; }
}
void SnakeGame::update(float dt)
{
    if (gameOver)
    {
        if (!gameOverSoundPlayed) { getSounds().playGameOver(); gameOverSoundPlayed = true; }
        return;
    }
    if (!started) return;
    moveTimer += dt;
    if (moveTimer < moveInterval) return;
    moveTimer = 0.f;
    dx = nextDx; dy = nextDy;
    Pos nh = { snake[0].x + dx, snake[0].y + dy };
    // wall collision
    if (nh.x < 0 || nh.x >= COLS || nh.y < 0 || nh.y >= ROWS)
    {
        gameOver = true;
        return;
    }
    // self collision
    for (int i = 0; i < snakeLen; i++)
        if (snake[i].x == nh.x && snake[i].y == nh.y)
        {
            gameOver = true; return;
        }

    bool ate = (nh.x == food.x && nh.y == food.y);

    if (!ate)
        for (int i = snakeLen - 1; i > 0; i--) snake[i] = snake[i - 1];
    else
    {
        if (snakeLen < COLS* ROWS)
        {
            for (int i = snakeLen; i > 0; i--) snake[i] = snake[i - 1]; snakeLen++;
        }
        score += 10;
        if (moveInterval > 0.06f) moveInterval -= 0.002f;
        spawnFood();
        getSounds().playEat();
    }
    snake[0] = nh;
}
void SnakeGame::draw(sf::RenderWindow& window)
{
    if (bgSprite) window.draw(*bgSprite);
    sf::RectangleShape overlay({ (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT });
    overlay.setFillColor(sf::Color(0, 0, 0, 80));
    window.draw(overlay);
    sf::RectangleShape border({ (float)(COLS * CELL + 4), (float)(ROWS * CELL + 4) });
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(2.f);
    border.setOutlineColor(sf::Color(0, 200, 0));
    border.setPosition({ offsetX - 2.f, offsetY - 2.f });
    window.draw(border);
    sf::RectangleShape foodShape({ (float)CELL - 4, (float)CELL - 4 });
    foodShape.setFillColor(sf::Color(255, 50, 50));
    foodShape.setPosition({ offsetX + food.x * CELL + 2.f, offsetY + food.y * CELL + 2.f });
    window.draw(foodShape);
    // head alternates between two greens every 0.3s
    float t = headClock.getElapsedTime().asSeconds();
    float mod = t - (int)(t / 0.6f) * 0.6f;
    bool  bright = (mod < 0.3f);
    for (int i = 0; i < snakeLen; i++)
    {
        sf::RectangleShape seg({ (float)CELL - 2, (float)CELL - 2 });
        sf::Color c = (i == 0)
            ? (bright ? sf::Color(100, 255, 100) : sf::Color(0, 255, 80))
            : sf::Color(0, 190, 40);
        seg.setFillColor(c);
        seg.setPosition({ offsetX + snake[i].x * CELL + 1.f,
                          offsetY + snake[i].y * CELL + 1.f });
        window.draw(seg);
    }
    char sc[20]; myIntToStr(score, sc);
    if (fontLoaded)
    {
        sf::Text nameScore(font);
        nameScore.setString(sf::String(playerName) + "  |  Score: " + sc);
        nameScore.setCharacterSize(22);
        nameScore.setFillColor(sf::Color::White);
        sf::FloatRect nb = nameScore.getLocalBounds();
        nameScore.setOrigin({ nb.position.x + nb.size.x / 2.f,
                              nb.position.y + nb.size.y / 2.f });
        nameScore.setPosition({ WINDOW_WIDTH / 2.f, 18.f });
        window.draw(nameScore);
    }
    if (!started && !gameOver)
        drawCentered(window, "Press Arrow Keys to Start!", 24,
            sf::Color::Yellow, WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 42.f);
    if (gameOver)
    {
        sf::RectangleShape pill({ 480.f, 120.f });
        pill.setFillColor(sf::Color(0, 0, 0, 180));
        pill.setOrigin({ 240.f, 60.f });
        pill.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
        window.draw(pill);
        drawCentered(window, "GAME OVER!", 42, sf::Color::Red,
            WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 28.f);
        drawCentered(window, sf::String("Final Score: ") + sc, 26,
            sf::Color::Yellow, WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 15.f);
        drawCentered(window, "R = Restart   ESC = Menu", 20,
            sf::Color::Green, WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 50.f);
    }
    if (fontLoaded)
    {
        sf::Text back(font); back.setString("ESC = Menu");
        back.setCharacterSize(18); back.setFillColor(sf::Color(120, 180, 120));
        back.setPosition({ WINDOW_WIDTH - 130.f, WINDOW_HEIGHT - 26.f });
        window.draw(back);
    }
}