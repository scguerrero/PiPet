/*
 * TrainWidget class implementation file.
 * In Train mode, the Player can play mini-games with their pet to increase the pet's attributes.
 * Author(s): Sasha C. Guerrero
 * Created: 2/23/2026
 * Last Edited: 3/16/2026
 */
#include "train.h"

Train::Train(QWidget *parent)
    : QWidget{parent}
{
    // Initialize widgets
    layout = new QVBoxLayout();
    b_minigame1 = new QPushButton("Minigame 1");
    b_minigame2 = new QPushButton("Minigame 2");
    b_minigame3 = new QPushButton("Minigame 3");
    b_back = new QPushButton("BACK");

    // Add buttons to grid
    layout->addWidget(b_minigame1);
    layout->addWidget(b_minigame2);
    layout->addWidget(b_minigame3);
    layout->addWidget(b_back);

    // Assign layout
    this->setLayout(layout);
}
