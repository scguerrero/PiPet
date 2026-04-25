/*  Track Rush – mini-game widget
 *
 *  Lives inside GUI/train_class/ alongside train.h / train.cc.
 *  Instantiated by Train when the player presses "Mini-game 2".
 *
 *  Pet sprite is rendered through the same Character widget used by
 *  mindReader, Feed, Battle, and every other screen in the project.
 *  The GameCanvas handles track / obstacle / collectible drawing only.
 *
 *  Dependencies:
 *    Pet/PiPet.h              – hunger / happiness / xp
 *    Player/Player.h          – live hat + age data  (same as mindReader)
 *    GUI/character_class/character.h
 *
 *  Qt modules: core  gui  widgets   (already in pipet.pro)
 *
 *  Author: Tanya Magurupira
 */

#ifndef piDash_H
#define piDash_H

#include <QWidget>
#include <QVector>
#include <QColor>
#include <QFont>
#include <QLabel>
#include <QMovie>
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
#include "../../Player/Player.h"
#include "../character_class/character.h"

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

// ─── Canvas: draws track, obstacles, collectibles, particles ──
//     Pet sprite is NOT drawn here — see piDash::m_character below.

class GameCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit GameCanvas(int width, int height, QWidget* parent = nullptr);

    // Called every frame from piDash::timerEvent
    void syncState(float petY, bool jumping, int frame, float speed,
                   const QVector<TRObstacle>&    obstacles,
                   const QVector<TRCollectible>& collectibles,
                   const QVector<TRParticle>&    particles,
                   int flashTimer, bool idle, bool dead,
                   int score, int xp, int lives);

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent*) override;
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

    int    m_cw;
    int    m_ch;
    float  m_groundY;
    QPixmap m_bgCache;
    static constexpr float PET_X = 80.f;

    void drawBackground (QPainter&) const;
    void drawTrack      (QPainter&) const;
    void drawObstacle   (QPainter&, const TRObstacle&)    const;
    void drawCollectible(QPainter&, const TRCollectible&) const;
    void drawParticles  (QPainter&) const;
    void drawOverlay    (QPainter&) const;
};

// ─── piDash: outer widget ─────────────────────────────────────

class piDash : public QWidget
{
    Q_OBJECT

public:
    // Constructor now takes Player* exactly like mindReader — so it can call
    // syncWithPlayer() and applyHatSprite() through the Character widget.
    explicit piDash(PiPet*  pet,
                    Player* player,
                    Character::PetType petType,
                    QWidget* parent = nullptr,
                    int canvasW = 600, int canvasH = 220);
    ~piDash() override = default;

    QPushButton* btnBack;

    // Mirrors mindReader::refreshCharacter() — called by Train whenever the
    // player equips a new hat or the pet ages up.
    void refreshCharacter();

signals:
    void gameFinished(int finalScore, int xpEarned);

protected:
    void keyPressEvent(QKeyEvent*) override;
    void timerEvent   (QTimerEvent*) override;

private slots:
    void onCanvasClicked();
    void onRestart();

private:
    // ── Layouts / chrome ─────────────────────────────────────────────────────
    QVBoxLayout*  root;
    QLabel*       title;
    QFrame*       sep;

    QWidget*      hudWidget;
    QHBoxLayout*  hudRow;
    QLabel*       scoreLabel;
    QLabel*       livesLabel;
    QLabel*       xpLabel;

    GameCanvas*   canvas;

    // Character widget — identical pattern to mindReader::m_character
    Character*    m_character;

    QLabel*       statusLabel;
    QLabel*       logLabel;

    QWidget*      btnWidget;
    QHBoxLayout*  btnRow;
    QPushButton*  btnJump;
    QPushButton*  btnRestart;

    QWidget*      resultPanel;
    QLabel*       resultLabel;

    // ── Game state ────────────────────────────────────────────────────────────
    enum class GState { Idle, Playing, Dead };
    GState m_state = GState::Idle;

    PiPet*             m_pet    = nullptr;
    Player*            m_player = nullptr;      // added — same as mindReader
    Character::PetType m_petType;

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

    int  m_timerId        = 0;
    int  m_canvasW;
    float m_groundY;
    static constexpr float PET_X    = 80.f;
    static constexpr int   FRAME_MS = 33;
    static constexpr float GRAVITY  = 0.55f;
    static constexpr float JUMP_VY  = -10.f;
    static constexpr int   SPRITE_SIZE = 64;   // Character widget size on canvas

    bool  m_sessionFinished = false;
    float m_lastCharY       = -1.f;

    // ── Helpers ───────────────────────────────────────────────────────────────
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

    // Sprite sync — mirrors mindReader::applyHatSprite() exactly
    void applyHatSprite();

    // Moves m_character so it sits on the canvas at the correct petY position
    void positionCharacterWidget();
};

#endif // piDash_H