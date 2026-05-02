/*
    student  : hafza naz
    roll no  : 25i-2086
    mainhub.cpp  main game loop, state machine, name entry flow,
    leaderboard auto-save using names entered before game started
*/
#include "MainHub.h"
MainHub::MainHub()
    : window(sf::VideoMode({ (unsigned int)WINDOW_WIDTH,
                              (unsigned int)WINDOW_HEIGHT }),
        "NUCES Gaming Hub"),
    currentState(STATE_MAIN_MENU),
    selectedGame(GAME_NONE),
    scoreSaved(false)
{
    window.setFramerateLimit(60);
    getSounds();
    p1NameBuffer[0] = '\0';
    // all objects on heap  freed in destructor
    menu = new MainMenu();
    ttt = new TicTacToe();
    snake = new SnakeGame();
    pong = new PongGame();
    leaderboard = new Leaderboard("scores.txt");
    nameInput = new NameInput();
}
MainHub::~MainHub()
{
    delete menu; delete ttt; delete snake;
    delete pong; delete leaderboard; delete nameInput;
}
void MainHub::run()
{
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        // cap to avoid big jumps
        if (dt > 0.05f) dt = 0.05f;
        handleEvents();
        update(dt);
        render();
    }
}
// resets the game and switches state after both names are collected
void MainHub::launchSelectedGame()
{
    scoreSaved = false;
    if (selectedGame == GAME_TICTACTOE)
    {
        ttt->reset();   currentState = STATE_TICTACTOE;
    }
    if (selectedGame == GAME_SNAKE)
    {
        snake->reset(); currentState = STATE_SNAKE;
    }
    if (selectedGame == GAME_PONG)
    {
        pong->reset();  currentState = STATE_PONG;
    }
}
// called once when game ends  uses names stored in each game object
void MainHub::saveScores()
{
    if (scoreSaved) return;
    scoreSaved = true;
    if (selectedGame == GAME_SNAKE)
    {
        leaderboard->addEntry(snake->getPlayerName(), "Snake", snake->getScore());
    }
    else if (selectedGame == GAME_PONG)
    {
        int w = pong->getWinner();
        if (w == 1) leaderboard->addEntry(pong->getPlayerName(1), "Pong", pong->getScore1());
        if (w == 2) leaderboard->addEntry(pong->getPlayerName(2), "Pong", pong->getScore2());
    }
    else if (selectedGame == GAME_TICTACTOE)
    {
        int res = ttt->getResult();
        if (res == 1)
            leaderboard->addEntry(ttt->getPlayerName(1), "TicTacToe", ttt->getScoreForPlayer(1));
        else if (res == 2)
            leaderboard->addEntry(ttt->getPlayerName(2), "TicTacToe", ttt->getScoreForPlayer(2));
        else if (res == 3) // draw -- save both
        {
            leaderboard->addEntry(ttt->getPlayerName(1), "TicTacToe", 50);
            leaderboard->addEntry(ttt->getPlayerName(2), "TicTacToe", 50);
        }
    }
}
void MainHub::handleEvents()
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>()) window.close();
        // name entry screens get all events
        if (currentState == STATE_ENTER_P1_NAME ||
            currentState == STATE_ENTER_P2_NAME)
        {
            nameInput->handleEvent(*event);
            continue;
        }
        if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>())
        {
            sf::Vector2f mPos = window.mapPixelToCoords(mb->position);

            if (currentState == STATE_MAIN_MENU)
            {
                int choice = menu->getClickedItem(mPos);
                // tictactoe
                if (choice == 0)
                {
                    selectedGame = GAME_TICTACTOE;
                    nameInput->reset("Player 1, enter your name:", "TIC TAC TOE");
                    currentState = STATE_ENTER_P1_NAME;
                }
                // snake only needs one name
                if (choice == 1)
                {
                    selectedGame = GAME_SNAKE;
                    nameInput->reset("Enter your name:", "SNAKE GAME");
                    currentState = STATE_ENTER_P1_NAME;
                }
                if (choice == 2) // pong
                {
                    selectedGame = GAME_PONG;
                    nameInput->reset("Player 1, enter your name:", "PONG GAME");
                    currentState = STATE_ENTER_P1_NAME;
                }
                if (choice == 3)
                {
                    currentState = STATE_LEADERBOARD; leaderboard->loadFromFile();
                }
                if (choice == 4)
                    window.close();
            }
            if (currentState == STATE_TICTACTOE)  ttt->handleClick(mPos);
            if (currentState == STATE_LEADERBOARD) leaderboard->handleClick(mPos);
        }

        if (const auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            if (key->code == sf::Keyboard::Key::Escape)
                currentState = STATE_MAIN_MENU;
            if (currentState == STATE_TICTACTOE)
                ttt->handleKey(key->code);
            if (currentState == STATE_SNAKE)
                snake->handleKey(key->code);
            if (currentState == STATE_PONG)
                pong->handleKey(key->code);
        }
    }
}
void MainHub::update(float dt)
{
    // p1 name entry
    if (currentState == STATE_ENTER_P1_NAME)
    {
        nameInput->update();
        if (nameInput->isDone())
        {
            myStrCopy(p1NameBuffer, nameInput->getName());
            // solo game, launch now
            if (selectedGame == GAME_SNAKE)
            {
                snake->setPlayerName(p1NameBuffer);
                launchSelectedGame();
            }
            else if (selectedGame == GAME_TICTACTOE)
            {
                nameInput->reset("Player 2, enter your name:", "TIC TAC TOE");
                currentState = STATE_ENTER_P2_NAME;
            }
            else if (selectedGame == GAME_PONG)
            {
                nameInput->reset("Player 2, enter your name:", "PONG GAME");
                currentState = STATE_ENTER_P2_NAME;
            }
        }
        return;
    }
    // p2 name entry
    if (currentState == STATE_ENTER_P2_NAME)
    {
        nameInput->update();
        if (nameInput->isDone())
        {
            const char* p2 = nameInput->getName();
            if (selectedGame == GAME_TICTACTOE) ttt->setPlayerNames(p1NameBuffer, p2);
            if (selectedGame == GAME_PONG)      pong->setPlayerNames(p1NameBuffer, p2);
            launchSelectedGame();
        }
        return;
    }
    if (currentState == STATE_MAIN_MENU)
    {
        sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        menu->update(mPos);
    }
    if (currentState == STATE_SNAKE)
    {
        snake->update(dt); if (snake->isOver()) saveScores();
    }
    if (currentState == STATE_PONG)
    {
        pong->update(dt); if (pong->isOver()) saveScores();
    }
    if (currentState == STATE_TICTACTOE)
    {
        if (ttt->isOver()) saveScores();
    }
}
void MainHub::render()
{
    window.clear(sf::Color(15, 15, 30));
    if (currentState == STATE_MAIN_MENU)
        menu->draw(window);
    if (currentState == STATE_TICTACTOE)
        ttt->draw(window);
    if (currentState == STATE_SNAKE)
        snake->draw(window);
    if (currentState == STATE_PONG)
        pong->draw(window);
    if (currentState == STATE_LEADERBOARD)
        leaderboard->draw(window);
    if (currentState == STATE_ENTER_P1_NAME ||
        currentState == STATE_ENTER_P2_NAME)
        nameInput->draw(window);
    window.display();
}