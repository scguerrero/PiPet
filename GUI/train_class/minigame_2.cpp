#include "minigame_2.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>
#include <algorithm>

// ═════════════════════════════════════════════════════════════
//  GameCanvas
// ═════════════════════════════════════════════════════════════

// ── CHANGED: constructor takes width & height ────────────────
GameCanvas::GameCanvas(int width, int height, QWidget* parent)
    : QWidget(parent), m_cw(width), m_ch(height)
{
    m_groundY = m_ch - 60.f;       // ground line relative to canvas height
    m_petY    = m_groundY;         // start pet on the ground
    setFixedSize(m_cw, m_ch);
    setCursor(Qt::PointingHandCursor);
    setFocusPolicy(Qt::NoFocus);
}

void GameCanvas::syncState(float petY, bool jumping, int frame, float speed,
                           const QVector<TRObstacle>&    obs,
                           const QVector<TRCollectible>& col,
                           const QVector<TRParticle>&    par,
                           int flash, bool idle, bool dead,
                           int score, int xp, int lives)
{
    m_petY    = petY;   m_jumping = jumping;
    m_frame   = frame;  m_speed   = speed;
    m_obs     = obs;    m_col     = col;    m_par = par;
    m_flash   = flash;
    m_idle    = idle;   m_dead    = dead;
    m_score   = score;  m_xp     = xp;     m_lives = lives;
}

// ── paintEvent ───────────────────────────────────────────────
void GameCanvas::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);

    drawBackground(p);
    drawTrack(p);
    for (const auto& o : m_obs) drawObstacle(p, o);
    for (const auto& c : m_col) drawCollectible(p, c);
    drawPet(p);
    drawParticles(p);

    if (m_flash > 0) {
        int alpha = static_cast<int>(m_flash / 10.f * 70.f);
        p.fillRect(rect(), QColor(228, 75, 74, alpha));
    }

    if (m_idle || m_dead) drawOverlay(p);
}

void GameCanvas::drawBackground(QPainter& p) const
{
    p.fillRect(rect(), QColor("#1a1a2e"));

    // ── CHANGED: sky height scales with canvas ───────────────
    int skyH = static_cast<int>(m_ch * 0.45f);
    p.fillRect(0, 0, m_cw, skyH, QColor("#16213e"));

    // moon — positioned proportionally to canvas width
    float moonX = m_cw * 0.867f;   // was 520/600
    float moonY = m_ch * 0.182f;   // was 40/220
    p.setBrush(QColor("#D3D1C7")); p.setPen(Qt::NoPen);
    p.drawEllipse(QPointF(moonX, moonY), 18, 18);
    p.setBrush(QColor("#1a1a2e"));
    p.drawEllipse(QPointF(moonX + 8, moonY - 4), 14, 14);

    // twinkling stars — scale positions proportionally
    const float starPx[][2] = {
        {0.083f, 0.091f}, {0.200f, 0.159f}, {0.333f, 0.068f}, {0.517f, 0.182f},
        {0.667f, 0.082f}, {0.783f, 0.250f}, {0.250f, 0.250f}, {0.467f, 0.273f}
    };
    p.setBrush(QColor(211, 209, 199,
                      static_cast<int>(128 + 80 * qSin(m_frame * 0.05f))));
    p.setPen(Qt::NoPen);
    for (auto& s : starPx)
        p.drawRect(static_cast<int>(s[0] * m_cw),
                   static_cast<int>(s[1] * m_ch), 2, 2);
}

void GameCanvas::drawTrack(QPainter& p) const
{
    const int GY = static_cast<int>(m_groundY);   // was GROUND_Y
    p.setPen(Qt::NoPen);
    p.fillRect(0, GY + 4, m_cw, 60, QColor("#16213e"));    // was CW
    p.fillRect(0, GY + 4, m_cw,  6, QColor("#0f3460"));
    p.fillRect(0, GY + 30, m_cw, 6, QColor("#0f3460"));

    p.setBrush(QColor("#0f3460"));
    int tieOff = static_cast<int>(m_frame * m_speed) % 40;
    for (int tx = -40 + tieOff; tx < m_cw; tx += 40)       // was CW
        p.drawRect(tx, GY + 4, 12, 32);
}

