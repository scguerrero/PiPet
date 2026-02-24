/*
 * TrainWidget class implementation file.
 * In Train mode, the Player can play mini-games with their pet to increase
 * the pet's attributes.
 * Author(s): Sasha C. Guerrero
 * Created: 2/23/2026
 * Last Edited: 2/23/2026
 */
#include "TrainWidget.h"

TrainWidget::TrainWidget(QWidget *parent)
    : QWidget{parent}
{
    // initialize widgets
    layout = new QVBoxLayout();
    minigame1 = new QPushButton("Minigame 1");
    minigame2 = new QPushButton("Minigame 2");
    minigame3 = new QPushButton("Minigame 3");

    // add buttons to grid
    layout->addWidget(minigame1);
    layout->addWidget(minigame2);
    layout->addWidget(minigame3);

    // assign layout
    this->setLayout(layout);
}
