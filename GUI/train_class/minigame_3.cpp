/* Author: Tanya Magurupira */

#include "minigame_3.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>
#include <algorithm>

// ═════════════════════════════════════════════════════════════
//  SnackCanvas
// ═════════════════════════════════════════════════════════════

SnackCanvas::SnackCanvas(int width, int height, QWidget* parent)
    : QWidget(parent), m_cw(width), m_ch(height)
{
    m_groundY = m_ch - 40.f;
    m_petX    = m_cw / 2.f;
    setFixedSize(m_cw, m_ch);
    setCursor(Qt::PointingHandCursor);
    setFocusPolicy(Qt::NoFocus);
}

void SnackCanvas::syncState(float petX, int frame,
                            const QVector<SSFood>&     foods,
                            const QVector<SSHazard>&   hazards,
                            const QVector<SSParticle>& particles,
                            int flash, bool idle, bool dead,
                            int score, int xp, int lives,
                            int comboCount)
{
    m_petX   = petX;    m_frame = frame;
    m_foods  = foods;   m_hazards = hazards;  m_particles = particles;
    m_flash  = flash;
    m_idle   = idle;    m_dead  = dead;
    m_score  = score;   m_xp    = xp;   m_lives = lives;
    m_combo  = comboCount;
}

void SnackCanvas::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);

    drawSky(p);
    drawGround(p);
    for (const auto& f : m_foods)   drawFood(p, f);
    for (const auto& h : m_hazards) drawHazard(p, h);
    drawPet(p);
    drawParticles(p);

    // combo text floating above pet
    if (m_combo >= 3 && !m_idle && !m_dead) {
        p.setPen(QColor("#FAC775"));
        p.setFont(QFont("Courier", 12, QFont::Bold));
        p.drawText(QRect(static_cast<int>(m_petX - 40),
                         static_cast<int>(m_groundY - PET_H - 30),
                         80, 20),
                   Qt::AlignCenter,
                   QString("x%1 COMBO!").arg(m_combo));
    }

    if (m_flash > 0) {
        int alpha = static_cast<int>(m_flash / 10.f * 70.f);
        p.fillRect(rect(), QColor(228, 75, 74, alpha));
    }

    if (m_idle || m_dead) drawOverlay(p);
}

// ── drawSky ──────────────────────────────────────────────────
void SnackCanvas::drawSky(QPainter& p) const
{
    QLinearGradient sky(0, 0, 0, m_ch);
    sky.setColorAt(0.0, QColor("#2D1B69"));
    sky.setColorAt(0.4, QColor("#7B2D8E"));
    sky.setColorAt(0.7, QColor("#E8654A"));
    sky.setColorAt(1.0, QColor("#FAC775"));
    p.fillRect(rect(), sky);

    const float starPx[][2] = {
        {0.08f, 0.05f}, {0.22f, 0.08f}, {0.35f, 0.03f}, {0.50f, 0.10f},
        {0.65f, 0.06f}, {0.78f, 0.12f}, {0.90f, 0.04f}, {0.15f, 0.15f},
        {0.42f, 0.14f}, {0.72f, 0.02f}
    };
    int starAlpha = static_cast<int>(160 + 80 * qSin(m_frame * 0.06f));
    p.setBrush(QColor(255, 255, 255, starAlpha));
    p.setPen(Qt::NoPen);
    for (auto& s : starPx)
        p.drawRect(static_cast<int>(s[0] * m_cw),
                   static_cast<int>(s[1] * m_ch), 2, 2);

    // drifting clouds
    p.setBrush(QColor(255, 255, 255, 40));
    float cloudOff = fmod(m_frame * 0.3f, static_cast<float>(m_cw + 120)) - 60.f;
    p.drawEllipse(QPointF(cloudOff,       m_ch * 0.18f), 30, 12);
    p.drawEllipse(QPointF(cloudOff + 22,  m_ch * 0.16f), 24, 10);
    p.drawEllipse(QPointF(cloudOff + 44,  m_ch * 0.19f), 28, 11);

    float cloud2 = fmod(m_frame * 0.2f + m_cw * 0.4f, static_cast<float>(m_cw + 120)) - 60.f;
    p.drawEllipse(QPointF(cloud2,       m_ch * 0.28f), 26, 10);
    p.drawEllipse(QPointF(cloud2 + 20,  m_ch * 0.26f), 22, 9);
}

