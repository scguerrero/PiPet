/*
 * ModeWidget class implementation file.
 * We create a button menu to open each game mode.
 * Author(s): Sasha C. Guerrero
 * Created: 2/19/2026
 * Last Edited: 2/19/2026
 */

#include "ModeWidget.h"

ModeWidget::ModeWidget(Player * player, QWidget *parent)
    : QWidget{parent}, player(player)
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

    //mode_select must be initialized before mode_care uses it
    mode_select = new QStackedWidget();

    // create game mode pages - pass player into CareWidget
    mode_care = new CareWidget(player, mode_select);
    mode_train = new TrainWidget();
    mode_affection = new AffectionWidget(player, mode_select);
    mode_groom = new GroomWidget(player, mode_select);

    // when a mode button is clicked, open the widget for that corresponding mode
    connect(care, SIGNAL(clicked()), this, SLOT(openCareWidget()));
    connect(train, SIGNAL(clicked()), this, SLOT(openTrainWidget()));
    connect(mode_care, SIGNAL(affectionRequested()), this, SLOT(openAffectionWidget()));
    connect(mode_care, SIGNAL(hygieneRequested()), this, SLOT(openGroomWidget()));

    // back button returns to mode select screen
    back = new QPushButton("Back");
    connect(back, SIGNAL(clicked()), this, SLOT(goBack()));
    connect(mode_affection->backBtn, SIGNAL(clicked()), this, SLOT(closeAffectionWidget()));
    connect(mode_groom->backBtn, SIGNAL(clicked()), this, SLOT(closeGroomWidget()));

    // pages of the stacked widget are as follows (indices 0 to 4): menu, Care, Train, Battle, Gear
    mode_select->addWidget(menu); // index 0
    mode_select->addWidget(mode_care); // index 1
    mode_select->addWidget(mode_train); // index 2
    mode_select->addWidget(mode_affection); // index 3
    mode_select->addWidget(mode_groom); // index 4

    layout->addWidget(mode_select);
    layout->addWidget(back);
    this->setLayout(layout);
}

void ModeWidget::openCareWidget() {
    mode_care->updateStats(); // refresh stats every time Care is opened
    mode_select->setCurrentIndex(1);
}

void ModeWidget::openTrainWidget() {
    mode_select->setCurrentIndex(2);
}

void ModeWidget::openAffectionWidget(){
    mode_affection->updateHappinessDisplay(); // refresh before showing
    mode_select->setCurrentIndex(3);
}

void ModeWidget::openGroomWidget() {
    mode_groom->updateHygieneDisplay(); // refresh before showing
    mode_select->setCurrentIndex(4);    // index 4
}
void ModeWidget::closeGroomWidget() {
    mode_select->setCurrentIndex(1);    // return to CareWidget
}

void ModeWidget::closeAffectionWidget(){
    mode_select->setCurrentIndex(1); // return to CareWidget
}

void ModeWidget::goBack()
{
    mode_select->setCurrentIndex(0);
}
