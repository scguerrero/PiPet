/*
 * NameWidget class implementation file.
 * This page of the game is for the Player to choose a name for their piPet from a given bank of names.
 * Author(s): Sasha C. Guerrero
 * Created: 2/16/2026
 * Last Edited: 2/18/2026
 */
#include "NameWidget.h"

NameWidget::NameWidget(QWidget *parent)
    : QWidget{parent}
{
    title = new QLabel("Choose a name for your pet:");
    done = new QPushButton("Done", this);
    menu = new QVBoxLayout();

    connect(done, SIGNAL(clicked()), this, SLOT(close()));
    // set widget attribute deleteonclosed
    // setup file IO for pet's name, species, palette

    menu->addWidget(title);
    menu->addWidget(done);
    this->setLayout(menu);
}