void GameCanvas::drawPet(QPainter& p) const
{
    const int px = static_cast<int>(PET_X);
    const int py = static_cast<int>(m_petY);
    p.setPen(Qt::NoPen);

    p.setBrush(QColor("#534AB7"));
    p.drawRect(px - 14, py - 26, 28, 26);

    p.setBrush(QColor("#AFA9EC"));
    p.drawRect(px - 10, py - 22, 20, 14);

    p.setBrush(Qt::white);
    p.drawRect(px - 8, py - 20, 8, 8);
    p.drawRect(px + 2, py - 20, 8, 8);

    p.setBrush(QColor("#534AB7"));
    p.drawRect(px - 6, py - 18, 5, 5);
    p.drawRect(px + 3, py - 18, 5, 5);

    p.setBrush(QColor("#AFA9EC"));
    p.drawRect(px - 18, py - 16, 6, 10);
    p.drawRect(px + 12, py - 16, 6, 10);

    if (!m_jumping) {
        int swing = static_cast<int>(qSin(m_frame * 0.5) * 6);
        p.setBrush(QColor("#534AB7"));
        p.drawRect(px - 10, py,     8, 8 + swing);
        p.drawRect(px + 2,  py,     8, 8 - swing);
        p.setBrush(QColor("#AFA9EC"));
        p.drawRect(px - 12, py + 7 + swing, 10, 5);
        p.drawRect(px + 2,  py + 6 - swing, 10, 5);
    } else {
        p.setBrush(QColor("#534AB7"));
        p.drawRect(px - 12, py, 10, 7);
        p.drawRect(px + 2,  py, 10, 7);
        p.setBrush(QColor("#AFA9EC"));
        p.drawRect(px - 14, py + 5, 12, 5);
        p.drawRect(px + 2,  py + 5, 12, 5);
    }
}

void GameCanvas::drawObstacle(QPainter& p, const TRObstacle& o) const
{
    p.setPen(Qt::NoPen);
    switch (o.type) {
    case TRObstacle::Rock:
        p.setBrush(QColor("#888780"));
        p.drawEllipse(QRectF(o.x, o.y, o.w, o.h));
        p.setBrush(QColor("#B4B2A9"));
        p.drawEllipse(QRectF(o.x+2, o.y+2, o.w-4, o.h-4));
        break;
    case TRObstacle::Crate:
        p.setBrush(QColor("#BA7517"));
        p.drawRect(QRectF(o.x, o.y, o.w, o.h));
        p.setPen(QPen(QColor("#633806"), 1.5f));
        p.drawLine(QPointF(o.x+12, o.y),     QPointF(o.x+12, o.y+o.h));
        p.drawLine(QPointF(o.x, o.y+14),     QPointF(o.x+o.w, o.y+14));
        p.setPen(Qt::NoPen);
        break;
    case TRObstacle::Spike: {
        p.setBrush(QColor("#E24B4A"));
        QPolygonF tri;
        tri << QPointF(o.x+10, o.y)
            << QPointF(o.x+20, o.y+30)
            << QPointF(o.x,    o.y+30);
        p.drawPolygon(tri);
        break;
    }
    }
}

