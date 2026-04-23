/*
 * mode.cc - Mode hub implementation.
 * Author(s): Sasha C. Guerrero, Luke Cerwin
 */
#include "mode.h"
#include <QPainter>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

Mode::Mode(Player *player, QWidget *parent)
    : QWidget{parent}, player(player)
{
    m_bg.load(":/images/Backgrounds/mode.png");
    m_kitchenPx.load (":/images/Backgrounds/kitchen_16bit.png");
    m_bathroomPx.load(":/images/Backgrounds/bathroom_16bit.png");
    m_bedroomPx.load (":/images/Backgrounds/bedroom_16bit.png");

    b_settings = new QPushButton(this);
    b_settings->setIcon(QIcon(":/images/Assets/settings.png"));
    b_settings->setIconSize(QSize(28, 28));
    b_settings->setFixedSize(38, 38);
    b_settings->setStyleSheet(
        "QPushButton { background-color: rgba(0,0,0,140); border-radius: 19px;"
        "border: 2px solid #FBA8FF; }"
        "QPushButton:pressed { background-color: rgba(72,80,219,180); }");
    connect(b_settings, SIGNAL(clicked()), this, SLOT(openAbout()));

    petNameLabel = new QLabel(this);
    petNameLabel->setAlignment(Qt::AlignCenter);
    petNameLabel->setStyleSheet(
        "QLabel { background-color: rgba(0,0,0,150); border-radius: 8px;"
        "padding: 4px 10px; color: #ffd700; font-size: 16px; font-weight: bold; }");

    character = new Character(this);
    character->setFixedSize(160, 160);

    m_angerPx.load(":/images/Sprites/pets/icons/anger_mark.png");
    angerMark = new QLabel(this);
    angerMark->setPixmap(m_angerPx.scaled(32, 32, Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation));
    angerMark->setStyleSheet("QLabel { background-color: transparent; }");
    angerMark->setFixedSize(36, 36);
    angerMark->hide();

    // ── Hunger hint label — appears below the character when pet is angry ──
    hungerHintLabel = new QLabel(this);
    hungerHintLabel->setAlignment(Qt::AlignCenter);
    hungerHintLabel->setWordWrap(true);
    hungerHintLabel->setText("Your pet is hungry!\nMake sure you feed them!");
    hungerHintLabel->setStyleSheet(
        "QLabel { background-color: rgba(180,30,30,200); border-radius: 8px;"
        "padding: 5px 10px; color: #ffd700; font-size: 13px; font-weight: bold; }");
    hungerHintLabel->hide();

    // Auto-hide after 5 s — restarted each decay tick while still angry
    m_hungerHintTimer = new QTimer(this);
    m_hungerHintTimer->setSingleShot(true);
    m_hungerHintTimer->setInterval(5000);
    connect(m_hungerHintTimer, &QTimer::timeout,
            hungerHintLabel, &QLabel::hide);

    timekeeper = new Clock();
    time       = new QTime();
    timer      = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(updateClock()));
    timer->start(1000);

    statsBox  = new QGroupBox("Condition", this);
    statsGrid = new QGridLayout();
    statsBox->setLayout(statsGrid);
    statsBox->setStyleSheet(
        "QGroupBox { background-color: rgba(0,0,0,150); border-radius: 8px;"
        "color: mistyrose; margin-top: 22px; }"
        "QGroupBox::title { color: #ffd700; subcontrol-origin: margin;"
        "subcontrol-position: top center; }");

    hunger_label    = new QLabel("Hunger",    this);
    energy_label    = new QLabel("Energy",    this);
    happiness_label = new QLabel("Happiness", this);
    hunger_bar      = new QProgressBar(this);
    energy_bar      = new QProgressBar(this);
    happiness_bar   = new QProgressBar(this);

    for (QProgressBar *bar : {hunger_bar, energy_bar, happiness_bar}) {
        bar->setRange(0, 100);
        bar->setTextVisible(true);
    }

    statsGrid->addWidget(hunger_label,    0, 0); statsGrid->addWidget(hunger_bar,    0, 1);
    statsGrid->addWidget(energy_label,    1, 0); statsGrid->addWidget(energy_bar,    1, 1);
    statsGrid->addWidget(happiness_label, 2, 0); statsGrid->addWidget(happiness_bar, 2, 1);

    feedBubble  = new QLabel(this);
    groomBubble = new QLabel(this);
    sleepBubble = new QLabel(this);

    for (QLabel *b : {feedBubble, groomBubble, sleepBubble}) {
        b->setAlignment(Qt::AlignCenter);
        b->setCursor(Qt::PointingHandCursor);
        b->setStyleSheet("QLabel { background-color: transparent; }");
    }

    b_care   = new QPushButton(this); b_care->hide();
    b_train  = new QPushButton("Train",  this);
    b_battle = new QPushButton("Battle", this);
    b_gear   = new QPushButton("Theater",   this);

    QString btnStyle = R"(
        QPushButton { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4850DB, stop:1 #4A71DB);
            border: 2px inset #FBA8FF; border-radius: 10px;
            padding: 6px; font: bold; color: mistyrose; }
        QPushButton:pressed { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4A71DB, stop:1 #4850DB); })";
    b_train->setStyleSheet(btnStyle);
    b_battle->setStyleSheet(btnStyle);
    b_gear->setStyleSheet(btnStyle);

    m_lastAgeGroup = player->getPet().age_group();
    refreshDisplay();
}

