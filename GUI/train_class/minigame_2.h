/*  Track Rush – mini-game widget
 *
 *  Lives inside GUI/train_class/ alongside train.h / train.cc.
 *  Instantiated by Train when the player presses "Mini-game 2".
 *
 *  Dependencies from this project:
 *    Pet/PiPet.h   – for PiPet* pointer (hunger / happiness / xp)
 *
 *  Qt modules used: core  gui  widgets   (already in pipet.pro)
 */

#ifndef MINIGAME_2_H
#define MINIGAME_2_H

#include <QWidget>
#include <QVector>
#include <QColor>
#include <QFont>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimerEvent>
#include <QRandomGenerator>
#include "../../Pet/PiPet.h"

// ─── Plain-old-data structs ───────────────────────────────────

struct TRObstacle {
    enum Type { Rock, Crate, Spike } type;
    float x, y, w, h;
    bool  hit = false;
};

struct TRCollectible {
    enum Type { Coin, Star } type;
    float x, y, r;
    bool  collected = false;
};

struct TRParticle {
    float  x, y, vx, vy, life;
    QColor color;
};

// ─── Canvas: the actual game drawing area ────────────────────

class GameCanvas : public QWidget
{
    Q_OBJECT
public:
    // ── CHANGED: accept width & height ───────────────────────
    explicit GameCanvas(int width, int height, QWidget* parent = nullptr);

    void syncState(float petY, bool jumping, int frame, float speed,
                   const QVector<TRObstacle>&    obstacles,
                   const QVector<TRCollectible>& collectibles,
                   const QVector<TRParticle>&    particles,
                   int flashTimer, bool idle, bool dead,
                   int score, int xp, int lives);

signals:
    void clicked();

protected:
    void paintEvent     (QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override { emit clicked(); }

private:
    // snapshot of game state for painting
    float  m_petY    = 160.f;
    bool   m_jumping = false;
    int    m_frame   = 0;
    float  m_speed   = 3.5f;
    int    m_flash   = 0;
    bool   m_idle    = true;
    bool   m_dead    = false;
    int    m_score   = 0;
    int    m_xp      = 0;
    int    m_lives   = 3;
    QVector<TRObstacle>    m_obs;
    QVector<TRCollectible> m_col;
    QVector<TRParticle>    m_par;

    void drawBackground (QPainter&) const;
    void drawTrack      (QPainter&) const;
    void drawPet        (QPainter&) const;
    void drawObstacle   (QPainter&, const TRObstacle&)    const;
    void drawCollectible(QPainter&, const TRCollectible&) const;
    void drawParticles  (QPainter&) const;
    void drawOverlay    (QPainter&) const;

    // ── CHANGED: no longer static constexpr — set by constructor ──
    int   m_cw;                // canvas width
    int   m_ch;                // canvas height
    float m_groundY;           // derived: m_ch - 60
    static constexpr float PET_X = 80.f;  // pet stays on left side
};

// ─── minigame_2: outer widget (mirrors Battle's structure) ────

class minigame_2 : public QWidget
{
    Q_OBJECT

public:
    // ── CHANGED: optional canvas size parameters ─────────────
    explicit minigame_2(PiPet* pet, QWidget* parent = nullptr,
                        int canvasW = 600, int canvasH = 220);
    ~minigame_2() override = default;

    QPushButton* btnBack;

signals:
    void gameFinished(int finalScore, int xpEarned);

protected:
    void keyPressEvent(QKeyEvent*) override;
    void timerEvent   (QTimerEvent*) override;

private slots:
    void onCanvasClicked();
    void onRestart();

private:
    // ── Layout (mirrors Battle) ───────────────────────────────
    QVBoxLayout*  root;
    QLabel*       title;
    QFrame*       sep;

    QWidget*      hudWidget;
    QHBoxLayout*  hudRow;
    QLabel*       scoreLabel;
    QLabel*       livesLabel;
    QLabel*       xpLabel;

    GameCanvas*   canvas;

    QLabel*       statusLabel;
    QLabel*       logLabel;

    QWidget*      btnWidget;
    QHBoxLayout*  btnRow;
    QPushButton*  btnJump;
    QPushButton*  btnRestart;

    // ── Game state ───────────────────────────────────────────
    enum class GState { Idle, Playing, Dead };
    GState m_state = GState::Idle;

    PiPet* m_pet = nullptr;

    int   m_score  = 0;
    int   m_xp     = 0;
    int   m_lives  = 3;
    int   m_frame  = 0;
    float m_speed  = 3.5f;

    float m_petY    = 160.f;
    float m_petVY   = 0.f;
    bool  m_jumping = false;

    int   m_spawnTimer    = 0;
    int   m_spawnInterval = 90;
    int   m_flashTimer    = 0;

    QVector<TRObstacle>    m_obstacles;
    QVector<TRCollectible> m_collectibles;
    QVector<TRParticle>    m_particles;

    int m_timerId = 0;

    // ── CHANGED: size-derived values instead of static constexpr ──
    int   m_canvasW;           // stored canvas width  (for spawning)
    float m_groundY;           // derived: canvasH - 60
    static constexpr float PET_X    = 80.f;
    static constexpr int   FRAME_MS = 16;
    static constexpr float GRAVITY  = 0.55f;
    static constexpr float JUMP_VY  = -10.f;

    // ── Helpers ──────────────────────────────────────────────
    void startGame();
    void resetState();
    void doJump();
    void tick();
    void refreshUI();
    void endGame();

    void spawnObstacle();
    void spawnCollectible();
    void addParticles(float x, float y, const QColor& c, int n = 10);
    bool overlaps(float ax, float ay, float aw, float ah,
                  float bx, float by, float bw, float bh) const;
};

#endif // MINIGAME_2_H
