/*
 * mindReader.cc - mindReader minigame implementation.
 *
 * Game loop (10 rounds per session):
 *   1. startNewRound() picks a secret number 1–3 and enables the guess buttons.
 *   2. Player taps one of the three buttons → onGuess() compares against secret.
 *   3. Feedback label shows result; streak / score updated; next round starts
 *      after a short delay so the player can read the feedback.
 *   4. After kMaxRounds guesses, endSession() shows the result panel and
 *      emits gameFinished(finalScore, xpEarned).
 *
 * Stat integration:
 *   Stats are read live from Player::getPet() — they always reflect the current
 *   state (including any decay that happened while the player was in the hub).
 *   Stats are NOT modified here; that is handled by Train::onmindReaderFinished()
 *   exactly as the other minigames do it.
 *
 * Author(s): <your name here>
 */

#include "guesser.h"
#include <QRandomGenerator>

// ─────────────────────────────────────────────────────────────────────────────
//  Construction
// ─────────────────────────────────────────────────────────────────────────────

mindReader::mindReader(Player *player,
                     Character::PetType petType,
                     QWidget *parent)
    : QWidget{parent}, m_player(player), m_petType(petType)
{
    buildUi();
    startNewRound();
}

// ─────────────────────────────────────────────────────────────────────────────
//  UI construction
// ─────────────────────────────────────────────────────────────────────────────

