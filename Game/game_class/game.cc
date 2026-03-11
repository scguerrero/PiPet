/*
 * Game class implementation file.
 * Author(s): Sasha C. Guerrero
 * Created: 2/9/2026
 * Last Edited: 3/7/2026 
 */
#include "game.h"

Game::Game(QWidget *parent) : QWidget{parent} {
	layout = new QVBoxLayout();
	pages = new QStackedWidget();
	start = new Start();
	create = new Create();
	//mode = new Mode();
	//care = new Care();
	//train = new Train();
	//battle = new Battle();
	//gear = new Gear();
	b_quit = new QPushButton("QUIT");

	this->setWindowTitle("PIPET"); // The title of the non-fullscreened application window
	this->setContentsMargins(30,30,30,30); // 30px margins
	
	this->setLayout(layout); // Vertically-arrange widgets inside Game
	layout->addWidget(pages); // Children of layout: pages and quit button
	layout->addWidget(b_quit); // For testing purposes, we want the ability to quit any time
	
	pages->addWidget(start);// Children of pages: Start, Create, Mode, Care, Train, Battle, Gear
	pages->addWidget(create);
	//pages->addWidget(mode);
	//pages->addWidget(care);
	//pages->addWidget(train);
	//pages->addWidget(battle);
	//pages->addWidget(gear);
	
	connect(b_quit, SIGNAL( clicked() ), QApplication::instance(), SLOT( quit() )); // Terminate running program
	if (new_game) { // For a new game, the first step is to create a pet
		connect(start->b_start, SIGNAL( clicked() ), this, SLOT( open_create() ));
	}
}

void Game::open_create() {
	pages->setCurrentIndex(1);
}
