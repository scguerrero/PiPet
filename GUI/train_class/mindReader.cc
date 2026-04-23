/*
 * mindReader.cc - mindReader minigame implementation.
 * Author(s): Luke Cerwin
 */

#include "mindReader.h"
#include <QRandomGenerator>
#include <QPainter>

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
//  UI construction  (no root layout — geometry set in resizeEvent, same as Feed)
// ─────────────────────────────────────────────────────────────────────────────

void mindReader::buildUi()
{
    m_bg.load(":/images/Backgrounds/mindReader.png");

    // ── Session info box ───────────────────────────────────────────────────
    m_infoBox = new QGroupBox("Session", this);
    QGridLayout *infoGrid = new QGridLayout();
    m_infoBox->setLayout(infoGrid);
    m_infoBox->setStyleSheet(
        "QGroupBox { background-color: rgba(0,0,0,150); border-radius: 8px;"
        "color: mistyrose; margin-top: 16px; padding-top: 2px; }"
        "QGroupBox::title { color: #ffd700; font-size: 11px;"
        "subcontrol-origin: margin; subcontrol-position: top center; }");

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

    // ── Back button ────────────────────────────────────────────────────────
    b_back = new QPushButton("BACK", this);
    b_back->setIcon(QIcon(":/images/Assets/left.png"));
    applyUtilityStyle(b_back);

    // ── Character sprite ───────────────────────────────────────────────────
    m_character = new Character(this);
    m_character->setFixedSize(140, 140);
    m_character->syncWithPlayer(*m_player, m_petType);

    // ── "Thinking" label ───────────────────────────────────────────────────
    m_thinkingLabel = new QLabel("Your pet is thinking of a number\u2026", this);
    m_thinkingLabel->setAlignment(Qt::AlignCenter);
    m_thinkingLabel->setWordWrap(true);
    m_thinkingLabel->setStyleSheet(
        "QLabel { color: mistyrose; font-size: 13px; font-style: italic;"
        "background-color: rgba(0,0,0,120); border-radius: 8px; padding: 6px; }");

    // ── Guess buttons  1 · 2 · 3 ──────────────────────────────────────────
    m_btnRow = new QWidget(this);
    QHBoxLayout *btnLayout = new QHBoxLayout(m_btnRow);
    btnLayout->setContentsMargins(0, 0, 0, 0);
    btnLayout->setSpacing(10);

    m_btn1 = new QPushButton("1", m_btnRow);
    m_btn2 = new QPushButton("2", m_btnRow);
    m_btn3 = new QPushButton("3", m_btnRow);

    for (QPushButton *b : {m_btn1, m_btn2, m_btn3}) {
        b->setFixedHeight(52);
        applyButtonStyle(b);
        btnLayout->addWidget(b);
    }

    connect(m_btn1, &QPushButton::clicked, this, [this]{ onGuess(1); });
    connect(m_btn2, &QPushButton::clicked, this, [this]{ onGuess(2); });
    connect(m_btn3, &QPushButton::clicked, this, [this]{ onGuess(3); });

    // ── Feedback label ─────────────────────────────────────────────────────
    m_feedbackLabel = new QLabel("", this);
    m_feedbackLabel->setAlignment(Qt::AlignCenter);
    m_feedbackLabel->setWordWrap(true);
    m_feedbackLabel->setStyleSheet(
        "QLabel { color: #ffd700; font-size: 15px; font-weight: bold;"
        "background-color: rgba(0,0,0,150); border-radius: 8px; padding: 4px; }");

    // ── Result panel ───────────────────────────────────────────────────────
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
        m_score        = 0;
        m_roundsPlayed = 0;
        m_streak       = 0;
        m_sessionFinished = false;   // allow a fresh session
        showResultPanel(false);
        startNewRound();
    });

    resultLayout->addWidget(m_resultLabel);
    resultLayout->addWidget(m_playAgainBtn, 0, Qt::AlignCenter);
    m_resultPanel->hide();
}

// ─────────────────────────────────────────────────────────────────────────────
//  Geometry  — mirrors Feed::resizeEvent(); called automatically by Qt
// ─────────────────────────────────────────────────────────────────────────────

void mindReader::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    int w = width(), h = height();

    // Session box — full width, pinned to very top
    m_infoBox->setGeometry(8, 8, w - 16, 58);

    // Back button — below session box, left-aligned
    b_back->setGeometry(8, 72, 90, 34);

    // Character — centred, sits in the upper-middle area
    int charX = (w - 140) / 2;
    m_character->setGeometry(charX, 260, 160, 160);

    // Bottom cluster stacked upward from 8 px above the bottom edge:
    //   feedback (44 px) → buttons (52 px) → thinking label (44 px)
    int bottomEdge = h - 8;
    int feedbackY  = bottomEdge - 44;
    int btnY       = feedbackY  - 52 - 6;
    int thinkingY  = btnY       - 44 - 6;

    m_feedbackLabel->setGeometry(8, feedbackY, w - 16, 44);
    m_btnRow->setGeometry(8, btnY, w - 16, 52);
    m_thinkingLabel->setGeometry(8, thinkingY, w - 16, 44);

    // FIX: result panel starts from just below the character so it has
    // plenty of vertical room instead of being cramped at the bottom.
    int resultTop = 200;   // charY + charH + small gap
    m_resultPanel->setGeometry(8, resultTop, w - 16, bottomEdge - resultTop);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Game loop