// ── drawGround ───────────────────────────────────────────────
void SnackCanvas::drawGround(QPainter& p) const
{
    const int GY = static_cast<int>(m_groundY);
    p.setPen(Qt::NoPen);
    p.fillRect(0, GY, m_cw, m_ch - GY, QColor("#3A7D44"));
    p.fillRect(0, GY, m_cw, 4, QColor("#4CAF50"));

    p.setBrush(QColor("#4CAF50"));
    for (int tx = 10; tx < m_cw; tx += 35) {
        p.drawRect(tx, GY - 3, 3, 5);
        p.drawRect(tx + 6, GY - 5, 3, 7);
        p.drawRect(tx + 12, GY - 2, 3, 4);
    }
}

// ── drawPet ──────────────────────────────────────────────────
void SnackCanvas::drawPet(QPainter& p) const
{
    const int px = static_cast<int>(m_petX);
    const int py = static_cast<int>(m_groundY);
    p.setPen(Qt::NoPen);

    // body
    p.setBrush(QColor("#534AB7"));
    p.drawRect(px - 16, py - 28, 32, 28);

    // belly
    p.setBrush(QColor("#AFA9EC"));
    p.drawRect(px - 12, py - 24, 24, 16);

    // eyes — looking UP to watch food fall
    p.setBrush(Qt::white);
    p.drawRect(px - 10, py - 22, 9, 9);
    p.drawRect(px + 2,  py - 22, 9, 9);
    p.setBrush(QColor("#534AB7"));
    p.drawRect(px - 8, py - 22, 5, 5);
    p.drawRect(px + 4, py - 22, 5, 5);

    // ears
    p.setBrush(QColor("#AFA9EC"));
    p.drawRect(px - 20, py - 18, 6, 10);
    p.drawRect(px + 14, py - 18, 6, 10);

    // open mouth — catching food!
    p.setBrush(QColor("#E24B4A"));
    p.drawRect(px - 5, py - 8, 10, 6);

    // legs with waddle
    int swing = static_cast<int>(qSin(m_frame * 0.4) * 4);
    p.setBrush(QColor("#534AB7"));
    p.drawRect(px - 12, py,     9, 8 + swing);
    p.drawRect(px + 3,  py,     9, 8 - swing);
    p.setBrush(QColor("#AFA9EC"));
    p.drawRect(px - 14, py + 7 + swing, 12, 5);
    p.drawRect(px + 2,  py + 6 - swing, 12, 5);
}

// ── drawFood ─────────────────────────────────────────────────
void SnackCanvas::drawFood(QPainter& p, const SSFood& f) const
{
    if (f.caught) return;
    p.setPen(Qt::NoPen);
    int fx = static_cast<int>(f.x);
    int fy = static_cast<int>(f.y);

    switch (f.type) {
    case SSFood::Apple:
        p.setBrush(QColor("#E24B4A"));
        p.drawEllipse(fx - 8, fy - 8, 16, 16);
        p.setBrush(QColor("#4CAF50"));
        p.drawRect(fx - 1, fy - 12, 4, 5);
        p.drawRect(fx + 2, fy - 14, 5, 3);
        break;

    case SSFood::Pizza: {
        QPainterPath path;
        path.moveTo(fx, fy - 12);
        path.lineTo(fx - 10, fy + 8);
        path.lineTo(fx + 10, fy + 8);
        path.closeSubpath();
        p.setBrush(QColor("#FAC775"));
        p.drawPath(path);
        QPainterPath inner;
        inner.moveTo(fx, fy - 6);
        inner.lineTo(fx - 6, fy + 6);
        inner.lineTo(fx + 6, fy + 6);
        inner.closeSubpath();
        p.setBrush(QColor("#E8654A"));
        p.drawPath(inner);
        p.setBrush(QColor("#C0392B"));
        p.drawEllipse(fx - 3, fy - 1, 4, 4);
        p.drawEllipse(fx + 1, fy + 3, 3, 3);
        break;
    }

    case SSFood::Carrot: {
        QPainterPath path;
        path.moveTo(fx - 5, fy - 8);
        path.lineTo(fx + 5, fy - 8);
        path.lineTo(fx,     fy + 10);
        path.closeSubpath();
        p.setBrush(QColor("#FF8C00"));
        p.drawPath(path);
        p.setBrush(QColor("#4CAF50"));
        p.drawRect(fx - 6, fy - 12, 4, 6);
        p.drawRect(fx - 1, fy - 14, 3, 8);
        p.drawRect(fx + 3, fy - 11, 4, 5);
        break;
    }

    case SSFood::Cake:
        p.setBrush(QColor("#F5CBA7"));
        p.drawRect(fx - 10, fy - 6, 20, 14);
        p.setBrush(QColor("#FF69B4"));
        p.drawRect(fx - 10, fy - 8, 20, 6);
        p.setBrush(QColor("#E24B4A"));
        p.drawEllipse(fx - 3, fy - 13, 6, 6);
        p.setBrush(QColor("#FAC775"));
        p.drawRect(fx - 1, fy - 16, 2, 5);
        break;
    }
}

