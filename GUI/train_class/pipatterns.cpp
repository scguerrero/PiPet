/*
 * PiPatterns is a minigame that improves the PiPet's intelligence.
 *
 * Author: Sasha C. Guerrero
 */

#include "pipatterns.h"
#include <QRandomGenerator>

static QString buttonStyle() {
    return R"(
        QPushButton {
            background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #4850DB, stop:1 #4A71DB);
            border: 2px inset #FBA8FF;
            border-radius: 10px;
            padding: 6px;
        }
        QPushButton:pressed {
            background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #4A71DB, stop:1 #4850DB);
        }
        QPushButton:disabled {
            background-color: rgba(60,60,80,180);
            border: 2px solid #555;
        }
    )";
}

static QString selectedDiffStyle() {
    return R"(
        QPushButton {
            background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #FBA8FF, stop:1 #FFD700);
            border: 2px solid mistyrose;
            border-radius: 10px;
            padding: 6px;
            color: #0d1b4b;
        }
    )";
}

PiPatterns::PiPatterns(Player *player, QWidget *parent)
    : QWidget(parent), m_player(player)
{
    QVBoxLayout *topLayout = new QVBoxLayout(this);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(0);

    m_stack = new QStackedWidget();
    topLayout->addWidget(m_stack);

    // ── Page 0: Hub ───────────────────────────────────────────────────────
    QWidget     *hubPage   = new QWidget();
    QVBoxLayout *hubLayout = new QVBoxLayout(hubPage);
    hubLayout->setContentsMargins(24, 24, 24, 24);
    hubLayout->setSpacing(14);
    hubLayout->setAlignment(Qt::AlignCenter);

    QLabel *logo = new QLabel();
    QImage *img  = new QImage(":/images/Assets/PiPatterns.png");
    logo->setPixmap(QPixmap::fromImage(img->scaled(160, 160, Qt::KeepAspectRatio)));
    logo->setAlignment(Qt::AlignCenter);

    QLabel *header = new QLabel(
        "<div style='text-align:center;font-size:18px;font-weight:bold;'>"
        "PiPatterns</div><br>"
        "To improve your PiPet's intelligence, you must train their pattern memory.<br><br>"
        "Your PiPet must memorize and recreate a pattern on a 4x4 matrix to earn a Lootbox.<br><br>"
        "Score is cumulative across all difficulty levels.<br><br>"
        "<b>Difficulty Levels</b><br>"
        "&nbsp;&nbsp;Bit &mdash; Easy<br>"
        "&nbsp;&nbsp;Nibble &mdash; Normal<br>"
        "&nbsp;&nbsp;Byte &mdash; Hard");
    header->setWordWrap(true);
    header->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    header->setStyleSheet(
        "QLabel { background-color: rgba(72,50,180,220);"
        "border: 2px solid rgba(251,168,255,150);"
        "border-radius: 10px; padding: 12px; }");

    b_play = new QPushButton("Play");
    b_back = new QPushButton("BACK");
    b_back->setIcon(QIcon(":/images/Assets/left.png"));

    for (QPushButton *btn : {b_play, b_back})
        btn->setStyleSheet(buttonStyle());

    hubLayout->addStretch();
    hubLayout->addWidget(logo);
    hubLayout->addWidget(header);
    hubLayout->addSpacing(8);
    hubLayout->addWidget(b_play);
    hubLayout->addWidget(b_back);
    hubLayout->addStretch();

    // ── Page 1: Matrix ────────────────────────────────────────────────────
    QWidget     *matrixPage   = new QWidget();
    QVBoxLayout *matrixLayout = new QVBoxLayout(matrixPage);
    matrixLayout->setContentsMargins(8, 8, 8, 8);
    matrixLayout->setSpacing(8);

    // Status and score labels
    m_statusLabel = new QLabel("Select difficulty and press Start.");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setStyleSheet(
        "QLabel { background-color: rgba(72,50,180,200);"
        "border: 2px solid rgba(251,168,255,150);"
        "border-radius: 8px; padding: 8px; }");

    m_scoreLabel = new QLabel("Score: 0");
    m_scoreLabel->setAlignment(Qt::AlignCenter);
    m_scoreLabel->setStyleSheet(
        "QLabel { background-color: #0d1b4b; }");

    // Difficulty buttons
    m_diffBit    = new QPushButton("Bit");
    m_diffNibble = new QPushButton("Nibble");
    m_diffByte   = new QPushButton("Byte");

    QWidget     *diffRow    = new QWidget();
    QHBoxLayout *diffLayout = new QHBoxLayout(diffRow);
    diffLayout->setContentsMargins(0, 0, 0, 0);
    diffLayout->setSpacing(8);
    diffLayout->addWidget(m_diffBit);
    diffLayout->addWidget(m_diffNibble);
    diffLayout->addWidget(m_diffByte);

    updateDiffButtons();

    // Matrix
    matrix = new Matrix();

    // Start + Back buttons
    m_startBtn       = new QPushButton("Start");
    b_backFromMatrix = new QPushButton("BACK");
    b_backFromMatrix->setIcon(QIcon(":/images/Assets/left.png"));

    for (QPushButton *btn : {m_startBtn, b_backFromMatrix})
        btn->setStyleSheet(buttonStyle());

    QWidget     *bottomRow    = new QWidget();
    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomRow);
    bottomLayout->setContentsMargins(0, 0, 0, 0);
    bottomLayout->setSpacing(8);
    bottomLayout->addWidget(m_startBtn);
    bottomLayout->addWidget(b_backFromMatrix);

    matrixLayout->addWidget(m_statusLabel);
    matrixLayout->addWidget(m_scoreLabel);
    matrixLayout->addWidget(diffRow);
    matrixLayout->addWidget(matrix, 1);
    matrixLayout->addWidget(bottomRow);

    // ── Stack pages ───────────────────────────────────────────────────────
    m_stack->addWidget(hubPage);     // 0
    m_stack->addWidget(matrixPage);  // 1

    // ── Pattern timer ─────────────────────────────────────────────────────
    m_patternTimer = new QTimer(this);
    m_patternTimer->setInterval(800);
    connect(m_patternTimer, &QTimer::timeout, this, &PiPatterns::showNextPatternTile);

    // ── Tile click connections ────────────────────────────────────────────
    for (int i = 0; i < 16; ++i) {
        connect(matrix->elements[i], &QPushButton::clicked,
                this, [this, i]() { onTileClicked(i); });
    }

    // ── Button connections ────────────────────────────────────────────────
    connect(b_play, &QPushButton::clicked, this, [this]() {
        // Reset session score when entering from hub
        m_score = 0;
        m_scoreLabel->setText("Score: 0");
        m_statusLabel->setText("Select difficulty and press Start.");
        m_state = GameState::Idle;
        resetMatrix();
        m_startBtn->setEnabled(true);
        for (QPushButton *d : {m_diffBit, m_diffNibble, m_diffByte}) d->setEnabled(true);
        m_stack->setCurrentIndex(1);
    });

    connect(b_backFromMatrix, &QPushButton::clicked, this, [this]() {
        m_patternTimer->stop();
        resetMatrix();
        m_state = GameState::Idle;
        m_stack->setCurrentIndex(0);
    });

    connect(m_startBtn,  &QPushButton::clicked, this, &PiPatterns::startGame);
    connect(m_diffBit,   &QPushButton::clicked, this, [this]() { setDifficulty(Difficulty::Bit);    });
    connect(m_diffNibble,&QPushButton::clicked, this, [this]() { setDifficulty(Difficulty::Nibble); });
    connect(m_diffByte,  &QPushButton::clicked, this, [this]() { setDifficulty(Difficulty::Byte);   });

    // b_back wired by train.cc to openTrainHub()
}

