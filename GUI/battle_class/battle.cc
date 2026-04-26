/*
 * In single-player battle, the PiPet battles against the computer.
 * Added: Dummy particle area for effect and interactable buttons,
 *
 * Author(s): Luke Cerwin
 */

#include "battle.h"
#include <QFrame>
#include <QtMath>

// ── Dummy hit-zone in image-space (0-1 normalised).
static constexpr float kDummyNX  = 0.22f;   // normalised centre-x  (left side — dummy)
static constexpr float kDummyNY  = 0.42f;   // normalised centre-y  (head / upper chest)

// ── Palette of wood-chip colours ─────────────────────────────────────────
static const QColor kWoodPalette[] = {
    QColor(0x6B, 0x3E, 0x26),   // dark brown
    QColor(0x8B, 0x5E, 0x3C),   // mid brown
    QColor(0xA0, 0x72, 0x50),   // light brown
    QColor(0xC4, 0x9A, 0x6D),   // tan
    QColor(0x5C, 0x33, 0x17),   // very dark
};

Battle::Battle(QWidget *parent) : QWidget(parent) {
    m_bg.load(":/images/Backgrounds/battle_16bit.png");

    // Particle tick timer (60 fps-ish)
    m_particleTimer = new QTimer(this);
    m_particleTimer->setInterval(16);
    connect(m_particleTimer, &QTimer::timeout, this, &Battle::tickParticles);
    // Started only when particles exist

    setAttribute(Qt::WA_OpaquePaintEvent, false);

    root = new QVBoxLayout();
    root->setContentsMargins(8, 8, 8, 8);
    root->setSpacing(4);
    this->setLayout(root);

    // ── Title ─────────────────────────────────────────────────────────────
    title = new QLabel("Wentz Dojo", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-weight: bold; margin-bottom: 4px;"
        "color: mistyrose; background-color: #0247a7;"
        "border-radius: 8px; padding: 4px 12px;");
    root->addWidget(title);

    // ── HP bars ───────────────────────────────────────────────────────────
    hpWidget = new QWidget();
    hpWidget->setAttribute(Qt::WA_TranslucentBackground);
    hpGrid = new QGridLayout();
    hpWidget->setLayout(hpGrid);
    hpWidget->setStyleSheet("background-color: rgba(0,0,0,140); border-radius: 8px;");
    root->addWidget(hpWidget);

    auto makeNameLabel = [&](const QString &text, Qt::Alignment align) {
        auto *l = new QLabel(text, this);
        l->setAlignment(align);
        l->setStyleSheet("font-weight: bold; color: mistyrose; background: #4b56d2; padding: 4px; border-radius: 8px");
        return l;
    };

    // CPU left you right
    hpGrid->addWidget(makeNameLabel("Wing Chun", Qt::AlignLeft),  0, 0);
    hpGrid->addWidget(makeNameLabel("You",       Qt::AlignRight), 0, 2);

    cpuBar = new QProgressBar();
    cpuBar->setRange(0, maxHP); cpuBar->setValue(maxHP);
    cpuBar->setTextVisible(false);
    cpuBar->setInvertedAppearance(false);
    cpuBar->setStyleSheet("QProgressBar::chunk{background:#f44336;}");

    playerBar = new QProgressBar();
    playerBar->setRange(0, maxHP); playerBar->setValue(maxHP);
    playerBar->setTextVisible(false);
    playerBar->setInvertedAppearance(true);
    playerBar->setStyleSheet("QProgressBar::chunk{background:#4caf50;}");

    hpGrid->addWidget(cpuBar,    1, 0);
    hpGrid->addWidget(new QLabel("vs"), 1, 1, Qt::AlignCenter);
    hpGrid->addWidget(playerBar, 1, 2);
    hpGrid->setColumnStretch(0, 1); hpGrid->setColumnStretch(2, 1);

    cpuHPLabel    = new QLabel(); cpuHPLabel->setAlignment(Qt::AlignLeft);
    playerHPLabel = new QLabel(); playerHPLabel->setAlignment(Qt::AlignRight);
    hpGrid->addWidget(cpuHPLabel,    2, 0);
    hpGrid->addWidget(playerHPLabel, 2, 2);

    cpuHPLabel->setStyleSheet("color: mistyrose; background: #1b264f; padding: 4px; border-radius: 8px;");
    playerHPLabel->setStyleSheet("color: mistyrose; background: #1b264f; padding: 4px; border-radius: 8px;");

    // ── Result / log labels constructed here
    logLabel = new QLabel("");
    logLabel->setAlignment(Qt::AlignCenter);
    logLabel->setStyleSheet(
        "font-style: italic; color: mistyrose;"
        "background-color: #1b264f; border-radius: 6px; padding: 4px;");

    resultLabel = new QLabel("Choose your move!");
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setWordWrap(true);
    resultLabel->setStyleSheet(
        "font-weight: bold; color: mistyrose;"
        "background-color: #0247a7; border-radius: 6px; padding: 4px;");

    // ── Player character sprite
    m_character = new Character(this);
    m_character->setFixedSize(160, 160);
    root->addSpacing(110);
    root->addWidget(m_character, 0, Qt::AlignRight);

    root->addStretch();

    // ── Log + result ────────────────
    root->addWidget(logLabel);
    root->addWidget(resultLabel);
    root->addSpacing(5);

    // ── Action buttons
    btnRow = new QHBoxLayout();
    btnRow->setSpacing(8);
    btnWidget = new QWidget();
    btnWidget->setLayout(btnRow);
    btnWidget->setAttribute(Qt::WA_TranslucentBackground);

    auto makeBtn = [&](const QString &text) {
        auto *b = new QPushButton(text, this);
        b->setStyleSheet(
            "QPushButton{padding:10px 6px;border-radius:6px;"
            "background-color:rgba(72,80,219,200); color:mistyrose; font-weight:bold;"
            "border: 2px inset #FBA8FF;}"
            "QPushButton:hover{background:rgba(100,120,240,220);}"
            "QPushButton:disabled{background:rgba(80,80,80,160);}");
        return b;
    };

    btnAttack  = makeBtn("Attack");
    btnCharge  = makeBtn("Charge");
    btnDefend  = makeBtn("Defend (5)");
    btnRestart = makeBtn("New Game");

    QIcon attack_icon(":/images/Assets/attack.png");
    QIcon charge_icon(":/images/Assets/charge.png");
    QIcon defend_icon(":/images/Assets/defend.png");
    QIcon restart_icon(":/images/Assets/restart.png");
    btnAttack->setIcon(attack_icon);
    btnCharge->setIcon(charge_icon);
    btnDefend->setIcon(defend_icon);
    btnRestart->setIcon(restart_icon);

    btnRow->addWidget(btnAttack);
    btnRow->addWidget(btnCharge);
    btnRow->addWidget(btnDefend);
    root->addWidget(btnWidget);

    btnRestart->setVisible(false);
    root->addWidget(btnRestart, 0, Qt::AlignCenter);

    connect(btnAttack,  &QPushButton::clicked, this, &Battle::onAttack);
    connect(btnCharge,  &QPushButton::clicked, this, &Battle::onCharge);
    connect(btnDefend,  &QPushButton::clicked, this, &Battle::onDefend);
    connect(btnRestart, &QPushButton::clicked, this, &Battle::onRestart);

    refreshUI();
}

