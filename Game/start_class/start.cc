/*
 * Start class implementation file.
 * Author(s): Sasha C. Guerrero
 * Created: 3/3/2026
 * Last Edited: 3/3/2026
 */
#include "start.h"

Start::Start(QWidget *parent)
    : QWidget{parent}
{
	// Instantiate widgets
	box = new QVBoxLayout();
	title = new QLabel("PIPET");
	b_start = new QPushButton("START");
	b_quit = new QPushButton("QUIT");

	// Arrange widgets
	box->addWidget(title);
	box->addWidget(b_start);
	box->addWidget(b_quit);
	this->setLayout(box);
    // when quit button is clicked, quit application
    connect(b_quit, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
    // when start button is clicked, add create widget to start_layout
    //connect(start, SIGNAL(clicked()), this, SLOT(openCreateWidget()) );
	
}

