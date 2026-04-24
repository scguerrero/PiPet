/* Author: Tanya Magurupira */

#include "pidash.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>
#include <algorithm>

// ═════════════════════════════════════════════════════════════
//  GameCanvas  — draws track, obstacles, collectibles, particles
//  The pet sprite is rendered by piDash::m_character (a Character
//  widget) layered on top of this canvas, NOT drawn here.
// ═════════════════════════════════════════════════════════════

GameCanvas::GameCanvas(int width, int height, QWidget* parent)
    : QWidget(parent), m_cw(width), m_ch(height)
{
    m_groundY = m_ch - 60.f;
    m_petY    = m_groundY;
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

void GameCanvas::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);

    drawBackground(p);
    drawTrack(p);
    for (const auto& o : m_obs) drawObstacle(p, o);
    for (const auto& c : m_col) drawCollectible(p, c);
    drawParticles(p);

    if (m_flash > 0) {
        int alpha = static_cast<int>(m_flash / 10.f * 70.f);
        p.fillRect(rect(), QColor(228, 75, 74, alpha));
    }

    if (m_idle || m_dead) drawOverlay(p);
}

void GameCanvas::drawBackground(QPainter& p) const
{
    QLinearGradient bg(0, 0, 0, m_ch);
    bg.setColorAt(0.0,  QColor("#00C8FF"));
    bg.setColorAt(0.4,  QColor("#006DFF"));
    bg.setColorAt(0.75, QColor("#1A1F7A"));
    bg.setColorAt(1.0,  QColor("#0A0D33"));
    p.fillRect(rect(), bg);

    QColor vignette(0, 0, 40, 70);
    p.fillRect(rect(), vignette);
}