void GameCanvas::drawCollectible(QPainter& p, const TRCollectible& c) const
{
    if (c.collected) return;
    p.setPen(Qt::NoPen);
    if (c.type == TRCollectible::Coin) {
        p.setBrush(QColor("#FAC775"));
        p.drawEllipse(QPointF(c.x, c.y), (double)c.r, (double)c.r);
        p.setBrush(QColor("#EF9F27"));
        p.drawEllipse(QPointF(c.x, c.y), (double)(c.r-3), (double)(c.r-3));
    } else {
        QPainterPath path;
        for (int i = 0; i < 5; ++i) {
            double ao = -M_PI/2.0 + i*(2.0*M_PI/5.0);
            double ai = ao + M_PI/5.0;
            QPointF outer(c.x + c.r*qCos(ao), c.y + c.r*qSin(ao));
            QPointF inner(c.x + c.r/2.f*qCos(ai), c.y + c.r/2.f*qSin(ai));
            if (i == 0) path.moveTo(outer); else path.lineTo(outer);
            path.lineTo(inner);
        }
        path.closeSubpath();
        p.setBrush(QColor("#5DCAA5"));
        p.drawPath(path);
    }
}

void GameCanvas::drawParticles(QPainter& p) const
{
    p.setPen(Qt::NoPen);
    for (const auto& pt : m_par) {
        QColor col = pt.color;
        col.setAlphaF(static_cast<float>(pt.life));
        p.setBrush(col);
        p.drawRect(QRectF(pt.x-3, pt.y-3, 6, 6));
    }
}

void GameCanvas::drawOverlay(QPainter& p) const
{
    p.fillRect(rect(), QColor(0, 0, 0, 140));

    // ── CHANGED: overlay text uses m_cw / m_ch for positioning ──
    int centerY = m_ch / 2;

    if (m_idle) {
        p.setPen(Qt::white);
        p.setFont(QFont("Courier", 18, QFont::Bold));
        p.drawText(QRect(0, centerY - 40, m_cw, 40), Qt::AlignCenter, "PiDash");
        p.setFont(QFont("Courier", 11));
        p.setPen(QColor("#9FE1CB"));
        p.drawText(QRect(0, centerY + 5, m_cw, 24), Qt::AlignCenter,
                   "Click canvas or press Space / Up to start");
        p.setPen(QColor("#AFA9EC"));
        p.setFont(QFont("Courier", 10));
        p.drawText(QRect(0, centerY + 32, m_cw, 20), Qt::AlignCenter,
                   "Jump over obstacles  |  Collect coins & stars");
    } else { // dead
        p.setPen(QColor("#E24B4A"));
        p.setFont(QFont("Courier", 16, QFont::Bold));
        p.drawText(QRect(0, centerY - 30, m_cw, 36), Qt::AlignCenter, "GAME OVER");
        p.setPen(Qt::white);
        p.setFont(QFont("Courier", 11));
        p.drawText(QRect(0, centerY + 12, m_cw, 24), Qt::AlignCenter,
                   QString("Score: %1   XP earned: %2").arg(m_score).arg(m_xp));
    }
}

// ═════════════════════════════════════════════════════════════
//  minigame_2  –  outer widget  (mirrors Battle layout)
// ═════════════════════════════════════════════════════════════

