/*
 * In single-player battle, the PiPet battles against the computer.
 * Added: dojo background + wood-chip particle effect on the training dummy.
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
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QPointF>
#include <QVector>

enum class Move { Attack, Charge, Defend };

// ── Wood chip particle ────────────────────────────────────────────────────
struct WoodParticle {
    QPointF pos;       // current position (widget-space, proportional → pixel in paintEvent)
    QPointF vel;       // pixels per tick
    float   rotation;  // degrees
    float   rotSpeed;  // degrees per tick
    float   alpha;     // 0-1
    float   fade;      // alpha lost per tick
    float   w, h;      // chip dimensions
    QColor  color;
};

class Battle : public QWidget
{
    Q_OBJECT

public:
    explicit Battle(QWidget *parent = nullptr);

signals:
    // Emitted when the player wins — game.cc connects to onBattleWon()
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

    // Spawns wood chips at the dummy head/chest area (red-annotated zone)
    void spawnWoodParticles(int count = 18);

    // Game state
    int  playerHP      = 100;
    int  cpuHP         = 100;
    int  playerAtk     = 20;
    int  cpuAtk        = 20;
    int  playerDefends = 5;
    bool playerSkip    = false;
    bool cpuSkip       = false;

    static constexpr int maxHP        = 100;
    static constexpr int baseAtk      = 20;
    static constexpr int chargeBonus  = 10;
    static constexpr int maxDefends   = 5;

    // Widgets
    QProgressBar *playerBar, *cpuBar;
    QLabel       *playerHPLabel, *cpuHPLabel;
    QLabel       *resultLabel, *logLabel;
    QPushButton  *btnAttack, *btnCharge, *btnDefend, *btnRestart;

    // Background
    QPixmap m_bg;

    // Particle system
    QVector<WoodParticle> m_particles;
    QTimer               *m_particleTimer;
};

#endif // BATTLE_H
