/*
 * In single-player battle, the PiPet battles against the computer.
 *
 * Author(s): Camden Gugel
 */

#ifndef BATTLE_H
#define BATTLE_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QRandomGenerator>

enum class Move { Attack, Charge, Defend };

class Battle : public QWidget
{
    Q_OBJECT

public:
    explicit Battle(QWidget *parent = nullptr);
    QPushButton *btnBack;
private slots:
    void onAttack();
    void onCharge();
    void onDefend();
    void onRestart();

private:
    QVBoxLayout *root;
    QLabel *title;
    QWidget *hpWidget; // hpgrid needs a widget!
    QGridLayout *hpGrid;
    QFrame *sep; // separator
    QWidget *btnWidget; // btnRow needs a widget!
    QHBoxLayout *btnRow;

    void playTurn(Move playerMove);
    Move cpuMove();
    void refreshUI();
    void endGame();

    // Game state
    int  playerHP      = 100;
    int  cpuHP         = 100;
    int  playerAtk     = 20;   // base + accumulated charge bonuses
    int  cpuAtk        = 20;
    int  playerDefends = 5;
    bool playerSkip    = false; // player loses next turn
    bool cpuSkip       = false; // cpu loses next turn

    static constexpr int maxHP        = 100;
    static constexpr int baseAtk      = 20;
    static constexpr int chargeBonus  = 10;
    static constexpr int maxDefends   = 5;

    // Widgets
    QProgressBar *playerBar, *cpuBar;
    QLabel       *playerHPLabel, *cpuHPLabel;
    QLabel       *resultLabel, *logLabel;
    QPushButton  *btnAttack, *btnCharge, *btnDefend, *btnRestart;
};

#endif
