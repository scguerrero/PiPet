/*
 * battle.h - Single-player battle screen.
 * Added: dojo background + wood-chip particle effect on the training dummy,
 *        player Character sprite shown in the arena (hat-aware).
 *
 * Author(s): Camden Gugel, Luke Cerwin
 */

#ifndef BATTLE_H
#define BATTLE_H
#include "../character_class/character.h"
#include "../../Player/Player.h"

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QRandomGenerator>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QPointF>
#include <QVector>

enum class Move { Attack, Charge, Defend };

// ── Wood chip particle ────────────────────────────────────────────────────
struct WoodParticle {
    QPointF pos;
    QPointF vel;
    float   rotation;
    float   rotSpeed;
    float   alpha;
    float   fade;
    float   w, h;
    QColor  color;
};

class Battle : public QWidget
{
    Q_OBJECT

public:
    explicit Battle(QWidget *parent = nullptr);

    // Call this from game.cc each time the battle screen is opened.
    // Syncs the character sprite to the correct pet type and equipped hat.
    void setPlayerInfo(Player *player, Character::PetType petType);

signals:
    void battleWon();

protected:
    void paintEvent(QPaintEvent *e) override;


private slots:
    void onAttack();
    void onCharge();
    void onDefend();
    void onRestart();
    void tickParticles();

private:
    QVBoxLayout *root;
    QLabel *title;
    QWidget *hpWidget;
    QGridLayout *hpGrid;
    QFrame *sep;
    QWidget *btnWidget;
    QHBoxLayout *btnRow;

    void playTurn(Move playerMove);
    Move cpuMove();
    void refreshUI();
    void endGame();
    void spawnWoodParticles(int count = 18);

    // Game state
    int  playerHP      = 100;
    int  cpuHP         = 100;
    int  playerAtk     = 20;
    int  cpuAtk        = 20;
    int  playerDefends = 5;
    bool playerSkip    = false;
    bool cpuSkip       = false;

    static constexpr int maxHP       = 100;
    static constexpr int baseAtk     = 20;
    static constexpr int chargeBonus = 10;
    static constexpr int maxDefends  = 5;

    // Widgets
    QProgressBar *playerBar, *cpuBar;
    QLabel       *playerHPLabel, *cpuHPLabel;
    QLabel       *resultLabel, *logLabel;
    QPushButton  *btnAttack, *btnCharge, *btnDefend, *btnRestart;

    // Player character sprite — always set via setPlayerInfo() before use
    Character          *m_character = nullptr;
    Player             *m_player    = nullptr;
    Character::PetType  m_petType;

    // Background
    QPixmap m_bg;

    // Particle system
    QVector<WoodParticle> m_particles;
    QTimer               *m_particleTimer;
};

#endif // BATTLE_H