void Mode::resizeEvent(QResizeEvent *e) { QWidget::resizeEvent(e); layoutWidgets(); }

void Mode::layoutWidgets() {
    int w = width(), h = height();
    static constexpr int kCharSize = 160, kStatsH = 100, kBubbleH = 80;
    static constexpr int kBtnH = 36, kMargin = 8, kSpacing = 6;

    int btnY    = h - kBtnH    - kMargin;
    int bubbleY = btnY  - kBubbleH - kSpacing;
    int statsY  = bubbleY - kStatsH  - kSpacing;
    int charY   = statsY  - kCharSize - kSpacing;

    b_settings->move(w - 46, kMargin);
    petNameLabel->setGeometry((w - 200) / 2, 8, 200, 28);

    int charX = (w - kCharSize) / 2;
    character->setGeometry(charX, charY, kCharSize, kCharSize);
    angerMark->setGeometry(charX + kCharSize/2 + 32, charY, 48, 48);

    // Hunger hint floats just below the character, centered
    int hintW = 220;
    hungerHintLabel->setGeometry((w - hintW) / 2, charY + kCharSize + 4, hintW, 44);

    statsBox->setGeometry(kMargin, statsY, w - kMargin*2, kStatsH);

    int bubbleW = (w - kMargin*4) / 3;
    feedBubble->setGeometry (kMargin,               bubbleY, bubbleW, kBubbleH);
    groomBubble->setGeometry(kMargin*2 + bubbleW,   bubbleY, bubbleW, kBubbleH);
    sleepBubble->setGeometry(kMargin*3 + bubbleW*2, bubbleY, bubbleW, kBubbleH);

    int btnW = (w - kMargin*4) / 3;
    b_train->setGeometry (kMargin,            btnY, btnW, kBtnH);
    b_battle->setGeometry(kMargin*2 + btnW,   btnY, btnW, kBtnH);
    b_gear->setGeometry  (kMargin*3 + btnW*2, btnY, btnW, kBtnH);
}

void Mode::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);
    QPainter p(this);
    if (!m_bg.isNull()) p.drawPixmap(0, 0, width(), height(), m_bg);
    drawBubble(p, feedBubble->geometry(),  m_kitchenPx,  "Feed",  true);
    drawBubble(p, groomBubble->geometry(), m_bathroomPx, "Groom", true);
    drawBubble(p, sleepBubble->geometry(), m_bedroomPx,  "Sleep", true);
}

