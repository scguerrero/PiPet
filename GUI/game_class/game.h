/*
 * game.h - Top-level game widget.
 * Author(s): Luke Cerwin, Sasha Guerrero
 */
#ifndef GAME_H
#define GAME_H
#include "../start_class/start.h"
#include "../create_class/create.h"
#include "../mode_class/mode.h"
#include "../care_class/feed.h"
#include "../care_class/groom.h"
#include "../care_class/sleep.h"
#include "../train_class/train.h"
#include "../battle_class/battle.h"
#include "../gear_class/gear.h"
#include "../character_class/character.h"

class Game : public QWidget {
    Q_OBJECT
public:
    explicit Game(QWidget *parent = nullptr);
    QJsonObject toJson() const;
    void read(const QJsonObject &json);
    PiPet  *pet;
    Player *player;

public slots:
    bool loadGame();
    bool saveGame();
    void setUtilityStyle(QPushButton &button);

    // Achievement trigger slots
    void onBattleWon();
    void onFedBone();
    void onAgeChanged(const QString &ageGroup);
    void onCrownHatEquipped();
    void onTuckIn();
    void onBedTimeStory(int totalUsed);
    void onVeteranCheck();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    bool new_game = true;
    Character::PetType currentPetType = Character::DragonDog;

    QVBoxLayout    *layout;
    QStackedWidget *pages;

    Start  *start;
    Create *create;
    Mode   *mode;
    Feed   *feed;
    Groom  *groom;
    Sleep  *sleep;
    Train  *train;
    Battle *battle;
    Gear   *gear;

    // Bottom bar — Home button only, normal size
    QHBoxLayout *utility_bar;
    QPushButton *b_save_mode; // top-left on Mode screen
    QPushButton *b_home;      // bottom bar, all screens except start/create
    QWidget     *utilityWidget;

    void showUtilityBar(bool show);
    void showHomeOnly(bool activeStyle);
    void showAchievementPopup(const QList<QString> &titles);

    QTimer *m_inactivityTimer;
    void    resetInactivityTimer();
    QTimer *m_marathonTimer;

private slots:
    void open_start();
    void open_create();
    void open_mode();
    void open_feed();
    void open_groom();
    void open_sleep();
    void open_train();
    void open_battle();
    void open_gear();
    void onCreateDone();
    void onInactivityTriggered();
    void onMarathonTriggered();
    void showAchievementsScreen();
};

#endif
