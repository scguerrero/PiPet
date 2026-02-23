/*
 * ModeWidget class implementation file.
 * We create a button menu to open each game mode.
 * Author(s): Sasha C. Guerrero
 * Created: 2/19/2026
 * Last Edited: 2/19/2026
 */

#include "ModeWidget.h"

ModeWidget::ModeWidget(QWidget *parent)
    : QWidget{parent}
{
    // create menu for selecting a mode
    layout = new QVBoxLayout();
    menu = new QWidget();
    buttons = new QVBoxLayout();
    care = new QPushButton("Care");
    train = new QPushButton("Train");
    battle = new QPushButton("Battle");
    gear = new QPushButton("Gear");
    buttons->addWidget(care);
    buttons->addWidget(train);
    buttons->addWidget(battle);
    buttons->addWidget(gear);
    menu->setLayout(buttons);

    // create game mode pages
    mode_care = new CareWidget();

    // when a mode button is clicked, open the widget for that corresponding mode
    connect(care, SIGNAL(clicked()), this, SLOT(openCareWidget()));

    // pages of the stacked widget are as follows (indices 0 to 4): menu, Care, Train, Battle, Gear
    mode_select = new QStackedWidget();
    mode_select->addWidget(menu);
    mode_select->addWidget(mode_care);
    layout->addWidget(mode_select);
    this->setLayout(layout);
}

void ModeWidget::openCareWidget() {
    mode_select->setCurrentIndex(1);
}