// ── CHANGED: constructor accepts canvas dimensions ───────────
minigame_2::minigame_2(PiPet* pet, QWidget* parent, int canvasW, int canvasH)
    : QWidget(parent), m_pet(pet), m_canvasW(canvasW)
{
    m_groundY = canvasH - 60.f;    // derive ground from canvas height
    m_petY    = m_groundY;         // start pet on the ground

    setFocusPolicy(Qt::StrongFocus);

    // ── Root layout (same as Battle::root) ───────────────────
    root = new QVBoxLayout(this);
    this->setLayout(root);

    // ── Title ────────────────────────────────────────────────
    title = new QLabel("PiDash", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 22px; font-weight: bold; margin-bottom: 4px;");
    root->addWidget(title);

    // ── HUD row: Score | Lives | XP  (mirrors Battle's hpGrid) ─
    hudWidget = new QWidget();
    hudRow    = new QHBoxLayout();
    hudWidget->setLayout(hudRow);
    root->addWidget(hudWidget);

    auto makeHUDLabel = [&](const QString& text) {
        auto* l = new QLabel(text, this);
        l->setAlignment(Qt::AlignCenter);
        l->setStyleSheet("font-weight: bold; font-size: 13px;");
        return l;
    };
    scoreLabel = makeHUDLabel("Score: 0");
    livesLabel = makeHUDLabel("Lives: 3");
    xpLabel    = makeHUDLabel("XP: 0");
    hudRow->addWidget(scoreLabel);
    hudRow->addWidget(livesLabel);
    hudRow->addWidget(xpLabel);

    // ── Separator ────────────────────────────────────────────
    sep = new QFrame();
    sep->setFrameShape(QFrame::HLine);
    sep->setFrameShadow(QFrame::Sunken);
    root->addWidget(sep);

    // ── CHANGED: pass canvas dimensions to GameCanvas ────────
    canvas = new GameCanvas(canvasW, canvasH, this);
    root->addWidget(canvas, 0, Qt::AlignHCenter);
    connect(canvas, &GameCanvas::clicked, this, &minigame_2::onCanvasClicked);

    // ── Status / log labels ──────────────────────────────────
    statusLabel = new QLabel("Click the canvas or press Space to start!", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setWordWrap(true);
    statusLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
    root->addWidget(statusLabel);

    logLabel = new QLabel("", this);
    logLabel->setAlignment(Qt::AlignCenter);
    logLabel->setStyleSheet("font-size: 12px; font-style: italic;");
    root->addWidget(logLabel);

    root->addStretch();

    // ── Button row ───────────────────────────────────────────
    btnWidget = new QWidget();
    btnRow    = new QHBoxLayout();
    btnWidget->setLayout(btnRow);

    auto makeBtn = [&](const QString& text) {
        auto* b = new QPushButton(text, this);
        b->setStyleSheet(
            "QPushButton{font-size:13px;padding:10px 6px;border-radius:6px;}"
            "QPushButton:hover{background:#ddeeff;}"
            "QPushButton:disabled{color:#aaa;background:#f0f0f0;}");
        return b;
    };

    btnJump    = makeBtn("Jump  (Space)");
    btnRestart = makeBtn("New Game");
    btnBack    = new QPushButton("BACK");

    QIcon left_icon(":/images/Assets/left.png");
    btnBack->setIcon(left_icon);

    btnRow->addWidget(btnJump);
    btnRow->addWidget(btnRestart);
    root->addWidget(btnWidget);
    root->addWidget(btnBack);

    btnRestart->setVisible(false);

    connect(btnJump,    &QPushButton::clicked, this, &minigame_2::onCanvasClicked);
    connect(btnRestart, &QPushButton::clicked, this, &minigame_2::onRestart);

    btnBack->setStyleSheet(R"(
        QPushButton { background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #4850DB, stop: 1 #4A71DB);
        border: 2px inset #FBA8FF;
        border-radius: 10px;
        padding: 4px;
        font: bold; }
        QPushButton:pressed {
        background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #4A71DB, stop: 1 #4850DB);
        }
        )");

    resetState();
    m_timerId = startTimer(FRAME_MS);
    refreshUI();
}

// ═════════════════════════════════════════════════════════════
//  State helpers
// ═════════════════════════════════════════════════════════════

void minigame_2::startGame()
{
    resetState();
    m_state = GState::Playing;
    btnRestart->setVisible(false);
    btnJump->setEnabled(true);
    statusLabel->setText("Jump over obstacles! Collect coins & stars.");
    logLabel->setText("");
    refreshUI();
}

void minigame_2::resetState()
{
    m_score = 0;  m_xp    = 0;
    m_lives = 3;  m_frame = 0;
    m_speed = 3.5f;
    m_petY  = m_groundY;           // ── CHANGED: was GROUND_Y ──
    m_petVY = 0.f;
    m_jumping       = false;
    m_spawnTimer    = 0;
    m_spawnInterval = 90;
    m_flashTimer    = 0;
    m_obstacles.clear();
    m_collectibles.clear();
    m_particles.clear();
}

// ═════════════════════════════════════════════════════════════
//  Input
// ═════════════════════════════════════════════════════════════

void minigame_2::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Space || e->key() == Qt::Key_Up)
        doJump();
    else
        QWidget::keyPressEvent(e);
}

