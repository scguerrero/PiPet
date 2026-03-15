/*
 * CareWidget class specification file.
 * In Care mode, the Player can feed, groom, and give affection to their pet.
 * They can also view their pet's Condition, or wellness attributes.
 * Author(s): Sasha C. Guerrero
 * Created: 2/19/2026
 * Last Edited: 2/23/2026
 *
 * Current Edits author: Tanya Magurupira
 * Goal: Making sure progress bar gets updates on the care stats of the pet
 * date: 3/13/2026
 */
#ifndef CAREWIDGET_H
#define CAREWIDGET_H
#include <QtWidgets>
#include "../../Player/Player.h"

class CareWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CareWidget(Player *player, QWidget *parent = nullptr);
    void updateStats(); // call this whenever widget becomes visible

signals:

    void affectionRequested(); // tell ModeWidget to open AffectionWidget
    void hygieneRequested(); // tell ModeWidget to open GroomWidget

private slots:
    void feedPet(); // feed
    void groomPet(); // groom
    void sendPetToSleep(); // sleep
    void givePetAffection(); //affection

private:
    Player *player; // pointer to player data

    QVBoxLayout *layout; // Pet's Condition and Care Actions go inside here
    QGridLayout *grid; // Text labels and progress bars for pet's condition will go inside grid
    QGridLayout *careGrid; // Care actions will go inside careGrid
    QGroupBox *careBox; // careGrid will go inside careBox
    QGroupBox *conditionBox; // Condition will go inside conditionBox

    // Text labels for the pet's hunger, energy, strength, hygiene, intelligence, happiness, days old, and age group.
    QLabel *hunger_label, *energy_label, *strength_label,
        *hygiene_label, *intelligence_label, *happiness_label,
        *age_days_label, *age_group_label, *age_days, *age_group;

    // Progress bars corresponding to the pet's attributes above.
    QProgressBar *hunger_bar, *energy_bar, *strength_bar,
        *hygiene_bar, *intelligence_bar, *happiness_bar;

    // Care actions: feed pet, groom pet, send pet to sleep, give pet affection
    QPushButton *feed, *groom, *sleep, *affection;

};

#endif // CAREWIDGET_H