// Hat-aware player sync
void Battle::setPlayerInfo(Player *player, Character::PetType petType) {
    m_player  = player;
    m_petType = petType;
    if (!m_player) return;

    delete m_hatMovie;
    m_hatMovie = nullptr;

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

    QMovie *movie = new QMovie(path, QByteArray(), this);
    if (movie->isValid()) {
        QLabel *disp = m_character->findChild<QLabel *>();
        if (disp) {
            if (disp->movie()) disp->movie()->stop();
            disp->setMovie(movie);
            movie->start();
            m_hatMovie = movie;
            return;
        }
    }
    delete movie;
    m_character->syncWithPlayer(*m_player, m_petType);
}

// Particle Effects
void Battle::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    // Draw background scaled to widget
    if (!m_bg.isNull())
        p.drawPixmap(0, 0, width(), height(), m_bg);

    // Draw wood particles
    for (const WoodParticle &chip : m_particles) {
        p.save();
        p.translate(chip.pos);
        p.rotate(chip.rotation);
        QColor c = chip.color;
        c.setAlphaF(chip.alpha);
        p.setBrush(c);
        // Darker outline
        QColor outline = c.darker(160);
        outline.setAlphaF(chip.alpha * 0.8f);
        p.setPen(QPen(outline, 1));
        p.drawRect(QRectF(-chip.w * 0.5f, -chip.h * 0.5f, chip.w, chip.h));
        p.restore();
    }

    QWidget::paintEvent(e);
}

