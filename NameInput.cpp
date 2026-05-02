/*
    student  : hafza naz
    roll no  : 25i-2086
    nameinput.cpp  draws overlay with game title, prompt, input box, blinking cursor
*/
#include "NameInput.h"
bool NameInput::loadFont()
{
    return loadFontFromSystem(font, fontData); // shared helper in common.h
}
NameInput::NameInput()
    : len(0), done(false), fontLoaded(false), cursorVisible(true)
{
    buffer[0] = '\0';
    prompt[0] = '\0';
    gameTitle[0] = '\0';
    fontLoaded = loadFont();
}
void NameInput::reset(const char* promptText, const char* title)
{
    buffer[0] = '\0';
    len = 0;
    done = false;
    myStrCopy(prompt, promptText);
    myStrCopy(gameTitle, title);
    cursorClock.restart();
    cursorVisible = true;
}
bool        NameInput::isDone()  const { return done; }
const char* NameInput::getName() const { return buffer; }
void NameInput::handleEvent(const sf::Event& event)
{
    if (done) return;
    if (const auto* te = event.getIf<sf::Event::TextEntered>())
    {
        uint32_t c = te->unicode;
        // backspace
        if (c == 8 && len > 0)
        {
            len--; buffer[len] = '\0';
        }
        // enter
        else if (c == 13 && len > 0)
        {
            done = true;
        }
        else if (c >= 32 && c < 127 && len < NAME_LIMIT - 1)
        {
            buffer[len++] = (char)c; buffer[len] = '\0';
        }
    }
}
void NameInput::update()
{
    // blink cursor every 0.5 seconds
    if (cursorClock.getElapsedTime().asSeconds() > 0.5f)
    {
        cursorVisible = !cursorVisible;
        cursorClock.restart();
    }
}
void NameInput::draw(sf::RenderWindow& window)
{
    // dark overlay
    sf::RectangleShape overlay({ (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT });
    overlay.setFillColor(sf::Color(0, 0, 0, 220));
    window.draw(overlay);
    if (!fontLoaded) return;
    // game title at top
    if (gameTitle[0] != '\0')
    {
        sf::Text title(font);
        title.setString(gameTitle);
        title.setCharacterSize(42);
        title.setFillColor(sf::Color::Cyan);
        title.setStyle(sf::Text::Bold);
        sf::FloatRect tb = title.getLocalBounds();
        title.setOrigin({ tb.position.x + tb.size.x / 2.f,
                          tb.position.y + tb.size.y / 2.f });
        title.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 140.f });
        window.draw(title);
        sf::RectangleShape line({ 300.f, 2.f });
        line.setFillColor(sf::Color::Cyan);
        line.setOrigin({ 150.f, 1.f });
        line.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 105.f });
        window.draw(line);
    }
    // prompt text
    sf::Text promptTxt(font);
    promptTxt.setString(prompt);
    promptTxt.setCharacterSize(26);
    promptTxt.setFillColor(sf::Color(180, 255, 180));
    sf::FloatRect pb = promptTxt.getLocalBounds();
    promptTxt.setOrigin({ pb.position.x + pb.size.x / 2.f,
                          pb.position.y + pb.size.y / 2.f });
    promptTxt.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 55.f });
    window.draw(promptTxt);
    // input box
    sf::RectangleShape box({ 420.f, 54.f });
    box.setFillColor(sf::Color(15, 15, 50));
    box.setOutlineThickness(2.f);
    box.setOutlineColor(sf::Color::Cyan);
    box.setOrigin({ 210.f, 27.f });
    box.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 10.f });
    window.draw(box);
    // typed text with blinking cursor
    char display[NAME_LIMIT + 2];
    myStrCopy(display, buffer);
    if (cursorVisible) { display[len] = '|'; display[len + 1] = '\0'; }
    sf::Text inputTxt(font);
    inputTxt.setString(display);
    inputTxt.setCharacterSize(28);
    inputTxt.setFillColor(sf::Color::White);
    sf::FloatRect ib = inputTxt.getLocalBounds();
    inputTxt.setOrigin({ ib.position.x + ib.size.x / 2.f,
                         ib.position.y + ib.size.y / 2.f });
    inputTxt.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 10.f });
    window.draw(inputTxt);
    // hint below box
    sf::Text hint(font);
    hint.setString("Press ENTER to confirm");
    hint.setCharacterSize(18);
    hint.setFillColor(sf::Color(140, 140, 140));
    sf::FloatRect hb = hint.getLocalBounds();
    hint.setOrigin({ hb.position.x + hb.size.x / 2.f,
                     hb.position.y + hb.size.y / 2.f });
    hint.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 58.f });
    window.draw(hint);
}