void mindReader::buildUi()
{
    // ── Root layout ────────────────────────────────────────────────────────
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(10, 10, 10, 10);
    root->setSpacing(8);

    // ── Back button (top-left, same pattern as PiPatterns) ─────────────────
    b_back = new QPushButton("BACK", this);
    b_back->setIcon(QIcon(":/images/Assets/left.png"));
    applyUtilityStyle(b_back);

    QHBoxLayout *topBar = new QHBoxLayout();
    topBar->addWidget(b_back);
    topBar->addStretch();

    QLabel *titleLabel = new QLabel("mindReader", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "QLabel { color: #ffd700; font-size: 20px; font-weight: bold;"
        "background-color: transparent; }");
    topBar->addWidget(titleLabel);
    topBar->addStretch();
    root->addLayout(topBar);

    // ── Character sprite ───────────────────────────────────────────────────
    // Centred, fixed 140×140 so it never crowds the stat bars on small screens.
    m_character = new Character(this);
    m_character->setFixedSize(140, 140);
    // Wire up the initial sprite (hat-aware refresh is done in refreshCharacter)
    m_character->syncWithPlayer(*m_player, m_petType);

    QHBoxLayout *charRow = new QHBoxLayout();
    charRow->addStretch();
    charRow->addWidget(m_character);
    charRow->addStretch();
    root->addLayout(charRow);

    // ── "Thinking" label (shown between rounds) ────────────────────────────
    m_thinkingLabel = new QLabel("Your pet is thinking of a number…", this);
    m_thinkingLabel->setAlignment(Qt::AlignCenter);
    m_thinkingLabel->setWordWrap(true);
    m_thinkingLabel->setStyleSheet(
        "QLabel { color: mistyrose; font-size: 13px; font-style: italic;"
        "background-color: rgba(0,0,0,120); border-radius: 8px; padding: 6px; }");
    root->addWidget(m_thinkingLabel);

    // ── Guess buttons  1 · 2 · 3 ──────────────────────────────────────────
    m_btnRow = new QWidget(this);
    QHBoxLayout *btnLayout = new QHBoxLayout(m_btnRow);
    btnLayout->setContentsMargins(0, 0, 0, 0);
    btnLayout->setSpacing(10);

    m_btn1 = new QPushButton("1", this);
    m_btn2 = new QPushButton("2", this);
    m_btn3 = new QPushButton("3", this);

    for (QPushButton *b : {m_btn1, m_btn2, m_btn3}) {
        b->setFixedHeight(52);
        b->setMinimumWidth(60);
        applyButtonStyle(b);
        btnLayout->addWidget(b);
    }

    connect(m_btn1, &QPushButton::clicked, this, [this]{ onGuess(1); });
    connect(m_btn2, &QPushButton::clicked, this, [this]{ onGuess(2); });
    connect(m_btn3, &QPushButton::clicked, this, [this]{ onGuess(3); });

    root->addWidget(m_btnRow);

    // ── Feedback label ─────────────────────────────────────────────────────
    m_feedbackLabel = new QLabel("", this);
    m_feedbackLabel->setAlignment(Qt::AlignCenter);
    m_feedbackLabel->setWordWrap(true);
    m_feedbackLabel->setFixedHeight(44);
    m_feedbackLabel->setStyleSheet(
        "QLabel { color: #ffd700; font-size: 15px; font-weight: bold;"
        "background-color: rgba(0,0,0,150); border-radius: 8px; padding: 4px; }");
    root->addWidget(m_feedbackLabel);

    // ── Info panel (score / streak / rounds left) ──────────────────────────
    // Mirrors the "Condition" GroupBox style from mode.cc.
    m_infoBox   = new QGroupBox("Session", this);
    m_statsGrid = new QGridLayout();   // reused below for stat box too, renamed
    QGridLayout *infoGrid = new QGridLayout();
    m_infoBox->setLayout(infoGrid);
    m_infoBox->setStyleSheet(
        "QGroupBox { background-color: rgba(0,0,0,150); border-radius: 8px;"
        "color: mistyrose; margin-top: 22px; }"
        "QGroupBox::title { color: #ffd700; subcontrol-origin: margin;"
        "subcontrol-position: top center; }");

    m_scoreLabel  = new QLabel("Score:  0", this);
    m_streakLabel = new QLabel("Streak: 0", this);
    m_roundsLabel = new QLabel(QString("Round:  1 / %1").arg(kMaxRounds), this);

    for (QLabel *l : {m_scoreLabel, m_streakLabel, m_roundsLabel}) {
        l->setStyleSheet(
            "QLabel { color: mistyrose; font-size: 12px;"
            "background-color: transparent; }");
    }

    infoGrid->addWidget(m_scoreLabel,  0, 0);
    infoGrid->addWidget(m_streakLabel, 0, 1);
    infoGrid->addWidget(m_roundsLabel, 0, 2);
    root->addWidget(m_infoBox);

    // ── Stat bars (Condition box — same style as Mode) ─────────────────────
    m_statsBox  = new QGroupBox("Condition", this);
    m_statsGrid = new QGridLayout();
    m_statsBox->setLayout(m_statsGrid);
    m_statsBox->setStyleSheet(
        "QGroupBox { background-color: rgba(0,0,0,150); border-radius: 8px;"
        "color: mistyrose; margin-top: 22px; }"
        "QGroupBox::title { color: #ffd700; subcontrol-origin: margin;"
        "subcontrol-position: top center; }");

    m_hungerLabel    = new QLabel("Hunger",    this);
    m_energyLabel    = new QLabel("Energy",    this);
    m_happinessLabel = new QLabel("Happiness", this);

    m_hungerBar    = new QProgressBar(this);
    m_energyBar    = new QProgressBar(this);
    m_happinessBar = new QProgressBar(this);

    for (QProgressBar *bar : {m_hungerBar, m_energyBar, m_happinessBar}) {
        bar->setRange(0, 100);
        bar->setTextVisible(true);
        bar->setStyleSheet(
            "QProgressBar { border: 1px solid #FBA8FF; border-radius: 6px;"
            "background-color: rgba(0,0,0,120); color: mistyrose; text-align: center; }"
            "QProgressBar::chunk { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:0,"
            "stop:0 #4850DB, stop:1 #4A71DB); border-radius: 5px; }");
    }

    for (QLabel *l : {m_hungerLabel, m_energyLabel, m_happinessLabel}) {
        l->setStyleSheet(
            "QLabel { color: mistyrose; font-size: 12px;"
            "background-color: transparent; }");
    }

    m_statsGrid->addWidget(m_hungerLabel,    0, 0); m_statsGrid->addWidget(m_hungerBar,    0, 1);
    m_statsGrid->addWidget(m_energyLabel,    1, 0); m_statsGrid->addWidget(m_energyBar,    1, 1);
    m_statsGrid->addWidget(m_happinessLabel, 2, 0); m_statsGrid->addWidget(m_happinessBar, 2, 1);
    root->addWidget(m_statsBox);

    // ── Result panel (shown after kMaxRounds) ─────────────────────────────
    m_resultPanel = new QWidget(this);
    m_resultPanel->setStyleSheet(
        "QWidget { background-color: rgba(18,8,40,230); border-radius: 12px; }");
    QVBoxLayout *resultLayout = new QVBoxLayout(m_resultPanel);
    resultLayout->setContentsMargins(16, 16, 16, 16);
    resultLayout->setSpacing(10);

    m_resultLabel = new QLabel("", m_resultPanel);
    m_resultLabel->setAlignment(Qt::AlignCenter);
    m_resultLabel->setWordWrap(true);
    m_resultLabel->setStyleSheet(
        "QLabel { color: #ffd700; font-size: 16px; font-weight: bold;"
        "background-color: transparent; }");

    m_playAgainBtn = new QPushButton("Play Again", m_resultPanel);
    applyUtilityStyle(m_playAgainBtn);
    connect(m_playAgainBtn, &QPushButton::clicked, this, [this]() {
        m_score       = 0;
        m_roundsPlayed = 0;
        m_streak      = 0;
        showResultPanel(false);
        startNewRound();
    });

    resultLayout->addStretch();
    resultLayout->addWidget(m_resultLabel);
    resultLayout->addWidget(m_playAgainBtn);
    resultLayout->addStretch();
    root->addWidget(m_resultPanel);

    // ── Initial visibility ─────────────────────────────────────────────────
    showResultPanel(false);

    // ── Apply hat sprite ───────────────────────────────────────────────────
    refreshCharacter();

    // ── Populate stat bars ─────────────────────────────────────────────────
    updateStatBars();
}