// ── Helpers ───────────────────────────────────────────────────────────────

int PiPatterns::pointsPerTile() const {
    switch (m_difficulty) {
        case Difficulty::Nibble: return 4;
        case Difficulty::Byte:   return 8;
        default:                 return 1;  // Bit
    }
}

void PiPatterns::generatePattern() {
    m_pattern.clear();
    for (int i = 0; i < 5; ++i)
        m_pattern.append(QRandomGenerator::global()->bounded(16));
}

void PiPatterns::resetMatrix() {
    for (int i = 0; i < 16; ++i)
        matrix->elements[i]->setHighlighted(false);
}

void PiPatterns::setDifficulty(Difficulty d) {
    m_difficulty = d;
    updateDiffButtons();
}

void PiPatterns::updateDiffButtons() {
    m_diffBit->setStyleSheet(    m_difficulty == Difficulty::Bit    ? selectedDiffStyle() : buttonStyle());
    m_diffNibble->setStyleSheet( m_difficulty == Difficulty::Nibble ? selectedDiffStyle() : buttonStyle());
    m_diffByte->setStyleSheet(   m_difficulty == Difficulty::Byte   ? selectedDiffStyle() : buttonStyle());
}

// ── Game flow ─────────────────────────────────────────────────────────────

void PiPatterns::startGame() {
    m_roundScore   = 0;
    m_roundCorrect = 0;
    m_playerStep   = 0;
    m_showStep     = 0;
    m_state        = GameState::Showing;

    generatePattern();
    resetMatrix();
    matrix->show();

    m_statusLabel->setText("Watch the pattern...");
    m_startBtn->setEnabled(false);
    for (QPushButton *d : {m_diffBit, m_diffNibble, m_diffByte}) d->setEnabled(false);

    m_patternTimer->start();
}

