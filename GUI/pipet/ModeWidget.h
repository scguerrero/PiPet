/*
 * ModeWidget class specification file.
 * From here the Player can select which game mode they want to play:
 * Care mode for feeding and grooming the pet, Train mode for choosing a mini-game,
 * Battle mode for playing single-player games versus a bot opponent, and
 * Gear mode for choosing equipment/accessories for the pet.
 * Author(s): Sasha C. Guerrero
 * Created: 2/19/2026
 * Last Edited: 2/19/2026
 */
#ifndef MODEWIDGET_H
#define MODEWIDGET_H
#include <QtWidgets>
#include "CareWidget.h"
#include "TrainWidget.h"
//#include "BattleWidget.h"
//#include "GearWidget.h"

class ModeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModeWidget(QWidget *parent = nullptr);
private:
    // button menu for game modes Care, Train, Battle, and Gear
    QVBoxLayout *layout;
    QWidget *menu;
    QVBoxLayout *buttons;
    QPushButton *care;
    QPushButton *train;
    QPushButton *battle;
    QPushButton *gear;

    // Button to navigate back to mode-select page
    QPushButton *back;

    // widgets for all game modes
    QStackedWidget *mode_select;
    CareWidget *mode_care;
    TrainWidget *mode_train;
    /*BattleWidget *mode_battle;
    GearWidget *mode_gear;
    */

    signals:
    private slots:
        void goBack();// navigate back to index 0, the mode-select page
        void openCareWidget(); // open widget at index 1
        void openTrainWidget(); // index 2
        /*void openBattleWidget(); // index 3
        void openGearWidget(); // index 4
        */

};

#endif // MODEWIDGET_H
