/*
 * Game class implementation file.
 * Author(s): Sasha C. Guerrero
 * Created: 2/9/2026
 * Last Edited: 3/3/2026
 */
#include "game.h"

Game::Game(QWidget *parent)
    : QWidget{parent}
{
	// Instantiate widgets
	layout = new QVBoxLayout();
	pages = new QStackedWidget();
	page_start = new Start(); // starting page
	
	// Arrange widgets
	layout->addWidget(pages);
	pages->addWidget(page_start);
/*
    // when quit button is clicked, quit application
    connect(quit, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
    // when start button is clicked, add create widget to start_layout
    connect(start, SIGNAL(clicked()), this, SLOT(openCreateWidget()) );
	*/
	
	// Game widget has these properties
    this->setLayout(layout);
	this->setWindowTitle("PIPET");
    this->setContentsMargins(50,50,50,50);
}

