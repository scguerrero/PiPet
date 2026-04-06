/*
 * Start class implementation file. This is the first "page" players will see.
 * Author(s): Sasha C. Guerrero
 * Created: 3/6/2026
 * Last Edited: 3/6/2026
 */
#include "start.h"

Start::Start(QWidget *parent) : QWidget{parent} {
	layout = new QVBoxLayout(); // Verticaly-arrange widgets
    b_start = new QPushButton("START"); // Either go to "Create" page or "Mode" page

    title = new QLabel(); // Title label
    QImage *img = new QImage(":/images/Assets/logo.png");
    QPixmap pxmap = QPixmap::fromImage(*img);
    title->setPixmap(pxmap.scaled(400, 400, Qt::KeepAspectRatio));

    this->setLayout(layout); // Start has vertical layout
    layout->addWidget(title); // Add title logo
    layout->addWidget(b_start); // Add start button

    b_start->setStyleSheet(R"(
        QPushButton { background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #4850DB, stop: 1 #4A71DB);
        border: 2px inset #FBA8FF;
        border-radius: 10px;
        padding: 4px;
        font: bold; }
        QPushButton:pressed {
        background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #4A71DB, stop: 1 #4850DB);
        }
    )");
}
