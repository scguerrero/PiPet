#ifndef MODEWIDGET_H
#define MODEWIDGET_H

#include <QtWidgets>
#include "CareWidget.h"
#include "TrainWidget.h"
//#include "BattleWidget.h"
#include "battlewindow.h"
//#include "GearWidget.h"
#include "AffectionWidget.h"
#include "GroomWidget.h"
#include "../../Player/Player.h"

class ModeWidget : public QWidget{
    Q_OBJECT

public:
    explicit ModeWidget(Player *player, QWidget *parent = nullptr);
    QStackedWidget *mode_select;
    CareWidget *mode_care;
    TrainWidget *mode_train;
    AffectionWidget *mode_affection;
    GroomWidget *mode_groom;
    /*BattleWidget *mode_battle;
    GearWidget *mode_gear;
    BattleWindow *mode_battle;
    GearWidget *mode_gear;
    */

signals:

    // any signals wanted to emit in here

private slots:
    void goBack();// navigate back to index 0, the mode-select page
    void openCareWidget(); // open widget at index 1
    void openTrainWidget(); // index 2
    void openAffectionWidget();
    void closeAffectionWidget();
    void openGroomWidget();
    void closeGroomWidget();
        /*void openBattleWidget(); // index 3
        void openGearWidget(); // index 4
        */

private:
    Player *player; // store the pointer

    QVBoxLayout *layout;
    QWidget *menu;
    QVBoxLayout *buttons;
    QPushButton *care;
    QPushButton *train;
    QPushButton *battle;
    QPushButton *gear;
    QPushButton *back;

};

#endif
