/*
 * CareWidget class specification file.
 * In Care mode, the Player can feed, groom, and give affection to their pet.
 * They can also view their pet's Condition, or wellness attributes.
 * Author(s): Sasha C. Guerrero, Tanya Magurupira
 * Created: 2/19/2026
 * Last Edited: 3/17/2026
 *
 * Current Edits author: Tanya Magurupira
 * Goal: Making sure progress bar gets updates on the care stats of the pet
 * date: 3/13/2026
 */
#ifndef CARE_H
#define CARE_H

#include <QtWidgets>
#include "groom.h"
#include "affection.h"
#include "feed.h"
#include "sleep.h"
#include "../../Player/Player.h"

class Care : public QWidget
{
    Q_OBJECT
public:
    // Correct constructor signature
    Care(Player* player, QStackedWidget* stack, QWidget *parent = nullptr);

    void updateStats();

    // Care action buttons
    QPushButton *b_feed, *b_groom, *b_sleep, *b_affection, *b_back;

signals:
    void affectionRequested();
    void hygieneRequested();
    void feedRequested();
    void sleepRequested();

private slots:
    void returnToHub();
    void feedPet();
    void groomPet();
    void sendPetToSleep();
    void givePetAffection();

private:
    Player *player;            // pointer matches Mode
    QStackedWidget *stack;

    QVBoxLayout *top_layout;
    QStackedWidget *pages;

    Groom *groom;
    Affection *affection;
    Feed *feed;
    Sleep *sleep;

    QWidget *hub;
    QVBoxLayout *layout;
    QGridLayout *grid;
    QGridLayout *careGrid;
    QGroupBox *careBox;
    QGroupBox *conditionBox;

    QLabel *hunger_label, *energy_label, *strength_label, *hygiene_label,
        *intelligence_label, *happiness_label, *age_days_label,
        *age_group_label, *age_days, *age_group;

    QProgressBar *hunger_bar, *energy_bar, *strength_bar, *hygiene_bar,
        *intelligence_bar, *happiness_bar;
};

#endif // CARE_H
