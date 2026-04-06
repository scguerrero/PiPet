/*
 * Mode class implementation file.
 * From here the Player can select which game mode they want to play:
 * Care mode for feeding and grooming the pet, Train mode for choosing a mini-game,
 * Battle mode for playing single-player games versus a bot opponent, and
 * Gear mode for choosing equipment/accessories for the pet.
 * Author(s): Sasha C. Guerrero
 * Created: 2/19/2026
 * Last Edited: 3/14/2026
 */
#include "mode.h"

Mode::Mode(QWidget *parent) : QWidget{parent} {
    layout = new QVBoxLayout(); // Vertically-arrange widgets inside Mode
    this->setLayout(layout);

    b_care = new QPushButton("🧽 Care"); // Buttons leading to corresponding widget
    b_train = new QPushButton("🥎 Train");
    b_battle = new QPushButton("🥊 Battle");
    b_gear = new QPushButton("🛡️ Gear");

    layout->addWidget(b_care); // Add buttons to layout
    layout->addWidget(b_train);
    layout->addWidget(b_battle);
    layout->addWidget(b_gear);
}
