/*
    student  : hafza naz
    roll no  : 25i-2086
    tictactoe.cpp click to place x or o, checks all rows/cols/diagonals
    win line flashes yellow using a timer, player names show on screen
*/
#include "TicTacToe.h"
bool TicTacToe::loadFont()
{
    return loadFontFromSystem(font, fontData);
}
// checks rows, columns, diagonals  stores winning line endpoints too
int TicTacToe::checkWinner()
{
    for (int i = 0; i < 3; i++)
    {
        if (board[i][0] && board[i][0] == board[i][1] && board[i][1] == board[i][2])
        {
            winR1 = i; winC1 = 0; winR2 = i; winC2 = 2; return board[i][0];
        }
        if (board[0][i] && board[0][i] == board[1][i] && board[1][i] == board[2][i])
        {
            winR1 = 0; winC1 = i; winR2 = 2; winC2 = i; return board[0][i];
        }
    }
    if (board[0][0] && board[0][0] == board[1][1] && board[1][1] == board[2][2])
    {
        winR1 = 0; winC1 = 0; winR2 = 2; winC2 = 2; return board[0][0];
    }
    if (board[0][2] && board[0][2] == board[1][1] && board[1][1] == board[2][0])
    {
        winR1 = 0; winC1 = 2; winR2 = 2; winC2 = 0; return board[0][2];
    }
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (!board[r][c]) return 0;
    return 3; // draw
}
void TicTacToe::drawCenteredText(sf::RenderWindow& window,
    const sf::String& str, int size, sf::Color color, float x, float y)
{
    if (!fontLoaded)
        return;
    sf::Text t(font);
    t.setString(str); t.setCharacterSize(size); t.setFillColor(color);
    sf::FloatRect b = t.getLocalBounds();
    t.setOrigin({ b.position.x + b.size.x / 2.f,
                  b.position.y + b.size.y / 2.f });
    t.setPosition({ x, y });
    window.draw(t);
}
TicTacToe::TicTacToe()
    : bgSprite(nullptr), currentPlayer(1), gameOver(false), winner(0),
    fontLoaded(false), soundPlayed(false), showWinLine(false),
    winR1(0), winC1(0), winR2(0), winC2(0)
{
    myStrCopy(p1Name, "Player 1");
    myStrCopy(p2Name, "Player 2");
    if (bgTexture.loadFromFile("bg_tictactoe.png"))
        bgSprite = new sf::Sprite(bgTexture);
    fontLoaded = loadFont();
    cellSize = 140.f;
    gridX = (WINDOW_WIDTH - cellSize * 3) / 2.f;
    gridY = 120.f;
    for (int i = 0; i < 2; i++)
    {
        hLines[i].setSize({ cellSize * 3, 4.f });
        hLines[i].setFillColor(sf::Color(0, 220, 220, 180));
        hLines[i].setPosition({ gridX, gridY + cellSize * (i + 1) });
        vLines[i].setSize({ 4.f, cellSize * 3 });
        vLines[i].setFillColor(sf::Color(0, 220, 220, 180));
        vLines[i].setPosition({ gridX + cellSize * (i + 1), gridY });
    }
    reset();
}
TicTacToe::~TicTacToe() { delete bgSprite; }
void TicTacToe::setPlayerNames(const char* name1, const char* name2)
{
    myStrCopy(p1Name, name1);
    myStrCopy(p2Name, name2);
}
void TicTacToe::reset()
{
    for (int r = 0; r < 3; r++) for (int c = 0; c < 3; c++) board[r][c] = 0;
    currentPlayer = 1; gameOver = false; winner = 0;
    soundPlayed = false; showWinLine = false;
}
bool        TicTacToe::isOver()              const { return gameOver; }
int         TicTacToe::getResult()           const { return winner; }
const char* TicTacToe::getPlayerName(int p)  const { return (p == 1) ? p1Name : p2Name; }
int TicTacToe::getScoreForPlayer(int p) const
{
    if (winner == p) return 100; // win
    if (winner == 3) return 50;  // draw
    return 0;
}
void TicTacToe::handleClick(sf::Vector2f mPos)
{
    if (gameOver) return;
    int col = (int)((mPos.x - gridX) / cellSize);
    int row = (int)((mPos.y - gridY) / cellSize);
    if (row < 0 || row > 2 || col < 0 || col > 2) return;
    if (board[row][col]) return;
    board[row][col] = currentPlayer;
    getSounds().playClick();
    winner = checkWinner();
    if (winner != 0)
    {
        gameOver = true;
        showWinLine = (winner != 3);
        winClock.restart();
        if (!soundPlayed)
        {
            if (winner == 3) getSounds().playGameOver();
            else             getSounds().playWin();
            soundPlayed = true;
        }
    }
    else currentPlayer = (currentPlayer == 1) ? 2 : 1;
}
void TicTacToe::handleKey(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Key::R) reset();
}
void TicTacToe::draw(sf::RenderWindow& window)
{
    if (bgSprite) window.draw(*bgSprite);

    sf::RectangleShape overlay({ (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT });
    overlay.setFillColor(sf::Color(0, 0, 0, 100));
    window.draw(overlay);

    drawCenteredText(window, "TIC TAC TOE", 40,
        sf::Color::Cyan, WINDOW_WIDTH / 2.f, 38.f);
    if (fontLoaded)
    {
        sf::Text p1(font); p1.setString(sf::String(p1Name) + " = X");
        p1.setCharacterSize(18); p1.setFillColor(sf::Color::Cyan);
        p1.setPosition({ 10.f, 10.f }); window.draw(p1);

        sf::Text p2(font); p2.setString(sf::String(p2Name) + " = O");
        p2.setCharacterSize(18); p2.setFillColor(sf::Color(255, 120, 120));
        p2.setPosition({ 10.f, 34.f }); window.draw(p2);
    }
    for (int i = 0; i < 2; i++) { window.draw(hLines[i]); window.draw(vLines[i]); }
    for (int r = 0; r < 3; r++) for (int c = 0; c < 3; c++)
    {
        if (!board[r][c]) continue;
        float cx = gridX + c * cellSize + cellSize / 2.f;
        float cy = gridY + r * cellSize + cellSize / 2.f;
        if (board[r][c] == 1) drawCenteredText(window, "X", 90, sf::Color::Cyan, cx, cy);
        else                  drawCenteredText(window, "O", 90, sf::Color(255, 120, 120), cx, cy);
    }
    // flashing win line  angle is exact so no atan2 or <cmath> needed
    if (showWinLine)
    {
        float el = winClock.getElapsedTime().asSeconds();
        float mod = el - (int)(el / 0.5f) * 0.5f;
        if (mod < 0.3f)
        {
            float x1 = gridX + winC1 * cellSize + cellSize / 2.f;
            float y1 = gridY + winR1 * cellSize + cellSize / 2.f;
            float x2 = gridX + winC2 * cellSize + cellSize / 2.f;
            float y2 = gridY + winR2 * cellSize + cellSize / 2.f;

            float angle = (winR1 == winR2) ? 0.f :
                (winC1 == winC2) ? 90.f :
                (winC2 > winC1) ? 45.f : 135.f;
            // line length via newton-raphson sqrt -- no <cmath>
            float dx = x2 - x1, dy = y2 - y1;
            float sq = dx * dx + dy * dy, len = sq / 2.f;
            for (int i = 0; i < 20; i++) len = (len + sq / len) / 2.f;
            sf::RectangleShape wl({ len, 7.f });
            wl.setFillColor(sf::Color(255, 240, 0, 230));
            wl.setOutlineThickness(1.f);
            wl.setOutlineColor(sf::Color(255, 200, 0, 180));
            wl.setOrigin({ 0.f, 3.5f });
            wl.setPosition({ x1, y1 });
            wl.setRotation(sf::degrees(angle));
            window.draw(wl);
        }
    }
    if (gameOver)
    {
        sf::String msg = (winner == 3) ? "It's a Draw!" :
            (winner == 1) ? sf::String(p1Name) + " Wins!" :
            sf::String(p2Name) + " Wins!";
        sf::RectangleShape pill({ 520.f, 50.f });
        pill.setFillColor(sf::Color(0, 0, 0, 170));
        pill.setOrigin({ 260.f, 25.f });
        pill.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 80.f });
        window.draw(pill);
        drawCenteredText(window, msg, 30, sf::Color::Yellow,
            WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 80.f);
        drawCenteredText(window, "R = Play Again   ESC = Menu", 20,
            sf::Color::Green, WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 42.f);
    }
    else
    {
        sf::String turn = (currentPlayer == 1)
            ? sf::String(p1Name) + "'s Turn  (X)"
            : sf::String(p2Name) + "'s Turn  (O)";
        sf::Color tc = (currentPlayer == 1) ? sf::Color::Cyan : sf::Color(255, 120, 120);
        drawCenteredText(window, turn, 24, tc, WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 48.f);
    }
    if (fontLoaded)
    {
        sf::Text back(font); back.setString("ESC = Menu");
        back.setCharacterSize(18); back.setFillColor(sf::Color(160, 160, 160));
        back.setPosition({ WINDOW_WIDTH - 130.f, WINDOW_HEIGHT - 28.f });
        window.draw(back);
    }
}