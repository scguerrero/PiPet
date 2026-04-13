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
    QPixmap pxmap_logo = QPixmap::fromImage(img_logo->scaled(275,275,Qt::KeepAspectRatio));

    // Initialize logo widget
    logo = new QLabel();
    logo->setPixmap(pxmap_logo);
    logo->setAlignment(Qt::AlignCenter);
    layout->addWidget(logo);

    // Back button
    b_back = new QPushButton("BACK");
    layout->addWidget(b_back);

    // Icons for buttons
    QIcon left_icon(":/images/Assets/left.png");
    b_back->setIcon(left_icon);
}