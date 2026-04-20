/*
 * sleep.cc - Sleep screen with character GIF.
 * Cuddle replaces Brush Teeth.
 * Author(s): Luke Cewin & Sasha Guerrero
 */
#include "sleep.h"
#include <QPainter>

Sleep::Sleep(Player *player, Character::PetType petType, QWidget *parent)
    : QWidget{parent}, player(player), petType(petType)
{
    m_bg.load(":/images/Backgrounds/bedroom_16bit.png");

    layout      = new QVBoxLayout();
    actionsBox  = new QGroupBox("Sleep Actions");
    actionsGrid = new QGridLayout();

    // ── Character GIF ─────────────────────────────────────────────────────
    character = new Character(this);
    character->setFixedSize(160, 160);
    character->syncWithPlayer(*player, petType);

    sleepDisplay = new QLabel();
    sleepDisplay->setAlignment(Qt::AlignCenter);
    sleepDisplay->setWordWrap(true);
    sleepDisplay->setStyleSheet(
        "QLabel { background-color: rgba(0,0,0,160); border-radius: 6px;"
        "padding: 6px; color: mistyrose; }");
    updateSleepDisplay();

    cuddleBtn   = new QPushButton("🤗 Cuddle Pet");
    wearPjsBtn  = new QPushButton("👖 Wear Pjs");
    readBookBtn = new QPushButton("📖 Bed Time Story");
    tuckInBtn   = new QPushButton("🛌 Tuck In Bed");

    actionsGrid->addWidget(cuddleBtn,   0, 0, Qt::AlignCenter);
    actionsGrid->addWidget(wearPjsBtn,  0, 1, Qt::AlignCenter);
    actionsGrid->addWidget(readBookBtn, 1, 0, Qt::AlignCenter);
    actionsGrid->addWidget(tuckInBtn,   1, 1, Qt::AlignCenter);
    actionsBox->setLayout(actionsGrid);

    connect(cuddleBtn,   SIGNAL(clicked()), this, SLOT(cuddle()));
    connect(wearPjsBtn,  SIGNAL(clicked()), this, SLOT(wearPjs()));
    connect(readBookBtn, SIGNAL(clicked()), this, SLOT(readBook()));
    connect(tuckInBtn,   SIGNAL(clicked()), this, SLOT(tuckIn()));

    actionsBox->setStyleSheet(
        "QGroupBox { background-color: rgba(0,0,0,155); border-radius: 8px;"
        "color: mistyrose; margin-top: 30px; }"
        "QGroupBox::title { color: mistyrose; subcontrol-origin: margin;"
        "subcontrol-position: top center; padding: 0 4px; }");

    backBtn     = new QPushButton("Back to Care Hub!");
    backBtn->setIcon(QIcon(":/images/Assets/left.png"));
    backBtn->setStyleSheet(R"(
        QPushButton { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4850DB, stop:1 #4A71DB);
            border: 2px inset #FBA8FF; border-radius: 10px;
            padding: 4px; font: bold; color: mistyrose; }
        QPushButton:pressed { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4A71DB, stop:1 #4850DB); })");
    // Character GIF at top center, buttons below
    layout->addSpacing(150);
    layout->addWidget(character, 0, Qt::AlignCenter);
    layout->addStretch();
    layout->addWidget(sleepDisplay);
    layout->addWidget(actionsBox);
    layout->addWidget(backBtn);
    this->setLayout(layout);
}

void Sleep::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    if (!m_bg.isNull())
        painter.drawPixmap(0, 0, width(), height(), m_bg);
}

void Sleep::updateSleepDisplay() {
    sleepDisplay->setText(
        QString("Energy: %1 / 100").arg(player->getPet().energy()));
}

void Sleep::applySleepAction(int boost, const QString &message) {
    PiPet pet = player->getPet();
    if (pet.energy() >= 100) {
        sleepDisplay->setText("Your pet is fully rested!");
        return;
    }
    int newVal = qMin(100, pet.energy() + boost);
    pet.set_energy(newVal);
    player->setPet(pet);
    sleepDisplay->setText(
        QString("%1  |  Energy: %2 / 100").arg(message).arg(newVal));
}

void Sleep::cuddle()    { applySleepAction(5,  "Aww so cute!"); }
void Sleep::wearPjs()   { applySleepAction(5,  "Wear Pjs!"); }
void Sleep::readBook()  { applySleepAction(10, "Bed Time Story!"); }
void Sleep::tuckIn()    { applySleepAction(20, "Tucked In!"); }