void Mode::drawBubble(QPainter &p, QRect rect,
                      const QPixmap &bg, const QString &label, bool dimmed)
{
    if (rect.isEmpty()) return;
    p.save();
    QPainterPath path;
    path.addRoundedRect(rect, 12, 12);
    p.setClipPath(path);
    if (!bg.isNull()) {
        QPixmap scaled = bg.scaled(rect.size(), Qt::KeepAspectRatioByExpanding,
                                   Qt::SmoothTransformation);
        int ox = (scaled.width()  - rect.width())  / 2;
        int oy = (scaled.height() - rect.height()) / 2;
        p.drawPixmap(rect, scaled, QRect(ox, oy, rect.width(), rect.height()));
    }
    if (dimmed) p.fillPath(path, QColor(0, 0, 0, 130));
    p.restore();
    p.setPen(QPen(QColor(0xFB, 0xA8, 0xFF), 2));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(rect.adjusted(1,1,-1,-1), 12, 12);
    p.setPen(Qt::white);
    p.setFont(QFont("monospace", 13, QFont::Bold));
    p.drawText(rect, Qt::AlignCenter, label);
}

void Mode::setPetType(Character::PetType type) {
    petType = type;
    character->setPetType(type);
    refreshDisplay();
}

void Mode::refreshDisplay() {
    updateStatBars();
    updateIndicators();
    petNameLabel->setText(player->getPet().name());

    QString hat  = player->getPet().hat();
    if (!hat.isEmpty()) {
        QString folder, prefix;
        QString type = player->getPet().pet_type();
        if      (type == "ElectricAxolotl") { folder = "axolotl";   prefix = "axolotl";   }
        else if (type == "SeelCat")         { folder = "seelcat";   prefix = "seelcat";   }
        else                                { folder = "dragondog"; prefix = "dragondog"; }

        QString stage = player->getPet().age_group();
        QString infix = (stage == "Teen") ? "teen_" : (stage == "Adult") ? "adult_" : "";
        QString path  = QString(":/images/Sprites/pets/%1/%2_%3%4.gif")
                            .arg(folder, prefix, infix, hat);

        // Only (re)allocate the QMovie when the path actually changes —
        // avoids a new heap allocation every 10-second decay tick.
        if (path != m_cachedHatPath || !m_cachedHatMovie || !m_cachedHatMovie->isValid()) {
            delete m_cachedHatMovie;
            m_cachedHatMovie = new QMovie(path, QByteArray(), character);
            m_cachedHatPath  = path;
        }

        if (m_cachedHatMovie->isValid()) {
            QLabel *disp = character->findChild<QLabel *>();
            if (disp) {
                // Only swap the movie if it isn't already playing — avoids
                // restarting the animation every decay tick.
                if (disp->movie() != m_cachedHatMovie) {
                    if (disp->movie()) disp->movie()->stop();
                    disp->setMovie(m_cachedHatMovie);
                    m_cachedHatMovie->start();
                }
            } else {
                // Fallback: no internal label found, sync without hat
                character->syncWithPlayer(*player, petType);
            }
        } else {
            // Hat path invalid — fall back to base idle sprite
            character->syncWithPlayer(*player, petType);
        }
    } else {
        // No hat equipped — clear the cache and show the base idle sprite
        if (m_cachedHatMovie) {
            delete m_cachedHatMovie;
            m_cachedHatMovie = nullptr;
            m_cachedHatPath  = "";
        }
        character->syncWithPlayer(*player, petType);
    }
    layoutWidgets();
}

void Mode::updateClock() {
    timekeeper->increment_elapsed_time(1);
    secondsSinceDecay++;
    if (secondsSinceDecay >= DECAY_INTERVAL_SECS) {
        secondsSinceDecay = 0;
        decayStats();
    }
}

void Mode::decayStats() {
    PiPet pet = player->getPet();
    pet.set_hunger   (qMax(0, pet.hunger()    - 1)); // FIX: was -100
    pet.set_energy   (qMax(0, pet.energy()    - 1));
    pet.set_happiness(qMax(0, pet.happiness() - 1));
    player->setPet(pet);
    // Route through refreshDisplay() so hat GIFs are preserved.
    // updateStatBars() and updateIndicators() are called inside refreshDisplay().
    refreshDisplay();
}

void Mode::updateStatBars() {
    PiPet pet = player->getPet();
    hunger_bar->setValue(pet.hunger());
    energy_bar->setValue(pet.energy());
    happiness_bar->setValue(pet.happiness());
}