// ─────────────────────────────────────────────────────────────────────────────

void mindReader::startNewRound()
{
    m_secretNumber = QRandomGenerator::global()->bounded(1, 4); // 1, 2, or 3

    m_thinkingLabel->setText("Your piPet is thinking of a number between 1 and 3\u2026");
    m_feedbackLabel->setText("");
    setGuessButtonsEnabled(true);
    updateInfoPanel();
}

void mindReader::onGuess(int guess)
{
    // Guard: ignore stray signals after session has ended
    if (m_sessionFinished) return;

    setGuessButtonsEnabled(false);   // prevent double-tap
    m_roundsPlayed++;

    bool correct = (guess == m_secretNumber);

    if (correct) {
        m_score++;
        m_streak++;
        m_feedbackLabel->setText(
            QString("\u2705  Correct!  Your pet was thinking of %1!  (Streak: %2)")
                .arg(m_secretNumber).arg(m_streak));
        m_feedbackLabel->setStyleSheet(
            "QLabel { color: #00ff88; font-size: 15px; font-weight: bold;"
            "background-color: rgba(0,80,40,180); border-radius: 8px; padding: 4px; }");
    } else {
        m_streak = 0;
        m_feedbackLabel->setText(
            QString("\u274c  Not quite!  Your pet was thinking of %1.")
                .arg(m_secretNumber));
        m_feedbackLabel->setStyleSheet(
            "QLabel { color: #ff6666; font-size: 15px; font-weight: bold;"
            "background-color: rgba(80,0,0,180); border-radius: 8px; padding: 4px; }");
    }

    // FIX: update panel BEFORE deciding to end so the final round label
    // displays correctly (shows "Round: 10/10" not "Round: 11/10").
    updateInfoPanel();

    if (m_roundsPlayed >= kMaxRounds) {
        m_sessionFinished = true;
        QTimer::singleShot(900, this, &mindReader::endSession);
    } else {
        QTimer::singleShot(1200, this, &mindReader::startNewRound);
    }
}

void mindReader::endSession()
{
    int xpEarned = m_score * 5 + (m_score * m_score) / 2;

    QString rating;
    if      (m_score == kMaxRounds)          rating = "Perfect! Your pet is amazed!";
    else if (m_score >= kMaxRounds * 8 / 10) rating = "Great job! Your pet is proud!";
    else if (m_score >= kMaxRounds / 2)      rating = "Your pet is happy!";
    else                                     rating = "Your pet believes in you!";
    m_resultLabel->setText(
        QString("%1\n\nCorrect: %2 / %3\nXP Earned: %4")
            .arg(rating)
            .arg(m_score).arg(kMaxRounds)
            .arg(xpEarned));

    showResultPanel(true);

    // FIX: emit gameFinished only once per session — NOT when Play Again is
    // pressed — so the parent screen doesn't navigate away on replay.
    emit gameFinished(m_score * 10, xpEarned);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Info panel helper
// ─────────────────────────────────────────────────────────────────────────────

void mindReader::updateInfoPanel()
{
    m_scoreLabel->setText(QString("Score:  %1").arg(m_score));
    m_streakLabel->setText(QString("Streak: %1").arg(m_streak));

    // FIX: before any guess m_roundsPlayed == 0 → show "Round: 1 / 10".
    // After the final guess m_roundsPlayed == kMaxRounds → cap at kMaxRounds
    // so we never display "Round: 11 / 10".
    int displayRound = qMin(m_roundsPlayed + 1, kMaxRounds);
    m_roundsLabel->setText(
        QString("Round:  %1 / %2").arg(displayRound).arg(kMaxRounds));
}

// ─────────────────────────────────────────────────────────────────────────────
//  Character / hat sync  — mirrors Feed::refreshCharacter() exactly
// ─────────────────────────────────────────────────────────────────────────────

void mindReader::refreshCharacter()
{
    applyHatSprite();
}

void mindReader::applyHatSprite()
{
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

// ─────────────────────────────────────────────────────────────────────────────
//  UI helpers
// ─────────────────────────────────────────────────────────────────────────────

void mindReader::setGuessButtonsEnabled(bool enabled)
{
    m_btn1->setEnabled(enabled);
    m_btn2->setEnabled(enabled);
    m_btn3->setEnabled(enabled);

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

void mindReader::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (!m_bg.isNull()) {
        painter.drawPixmap(0, 0, width(), height(), m_bg);
    }
    QWidget::paintEvent(event);
}
