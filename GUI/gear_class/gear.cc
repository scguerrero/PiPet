/*
 * Gear mode is where the player can collect items for their PiPet.
 *
 * Author: Sasha C. Guerrero
 */

#include "gear.h"

Gear::Gear(QWidget *parent) : QWidget(parent) {
    // Vertical top-level layout
    layout = new QVBoxLayout();
    this->setLayout(layout);

    // Load logo image
    QImage *img_logo = new QImage(":/images/Assets/gear.png");
    QPixmap pxmap_logo = QPixmap::fromImage(img_logo->scaled(200,200,Qt::KeepAspectRatio));

    // Initialize logo widget
    logo = new QLabel();
    logo->setPixmap(pxmap_logo);
    logo->setAlignment(Qt::AlignCenter);
    layout->addWidget(logo);

    QStringList item_names = {"Cowboy", "Santa", "Crown", "Wizard"};
    QStringList item_paths = {":/images/Sprites/pets/icons/cowboy_hat.png",
        ":/images/Sprites/pets/icons/santa_hat.png",
        ":/images/Sprites/pets/icons/crown_hat.png",
        ":/images/Sprites/pets/icons/wizard_hat.png"};
    QStringList item_flavortexts = {"This PiPet ain't big enough for the two of us...",
        "Happy Holidays? In May? That can't be right.",
        "This PiPet has achieved monarch status. Bow!"
        "Where did this PiPet leave their grimoire and wand?"};

    // Test item
    item = new Item(item_names[0], item_paths[0], item_flavortexts[0]);
    layout->addWidget(item);

    // Back button
    b_back = new QPushButton("BACK");
    layout->addWidget(b_back);

    // Icons for buttons
    QIcon left_icon(":/images/Assets/left.png");
    b_back->setIcon(left_icon);
}