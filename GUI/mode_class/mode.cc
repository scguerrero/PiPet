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

Mode::Mode(Player* player, QWidget *parent) : QWidget{parent}, player(player)
{
    // rest stays the same{
    // create menu for selecting a mode
    layout = new QVBoxLayout();
    menu = new QWidget();
    buttons = new QVBoxLayout();

    b_care = new QPushButton("Care");
    b_train = new QPushButton("Train");
    b_battle = new QPushButton("Battle");
    b_gear = new QPushButton("Gear");

    buttons->addWidget(b_care);
    buttons->addWidget(b_train);
    buttons->addWidget(b_battle);
    buttons->addWidget(b_gear);
    menu->setLayout(buttons);

    //mode_select must be initialized before mode_care uses it
    mode_select = new QStackedWidget();

    // create game mode pages - pass player into CareWidget
    mode_care = new Care(player, mode_select, this);
    mode_train = new Train();
    mode_feed = new Feed(player, mode_select);
    mode_affection = new Affection(player, mode_select);
    mode_groom = new Groom(player, mode_select);
    mode_sleep = new Sleep(player, mode_select);

    // when a mode button is clicked, open the widget for that corresponding mode
    connect(b_care, SIGNAL(clicked()), this, SLOT(openCare()));
    connect(b_train, SIGNAL(clicked()), this, SLOT(openTrain()));
    connect(mode_care, SIGNAL(feedRequested()), this, SLOT(openfeed()));
    connect(mode_care, SIGNAL(affectionRequested()), this, SLOT(openAffection()));
    connect(mode_care, SIGNAL(hygieneRequested()), this, SLOT(openGroom()));
    connect(mode_sleep, SIGNAL(sleepRequested()), this, SLOT(openSleep()));

    // back button returns to mode select screen
    back = new QPushButton("Back");
    connect(back, SIGNAL(clicked()), this, SLOT(goBack()));
    connect(mode_feed->backBtn, SIGNAL(clicked()), this, SLOT(closefeed()));
    connect(mode_affection->backBtn, SIGNAL(clicked()), this, SLOT(closeAffection()));
    connect(mode_groom->backBtn, SIGNAL(clicked()), this, SLOT(closeGroom()));
    connect(mode_sleep->backBtn, SIGNAL(clicked()), this, SLOT(closeSleep()));
    connect(b_battle, SIGNAL(clicked()), this, SIGNAL(battleRequested()));

    // pages of the stacked widget are as follows (indices 0 to 4): menu, Care, Train, Battle, Gear
    mode_select->addWidget(menu); // index 0
    mode_select->addWidget(mode_care); // index 1
    mode_select->addWidget(mode_train); // index 2
    mode_select->addWidget(mode_feed); // index 3
    mode_select->addWidget(mode_affection); // index 4
    mode_select->addWidget(mode_groom); // index 5
    mode_select->addWidget(mode_sleep); // index 6

    layout->addWidget(mode_select);
    layout->addWidget(back);
    this->setLayout(layout);
}

void Mode::openCare() {
    mode_care->updateStats(); // refresh stats every time Care is opened
    mode_select->setCurrentIndex(1);
}

void Mode::openTrain() {
    mode_select->setCurrentIndex(2);
}

void Mode::openfeed(){
    mode_feed->updateHungerDisplay(); // refresh before showing
    mode_select->setCurrentIndex(3); // index 3
}

void Mode::openAffection(){
    mode_affection->updateHappinessDisplay(); // refresh before showing
    mode_select->setCurrentIndex(4); // index 4
}

void Mode::openGroom() {
    mode_groom->updateHygieneDisplay(); // refresh before showing
    mode_select->setCurrentIndex(5);    // index 5
}

void Mode::openSleep(){
    mode_sleep->updateSleepDisplay(); // refresh before showing
    mode_select->setCurrentIndex(6);  // index 6
}

void Mode::closefeed(){
    mode_select->setCurrentIndex(1); // return to CareWidget
}

void Mode::closeGroom() {
    mode_select->setCurrentIndex(1);    // return to CareWidget
}

void Mode::closeAffection(){
    mode_select->setCurrentIndex(1); // return to CareWidget
}

void Mode::closeSleep() {
    mode_select->setCurrentIndex(1); // return to CareWidget
}

void Mode::goBack()
{
    mode_select->setCurrentIndex(0);
}