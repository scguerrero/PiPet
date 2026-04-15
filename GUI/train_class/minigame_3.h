/* Author: Tanya Magurupira */

#ifndef MINIGAME_3_H
#define MINIGAME_3_H

#include <QWidget>
#include <QVector>
#include <QColor>
#include <QFont>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QKeyEvent>
#include <QTimerEvent>
#include <QMouseEvent>
#include <QRandomGenerator>
#include "../../Pet/PiPet.h"

// ─── Data structs ─────────────────────────────────────────────

struct SSFood {
    enum Type { Apple, Pizza, Carrot, Cake } type;
    float x, y, w, h;
    float vy;               // fall speed
    float drift;            // horizontal wobble
    bool  caught = false;
    int   points;
    int   xp;
};

struct SSHazard {
    float x, y, w, h;
    float vy;
    bool  hit = false;
};

struct SSParticle {
    float  x, y, vx, vy, life;
    QColor color;
};

// ─── SnackCanvas: the drawing surface ─────────────────────────

class SnackCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit SnackCanvas(int width, int height, QWidget* parent = nullptr);

    void syncState(float petX, int frame,
                   const QVector<SSFood>&     foods,
                   const QVector<SSHazard>&   hazards,
                   const QVector<SSParticle>& particles,
                   int flash, bool idle, bool dead,
                   int score, int xp, int lives,
                   int comboCount);

signals:
    void clicked();

protected:
    void paintEvent     (QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override { emit clicked(); }

private:
    float  m_petX   = 0.f;
    int    m_frame  = 0;
    int    m_flash  = 0;
    bool   m_idle   = true;
    bool   m_dead   = false;
    int    m_score  = 0;
    int    m_xp     = 0;
    int    m_lives  = 3;
    int    m_combo  = 0;
    QVector<SSFood>     m_foods;
    QVector<SSHazard>   m_hazards;
    QVector<SSParticle> m_particles;

    void drawSky       (QPainter&) const;
    void drawGround    (QPainter&) const;
    void drawPet       (QPainter&) const;
    void drawFood      (QPainter&, const SSFood&)    const;
    void drawHazard    (QPainter&, const SSHazard&)  const;
    void drawParticles (QPainter&) const;
    void drawOverlay   (QPainter&) const;

    int   m_cw;
    int   m_ch;
    float m_groundY;
    static constexpr float PET_W = 32.f;
    static constexpr float PET_H = 28.f;
};

// ─── minigame_3: outer widget (mirrors Battle / minigame_2) ───

class minigame_3 : public QWidget
{
    Q_OBJECT

public:
    explicit minigame_3(PiPet* pet, QWidget* parent = nullptr,
                        int canvasW = 600, int canvasH = 300);
    ~minigame_3() override = default;

    QPushButton* btnBack;

signals:
    void gameFinished(int finalScore, int xpEarned);

protected:
    void keyPressEvent  (QKeyEvent*)  override;
    void keyReleaseEvent(QKeyEvent*) override;
    void timerEvent     (QTimerEvent*) override;

private slots:
    void onCanvasClicked();
    void onRestart();

private:
    // ── Layout ────────────────────────────────────────────────
    QVBoxLayout*  root;
    QLabel*       title;
    QFrame*       sep;

    QWidget*      hudWidget;
    QHBoxLayout*  hudRow;
    QLabel*       scoreLabel;
    QLabel*       livesLabel;
    QLabel*       xpLabel;
    QLabel*       comboLabel;

    SnackCanvas*  canvas;

    QLabel*       statusLabel;
    QLabel*       logLabel;

    QWidget*      btnWidget;
    QHBoxLayout*  btnRow;
    QPushButton*  btnLeft;
    QPushButton*  btnRight;
    QPushButton*  btnRestart;

    // ── Game state ────────────────────────────────────────────
    enum class GState { Idle, Playing, Dead };
    GState m_state = GState::Idle;

    PiPet* m_pet = nullptr;

    int   m_score  = 0;
    int   m_xp     = 0;
    int   m_lives  = 3;
    int   m_frame  = 0;
    float m_speed  = 2.0f;

    float m_petX   = 0.f;
    float m_petVX  = 0.f;
    bool  m_moveLeft  = false;
    bool  m_moveRight = false;

    int   m_spawnTimer    = 0;
    int   m_spawnInterval = 55;
    int   m_flashTimer    = 0;
    int   m_comboCount    = 0;

    QVector<SSFood>     m_foods;
    QVector<SSHazard>   m_hazards;
    QVector<SSParticle> m_particles;

    int m_timerId = 0;

    int   m_canvasW;
    int   m_canvasH;
    float m_groundY;

    static constexpr int   FRAME_MS  = 16;
    static constexpr float PET_W     = 32.f;
    static constexpr float PET_H     = 28.f;
    static constexpr float PET_SPEED = 5.5f;

    void startGame();
    void resetState();
    void tick();
    void refreshUI();
    void endGame();

    void spawnFood();
    void spawnHazard();
    void addParticles(float x, float y, const QColor& c, int n = 10);
    bool overlaps(float ax, float ay, float aw, float ah,
                  float bx, float by, float bw, float bh) const;
};

#endif // MINIGAME_3_H

