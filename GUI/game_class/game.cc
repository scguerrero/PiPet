/*
 * Game class implementation file.
 * Author(s): Sasha C. Guerrero
 * Created: 2/9/2026
 * Last Edited: 3/14/2026
 */
#include "game.h"

Game::Game(QWidget *parent) : QWidget{parent} {
    layout = new QVBoxLayout();
    pages = new QStackedWidget();
    start = new Start();
    create = new Create();

    // Create a Player first, then pass it into Mode
    piPet* pet = new piPet();
    player = new Player(pet);
    mode = new Mode(player);  // FIX: pass player into Mode

    battle = new Battle();


    b_quit = new QPushButton("QUIT");
    this->setWindowTitle("PIPET");
    this->setContentsMargins(30, 30, 30, 30);

    this->setLayout(layout);
    layout->addWidget(pages);
    layout->addWidget(b_quit);

    // FIX: Only add Start, Create, Mode, Battle as top-level pages
    // Care/Train/Affection/Groom are handled INSIDE Mode's own QStackedWidget
    pages->addWidget(start);   // index 0
    pages->addWidget(create);  // index 1
    pages->addWidget(mode);    // index 2
    pages->addWidget(battle);  // index 3

    connect(b_quit, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));

    if (new_game) {
        connect(start->b_start, SIGNAL(clicked()), this, SLOT(open_create()));
    } else {
        connect(start->b_start, SIGNAL(clicked()), this, SLOT(open_mode()));
    }

    connect(create->b_done, SIGNAL(clicked()), this, SLOT(open_mode()));

    // FIX: Remove all mode->b_care / mode->b_train connects — Mode handles those internally
    // Only wire Battle since it's a top-level page outside Mode
    connect(mode, SIGNAL(battleRequested()), this, SLOT(open_battle()));
}

void Game::open_create() {
    pages->setCurrentIndex(1);
}
void Game::open_mode() {
    pages->setCurrentIndex(2);
}
void Game::open_battle() {
    pages->setCurrentIndex(3);
}