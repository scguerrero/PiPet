/*
 * care.h - Care hub.
 * Back buttons on sub-screens emit requestReturnToMode signal
 * so game.cc navigates back to the Mode screen directly.
 * Author(s): Sasha C. Guerrero, Tanya Magurupira
 */
#ifndef CARE_H
#define CARE_H
#include <QtWidgets>
#include "groom.h"
#include "feed.h"
#include "sleep.h"
#include "../../Player/Player.h"
#include "../character_class/character.h"

class Care : public QWidget
{
    Q_OBJECT
public:
    void updateStats();
    Care(Player* player, Character::PetType petType, QWidget *parent = nullptr);

    QPushButton *b_back; // kept for game.cc compatibility

    void goToFeed();
    void goToGroom();
    void goToSleep();

signals:
    // Emitted when any sub-screen's back button is pressed
    void requestReturnToMode();

private slots:
    void returnToMode(); // calls emit requestReturnToMode()

private:
    Player             *player;
    Character::PetType  petType;

    QVBoxLayout    *top_layout;
    QStackedWidget *pages;

    Groom *groom;
    Feed  *feed;
    Sleep *sleep;

    QWidget *hub;
};

#endif // CARE_H
