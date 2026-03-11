/*
 * Start class implementation file. This is the first "page" players will see.
 * Author(s): Sasha C. Guerrero
 * Created: 3/6/2026
 * Last Edited: 3/6/2026
 */
#include "start.h"

Start::Start(QWidget *parent) : QWidget{parent} {
	layout = new QVBoxLayout(); // Verticaly-arrange widgets
	title = new QLabel("PIPET"); // Title text
	b_start = new QPushButton("START"); // Either go to "Create" page or "Mode" page

	this->setLayout(layout); // Assign layout to Start
	layout->addWidget(title); // Assign layout's children
	layout->addWidget(b_start);

	title->setAlignment(Qt::AlignHCenter); // Center-align title
	title->setStyleSheet("font-size: 36px"); // Large font for title
}