void GameCanvas::drawTrack(QPainter& p) const
{
    const int GY = static_cast<int>(m_groundY);
    p.setPen(Qt::NoPen);
    p.fillRect(0, GY + 4, m_cw, 60, QColor("#16213e"));
    p.fillRect(0, GY + 4, m_cw,  6, QColor("#0f3460"));
    p.fillRect(0, GY + 30, m_cw, 6, QColor("#0f3460"));

    p.setBrush(QColor("#0f3460"));
    int tieOff = static_cast<int>(m_frame * m_speed) % 40;
    for (int tx = -40 + tieOff; tx < m_cw; tx += 40)
        p.drawRect(tx, GY + 4, 12, 32);
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
        p.drawLine(QPointF(o.x+12, o.y),    QPointF(o.x+12, o.y+o.h));
        p.drawLine(QPointF(o.x,    o.y+14), QPointF(o.x+o.w, o.y+14));
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

    int centerY = m_ch / 2;

    if (m_idle) {
        p.setPen(Qt::white);
        p.setFont(QFont("Courier", 18, QFont::Bold));
        p.drawText(QRect(0, centerY - 40, m_cw, 40), Qt::AlignCenter, "PiDash");
        p.setFont(QFont("Courier", 11));
        p.setPen(QColor("#9FE1CB"));
        p.drawText(QRect(0, centerY + 5, m_cw, 24), Qt::AlignCenter,
                   "Your piPet is ready to dash!");
        p.setPen(QColor("#AFA9EC"));
        p.setFont(QFont("Courier", 10));
        p.drawText(QRect(0, centerY + 32, m_cw, 20), Qt::AlignCenter,
                   "Tap or press Space / Up to begin");
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
//  piDash  –  outer widget
// ═════════════════════════════════════════════════════════════

piDash::piDash(PiPet*  pet,
               Player* player,
               Character::PetType petType,
               QWidget* parent,
               int canvasW, int canvasH)
    : QWidget(parent),
    m_pet(pet),
    m_player(player),
    m_petType(petType),
    m_canvasW(canvasW)
{
    m_groundY = canvasH - 60.f;
    m_petY    = m_groundY;

    setFocusPolicy(Qt::StrongFocus);

    // Root layout
    root = new QVBoxLayout(this);
    this->setLayout(root);

    // Top row: Back button pinned left, title centered
    QWidget*     topRow    = new QWidget(this);
    QHBoxLayout* topLayout = new QHBoxLayout(topRow);
    topLayout->setContentsMargins(4, 4, 4, 0);

    btnBack = new QPushButton("Back", this);
    btnBack->setIcon(QIcon(":/images/Assets/left.png"));
    btnBack->setFixedWidth(80);
    btnBack->setStyleSheet(R"(
        QPushButton {
            background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,
                stop: 0 #4850DB, stop: 1 #4A71DB);
            border: 2px inset #FBA8FF;
            border-radius: 10px;
            padding: 4px;
            font: bold;
            color: mistyrose;
        }
        QPushButton:pressed {
            background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,
                stop: 0 #4A71DB, stop: 1 #4850DB);
        }
    )");

    title = new QLabel("PiDash", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 22px; font-weight: bold; margin-bottom: 4px;");

    topLayout->addWidget(btnBack);
    topLayout->addWidget(title, 1);
    topLayout->addSpacing(80); // mirror back button width to keep title centred

    root->addWidget(topRow);

    // HUD row
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

    // Separator
    sep = new QFrame();
    sep->setFrameShape(QFrame::HLine);
    sep->setFrameShadow(QFrame::Sunken);
    root->addWidget(sep);

    // ── Canvas container ────────────────────────────────────────
    // The canvas draws background / track / obstacles / collectibles.
    // The Character widget floats on TOP of the canvas as a sibling
    // child of piDash, positioned via positionCharacterWidget() each tick.
    canvas = new GameCanvas(canvasW, canvasH, this);
    connect(canvas, &GameCanvas::clicked, this, &piDash::onCanvasClicked);

    // We use a QWidget wrapper so we can stack the Character on top
    // without fighting QVBoxLayout's fixed-size logic.
    QWidget* canvasContainer = new QWidget(this);
    canvasContainer->setFixedSize(canvasW, canvasH);
    canvas->setParent(canvasContainer);
    canvas->move(0, 0);

    // ── Character widget  (same construction as mindReader) ─────
    //   Parent = canvasContainer so it floats inside the game area.
    m_character = new Character(canvasContainer);
    m_character->setFixedSize(SPRITE_SIZE, SPRITE_SIZE);
    m_character->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_character->raise();   // always on top of canvas pixels

    // Sync sprite + hat on first open — identical call to mindReader
    if (m_player)
        m_character->syncWithPlayer(*m_player, m_petType);

    // Apply hat gif if one is equipped (mirrors mindReader::applyHatSprite)
    applyHatSprite();

    // Position sprite at the initial ground level
    positionCharacterWidget();

    // Result panel — child of canvasContainer so it overlays the game on game-over
    resultPanel = new QWidget(canvasContainer);
    resultPanel->setStyleSheet(
        "QWidget { background-color: rgba(18,8,40,210); border-radius: 12px; }");
    resultPanel->setFixedWidth(canvasW - 40);
    auto *resLayout = new QVBoxLayout(resultPanel);
    resLayout->setContentsMargins(20, 20, 20, 20);
    resLayout->setSpacing(10);

    resultLabel = new QLabel("", resultPanel);
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setWordWrap(true);
    resultLabel->setStyleSheet(
        "QLabel { color: #ffd700; font-size: 15px; font-weight: bold; background: transparent; }");

    resLayout->addWidget(resultLabel);
    resultPanel->hide();

    root->addWidget(canvasContainer, 0, Qt::AlignHCenter);

    // Status / log
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

    // Button row
    btnWidget = new QWidget();
    btnRow    = new QHBoxLayout();
    btnWidget->setLayout(btnRow);

    auto makeBtn = [&](const QString& text) {
        auto* b = new QPushButton(text, this);
        b->setStyleSheet(R"(
            QPushButton {
                background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                    stop:0 #4850DB, stop:1 #4A71DB);
                border: 2px inset #FBA8FF;
                border-radius: 10px;
                padding: 6px;
                font: bold 14px;
                color: mistyrose;
            }
            QPushButton:pressed {
                background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                    stop:0 #4A71DB, stop:1 #4850DB);
            }
        )");
        return b;
    };

    btnJump    = makeBtn("Jump  (Space)");
    btnRestart = makeBtn("New Game");

    btnRow->addWidget(btnJump);
    btnRow->addWidget(btnRestart);
    root->addWidget(btnWidget);

    btnRestart->setVisible(false);

    connect(btnJump,    &QPushButton::clicked, this, &piDash::onCanvasClicked);
    connect(btnRestart, &QPushButton::clicked, this, &piDash::onRestart);

    resetState();
    m_timerId = startTimer(FRAME_MS);
    refreshUI();
}

// ═════════════════════════════════════════════════════════════
//  Character / hat sync  — mirrors mindReader exactly
// ═════════════════════════════════════════════════════════════

void piDash::refreshCharacter()
{
    applyHatSprite();
}

void piDash::applyHatSprite()
{
    if (!m_player || !m_character) return;

    QString hat = m_player->getPet().hat();
    if (hat.isEmpty()) {
        m_character->syncWithPlayer(*m_player, m_petType);
        return;
    }

    QString folder, prefix;
    QString type = m_player->getPet().pet_type();
    if      (type == "ElectricAxolotl") { folder = "axolotl";   prefix = "axolotl";   }
    else if (type == "SeelCat")         { folder = "seelcat";   prefix = "seelcat";   }
    else                                { folder = "dragondog"; prefix = "dragondog"; }

    QString stage = m_player->getPet().age_group();
    QString infix = (stage == "Teen") ? "teen_" : (stage == "Adult") ? "adult_" : "";
    QString path  = QString(":/images/Sprites/pets/%1/%2_%3%4.gif")
                       .arg(folder, prefix, infix, hat);

    QMovie *movie = new QMovie(path, QByteArray(), m_character);
    if (movie->isValid()) {
        QLabel *disp = m_character->findChild<QLabel *>();
        if (disp) {
            if (disp->movie()) disp->movie()->stop();
            disp->setMovie(movie);
            movie->start();
            return;
        }
    }
    delete movie;
    m_character->syncWithPlayer(*m_player, m_petType);
}

// ─── Keep the Character widget positioned at the pet's Y on the canvas ───────
//
//   PET_X is the horizontal centre on the canvas (80 px from left).
//   petY is the bottom of the pet's bounding box, so the top of the
//   Character widget = petY - SPRITE_SIZE.
//   We clamp so it never slides above the canvas top.

void piDash::positionCharacterWidget()
{
    if (!m_character) return;
    int x = static_cast<int>(PET_X) - SPRITE_SIZE / 2;
    int y = static_cast<int>(m_petY) - SPRITE_SIZE;
    y = qMax(y, 0);
    m_character->move(x, y);
}

// ═════════════════════════════════════════════════════════════
//  State helpers
// ═════════════════════════════════════════════════════════════

void piDash::startGame()
{
    resetState();
    m_state = GState::Playing;
    m_sessionFinished = false;

    btnRestart->setVisible(false);
    btnJump->setVisible(true);
    btnJump->setEnabled(true);

    resultPanel->hide();
    canvas->show();
    m_character->show();
    hudWidget->show();
    btnWidget->show();
    statusLabel->show();
    logLabel->show();

    statusLabel->setText("Jump over obstacles! Collect coins & stars.");
    logLabel->setText("");
    refreshUI();
}

void piDash::resetState()
{
    m_score = 0;  m_xp    = 0;
    m_lives = 3;  m_frame = 0;
    m_speed = 3.5f;
    m_petY  = m_groundY;
    m_petVY = 0.f;
    m_jumping       = false;
    m_spawnTimer    = 0;
    m_spawnInterval = 90;
    m_flashTimer    = 0;
    m_obstacles.clear();
    m_collectibles.clear();
    m_particles.clear();

    positionCharacterWidget();
}

// ═════════════════════════════════════════════════════════════
//  Input
// ═════════════════════════════════════════════════════════════

void piDash::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Space || e->key() == Qt::Key_Up)
        doJump();
    else
        QWidget::keyPressEvent(e);
}

void piDash::onCanvasClicked() { doJump(); }

void piDash::doJump()
{
    if (m_state == GState::Dead) return;

    if (m_state == GState::Idle) {
        startGame();
        return;
    }
    if (!m_jumping) {
        m_petVY   = JUMP_VY;
        m_jumping = true;
    }
}

void piDash::onRestart()
{
    startGame();
}

// ═════════════════════════════════════════════════════════════
//  Timer → tick → refreshUI
// ═════════════════════════════════════════════════════════════

void piDash::timerEvent(QTimerEvent* e)
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

    // Keep Character widget in sync with petY every frame
    positionCharacterWidget();
}

