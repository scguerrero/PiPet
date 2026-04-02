/*
 * Care class specification file.
 * In Care mode, the Player can feed, groom, and give affection to their pet.
 * They can also view their pet's Condition, or wellness attributes.
 * Author(s): Sasha C. Guerrero, Tanya Magurupira
 * Created: 2/19/2026
 * Last Edited: 3/17/2026
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
    void updateStats(); // call this whenever widget becomes visible
    Care(Player* player, QWidget *parent = nullptr);

    // Care actions: feed pet, groom pet, send pet to sleep, give pet affection
    QPushButton *b_feed, *b_groom, *b_sleep, *b_affection, *b_back;
signals:

    void affectionRequested(); // tell ModeWidget to open AffectionWidget
    void hygieneRequested(); // tell ModeWidget to open GroomWidget

private slots:
    void returnToHub(); // hub menu
    void feedPet(); // feed
    void groomPet(); // groom
    void sendPetToSleep(); // sleep
    void givePetAffection(); //affection

private:
    Player *player;            // pointer matches Mode
    QStackedWidget *stack;

    QVBoxLayout *top_layout; // Top-level layout that will hold stacked widget
    QStackedWidget *pages; // Each page is a different Care widget

    Groom *groom; // Groom widget lets player manage pet's hygiene
    Affection *affection; // Affection widget lets player give affection to pet
    Feed *feed;
    Sleep *sleep;

    QWidget *hub; // Menu of pet's stats and actions
    QVBoxLayout *layout; // Pet's Condition and Care Actions go inside here
    QGridLayout *grid; // Text labels and progress bars for pet's condition will go inside grid
    QGridLayout *careGrid; // Care actions will go inside careGrid
    QGroupBox *careBox; // careGrid will go inside careBox
    QGroupBox *conditionBox; // Condition will go inside conditionBox

    // Text labels for the pet's hunger, energy, strength, hygiene, intelligence, happiness, days old, and age group.
    QLabel *hunger_label, *energy_label, *strength_label, *hygiene_label, *intelligence_label,
        *happiness_label, *age_days_label, *age_group_label, *age_days, *age_group;

    // Progress bars corresponding to the pet's attributes above.
    QProgressBar *hunger_bar, *energy_bar, *strength_bar, *hygiene_bar, *intelligence_bar, *happiness_bar;
};

#endif // CARE_H