// Particle spawner
void Battle::spawnWoodParticles(int count)
{
    float cx = kDummyNX * width();
    float cy = kDummyNY * height();

    auto *rng = QRandomGenerator::global();

    for (int i = 0; i < count; ++i) {
        WoodParticle chip;

        // Spawn with slight random offset inside the hit zone (~20 px spread)
        chip.pos.setX(cx + rng->bounded(40) - 20);
        chip.pos.setY(cy + rng->bounded(40) - 20);
        float angle   = static_cast<float>(rng->bounded(260)) - 40.0f;
        float speed   = 2.5f + static_cast<float>(rng->bounded(40)) * 0.15f;
        float rad     = qDegreesToRadians(angle);
        chip.vel.setX(qCos(rad) * speed);
        chip.vel.setY(qSin(rad) * speed);

        chip.rotation = static_cast<float>(rng->bounded(360));
        chip.rotSpeed = (rng->bounded(14) - 7) * 2.0f;

        chip.alpha    = 0.85f + static_cast<float>(rng->bounded(15)) * 0.01f;
        chip.fade     = 0.018f + static_cast<float>(rng->bounded(20)) * 0.001f;

        chip.w = 4.0f + rng->bounded(6);
        chip.h = 2.0f + rng->bounded(4);
        chip.color = kWoodPalette[rng->bounded(5)];
        m_particles.append(chip);
    }

    if (!m_particleTimer->isActive())
        m_particleTimer->start();
}

// Particle tick Logic
void Battle::tickParticles()
{
    for (int i = m_particles.size() - 1; i >= 0; --i) {
        WoodParticle &chip = m_particles[i];
        chip.pos      += chip.vel;
        chip.vel.setY(chip.vel.y() + 0.18f); // gravity
        chip.rotation += chip.rotSpeed;
        chip.alpha    -= chip.fade;
        if (chip.alpha <= 0.0f)
            m_particles.removeAt(i);
    }
    update(); // repaint
    if (m_particles.isEmpty())
        m_particleTimer->stop();
}

// Buttons
void Battle::onAttack()  { playTurn(Move::Attack); }
void Battle::onCharge()  { playTurn(Move::Charge); }
void Battle::onDefend()  { playTurn(Move::Defend); }

//Battle Logic Restart
void Battle::onRestart()
{
    playerHP = cpuHP = maxHP;
    playerAtk = cpuAtk = baseAtk;
    playerDefends = maxDefends;
    playerSkip = cpuSkip = false;
    btnAttack->setVisible(true);
    btnCharge->setVisible(true);
    btnDefend->setVisible(true);
    btnAttack->setEnabled(true);
    btnCharge->setEnabled(true);
    btnDefend->setEnabled(true);
    btnRestart->setVisible(false);
    resultLabel->setText("Choose your move!");
    logLabel->setText("");
    refreshUI();
}

// CPU AI
Move Battle::cpuMove()
{
    switch (QRandomGenerator::global()->bounded(3)) {
    case 0:  return Move::Attack;
    case 1:  return Move::Charge;
    default: return Move::Defend;
    }
}