void piDash::tick()
{
    ++m_frame;

    // Physics
    m_petVY += GRAVITY;
    m_petY  += m_petVY;
    if (m_petY >= m_groundY) {
        m_petY    = m_groundY;
        m_petVY   = 0.f;
        m_jumping = false;
    }

    // Difficulty ramp
    m_speed         = 3.5f + m_score * 0.015f;
    m_spawnInterval = qMax(50, 90 - m_score / 3);

    // Spawning
    if (++m_spawnTimer >= m_spawnInterval) {
        m_spawnTimer = 0;
        spawnObstacle();
        if (QRandomGenerator::global()->bounded(100) < 60)
            spawnCollectible();
    }

    // Scroll
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

    // Particles
    for (auto& p : m_particles) {
        p.x += p.vx; p.y += p.vy;
        p.vy += 0.15f; p.life -= 0.05f;
    }
    m_particles.erase(
        std::remove_if(m_particles.begin(), m_particles.end(),
                       [](const TRParticle& p){ return p.life <= 0.f; }),
        m_particles.end());

    if (m_flashTimer > 0) --m_flashTimer;

    // Bounding box (unchanged — Character widget size doesn't affect physics)
    const float PW = 28.f, PH = 26.f;
    const float PL = PET_X - PW / 2.f;
    const float PT = m_petY - PH;

    // Obstacle collision
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

    // Collectible collision
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

void piDash::spawnObstacle()
{
    static const TRObstacle::Type types[3] = {
        TRObstacle::Rock, TRObstacle::Crate, TRObstacle::Spike
    };
    TRObstacle o;
    o.type = types[QRandomGenerator::global()->bounded(3)];
    o.hit  = false;
    o.x    = static_cast<float>(m_canvasW) + 10.f;
    switch (o.type) {
    case TRObstacle::Rock:  o.w = 24.f; o.h = 24.f; break;
    case TRObstacle::Crate: o.w = 24.f; o.h = 28.f; break;
    case TRObstacle::Spike: o.w = 20.f; o.h = 30.f; break;
    }
    o.y = m_groundY + 4.f - o.h;
    m_obstacles.append(o);
}

void piDash::spawnCollectible()
{
    TRCollectible c;
    c.type = (QRandomGenerator::global()->bounded(100) < 25)
                 ? TRCollectible::Star : TRCollectible::Coin;
    c.r    = (c.type == TRCollectible::Star) ? 10.f : 8.f;
    c.x    = static_cast<float>(m_canvasW) + 20.f;
    c.y    = m_groundY - 20.f
          - static_cast<float>(QRandomGenerator::global()->bounded(60));
    c.collected = false;
    m_collectibles.append(c);
}

void piDash::addParticles(float x, float y, const QColor& col, int n)
{
    for (int i = 0; i < n; ++i) {
        float angle = QRandomGenerator::global()->bounded(628) / 100.f;
        float spd   = 1.f + QRandomGenerator::global()->bounded(300) / 100.f;
        m_particles.append({ x, y,
                            qCos(angle)*spd, qSin(angle)*spd - 2.f,
                            1.0f, col });
    }
}

bool piDash::overlaps(float ax, float ay, float aw, float ah,
                      float bx, float by, float bw, float bh) const
{
    return ax < bx+bw && ax+aw > bx && ay < by+bh && ay+ah > by;
}

// ═════════════════════════════════════════════════════════════
//  UI helpers
// ═════════════════════════════════════════════════════════════

void piDash::refreshUI()
{
    scoreLabel->setText(QString("Score: %1").arg(m_score));
    livesLabel->setText(QString("Lives: %1").arg(m_lives));
    xpLabel->setText(   QString("XP: %1")   .arg(m_xp));
}

void piDash::endGame()
{
    if (m_sessionFinished) return;
    m_sessionFinished = true;

    btnJump->setVisible(false);
    btnRestart->setVisible(true);
    refreshUI();

    QString rating;
    if      (m_score > 2000) rating = "Perfect run! Your pet is amazed!";
    else if (m_score > 1200) rating = "Great job! Your pet is proud!";
    else if (m_score > 600)  rating = "Your pet is happy!";
    else                     rating = "Your pet believes in you!";

    resultLabel->setText(
        QString("<span style='color:#E24B4A;font-size:18px;'>GAME OVER</span>"
                "<br><br>%1<br><br>"
                "Score: %2 &nbsp;&nbsp; XP Earned: %3<br>"
                "<span style='color:#AFA9EC;font-size:12px;'>Press New Game to retry</span>")
            .arg(rating).arg(m_score).arg(m_xp));

    resultPanel->adjustSize();
    resultPanel->move((m_canvasW - resultPanel->width())  / 2,
                      (canvas->height() - resultPanel->height()) / 2);
    resultPanel->show();
    resultPanel->raise();

    canvas->show();
    hudWidget->show();
    btnWidget->show();
    statusLabel->hide();
    logLabel->hide();

    emit gameFinished(m_score, m_xp);
}