void minigame_2::onCanvasClicked() { doJump(); }

void minigame_2::doJump()
{
    if (m_state == GState::Idle || m_state == GState::Dead) {
        startGame();
        return;
    }
    if (!m_jumping) {
        m_petVY   = JUMP_VY;
        m_jumping = true;
    }
}

void minigame_2::onRestart()
{
    startGame();
}

// ═════════════════════════════════════════════════════════════
//  Timer → tick → refreshUI
// ═════════════════════════════════════════════════════════════

void minigame_2::timerEvent(QTimerEvent* e)
{
    if (e->timerId() != m_timerId) return;
    if (m_state == GState::Playing) tick();

    canvas->syncState(m_petY, m_jumping, m_frame, m_speed,
                      m_obstacles, m_collectibles, m_particles,
                      m_flashTimer,
                      m_state == GState::Idle,
                      m_state == GState::Dead,
                      m_score, m_xp, m_lives);
    canvas->update();
}

// ─────────────────────────────────────────────────────────────
void minigame_2::tick()
{
    ++m_frame;

    // ── Physics ───────────────────────────────────────────────
    m_petVY += GRAVITY;
    m_petY  += m_petVY;
    if (m_petY >= m_groundY) {         // ── CHANGED: was GROUND_Y ──
        m_petY    = m_groundY;
        m_petVY   = 0.f;
        m_jumping = false;
    }

    // ── Difficulty ramp ───────────────────────────────────────
    m_speed         = 3.5f + m_score * 0.015f;
    m_spawnInterval = qMax(50, 90 - m_score / 3);

    // ── Spawning ──────────────────────────────────────────────
    if (++m_spawnTimer >= m_spawnInterval) {
        m_spawnTimer = 0;
        spawnObstacle();
        if (QRandomGenerator::global()->bounded(100) < 60)
            spawnCollectible();
    }

    // ── Scroll ────────────────────────────────────────────────
    for (auto& o : m_obstacles)    o.x -= m_speed;
    for (auto& c : m_collectibles) c.x -= m_speed;

    m_obstacles.erase(
        std::remove_if(m_obstacles.begin(), m_obstacles.end(),
                       [](const TRObstacle& o){ return o.x < -60.f; }),
        m_obstacles.end());
    m_collectibles.erase(
        std::remove_if(m_collectibles.begin(), m_collectibles.end(),
                       [](const TRCollectible& c){ return c.x < -60.f; }),
        m_collectibles.end());

    // ── Particles ─────────────────────────────────────────────
    for (auto& p : m_particles) {
        p.x += p.vx; p.y += p.vy;
        p.vy += 0.15f; p.life -= 0.05f;
    }
    m_particles.erase(
        std::remove_if(m_particles.begin(), m_particles.end(),
                       [](const TRParticle& p){ return p.life <= 0.f; }),
        m_particles.end());

    if (m_flashTimer > 0) --m_flashTimer;

    // ── Bounding box ─────────────────────────────────────────
    const float PW = 28.f, PH = 26.f;
    const float PL = PET_X - PW / 2.f;
    const float PT = m_petY - PH;

    // ── Obstacle collision ────────────────────────────────────
    for (auto& o : m_obstacles) {
        if (o.hit) continue;
        if (overlaps(PL, PT, PW, PH, o.x, o.y, o.w, o.h)) {
            o.hit = true;
            addParticles(PET_X, m_petY - 13.f, QColor("#E24B4A"));
            m_flashTimer = 10;
            --m_lives;
            refreshUI();
            if (m_lives <= 0) {
                m_state = GState::Dead;
                endGame();
                return;
            } else {
                logLabel->setText(
                    QString("Ouch! %1 %2 left.")
                        .arg(m_lives)
                        .arg(m_lives == 1 ? "life" : "lives"));
            }
        }
    }

    // ── Collectible collision ─────────────────────────────────
    for (auto& c : m_collectibles) {
        if (c.collected) continue;
        if (overlaps(PL, PT, PW, PH,
                     c.x - c.r, c.y - c.r, c.r*2.f, c.r*2.f)) {
            c.collected = true;
            if (c.type == TRCollectible::Coin) {
                m_score += 5;
                addParticles(c.x, c.y, QColor("#FAC775"), 8);
                logLabel->setText("+5 pts — coin!");
            } else {
                m_score += 15;
                m_xp    += 5;
                addParticles(c.x, c.y, QColor("#5DCAA5"), 12);
                logLabel->setText("+15 pts  +5 XP — star!");
            }
            refreshUI();
        }
    }

    ++m_score;

    if (m_frame % 30 == 0) refreshUI();
}

