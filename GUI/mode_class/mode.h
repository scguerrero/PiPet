/*
 * Mode class specification file.
 * From here the Player can select which game mode they want to play:
 * Care mode for feeding and grooming the pet, Train mode for choosing a mini-game,
 * Battle mode for playing single-player games versus a bot opponent, and
 * Gear mode for choosing equipment/accessories for the pet.
 * Author(s): Sasha C. Guerrero
 * Created: 2/19/2026
 * Last Edited: 3/14/2026
 */
#ifndef MODE_H
#define MODE_H

#include <QtWidgets>
#include "../care_class/care.h"
#include "../train_class/train.h"
//#include "BattleWidget.h"
#include "../battle_class/battle.h"
//#include "GearWidget.h"
#include "../care_class/affection.h"
#include "../care_class/groom.h"
#include "../care_class/feed.h"
#include "../care_class/sleep.h"
#include "../../Player/Player.h"

class Mode : public QWidget{
    Q_OBJECT

public:
explicit Mode(Player* player, QWidget* parent = nullptr);    QStackedWidget *mode_select;
    Care *mode_care;
    Train *mode_train;
    Affection *mode_affection;
    Groom *mode_groom;
    Feed *mode_feed;
    Sleep *mode_sleep;
    /*BattleWidget *mode_battle;
    GearWidget *mode_gear;
    BattleWindow *mode_battle;
    GearWidget *mode_gear;
    */

signals:

    // any signals wanted to emit in here
    void battleRequested(); //navigates game to battle

private slots:
    void goBack();// navigate back to index 0, the mode-select page
    void openCare();
    void openTrain();
    void openfeed();
    void openAffection();
    void openGroom();
    void openSleep();
    void closefeed();
    void closeGroom();
    void closeAffection();
    void closeSleep();
    /*void openBattleWidget(); // index 3
        void openGearWidget(); // index 4
        */

private:
    Player *player; // store the pointer

    QVBoxLayout *layout;
    QWidget *menu;
    QVBoxLayout *buttons;

    QPushButton* b_care;
    QPushButton* b_train;
    QPushButton* b_battle;
    QPushButton* b_gear;
    QPushButton *back;

};

#endif