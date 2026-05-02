/*
    student  : hafza naz
    roll no  : 25i-2086
    mainmenu.cpp 5 centered buttons, hover scale effect, pulsing title
    sine approximated without <cmath> using piecewise linear formula
*/
#include "MainMenu.h"
bool MainMenu::loadFont()
{
    return loadFontFromSystem(font, fontData);
}
MainMenu::MainMenu()
    : bgSprite(nullptr), fontLoaded(false), titleScale(1.f),
    normalColor(20, 20, 50, 200), hoverColor(60, 60, 130, 230),
    outlineNormal(80, 80, 180), outlineHover(150, 150, 255)
{
    names[0] = "TIC TAC TOE";
    names[1] = "SNAKE GAME";
    names[2] = "PONG GAME";
    names[3] = "LEADERBOARD";
    names[4] = "EXIT";

    for (int i = 0; i < BTN_COUNT; i++)
        labels[i] = nullptr;
    if (bgTexture.loadFromFile("bg_menu.png"))
        bgSprite = new sf::Sprite(bgTexture);
    fontLoaded = loadFont();
    for (int i = 0; i < BTN_COUNT; i++)
    {
        buttons[i].setSize({ 320.f, 58.f });
        buttons[i].setFillColor(normalColor);
        buttons[i].setOutlineThickness(2.f);
        buttons[i].setOutlineColor(outlineNormal);
        buttons[i].setOrigin({ 160.f, 29.f });
        buttons[i].setPosition({ WINDOW_WIDTH / 2.f, 195.f + i * 75.f });
        if (!fontLoaded)
            continue;
        labels[i] = new sf::Text(font);
        labels[i]->setString(names[i]);
        labels[i]->setCharacterSize(24);
        labels[i]->setFillColor(sf::Color::White);
        sf::FloatRect b = labels[i]->getLocalBounds();
        labels[i]->setOrigin({ b.position.x + b.size.x / 2.f,
                               b.position.y + b.size.y / 2.f });
        labels[i]->setPosition(buttons[i].getPosition());
    }
}
MainMenu::~MainMenu()
{
    delete bgSprite;
    for (int i = 0; i < BTN_COUNT; i++) delete labels[i];
}
void MainMenu::update(sf::Vector2f mousePos)
{
    // hover effect
    for (int i = 0; i < BTN_COUNT; i++)
    {
        if (buttons[i].getGlobalBounds().contains(mousePos))
        {
            buttons[i].setFillColor(hoverColor);
            buttons[i].setOutlineColor(outlineHover);
            buttons[i].setScale({ 1.05f, 1.05f });
            if (labels[i]) labels[i]->setScale({ 1.05f, 1.05f });
        }
        else
        {
            buttons[i].setFillColor(normalColor);
            buttons[i].setOutlineColor(outlineNormal);
            buttons[i].setScale({ 1.f, 1.f });
            if (labels[i]) labels[i]->setScale({ 1.f, 1.f });
        }
    }
    // piecewise sine approximation for title pulse 
    float t = animClock.getElapsedTime().asSeconds();
    float mod = t - (int)(t / 6.2832f) * 6.2832f;
    float s;
    if (mod < 1.5708f) s = mod / 1.5708f;
    else if (mod < 3.1416f) s = 1.f - (mod - 1.5708f) / 1.5708f;
    else if (mod < 4.7124f) s = -(mod - 3.1416f) / 1.5708f;
    else                    s = -1.f + (mod - 4.7124f) / 1.5708f;
    titleScale = 1.f + s * 0.03f;
}
int MainMenu::getClickedItem(sf::Vector2f mPos)
{
    for (int i = 0; i < BTN_COUNT; i++)
        if (buttons[i].getGlobalBounds().contains(mPos))
        {
            getSounds().playClick(); return i;
        }
    return -1;
}
void MainMenu::draw(sf::RenderWindow& window)
{
    if (bgSprite)
        window.draw(*bgSprite);
    sf::RectangleShape banner({ (float)WINDOW_WIDTH, 155.f });
    banner.setFillColor(sf::Color(0, 0, 0, 140));
    window.draw(banner);
    if (fontLoaded)
    {
        sf::Text title(font);
        title.setString("NUCES GAMING HUB");
        title.setCharacterSize(50);
        title.setFillColor(sf::Color::Cyan);
        title.setStyle(sf::Text::Bold);
        sf::FloatRect tb = title.getLocalBounds();
        title.setOrigin({ tb.position.x + tb.size.x / 2.f,
                          tb.position.y + tb.size.y / 2.f });
        title.setPosition({ WINDOW_WIDTH / 2.f, 55.f });
        // pulse animation
        title.setScale({ titleScale, titleScale });
        window.draw(title);
        sf::Text sub(font);
        sub.setString("Select a Game to Play");
        sub.setCharacterSize(20);
        sub.setFillColor(sf::Color(180, 180, 220));
        sf::FloatRect sb = sub.getLocalBounds();
        sub.setOrigin({ sb.position.x + sb.size.x / 2.f,
                        sb.position.y + sb.size.y / 2.f });
        sub.setPosition({ WINDOW_WIDTH / 2.f, 110.f });
        window.draw(sub);
    }
    sf::RectangleShape divLine({ 420.f, 2.f });
    divLine.setFillColor(sf::Color(80, 80, 200));
    divLine.setOrigin({ 210.f, 1.f });
    divLine.setPosition({ WINDOW_WIDTH / 2.f, 138.f });
    window.draw(divLine);
    for (int i = 0; i < BTN_COUNT; i++)
    {
        window.draw(buttons[i]);
        if (labels[i]) window.draw(*labels[i]);
    }
}