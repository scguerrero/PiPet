/*
 * mode.cc - Mode hub implementation.
 * Clock removed from main screen — shown in About/Settings only.
 * About page uses main.png background, no blue box behind text.
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

    // ── Settings button top-right ─────────────────────────────────────────
    b_settings = new QPushButton(this);
    b_settings->setIcon(QIcon(":/images/Assets/settings.png"));
    b_settings->setIconSize(QSize(28, 28));
    b_settings->setFixedSize(38, 38);
    b_settings->setStyleSheet(
        "QPushButton { background-color: rgba(0,0,0,140); border-radius: 19px;"
        "border: 2px solid #FBA8FF; }"
        "QPushButton:pressed { background-color: rgba(72,80,219,180); }");
    connect(b_settings, SIGNAL(clicked()), this, SLOT(openAbout()));

    // ── Pet name label ────────────────────────────────────────────────────
    petNameLabel = new QLabel(this);
    petNameLabel->setAlignment(Qt::AlignCenter);
    petNameLabel->setStyleSheet(
        "QLabel { background-color: rgba(0,0,0,150); border-radius: 8px;"
        "padding: 4px 10px; color: #ffd700;"
        "font-size: 16px; font-weight: bold; }");

    // ── Character GIF ─────────────────────────────────────────────────────
    character = new Character(this);
    character->setFixedSize(160, 160);

    // ── Anger mark ────────────────────────────────────────────────────────
    m_angerPx.load(":/images/Sprites/pets/icons/anger_mark.png");
    angerMark = new QLabel(this);
    angerMark->setPixmap(m_angerPx.scaled(32, 32, Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation));
    angerMark->setStyleSheet("QLabel { background-color: transparent; }");
    angerMark->setFixedSize(36, 36);
    angerMark->hide();

    // ── Clock — private, only used in About page ──────────────────────────
    timekeeper = new Clock();
    time       = new QTime();
    timer      = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(updateClock()));
    timer->start(1000);

    // ── Stat bars ─────────────────────────────────────────────────────────
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

    statsGrid->addWidget(hunger_label,    0, 0);
    statsGrid->addWidget(hunger_bar,      0, 1);
    statsGrid->addWidget(energy_label,    1, 0);
    statsGrid->addWidget(energy_bar,      1, 1);
    statsGrid->addWidget(happiness_label, 2, 0);
    statsGrid->addWidget(happiness_bar,   2, 1);

    // ── Bubble care labels ────────────────────────────────────────────────
    feedBubble  = new QLabel(this);
    groomBubble = new QLabel(this);
    sleepBubble = new QLabel(this);

    for (QLabel *b : {feedBubble, groomBubble, sleepBubble}) {
        b->setAlignment(Qt::AlignCenter);
        b->setCursor(Qt::PointingHandCursor);
        b->setStyleSheet("QLabel { background-color: transparent; }");
    }

    // ── Navigation buttons ────────────────────────────────────────────────
    b_care   = new QPushButton(this);
    b_train  = new QPushButton("Train",  this);
    b_battle = new QPushButton("Battle", this);
    b_gear   = new QPushButton("Gear",   this);
    b_care->hide();

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

    refreshDisplay();
}

// ── Layout ────────────────────────────────────────────────────────────────
void Mode::resizeEvent(QResizeEvent *e) {
    QWidget::resizeEvent(e);
    layoutWidgets();
}

void Mode::layoutWidgets() {
    int w = width();

    // Settings top-right — no clock labels on main screen
    b_settings->move(w - 46, 8);

    // Pet name centered at top
    petNameLabel->setGeometry((w - 200) / 2, 8, 200, 28);

    // Character centered below name
    int charSize = 160;
    int charX = (w - charSize) / 2;
    int charY = 48;
    character->setGeometry(charX, charY, charSize, charSize);

    // Anger mark just above character head
    angerMark->setGeometry(charX + charSize/2 + 32, charY, 48, 48);

    // Stat bars below character
    statsBox->setGeometry(8, charY + charSize + 8, w - 16, 100);

    // Bubble buttons below stat bars
    int bubbleY = charY + charSize + 118;
    int bubbleH = 80;
    int bubbleW = (w - 32) / 3;
    int sp      = 8;
    feedBubble->setGeometry (sp,                   bubbleY, bubbleW, bubbleH);
    groomBubble->setGeometry(sp + bubbleW + sp,     bubbleY, bubbleW, bubbleH);
    sleepBubble->setGeometry(sp + (bubbleW+sp)*2,   bubbleY, bubbleW, bubbleH);

    // Train / Battle / Gear below bubbles
    int btnY = bubbleY + bubbleH + 8;
    int btnW = (w - 32) / 3;
    b_train->setGeometry (8,               btnY, btnW, 36);
    b_battle->setGeometry(8 + btnW + 8,    btnY, btnW, 36);
    b_gear->setGeometry  (8+(btnW+8)*2,    btnY, btnW, 36);
}

// ── Paint ─────────────────────────────────────────────────────────────────
void Mode::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);
    QPainter p(this);
    if (!m_bg.isNull())
        p.drawPixmap(0, 0, width(), height(), m_bg);

    drawBubble(p, feedBubble->geometry(),  m_kitchenPx,  "Feed",  true);
    drawBubble(p, groomBubble->geometry(), m_bathroomPx, "Groom", true);
    drawBubble(p, sleepBubble->geometry(), m_bedroomPx,  "Sleep", true);

}

void Mode::drawBubble(QPainter &p, QRect rect,
                const QPixmap &bg, const QString &label,
                bool dimmed)
{
    if (rect.isEmpty()) return;
    p.save();
    QPainterPath path;
    path.addRoundedRect(rect, 12, 12);
    p.setClipPath(path);
    if (!bg.isNull()) {
        QPixmap scaled = bg.scaled(rect.size(),
                                   Qt::KeepAspectRatioByExpanding,
                                   Qt::SmoothTransformation);
        int ox = (scaled.width()  - rect.width())  / 2;
        int oy = (scaled.height() - rect.height()) / 2;
        p.drawPixmap(rect, scaled, QRect(ox, oy, rect.width(), rect.height()));
    }
    if (dimmed) {
        p.fillPath(path, QColor(0, 0, 0, 130));}
    p.restore();
    p.setPen(QPen(QColor("#FBA8FF"), 2));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(rect.adjusted(1,1,-1,-1), 12, 12);
    p.setPen(Qt::white);
    p.setFont(QFont("monospace", 13, QFont::Bold));
    p.drawText(rect, Qt::AlignCenter, label);
}

// ── Public API ────────────────────────────────────────────────────────────
void Mode::setPetType(Character::PetType type) {
    petType = type;
    character->setPetType(type);
    refreshDisplay();
}

void Mode::refreshDisplay() {
    updateStatBars();
    updateIndicators();
    petNameLabel->setText(player->getPet().name());
    character->syncWithPlayer(*player, petType);
    layoutWidgets();
}

// ── Clock tick (private — feeds About page only) ──────────────────────────
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
    pet.set_hunger   (qMax(0, pet.hunger()    - 100));
    pet.set_energy   (qMax(0, pet.energy()    - 1));
    pet.set_happiness(qMax(0, pet.happiness() - 1));
    player->setPet(pet);
    updateStatBars();
    updateIndicators();
    character->syncWithPlayer(*player, petType);
}

void Mode::updateStatBars() {
    PiPet pet = player->getPet();
    hunger_bar->setValue(pet.hunger());
    energy_bar->setValue(pet.energy());
    happiness_bar->setValue(pet.happiness());
}

void Mode::updateIndicators() {
    PiPet pet = player->getPet();
    if (pet.hunger() < 25) angerMark->show();
    else                    angerMark->hide();
    character->updateEmotionFromStats(pet.energy(), pet.hunger());
}

// ── About page —──────────────────
void Mode::openAbout() {
    PiPet pet = player->getPet();

    // Build a QDialog instead of QMessageBox so we can paint a background
    QDialog about(this);
    about.setWindowTitle("About piPet");
    about.setFixedSize(400, 560);

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
        "<b style='color:#ffd700;font-size:16px;'>piPet</b><br>"
        "<span style='color:#aaa;'>Digital Pet Game</span><br><br>"
        "<b style='color:#ffd700;'>── Session ──</b><br>"
        "Current Time: %1<br>"
        "Time Played:  %2s<br><br>"
        "<b style='color:#ffd700;'>── Pet ──</b><br>"
        "Name:         %3<br>"
        "Age Group:    %4<br>"
        "Days Old:     %5<br><br>"
        "<b style='color:#ffd700;'>── Condition ──</b><br>"
        "Hunger:       %6 / 100<br>"
        "Energy:       %7 / 100<br>"
        "Happiness:    %8 / 100<br>"
        "Strength:     %9 / 100<br>"
        "Hygiene:      %10 / 100<br>"
        "Intelligence: %11 / 100<br><br>"
        "<b style='color:#ffd700;'>── Battle Stats ──</b><br>"
        "Attack:       %12<br>"
        "Defense:      %13<br>"
        "Hit Points:   %14<br><br>"
        "<span style='color:#aaa;font-size:11px;'>"
        "Luke C. · Sasha G. · Camden G.<br>"
        "Tanya M. · Cesar R.</span>"
    )
    .arg(currentTime)
    .arg(elapsed)
    .arg(pet.name())        .arg(pet.age_group())
    .arg(pet.days_old())    .arg(pet.hunger())
    .arg(pet.energy())      .arg(pet.happiness())
    .arg(pet.strength())    .arg(pet.hygiene())
    .arg(pet.intelligence()).arg(pet.attack())
    .arg(pet.defense())     .arg(pet.hit_points()));

    // Close button
    QPushButton *closeBtn = new QPushButton("Close", &about);
    closeBtn->setFixedHeight(36);
    closeBtn->setStyleSheet(
        "QPushButton { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #4850DB, stop:1 #4A71DB);"
        "border: 2px inset #FBA8FF; border-radius: 10px;"
        "padding: 4px; font: bold; color: mistyrose; }"
        "QPushButton:pressed { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #4A71DB, stop:1 #4850DB); }");
    connect(closeBtn, &QPushButton::clicked, &about, &QDialog::accept);

    dlgLayout->addWidget(infoLabel, 1);
    dlgLayout->addWidget(closeBtn,  0);

    about.exec();
}
