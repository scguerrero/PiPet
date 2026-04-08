/*
 * care.h - Care hub.
 * Removed: Give Affection tab
 * Added: Character GIF display on hub
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

    // petType needed so the hub can show the character GIF
    Care(Player* player, Character::PetType petType, QWidget *parent = nullptr);

    QPushButton *b_feed, *b_groom, *b_sleep, *b_back;

private slots:
    void returnToHub();
    void feedPet();
    void groomPet();
    void sendPetToSleep();

private:
    Player             *player;
    Character::PetType  petType;

    QVBoxLayout    *top_layout;
    QStackedWidget *pages;

    Groom *groom;
    Feed  *feed;
    Sleep *sleep;

    QWidget     *hub;
    QVBoxLayout *layout;
    QGridLayout *grid;
    QGridLayout *careGrid;
    QGroupBox   *careBox;
    QGroupBox   *conditionBox;

    // Character GIF on hub
    Character *character;

    QLabel *hunger_label, *energy_label, *strength_label, *hygiene_label,
           *intelligence_label, *happiness_label, *age_days_label,
           *age_group_label, *age_days, *age_group;

    QProgressBar *hunger_bar, *energy_bar, *strength_bar, *hygiene_bar,
                 *intelligence_bar, *happiness_bar;
};

#endif // CARE_H