// ─────────────────────────────────────────────────────────────────────────────
//  Game logic
// ─────────────────────────────────────────────────────────────────────────────

void mindReader::startNewRound()
{
    // Pick a random secret (1, 2, or 3)
    m_secretNumber = static_cast<int>(
        QRandomGenerator::global()->bounded(3) + 1);

    m_thinkingLabel->setText("Your pet is thinking of a number between 1 and 3…");
    m_feedbackLabel->setText("");
    setGuessButtonsEnabled(true);
    updateStatBars();
    updateInfoPanel();
}

void mindReader::onGuess(int guess)
{
    setGuessButtonsEnabled(false);   // prevent double-tap
    m_roundsPlayed++;

    bool correct = (guess == m_secretNumber);

    if (correct) {
        m_score++;
        m_streak++;
        m_feedbackLabel->setText(
            QString("✅  Correct!  Your pet was thinking of %1!  (Streak: %2)")
                .arg(m_secretNumber).arg(m_streak));
        m_feedbackLabel->setStyleSheet(
            "QLabel { color: #00ff88; font-size: 15px; font-weight: bold;"
            "background-color: rgba(0,80,40,180); border-radius: 8px; padding: 4px; }");
    } else {
        m_streak = 0;
        m_feedbackLabel->setText(
            QString("❌  Not quite!  Your pet was thinking of %1.")
                .arg(m_secretNumber));
        m_feedbackLabel->setStyleSheet(
            "QLabel { color: #ff6666; font-size: 15px; font-weight: bold;"
            "background-color: rgba(80,0,0,180); border-radius: 8px; padding: 4px; }");
    }

    updateInfoPanel();

    if (m_roundsPlayed >= kMaxRounds) {
        // Short pause so the player can read the last feedback before the
        // result panel slides in.
        QTimer::singleShot(900, this, &mindReader::endSession);
    } else {
        // Brief pause then auto-advance to next round.
        QTimer::singleShot(1200, this, &mindReader::startNewRound);
    }
}

void mindReader::endSession()
{
    // XP formula: base 5 per correct + streak bonus proportional to score.
    // Mirrors the spirit of the other minigame reward formulas in train.cc.
    int xpEarned = m_score * 5 + (m_score * m_score) / 2;

    QString rating;
    if      (m_score == kMaxRounds)                     rating = "Perfect! Your pet is amazed!";
    else if (m_score >= kMaxRounds * 8 / 10)            rating = "Great job! Your pet is proud!";
    else if (m_score >= kMaxRounds / 2)                 rating = "Not bad! Your pet is happy!";
    else                                                 rating = "Keep practicing! Your pet believes in you!";

    m_resultLabel->setText(
        QString("%1\n\nCorrect: %2 / %3\nXP Earned: %4")
            .arg(rating)
            .arg(m_score).arg(kMaxRounds)
            .arg(xpEarned));

    showResultPanel(true);
    emit gameFinished(m_score * 10, xpEarned);  // finalScore scaled to 0-100 range
}

// ─────────────────────────────────────────────────────────────────────────────
//  Stat / Info helpers
// ─────────────────────────────────────────────────────────────────────────────

void mindReader::updateStatBars()
{
    PiPet pet = m_player->getPet();
    m_hungerBar->setValue(pet.hunger());
    m_energyBar->setValue(pet.energy());
    m_happinessBar->setValue(pet.happiness());
}