void PiPatterns::showNextPatternTile() {
    // Un-highlight the tile shown last tick
    if (m_showStep > 0)
        matrix->elements[m_pattern[m_showStep - 1]]->setHighlighted(false);

    if (m_showStep < 5) {
        matrix->elements[m_pattern[m_showStep]]->setHighlighted(true);
        m_showStep++;
    } else {
        // All 5 tiles shown — hand control to the player
        m_patternTimer->stop();
        m_state = GameState::Input;
        m_statusLabel->setText("Your turn! Repeat the pattern.");
    }
}

void PiPatterns::onTileClicked(int index) {
    if (m_state != GameState::Input) return;

    bool correct = (index == m_pattern[m_playerStep]);
    int  pts     = pointsPerTile();

    if (correct) {
        m_score        += pts;
        m_roundScore   += pts;
        m_roundCorrect++;
    } else {
        m_score      -= pts;
        m_roundScore -= pts;
    }

    matrix->elements[index]->flashFeedback(correct);
    m_scoreLabel->setText(QString("Score: %1").arg(m_score));

    m_playerStep++;
    if (m_playerStep == 5)
        QTimer::singleShot(400, this, &PiPatterns::finishRound);
}

void PiPatterns::finishRound() {
    m_state = GameState::Result;

    QString rating;
    if      (m_roundCorrect == 5) rating = "Perfect! Your PiPet is amazed!";
    else if (m_roundCorrect >= 4) rating = "Great job! So close!";
    else if (m_roundCorrect >= 3) rating = "Good job! Keep it up!";
    else if (m_roundCorrect >= 1) rating = "Keep practicing!";
    else                          rating = "Your pet believes in you!";

    int xpEarned       = qMax(0, m_roundScore);
    int intelligenceDelta = qMax(1, xpEarned / 5) + (m_roundCorrect == 5 ? 25 : 0);
    int happinessDelta    = m_roundScore / 25;

    QString intSign  = QString("+%1").arg(intelligenceDelta);
    QString hapSign  = (happinessDelta >= 0)
                       ? QString("+%1").arg(happinessDelta)
                       : QString::number(happinessDelta);

    m_statusLabel->setText(
        QString("%1\n\nIntelligence: %2\nHappiness: %3")
            .arg(rating, intSign, hapSign));

    matrix->hide();

    emit gameFinished(m_roundScore, xpEarned, m_roundCorrect == 5);

    // Re-enable controls for another round
    m_startBtn->setEnabled(true);
    for (QPushButton *d : {m_diffBit, m_diffNibble, m_diffByte}) d->setEnabled(true);
    updateDiffButtons();
}