// ── drawHazard ───────────────────────────────────────────────
void SnackCanvas::drawHazard(QPainter& p, const SSHazard& h) const
{
    if (h.hit) return;
    p.setPen(Qt::NoPen);
    int hx = static_cast<int>(h.x);
    int hy = static_cast<int>(h.y);

    // bomb body
    p.setBrush(QColor("#2C3E50"));
    p.drawEllipse(hx - 10, hy - 8, 20, 20);

    // fuse
    p.setPen(QPen(QColor("#7F8C8D"), 2));
    p.drawLine(hx, hy - 8, hx + 3, hy - 14);
    p.setPen(Qt::NoPen);

    // spark
    int sparkAlpha = static_cast<int>(180 + 75 * qSin(m_frame * 0.3f));
    p.setBrush(QColor(255, 165, 0, sparkAlpha));
    p.drawEllipse(hx + 1, hy - 17, 5, 5);

    // skull face
    p.setBrush(QColor("#ECF0F1"));
    p.drawRect(hx - 4, hy - 2, 4, 4);
    p.drawRect(hx + 1, hy - 2, 4, 4);
    p.setBrush(QColor("#2C3E50"));
    p.drawRect(hx - 3, hy - 1, 2, 2);
    p.drawRect(hx + 2, hy - 1, 2, 2);
}

// ── drawParticles ────────────────────────────────────────────
void SnackCanvas::drawParticles(QPainter& p) const
{
    p.setPen(Qt::NoPen);
    for (const auto& pt : m_particles) {
        QColor col = pt.color;
        col.setAlphaF(static_cast<float>(pt.life));
        p.setBrush(col);
        p.drawRect(QRectF(pt.x - 3, pt.y - 3, 6, 6));
    }
}