void mindReader::updateInfoPanel()
{
    m_scoreLabel->setText(QString("Score:  %1").arg(m_score));
    m_streakLabel->setText(QString("Streak: %1").arg(m_streak));
    m_roundsLabel->setText(
        QString("Round:  %1 / %2").arg(m_roundsPlayed + 1).arg(kMaxRounds));
}

// ─────────────────────────────────────────────────────────────────────────────
//  Character / hat sync
// ─────────────────────────────────────────────────────────────────────────────

void mindReader::refreshCharacter()
{
    applyHatSprite();
    updateStatBars();
}

void mindReader::applyHatSprite()
{
    // Exact same hat-sprite resolution logic as Mode::refreshDisplay() so the
    // sprite shown here always matches the main Mode screen.
    QString hat = m_player->getPet().hat();
    if (!hat.isEmpty()) {
        QString folder, prefix;
        QString type = m_player->getPet().pet_type();
        if      (type == "ElectricAxolotl") { folder = "axolotl";   prefix = "axolotl";   }
        else if (type == "SeelCat")         { folder = "seelcat";   prefix = "seelcat";   }
        else                                { folder = "dragondog"; prefix = "dragondog"; }

        QString stage = m_player->getPet().age_group();
        QString infix = (stage == "Teen") ? "teen_" : (stage == "Adult") ? "adult_" : "";
        QString path  = QString(":/images/Sprites/pets/%1/%2_%3%4.gif")
                            .arg(folder, prefix, infix, hat);

        if (path != m_cachedHatPath || !m_cachedHatMovie || !m_cachedHatMovie->isValid()) {
            delete m_cachedHatMovie;
            m_cachedHatMovie = new QMovie(path, QByteArray(), m_character);
            m_cachedHatPath  = path;
        }

        if (m_cachedHatMovie->isValid()) {
            QLabel *disp = m_character->findChild<QLabel *>();
            if (disp) {
                if (disp->movie() != m_cachedHatMovie) {
                    if (disp->movie()) disp->movie()->stop();
                    disp->setMovie(m_cachedHatMovie);
                    m_cachedHatMovie->start();
                }
                return;
            }
        }
    }

    // No hat (or hat path invalid) — clear cache and show base idle sprite.
    if (m_cachedHatMovie) {
        delete m_cachedHatMovie;
        m_cachedHatMovie = nullptr;
        m_cachedHatPath  = "";
    }
    m_character->syncWithPlayer(*m_player, m_petType);
}

// ─────────────────────────────────────────────────────────────────────────────
//  UI helpers
// ─────────────────────────────────────────────────────────────────────────────

void mindReader::setGuessButtonsEnabled(bool enabled)
{
    m_btn1->setEnabled(enabled);
    m_btn2->setEnabled(enabled);
    m_btn3->setEnabled(enabled);

    // Dim the buttons visually when disabled so feedback is clear.
    QString disabledExtra = enabled ? "" : "opacity: 0.4;";
    for (QPushButton *b : {m_btn1, m_btn2, m_btn3})
        applyButtonStyle(b, enabled ? "" : "#333355");
}

void mindReader::showResultPanel(bool visible)
{
    m_resultPanel->setVisible(visible);
    m_btnRow->setVisible(!visible);
    m_thinkingLabel->setVisible(!visible);
    m_feedbackLabel->setVisible(!visible);
}

void mindReader::applyButtonStyle(QPushButton *btn, const QString &extraColor)
{
    QString bg = extraColor.isEmpty()
        ? "qlineargradient(x1:0,y1:0,x2:1,y2:1,stop:0 #4850DB,stop:1 #4A71DB)"
        : extraColor;

    btn->setStyleSheet(QString(R"(
        QPushButton {
            background-color: %1;
            border: 2px inset #FBA8FF;
            border-radius: 10px;
            padding: 6px;
            font: bold 22px;
            color: mistyrose;
        }
        QPushButton:pressed {
            background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #4A71DB,stop:1 #4850DB);
        }
    )").arg(bg));
}

void mindReader::applyUtilityStyle(QPushButton *btn)
{
    btn->setStyleSheet(R"(
        QPushButton {
            background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #4850DB, stop:1 #4A71DB);
            border: 2px inset #FBA8FF;
            border-radius: 10px;
            padding: 4px;
            font: bold;
            color: mistyrose;
        }
        QPushButton:pressed {
            background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #4A71DB, stop:1 #4850DB);
        }
    )");
}