void Mode::updateIndicators() {
    PiPet pet = player->getPet();

    bool angry    = (pet.hunger() < 25);
    bool sleeping = (pet.energy() < 25);

    if (angry) {
        angerMark->show();
        // Show the hint and restart the auto-hide timer every decay tick
        // so it stays visible as long as the pet remains hungry.
        hungerHintLabel->show();
        hungerHintLabel->raise();
        m_hungerHintTimer->start();  // restarts if already running
    } else {
        angerMark->hide();
        hungerHintLabel->hide();
        m_hungerHintTimer->stop();
    }

    // NOTE: do NOT call character->updateEmotionFromStats() here.
    // Emotion is resolved inside refreshDisplay() → syncWithPlayer(), so that
    // the hat GIF path is always chosen AFTER the emotion state is current.
    // Calling it here would race against / overwrite the hat GIF.

    // ── Achievement signals ───────────────────────────────────────────────
    // Temper Tantrum: pet sleeping AND angry at the same time
    if (sleeping && angry)
        emit temperTantrum();

    // Age-up detection
    QString currentAge = pet.age_group();
    if (currentAge != m_lastAgeGroup) {
        m_lastAgeGroup = currentAge;
        emit petAgedUp(currentAge);
    }
}

// ── About page —──────────────────
void Mode::openAbout() {
    PiPet pet = player->getPet();

    // Build a QDialog instead of QMessageBox so we can paint a background
    QDialog about(this);
    about.setWindowTitle("About piPet");
    about.setFixedSize(400, 460);

    // Outer layout
    QVBoxLayout *dlgLayout = new QVBoxLayout(&about);
    dlgLayout->setContentsMargins(20, 20, 20, 20);
    dlgLayout->setSpacing(0);

    // Text label
    QLabel *infoLabel = new QLabel(&about);
    infoLabel->setWordWrap(true);
    infoLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    infoLabel->setStyleSheet(
        "QLabel { background-color: transparent;"
        "color: mistyrose; font-family: monospace; font-size: 13px; }");

    QString currentTime = time->currentTime().toString("hh:mm:ss AP");
    int elapsed = timekeeper->elapsed_time();

    infoLabel->setText(QString(
                           "<b style='color:#ffd700;font-size:16px;'>piPet About</b><br><br>"
                           "<b style='color:#ffd700;'>── Session ──</b><br>"
                           "Time Played:  %1s<br><br>"
                           "<b style='color:#ffd700;'>── Pet ──</b><br>"
                           "Name:         %2<br>"
                           "Age Group:    %3<br>"
                           "Days Old:     %4<br><br>"
                           "<b style='color:#ffd700;'>── Condition ──</b><br>"
                           "Hunger:     %5 / 100<br>"
                           "Energy:     %6 / 100<br>"
                           "Happiness:  %7 / 100<br>"
                           "Hygiene: %8  Strength: %9   Intellect: %10<br><br>"
                           "<b style='color:#ffd700;'>── Battle Stats ──</b><br>"
                           "Attack: %12  Defense: %13<br><br>"
                           "<span style='color:#aaa;font-size:11px;'>"
                           "Luke C. · Sasha G. · Cesar R.<br>"
                           "Camden G. · Tanya M.</span>"
                           )
                           .arg(elapsed)
                           .arg(pet.name()) .arg(pet.age_group())
                           .arg(pet.days_old())    .arg(pet.hunger())
                           .arg(pet.energy())      .arg(pet.happiness())
                           .arg(pet.hygiene())     .arg(pet.strength())
                           .arg(pet.intelligence()) .arg(pet.attack())
                           .arg(pet.defense()));

    // Close button
    QPushButton *closeBtn = new QPushButton("Close", &about);
    closeBtn->setFixedHeight(36);
    closeBtn->setStyleSheet(
        "QPushButton { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #4850DB,stop:1 #4A71DB); border:2px inset #FBA8FF;"
        "border-radius:10px; padding:4px; font:bold; color:mistyrose; }"
        "QPushButton:pressed { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #4A71DB,stop:1 #4850DB); }");
    connect(closeBtn, &QPushButton::clicked, &about, &QDialog::accept);

    dlgLayout->addWidget(infoLabel, 1);
    dlgLayout->addWidget(closeBtn,  0);

    about.exec();
}