void Battle::playTurn(Move pm)
{
    QString result, log;

    // PLAYER GETS TURN SKIPPED
    if (playerSkip) {
        playerSkip = false;
        btnAttack->setEnabled(true);
        btnCharge->setEnabled(true);
        btnDefend->setEnabled(playerDefends > 0);

        Move cm = cpuMove();
        log = QString("You: [skipped]  |  CPU: %1").arg(cm == Move::Attack ? "Attack " : cm == Move::Charge ? "Charge " : "Defend ");

        if (cm == Move::Attack) {
            playerHP -= cpuAtk;
            result = QString("You were stunned! CPU attacked freely for %1 damage.").arg(cpuAtk);
            // CPU hitting the (conceptual) dummy — particles still fire from the dummy
            spawnWoodParticles(14);
        } else if (cm == Move::Charge) {
            cpuAtk += chargeBonus;
            result = QString("You were stunned! CPU charged. CPU attack is now %1.").arg(cpuAtk);
        } else {
            result = "You were stunned! CPU defended.";
        }

        playerHP = qMax(0, playerHP);
        refreshUI();
        resultLabel->setText(result);
        logLabel->setText(log);
        endGame();
        return;
    }

    // CPU GETS TURN SKIPPED
    if (cpuSkip) {
        cpuSkip = false;
        log = QString("You: %1  |  CPU: [skipped]").arg(pm == Move::Attack ? "Attack " : pm == Move::Charge ? "Charge " : "Defend ");

        if (pm == Move::Attack) {
            cpuHP -= playerAtk;
            result = QString("CPU was stunned! You attacked freely for %1 damage.").arg(playerAtk);
            spawnWoodParticles(22);
        } else if (pm == Move::Charge) {
            playerAtk += chargeBonus;
            result = QString("CPU was stunned! You charged. Your attack is now %1.").arg(playerAtk);
        } else {
            playerDefends--;
            result = "CPU was stunned! You defended.";
        }

        cpuHP = qMax(0, cpuHP);
        refreshUI();
        resultLabel->setText(result);
        logLabel->setText(log);
        endGame();
        return;
    }

    // NORMAL TURN
    Move cm = cpuMove();
    log = QString("You: %1  |  CPU: %2")
              .arg(pm == Move::Attack ? "Attack " : pm == Move::Charge ? "Charge " : "Defend ",
                   cm == Move::Attack ? "Attack " : cm == Move::Charge ? "Charge " : "Defend ");

    if (pm == Move::Attack && cm == Move::Attack) {
        playerHP -= cpuAtk;
        cpuHP    -= playerAtk;
        result = "⚔ Both attack! You and CPU exchange damage.";
        spawnWoodParticles(18); // mutual hit

    } else if (pm == Move::Attack && cm == Move::Charge) {
        cpuHP -= playerAtk;
        result = QString("⚔ You attack into CPU's charge! CPU takes %1 damage.").arg(playerAtk);
        spawnWoodParticles(24); // clean hit, big burst

    } else if (pm == Move::Attack && cm == Move::Defend) {
        playerSkip = true;
        btnAttack->setEnabled(false);
        btnCharge->setEnabled(false);
        btnDefend->setEnabled(false);
        result = "CPU defended your attack! You lose your next turn.";
        // Attack blocked — small deflect spray
        spawnWoodParticles(8);

    } else if (pm == Move::Charge && cm == Move::Attack) {
        playerHP -= cpuAtk;
        result = QString("CPU attacked while you were charging! You take %1 damage and don't get the charge.").arg(cpuAtk);
        spawnWoodParticles(16);

    } else if (pm == Move::Charge && cm == Move::Defend) {
        playerAtk += chargeBonus;
        result = QString("CPU defended but you still charge up! Your attack is now %1.").arg(playerAtk);

    } else if (pm == Move::Charge && cm == Move::Charge) {
        playerAtk += chargeBonus;
        cpuAtk    += chargeBonus;
        result = QString("Both fighters charge up! Your attack: %1 | CPU attack: %2.").arg(playerAtk).arg(cpuAtk);

    } else if (pm == Move::Defend && cm == Move::Attack) {
        playerDefends--;
        cpuSkip = true;
        result = "You defended CPU's attack! CPU loses their next turn.";
        spawnWoodParticles(10); // block impact

    } else if (pm == Move::Defend && cm == Move::Charge) {
        playerDefends--;
        cpuAtk += chargeBonus;
        result = QString("You defended, but CPU charges anyway! CPU attack is now %1.").arg(cpuAtk);

    } else { // Defend vs Defend
        playerDefends--;
        result = "Both players defend. Nothing happens.";
    }

    playerHP = qMax(0, playerHP);
    cpuHP    = qMax(0, cpuHP);
    refreshUI();
    resultLabel->setText(result);
    logLabel->setText(log);
    endGame();
}

// UI refresh
void Battle::refreshUI()
{
    playerBar->setValue(playerHP);
    cpuBar->setValue(cpuHP);
    playerHPLabel->setText(QString("HP: %1  ATK: %2").arg(playerHP).arg(playerAtk));
    cpuHPLabel->setText(QString("ATK: %1  HP: %2").arg(cpuAtk).arg(cpuHP));
    btnDefend->setText(QString("Defend (%1)").arg(playerDefends));
    btnDefend->setEnabled(playerDefends > 0);
}

// End-game check
void Battle::endGame()
{
    if (playerHP > 0 && cpuHP > 0) return;

    btnAttack->setVisible(false);
    btnCharge->setVisible(false);
    btnDefend->setVisible(false);
    btnRestart->setVisible(true);

    if (playerHP <= 0 && cpuHP <= 0)
        resultLabel->setText("Double KO — it's a draw!");
    else if (playerHP <= 0)
        resultLabel->setText("You were defeated! CPU wins.");
    else {
        resultLabel->setText("You won!");
        if (m_player) {
            PiPet p = m_player->getPet();
            p.increase_strength(25);
            p.increase_happiness(25);
            m_player->setPet(p);
        }
        emit battleWon();
    }
}
