/*
 * care.cc - Care hub.
 * Back buttons now emit requestReturnToMode() so game.cc
 * navigates straight back to the Mode screen.
 * Author(s): Sasha C. Guerrero, Tanya Magurupira
 */
#include "care.h"
#include "../../Player/Player.h"

Care::Care(Player* player, Character::PetType petType, QWidget *parent)
    : QWidget(parent), player(player), petType(petType)
{
    top_layout = new QVBoxLayout(this);

    hub   = new QWidget(this);
    groom = new Groom(player, this);
    feed  = new Feed(player, petType, this);
    sleep = new Sleep(player, petType, this);

    pages = new QStackedWidget();
    pages->addWidget(hub);   // 0 — empty, never shown directly
    pages->addWidget(groom); // 1
    pages->addWidget(feed);  // 2
    pages->addWidget(sleep); // 3

    // b_back on hub kept for game.cc compatibility
    b_back = new QPushButton("BACK", hub);
    b_back->setIcon(QIcon(":/images/Assets/left.png"));
    b_back->setStyleSheet(R"(
        QPushButton { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4850DB, stop:1 #4A71DB);
            border: 2px inset #FBA8FF; border-radius: 10px;
            padding: 4px; font: bold; }
        QPushButton:pressed { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4A71DB, stop:1 #4850DB); })");

    top_layout->addWidget(pages);

    // All three sub-screen back buttons go back to Mode, not the blank hub
    connect(groom->backBtn, SIGNAL(clicked()), this, SLOT(returnToMode()));
    connect(feed->backBtn,  SIGNAL(clicked()), this, SLOT(returnToMode()));
    connect(sleep->backBtn, SIGNAL(clicked()), this, SLOT(returnToMode()));
}

void Care::updateStats() {
    groom->updateHygieneDisplay();
    feed->updateHungerDisplay();
    sleep->updateSleepDisplay();
}

void Care::returnToMode() {
    emit requestReturnToMode();
}

void Care::goToFeed()  { pages->setCurrentIndex(2); }
void Care::goToGroom() { pages->setCurrentIndex(1); }
void Care::goToSleep() { pages->setCurrentIndex(3); }