// ═════════════════════════════════════════════════════════════
//  Spawn helpers
// ═════════════════════════════════════════════════════════════

void minigame_2::spawnObstacle()
{
    static const TRObstacle::Type types[3] = {
                                              TRObstacle::Rock, TRObstacle::Crate, TRObstacle::Spike };
    TRObstacle o;
    o.type = types[QRandomGenerator::global()->bounded(3)];
    o.hit  = false;
    o.x    = static_cast<float>(m_canvasW) + 10.f;  // ── CHANGED: was 600 ──
    switch (o.type) {
    case TRObstacle::Rock:  o.w = 24.f; o.h = 24.f; break;
    case TRObstacle::Crate: o.w = 24.f; o.h = 28.f; break;
    case TRObstacle::Spike: o.w = 20.f; o.h = 30.f; break;
    }
    o.y = m_groundY + 4.f - o.h;                    // ── CHANGED: was GROUND_Y ──
    m_obstacles.append(o);
}

void minigame_2::spawnCollectible()
{
    TRCollectible c;
    c.type = (QRandomGenerator::global()->bounded(100) < 25)
                 ? TRCollectible::Star : TRCollectible::Coin;
    c.r    = (c.type == TRCollectible::Star) ? 10.f : 8.f;
    c.x    = static_cast<float>(m_canvasW) + 20.f;  // ── CHANGED: was 600 ──
    c.y    = m_groundY - 20.f                        // ── CHANGED: was GROUND_Y ──
          - static_cast<float>(QRandomGenerator::global()->bounded(60));
    c.collected = false;
    m_collectibles.append(c);
}

void minigame_2::addParticles(float x, float y, const QColor& col, int n)
{
    for (int i = 0; i < n; ++i) {
        float angle = QRandomGenerator::global()->bounded(628) / 100.f;
        float spd   = 1.f + QRandomGenerator::global()->bounded(300) / 100.f;
        m_particles.append({ x, y,
                            qCos(angle)*spd, qSin(angle)*spd - 2.f,
                            1.0f, col });
    }
}

bool minigame_2::overlaps(float ax, float ay, float aw, float ah,
                          float bx, float by, float bw, float bh) const
{
    return ax < bx+bw && ax+aw > bx && ay < by+bh && ay+ah > by;
}

// ═════════════════════════════════════════════════════════════
//  UI helpers
// ═════════════════════════════════════════════════════════════

void minigame_2::refreshUI()
{
    scoreLabel->setText(QString("Score: %1").arg(m_score));
    livesLabel->setText(QString("Lives: %1").arg(m_lives));
    xpLabel->setText(   QString("XP: %1")   .arg(m_xp));
}

void minigame_2::endGame()
{
    // Don't modify m_pet here — Train::onTrackRushFinished handles rewards

    btnJump->setEnabled(false);
    btnRestart->setVisible(true);

    statusLabel->setText("You were defeated! Press New Game to retry.");
    logLabel->setText(QString("Final score: %1   XP earned: %2")
                          .arg(m_score).arg(m_xp));
    refreshUI();
    emit gameFinished(m_score, m_xp);
}
