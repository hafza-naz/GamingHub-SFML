/*
    student  : hafza naz
    roll no  : 25i-2086
    leaderboard.cpp  file i/o, bubble sort, graphical display with filter
*/
#include "Leaderboard.h"
#include <fstream>
bool Leaderboard::loadFont()
{
    return loadFontFromSystem(font, fontData);
}
// bubble sort  swaps neighbours until highest score is at index 0
void Leaderboard::sortRecords()
{
    for (int i = 0; i < count - 1; i++)
        for (int j = 0; j < count - i - 1; j++)
            if (records[j].score < records[j + 1].score)
            {
                PlayerRecord temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
}
void Leaderboard::drawCentered(sf::RenderWindow& window,
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
Leaderboard::Leaderboard(const char* file)
    : bgSprite(nullptr), count(0), fileName(file),
    fontLoaded(false), filterMode(0)
{
    if (bgTexture.loadFromFile("bg_leaderboard.png"))
        bgSprite = new sf::Sprite(bgTexture);
    fontLoaded = loadFont();
    // load existing scores, no crash if missing
    loadFromFile();
}
Leaderboard::~Leaderboard()
{
    delete bgSprite;
}
// reads scores.txt  returns silently if file doesnt exist yet
void Leaderboard::loadFromFile()
{
    std::ifstream in(fileName);
    if (!in) return;
    count = 0;
    while (count < MAX_RECORDS &&
        in >> records[count].name
        >> records[count].gameName
        >> records[count].score)
        count++;
    in.close();
    sortRecords();
}
void Leaderboard::saveToFile()
{
    std::ofstream out(fileName);
    if (!out) return;
    for (int i = 0; i < count; i++)
        out << records[i].name << " "
        << records[i].gameName << " "
        << records[i].score << "\n";
    out.close();
}
// adds entry, replaces lowest if board is full
void Leaderboard::addEntry(const char* name, const char* gameName, int score)
{
    if (count < MAX_RECORDS)
    {
        myStrCopy(records[count].name, name);
        myStrCopy(records[count].gameName, gameName);
        records[count].score = score;
        count++;
    }
    else if (score > records[MAX_RECORDS - 1].score)
    {
        myStrCopy(records[MAX_RECORDS - 1].name, name);
        myStrCopy(records[MAX_RECORDS - 1].gameName, gameName);
        records[MAX_RECORDS - 1].score = score;
    }
    sortRecords();
    saveToFile();
}
void Leaderboard::cycleFilter() { filterMode = (filterMode + 1) % 4; }
void Leaderboard::handleClick(sf::Vector2f mPos)
{
    sf::FloatRect btn({ WINDOW_WIDTH - 200.f, WINDOW_HEIGHT - 45.f }, { 180.f, 34.f });
    if (btn.contains(mPos)) { cycleFilter(); getSounds().playClick(); }
}
int          Leaderboard::getCount()       const { return count; }
PlayerRecord Leaderboard::getRecord(int i) const { return records[i]; }
void Leaderboard::draw(sf::RenderWindow& window)
{
    if (bgSprite) window.draw(*bgSprite);
    sf::RectangleShape overlay({ (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT });
    overlay.setFillColor(sf::Color(0, 0, 0, 140));
    window.draw(overlay);
    if (!fontLoaded)
        return;
    drawCentered(window, "LEADERBOARD", 52,
        sf::Color::Yellow, WINDOW_WIDTH / 2.f, 48.f);
    sf::RectangleShape titleLine({ (float)WINDOW_WIDTH - 80.f, 3.f });
    titleLine.setFillColor(sf::Color::Yellow);
    titleLine.setPosition({ 40.f, 88.f });
    window.draw(titleLine);
    // active filter label
    const char* filterLabels[4] = { "All Games", "TicTacToe", "Snake", "Pong" };
    sf::Color   filterColors[4] = {
        sf::Color::White, sf::Color::Cyan,
        sf::Color(0, 255, 80), sf::Color(255, 120, 120)
    };
    {
        sf::Text fl(font);
        fl.setString(sf::String("Filter: ") + filterLabels[filterMode]);
        fl.setCharacterSize(18);
        fl.setFillColor(filterColors[filterMode]);
        fl.setPosition({ 40.f, 94.f });
        window.draw(fl);
    }
    // column headers
    auto hdr = [&](const char* s, float x)
        {
            sf::Text h(font); h.setString(s); h.setCharacterSize(20);
            h.setFillColor(sf::Color(200, 200, 200)); h.setPosition({ x, 110.f });
            window.draw(h);
        };
    hdr("#", 60.f); hdr("PLAYER", 120.f); hdr("GAME", 370.f); hdr("SCORE", 610.f);
    sf::RectangleShape headerLine({ (float)WINDOW_WIDTH - 80.f, 1.f });
    headerLine.setFillColor(sf::Color(80, 80, 80));
    headerLine.setPosition({ 40.f, 136.f });
    window.draw(headerLine);
    // rows
    int shown = 0;
    for (int i = 0; i < count; i++)
    {
        bool match = true;
        if (filterMode == 1) match = (myStrCmp(records[i].gameName, "TicTacToe") == 0);
        if (filterMode == 2) match = (myStrCmp(records[i].gameName, "Snake") == 0);
        if (filterMode == 3) match = (myStrCmp(records[i].gameName, "Pong") == 0);
        if (!match) continue;
        float y = 144.f + shown * 42.f;
        if (y > WINDOW_HEIGHT - 70.f) break;
        // gold/silver/bronze highlight for top 3
        if (shown < 3)
        {
            sf::RectangleShape rowBg({ (float)WINDOW_WIDTH - 80.f, 36.f });
            rowBg.setPosition({ 40.f, y - 3.f });
            sf::Color rc = (shown == 0) ? sf::Color(70, 55, 0, 130) :
                (shown == 1) ? sf::Color(55, 55, 55, 130) :
                sf::Color(50, 30, 0, 130);
            rowBg.setFillColor(rc);
            window.draw(rowBg);
        }
        sf::Color rankColor = (shown == 0) ? sf::Color::Yellow :
            (shown == 1) ? sf::Color(210, 210, 210) :
            (shown == 2) ? sf::Color(210, 140, 60) :
            sf::Color::White;
        char rankStr[5], scoreStr[20];
        myIntToStr(shown + 1, rankStr);
        myIntToStr(records[i].score, scoreStr);
        auto cell = [&](const sf::String& s, int sz, sf::Color col, float x)
            {
                sf::Text t(font); t.setString(s); t.setCharacterSize(sz);
                t.setFillColor(col); t.setPosition({ x, y }); window.draw(t);
            };
        cell(rankStr, 22, rankColor, 60.f);
        cell(records[i].name, 22, rankColor, 120.f);
        cell(records[i].gameName, 22, sf::Color(130, 210, 255), 370.f);
        cell(scoreStr, 22, sf::Color::Green, 610.f);
        shown++;
    }
    if (shown == 0)
        drawCentered(window, "No records yet - play a game!",
            26, sf::Color(160, 160, 160),
            WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
    // filter button
    sf::RectangleShape filterBtn({ 180.f, 34.f });
    filterBtn.setFillColor(sf::Color(30, 30, 80));
    filterBtn.setOutlineThickness(1.f);
    filterBtn.setOutlineColor(sf::Color::Cyan);
    filterBtn.setPosition({ WINDOW_WIDTH - 200.f, WINDOW_HEIGHT - 45.f });
    window.draw(filterBtn);
    drawCentered(window, "Click to Filter", 16, sf::Color::Cyan,
        WINDOW_WIDTH - 110.f, WINDOW_HEIGHT - 28.f);
    sf::Text back(font);
    back.setString("ESC = Back to Menu");
    back.setCharacterSize(20);
    back.setFillColor(sf::Color(160, 160, 160));
    back.setPosition({ 20.f, WINDOW_HEIGHT - 30.f });
    window.draw(back);
}