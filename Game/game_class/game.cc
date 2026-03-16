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
    mode = new Mode();
    care = new Care();
    train = new Train();
	//battle = new Battle();
	//gear = new Gear();
	b_quit = new QPushButton("QUIT");

	this->setWindowTitle("PIPET"); // The title of the non-fullscreened application window
	this->setContentsMargins(30,30,30,30); // 30px margins
	
	this->setLayout(layout); // Vertically-arrange widgets inside Game
	layout->addWidget(pages); // Children of layout: pages and quit button
	layout->addWidget(b_quit); // For testing purposes, we want the ability to quit any time
	
	pages->addWidget(start);// Children of pages: Start, Create, Mode, Care, Train, Battle, Gear
    pages->addWidget(create); // index 1
    pages->addWidget(mode); // index 2
    pages->addWidget(care); // 3
    pages->addWidget(train); // 4
    //pages->addWidget(battle); // 5
    //pages->addWidget(gear); // 6
	
	connect(b_quit, SIGNAL( clicked() ), QApplication::instance(), SLOT( quit() )); // Terminate running program

    if (new_game) { // For a new game, the first step is to create a pet
		connect(start->b_start, SIGNAL( clicked() ), this, SLOT( open_create() ));
    } else { // When loading a previous game, send them to the mode-selection page
        connect(start->b_start, SIGNAL( clicked() ), this, SLOT( open_mode() ));
    }

    connect(create->b_done, SIGNAL( clicked() ), this, SLOT( open_mode() )); // Open Mode from Create

    connect(mode->b_care, SIGNAL( clicked() ), this, SLOT( open_care() )); // from Mode, open Care widget

    connect(care->b_back, SIGNAL( clicked() ), this, SLOT( open_mode() )); // from Care, go back to Mode widget

    connect(mode->b_train, SIGNAL( clicked() ), this, SLOT( open_train() )); // from Mode, go to Train

    connect(train->b_back, SIGNAL( clicked() ), this, SLOT( open_mode() )); // from Train, go back to Mode
}

void Game::open_create() {
	pages->setCurrentIndex(1);
}

void Game::open_mode() {
    pages->setCurrentIndex(2);
}

void Game::open_care() {
    pages->setCurrentIndex(3);
}

void Game::open_train() {
    pages->setCurrentIndex(4);
}