// ── drawOverlay ──────────────────────────────────────────────
void SnackCanvas::drawOverlay(QPainter& p) const
{
    p.fillRect(rect(), QColor(0, 0, 0, 140));
    int centerY = m_ch / 2;

    if (m_idle) {
        p.setPen(Qt::white);
        p.setFont(QFont("Courier", 18, QFont::Bold));
        p.drawText(QRect(0, centerY - 40, m_cw, 40), Qt::AlignCenter, "PiCatcher");
        p.setFont(QFont("Courier", 11));
        p.setPen(QColor("#FAC775"));
        p.drawText(QRect(0, centerY + 5, m_cw, 24), Qt::AlignCenter,
                   "Press Left/Right or A/D to start");
        p.setPen(QColor("#AFA9EC"));
        p.setFont(QFont("Courier", 10));
        p.drawText(QRect(0, centerY + 32, m_cw, 20), Qt::AlignCenter,
                   "Catch food  |  Dodge bombs  |  Build combos!");
    } else {
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
//  minigame_3  –  outer widget
// ═════════════════════════════════════════════════════════════

minigame_3::minigame_3(PiPet* pet, QWidget* parent, int canvasW, int canvasH)
    : QWidget(parent), m_pet(pet),
    m_canvasW(canvasW), m_canvasH(canvasH)
{
    m_groundY = canvasH - 40.f;
    m_petX    = canvasW / 2.f;
    setFocusPolicy(Qt::StrongFocus);

    root = new QVBoxLayout(this);
    this->setLayout(root);

    title = new QLabel("PiCatcher", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 22px; font-weight: bold; margin-bottom: 4px;");
    root->addWidget(title);

    // ── HUD row ──────────────────────────────────────────────
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
    comboLabel = makeHUDLabel("Combo: 0");
    hudRow->addWidget(scoreLabel);
    hudRow->addWidget(livesLabel);
    hudRow->addWidget(xpLabel);
    hudRow->addWidget(comboLabel);

    sep = new QFrame();
    sep->setFrameShape(QFrame::HLine);
    sep->setFrameShadow(QFrame::Sunken);
    root->addWidget(sep);

    // ── Canvas ───────────────────────────────────────────────
    canvas = new SnackCanvas(canvasW, canvasH, this);
    root->addWidget(canvas, 0, Qt::AlignHCenter);
    connect(canvas, &SnackCanvas::clicked, this, &minigame_3::onCanvasClicked);

    statusLabel = new QLabel("Use arrow keys or A/D to move. Catch falling food!", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setWordWrap(true);
    statusLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
    root->addWidget(statusLabel);

    logLabel = new QLabel("", this);
    logLabel->setAlignment(Qt::AlignCenter);
    logLabel->setStyleSheet("font-size: 12px; font-style: italic;");
    root->addWidget(logLabel);

    root->addStretch();

    // ── Buttons ──────────────────────────────────────────────
    btnWidget = new QWidget();
    btnRow    = new QHBoxLayout();
    btnWidget->setLayout(btnRow);

    auto makeBtn = [&](const QString& text) {
        auto* b = new QPushButton(text, this);
        b->setStyleSheet(
            "QPushButton{font-size:13px;padding:10px 6px;border-radius:6px;}"
            "QPushButton:hover{background:#ddeeff;}"
            "QPushButton:disabled{color:#aaa;background:#f0f0f0;}");
        b->setFocusPolicy(Qt::NoFocus);
        return b;
    };

    btnLeft    = makeBtn("<  Left  (A)");
    btnRight   = makeBtn("Right  (D)  >");
    btnRestart = makeBtn("New Game");
    btnBack    = new QPushButton("BACK");

    QIcon left_icon(":/images/Assets/left.png");
    btnBack->setIcon(left_icon);
    btnBack->setFocusPolicy(Qt::NoFocus);

    btnRow->addWidget(btnLeft);
    btnRow->addWidget(btnRight);
    btnRow->addWidget(btnRestart);
    root->addWidget(btnWidget);
    root->addWidget(btnBack);

    btnRestart->setVisible(false);

    // hold-to-move for mouse users
    connect(btnLeft, &QPushButton::pressed, this, [this]() {
        m_moveLeft = true;
        if (m_state == GState::Idle) startGame();
    });
    connect(btnLeft, &QPushButton::released, this, [this]() {
        m_moveLeft = false;
    });
    connect(btnRight, &QPushButton::pressed, this, [this]() {
        m_moveRight = true;
        if (m_state == GState::Idle) startGame();
    });
    connect(btnRight, &QPushButton::released, this, [this]() {
        m_moveRight = false;
    });
    connect(btnRestart, &QPushButton::clicked, this, &minigame_3::onRestart);

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

void minigame_3::startGame()
{
    resetState();
    m_state = GState::Playing;
    btnRestart->setVisible(false);
    btnLeft->setEnabled(true);
    btnRight->setEnabled(true);
    statusLabel->setText("Catch food! Dodge the bombs!");
    logLabel->setText("");
    refreshUI();
}

void minigame_3::resetState()
{
    m_score = 0;   m_xp    = 0;
    m_lives = 3;   m_frame = 0;
    m_speed = 2.0f;
    m_petX  = m_canvasW / 2.f;
    m_petVX = 0.f;
    m_moveLeft = false;  m_moveRight = false;
    m_spawnTimer    = 0;
    m_spawnInterval = 55;
    m_flashTimer    = 0;
    m_comboCount    = 0;
    m_foods.clear();
    m_hazards.clear();
    m_particles.clear();
}

// ═════════════════════════════════════════════════════════════
//  Input
// ═════════════════════════════════════════════════════════════

void minigame_3::keyPressEvent(QKeyEvent* e)
{
    if (e->isAutoRepeat()) return;
    switch (e->key()) {
    case Qt::Key_Left:  case Qt::Key_A:
        m_moveLeft = true;
        if (m_state == GState::Idle) startGame();
        break;
    case Qt::Key_Right: case Qt::Key_D:
        m_moveRight = true;
        if (m_state == GState::Idle) startGame();
        break;
    default: QWidget::keyPressEvent(e);
    }
}

void minigame_3::keyReleaseEvent(QKeyEvent* e)
{
    if (e->isAutoRepeat()) return;
    switch (e->key()) {
    case Qt::Key_Left:  case Qt::Key_A:  m_moveLeft  = false; break;
    case Qt::Key_Right: case Qt::Key_D:  m_moveRight = false; break;
    default: QWidget::keyReleaseEvent(e);
    }
}

void minigame_3::onCanvasClicked()
{
    if (m_state == GState::Idle || m_state == GState::Dead)
        startGame();
}

void minigame_3::onRestart() { startGame(); }

// ═════════════════════════════════════════════════════════════
//  Timer → tick
// ═════════════════════════════════════════════════════════════

void minigame_3::timerEvent(QTimerEvent* e)
{
    if (e->timerId() != m_timerId) return;
    if (m_state == GState::Playing) tick();

    canvas->syncState(m_petX, m_frame,
                      m_foods, m_hazards, m_particles,
                      m_flashTimer,
                      m_state == GState::Idle,
                      m_state == GState::Dead,
                      m_score, m_xp, m_lives,
                      m_comboCount);
    canvas->update();
}

void minigame_3::tick()
{
    ++m_frame;

    // ── Pet movement ─────────────────────────────────────────
    float accel = 0.f;
    if (m_moveLeft)  accel -= PET_SPEED;
    if (m_moveRight) accel += PET_SPEED;
    m_petX += accel;

    const float halfW = PET_W / 2.f;
    if (m_petX < halfW)             m_petX = halfW;
    if (m_petX > m_canvasW - halfW) m_petX = m_canvasW - halfW;

    // ── Difficulty ramp ──────────────────────────────────────
    m_speed         = 2.0f + m_score * 0.008f;
    m_spawnInterval = qMax(25, 55 - m_score / 8);

    // ── Spawning ─────────────────────────────────────────────
    if (++m_spawnTimer >= m_spawnInterval) {
        m_spawnTimer = 0;
        spawnFood();
        if (QRandomGenerator::global()->bounded(100) < 25)
            spawnHazard();
    }

    // ── Fall ─────────────────────────────────────────────────
    for (auto& f : m_foods) {
        f.y += f.vy;
        f.x += f.drift * qSin(m_frame * 0.05f);
    }
    for (auto& h : m_hazards)
        h.y += h.vy;

    // ── Missed food = combo break ────────────────────────────
    for (auto& f : m_foods) {
        if (!f.caught && f.y > m_groundY + 20.f) {
            f.caught = true;
            m_comboCount = 0;
        }
    }

    // ── Remove off-screen ────────────────────────────────────
    m_foods.erase(
        std::remove_if(m_foods.begin(), m_foods.end(),
                       [&](const SSFood& f){ return f.y > m_canvasH + 30.f; }),
        m_foods.end());
    m_hazards.erase(
        std::remove_if(m_hazards.begin(), m_hazards.end(),
                       [&](const SSHazard& h){ return h.y > m_canvasH + 30.f; }),
        m_hazards.end());

    // ── Particles ────────────────────────────────────────────
    for (auto& pt : m_particles) {
        pt.x += pt.vx; pt.y += pt.vy;
        pt.vy += 0.12f; pt.life -= 0.05f;
    }
    m_particles.erase(
        std::remove_if(m_particles.begin(), m_particles.end(),
                       [](const SSParticle& pt){ return pt.life <= 0.f; }),
        m_particles.end());

    if (m_flashTimer > 0) --m_flashTimer;

    // ── Pet bounding box ─────────────────────────────────────
    const float PL = m_petX - PET_W / 2.f;
    const float PT = m_groundY - PET_H;

    // ── Food collision ───────────────────────────────────────
    for (auto& f : m_foods) {
        if (f.caught) continue;
        if (overlaps(PL, PT, PET_W, PET_H,
                     f.x - f.w/2, f.y - f.h/2, f.w, f.h)) {
            f.caught = true;
            ++m_comboCount;

            int multi = 1;
            if (m_comboCount >= 10)     multi = 3;
            else if (m_comboCount >= 5) multi = 2;

            int pts = f.points * multi;
            int xp  = f.xp * multi;
            m_score += pts;
            m_xp    += xp;

            QColor particleCol;
            QString foodName;
            switch (f.type) {
            case SSFood::Apple:  particleCol = QColor("#E24B4A"); foodName = "Apple";  break;
            case SSFood::Pizza:  particleCol = QColor("#FAC775"); foodName = "Pizza";  break;
            case SSFood::Carrot: particleCol = QColor("#FF8C00"); foodName = "Carrot"; break;
            case SSFood::Cake:   particleCol = QColor("#FF69B4"); foodName = "Cake";   break;
            }
            addParticles(f.x, f.y, particleCol, 8);

            QString msg = QString("+%1 pts").arg(pts);
            if (xp > 0) msg += QString("  +%1 XP").arg(xp);
            if (multi > 1) msg += QString("  (x%1!)").arg(multi);
            msg += QString(" — %1!").arg(foodName);
            logLabel->setText(msg);
            refreshUI();
        }
    }

    // ── Hazard collision ─────────────────────────────────────
    for (auto& h : m_hazards) {
        if (h.hit) continue;
        if (overlaps(PL, PT, PET_W, PET_H,
                     h.x - h.w/2, h.y - h.h/2, h.w, h.h)) {
            h.hit = true;
            addParticles(h.x, h.y, QColor("#2C3E50"), 12);
            m_flashTimer = 10;
            m_comboCount = 0;
            --m_lives;
            refreshUI();
            if (m_lives <= 0) {
                m_state = GState::Dead;
                endGame();
                return;
            } else {
                logLabel->setText(
                    QString("BOOM! %1 %2 left.")
                        .arg(m_lives)
                        .arg(m_lives == 1 ? "life" : "lives"));
            }
        }
    }

    if (m_frame % 30 == 0) refreshUI();
}

// ═════════════════════════════════════════════════════════════
//  Spawn helpers
// ═════════════════════════════════════════════════════════════

void minigame_3::spawnFood()
{
    SSFood f;
    int roll = QRandomGenerator::global()->bounded(100);
    if (roll < 40) {
        f.type = SSFood::Apple;   f.w = 16; f.h = 16; f.points = 5;  f.xp = 0;
    } else if (roll < 65) {
        f.type = SSFood::Pizza;   f.w = 20; f.h = 20; f.points = 10; f.xp = 2;
    } else if (roll < 85) {
        f.type = SSFood::Carrot;  f.w = 14; f.h = 22; f.points = 8;  f.xp = 1;
    } else {
        f.type = SSFood::Cake;    f.w = 20; f.h = 20; f.points = 20; f.xp = 5;
    }

    f.x     = f.w / 2.f + QRandomGenerator::global()->bounded(
              qMax(1, m_canvasW - static_cast<int>(f.w)));
    f.y     = -20.f;
    f.vy    = m_speed + QRandomGenerator::global()->bounded(100) / 100.f;
    f.drift = (QRandomGenerator::global()->bounded(200) - 100) / 100.f;
    f.caught = false;
    m_foods.append(f);
}

void minigame_3::spawnHazard()
{
    SSHazard h;
    h.w  = 20;  h.h = 20;
    h.x  = 10.f + QRandomGenerator::global()->bounded(qMax(1, m_canvasW - 20));
    h.y  = -20.f;
    h.vy = m_speed * 0.85f + QRandomGenerator::global()->bounded(80) / 100.f;
    h.hit = false;
    m_hazards.append(h);
}

void minigame_3::addParticles(float x, float y, const QColor& col, int n)
{
    for (int i = 0; i < n; ++i) {
        float angle = QRandomGenerator::global()->bounded(628) / 100.f;
        float spd   = 1.f + QRandomGenerator::global()->bounded(300) / 100.f;
        m_particles.append({ x, y,
                            qCos(angle)*spd, qSin(angle)*spd - 2.f,
                            1.0f, col });
    }
}

bool minigame_3::overlaps(float ax, float ay, float aw, float ah,
                          float bx, float by, float bw, float bh) const
{
    return ax < bx+bw && ax+aw > bx && ay < by+bh && ay+ah > by;
}

// ═════════════════════════════════════════════════════════════
//  UI helpers
// ═════════════════════════════════════════════════════════════

void minigame_3::refreshUI()
{
    scoreLabel->setText(QString("Score: %1").arg(m_score));
    livesLabel->setText(QString("Lives: %1").arg(m_lives));
    xpLabel->setText(   QString("XP: %1")   .arg(m_xp));
    comboLabel->setText(QString("Combo: %1").arg(m_comboCount));
}

void minigame_3::endGame()
{
    btnLeft->setEnabled(false);
    btnRight->setEnabled(false);
    btnRestart->setVisible(true);

    statusLabel->setText("No more lives! Press New Game to retry.");
    logLabel->setText(QString("Final score: %1   XP earned: %2")
                          .arg(m_score).arg(m_xp));
    refreshUI();
    emit gameFinished(m_score, m_xp);
}

