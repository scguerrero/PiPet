/*
 * game.h - Top-level game widget.
 * Author(s): Sasha C. Guerrero
 */

#ifndef GAME_H
#define GAME_H
#include "../start_class/start.h"
#include "../create_class/create.h"
#include "../mode_class/mode.h"
#include "../care_class/care.h"
#include "../train_class/train.h"
#include "../battle_class/battle.h"
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

private:
    bool new_game = true;

    QVBoxLayout    *top_layout;
    QVBoxLayout    *layout;
    QStackedWidget *pages;

    Start  *start;   // page 0
    Create *create;  // page 1
    Mode   *mode;    // page 2
    Care   *care;    // page 3
    Train  *train;   // page 4
    Battle *battle;  // page 5

    QHBoxLayout *utility_bar;
    QPushButton *b_save, *b_home, *b_quit;

private slots:
    void open_start();
    void open_create();
    void open_mode();
    void open_care();
    void open_train();
    void open_battle();
    void onCreateDone(); // reads species + name, tells Mode which GIF to show
};

#endif
