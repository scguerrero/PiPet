#include "battle.h"
#include <QFrame>

Battle::Battle(QWidget *parent) : QWidget(parent)
{
    root = new QVBoxLayout();
    this->setLayout(root);

    // Title
    title = new QLabel("piPetBattle", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 22px; font-weight: bold; margin-bottom: 4px;");
    root->addWidget(title);

    // HP bars
    hpWidget = new QWidget();
    hpGrid = new QGridLayout();
    hpWidget->setLayout(hpGrid);
    root->addWidget(hpWidget);

    auto makeNameLabel = [&](const QString &text, Qt::Alignment align) {
        auto *l = new QLabel(text, this);
        l->setAlignment(align);
        l->setStyleSheet("font-weight: bold; font-size: 13px;");
        return l;
    };

    hpGrid->addWidget(makeNameLabel("You",  Qt::AlignLeft),  0, 0);
    hpGrid->addWidget(makeNameLabel("CPU",  Qt::AlignRight), 0, 2);

    playerBar = new QProgressBar();
    playerBar->setRange(0, maxHP); playerBar->setValue(maxHP);
    playerBar->setTextVisible(false);
    playerBar->setStyleSheet("QProgressBar::chunk{background:#4caf50;}");

    cpuBar = new QProgressBar();
    cpuBar->setRange(0, maxHP); cpuBar->setValue(maxHP);
    cpuBar->setTextVisible(false);
    cpuBar->setInvertedAppearance(true);
    cpuBar->setStyleSheet("QProgressBar::chunk{background:#f44336;}");

    hpGrid->addWidget(playerBar, 1, 0);
    hpGrid->addWidget(new QLabel("vs"), 1, 1, Qt::AlignCenter);
    hpGrid->addWidget(cpuBar,    1, 2);
    hpGrid->setColumnStretch(0, 1); hpGrid->setColumnStretch(2, 1);

    playerHPLabel = new QLabel(); playerHPLabel->setAlignment(Qt::AlignLeft);
    cpuHPLabel    = new QLabel(); cpuHPLabel->setAlignment(Qt::AlignRight);
    hpGrid->addWidget(playerHPLabel, 2, 0);
    hpGrid->addWidget(cpuHPLabel,    2, 2);

    // Adjust font size of HP labels
    playerHPLabel->setStyleSheet("font-size: 14px");
    cpuHPLabel->setStyleSheet("font-size: 14px");

    sep = new QFrame();
    sep->setFrameShape(QFrame::HLine); sep->setFrameShadow(QFrame::Sunken);
    root->addWidget(sep);

    // Result and log labels
    resultLabel = new QLabel("Choose your move!");
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setWordWrap(true);
    resultLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
    root->addWidget(resultLabel);

    logLabel = new QLabel("");
    logLabel->setAlignment(Qt::AlignCenter);
    logLabel->setStyleSheet("font-size: 12px; color: #666; font-style: italic;");
    root->addWidget(logLabel);

    root->addStretch();

    // Buttons
    btnRow = new QHBoxLayout();
    btnRow->setSpacing(8);
    btnWidget = new QWidget();
    btnWidget->setLayout(btnRow);

    auto makeBtn = [&](const QString &text) {
        auto *b = new QPushButton(text, this);
        b->setStyleSheet(
            "QPushButton{font-size:13px;padding:10px 6px;border-radius:6px;}"
            "QPushButton:hover{background:#ddeeff;}"
            "QPushButton:disabled{color:#aaa;background:#f0f0f0;}");
        return b;
    };

    btnAttack  = makeBtn("⚔ Attack");
    btnCharge  = makeBtn("⚡ Charge");
    btnDefend  = makeBtn("🛡 Defend (5)");
    btnRestart = makeBtn("🔄 New Game");
    btnBack = new QPushButton("⬅️ BACK");

    btnRow->addWidget(btnAttack);
    btnRow->addWidget(btnCharge);
    btnRow->addWidget(btnDefend);
    root->addWidget(btnWidget);
    root->addWidget(btnBack);

    btnRestart->setVisible(false);
    root->addWidget(btnRestart, 0, Qt::AlignCenter);

    connect(btnAttack,  &QPushButton::clicked, this, &Battle::onAttack);
    connect(btnCharge,  &QPushButton::clicked, this, &Battle::onCharge);
    connect(btnDefend,  &QPushButton::clicked, this, &Battle::onDefend);
    connect(btnRestart, &QPushButton::clicked, this, &Battle::onRestart);

    refreshUI();

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
}

// Slots

void Battle::onAttack()  { playTurn(Move::Attack); }
void Battle::onCharge()  { playTurn(Move::Charge); }
void Battle::onDefend()  { playTurn(Move::Defend); }

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

// CPU (picks a random move for now; can be updated to be smarter in future)

Move Battle::cpuMove()
{
    switch (QRandomGenerator::global()->bounded(3)) {
    case 0:  return Move::Attack;
    case 1:  return Move::Charge;
    default: return Move::Defend;
    }
}

// Full turn logic
//
// Full scenario matrix:
//  P.Attack  vs  CPU.Attack  → both take damage
//  P.Attack  vs  CPU.Charge  → player deals damage, CPU loses that charge
//  P.Attack  vs  CPU.Defend  → player loses next turn
//  P.Charge  vs  CPU.Attack  → player takes damage, player does NOT get the charge
//  P.Charge  vs  CPU.Defend  → player gets the charge
//  P.Charge  vs  CPU.Charge  → both get the charge
//  P.Defend  vs  CPU.Attack  → CPU loses next turn
//  P.Defend  vs  CPU.Charge  → CPU gets the charge
//  P.Defend  vs  CPU.Defend  → nothing happens
//
// Player skip: player was stunned — CPU acts freely with no interaction.
// CPU skip:    CPU was stunned   — player acts freely with no interaction.

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
        log = QString("You: [skipped]  |  CPU: %1").arg(cm == Move::Attack ? "Attack ⚔" : cm == Move::Charge ? "Charge ⚡" : "Defend 🛡");

        if (cm == Move::Attack) {
            playerHP -= cpuAtk;
            result = QString("💫 You were stunned! CPU attacked freely for %1 damage.").arg(cpuAtk);
        } else if (cm == Move::Charge) {
            cpuAtk += chargeBonus;
            result = QString("💫 You were stunned! CPU charged. CPU attack is now %1.").arg(cpuAtk);
        } else {
            result = "💫 You were stunned! CPU defended.";
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
        log = QString("You: %1  |  CPU: [skipped]").arg(pm == Move::Attack ? "Attack ⚔" : pm == Move::Charge ? "Charge ⚡" : "Defend 🛡");

        if (pm == Move::Attack) {
            cpuHP -= playerAtk;
            result = QString("🏃 CPU was stunned! You attacked freely for %1 damage.").arg(playerAtk);
        } else if (pm == Move::Charge) {
            playerAtk += chargeBonus;
            result = QString("🏃 CPU was stunned! You charged. Your attack is now %1.").arg(playerAtk);
        } else {
            playerDefends--;
            result = "🏃 CPU was stunned! You defended.";
        }

        cpuHP = qMax(0, cpuHP);
        refreshUI();
        resultLabel->setText(result);
        logLabel->setText(log);
        endGame();
        return;
    }

    // NORMAL TURN - Both choose
    Move cm = cpuMove();
    log = QString("You: %1  |  CPU: %2")
              .arg(pm == Move::Attack ? "Attack ⚔" : pm == Move::Charge ? "Charge ⚡" : "Defend 🛡")
              .arg(cm == Move::Attack ? "Attack ⚔" : cm == Move::Charge ? "Charge ⚡" : "Defend 🛡");

    if (pm == Move::Attack && cm == Move::Attack) {
        playerHP -= cpuAtk;
        cpuHP    -= playerAtk;
        result = "⚔ Both attack! You and CPU exchange damage.";

    } else if (pm == Move::Attack && cm == Move::Charge) {
        cpuHP -= playerAtk;
        result = QString("⚔ You attack into CPU's charge! CPU takes %1 damage.").arg(playerAtk);

    } else if (pm == Move::Attack && cm == Move::Defend) {
        playerSkip = true;
        btnAttack->setEnabled(false);
        btnCharge->setEnabled(false);
        btnDefend->setEnabled(false);
        result = "🛡 CPU defended your attack! You lose your next turn.";

    } else if (pm == Move::Charge && cm == Move::Attack) {
        // CPU attacks into player's charge — player takes damage, no charge gained
        playerHP -= cpuAtk;
        result = QString("💥 CPU attacked while you were charging! You take %1 damage and don't get the charge.").arg(cpuAtk);

    } else if (pm == Move::Charge && cm == Move::Defend) {
        // CPU defended player's charge — player still gets the charge
        playerAtk += chargeBonus;
        result = QString("⚡ CPU defended but you still charge up! Your attack is now %1.").arg(playerAtk);

    } else if (pm == Move::Charge && cm == Move::Charge) {
        // Both charge
        playerAtk += chargeBonus;
        cpuAtk    += chargeBonus;
        result = QString("⚡ Both fighters charge up! Your attack: %1 | CPU attack: %2.").arg(playerAtk).arg(cpuAtk);

    } else if (pm == Move::Defend && cm == Move::Attack) {
        playerDefends--;
        cpuSkip = true;
        result = "🛡 You defended CPU's attack! CPU loses their next turn.";

    } else if (pm == Move::Defend && cm == Move::Charge) {
        // Player defended but CPU charges — CPU gets the charge
        playerDefends--;
        cpuAtk += chargeBonus;
        result = QString("🛡 You defended, but CPU charges anyway! CPU attack is now %1.").arg(cpuAtk);

    } else { // pm == Defend && cm == Defend
        playerDefends--;
        result = "🤝 Both players defend. Nothing happens.";
    }

    playerHP = qMax(0, playerHP);
    cpuHP    = qMax(0, cpuHP);
    refreshUI();
    resultLabel->setText(result);
    logLabel->setText(log);
    endGame();
}

// Update HP bars and button labels
void Battle::refreshUI()
{
    playerBar->setValue(playerHP);
    cpuBar->setValue(cpuHP);
    playerHPLabel->setText(QString("HP: %1  ATK: %2").arg(playerHP).arg(playerAtk));
    cpuHPLabel->setText(QString("ATK: %1  HP: %2").arg(cpuAtk).arg(cpuHP));
    btnDefend->setText(QString("🛡 Defend (%1)").arg(playerDefends));
    btnDefend->setEnabled(playerDefends > 0);
}

// Check win/loss
void Battle::endGame()
{
    if (playerHP > 0 && cpuHP > 0) return;

    btnAttack->setVisible(false);
    btnCharge->setVisible(false);
    btnDefend->setVisible(false);
    btnRestart->setVisible(true);

    if (playerHP <= 0 && cpuHP <= 0)
        resultLabel->setText("💥 Double KO — it's a draw!");
    else if (playerHP <= 0)
        resultLabel->setText("💀 You were defeated! CPU wins.");
    else
        resultLabel->setText("🏆 You won!");
}
