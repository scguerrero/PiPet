/*
 * feed.cc - Feed screen.
 * Background: kitchen_16bit.jpg
 * Uses paintEvent for reliable background — QPalette fails when
 * widget size is 0x0 during construction.
 * Author(s): Tanya Magurupira
 */
#include "feed.h"
#include <QPainter>

Feed::Feed(Player *player, QWidget *parent)
    : QWidget{parent}, player(player)
{
    // Load background once at construction — drawn in paintEvent
    m_bg.load(":/images/Backgrounds/kitchen_16bit.jpg");

    layout      = new QVBoxLayout();
    actionsBox  = new QGroupBox("Feed Actions");
    actionsGrid = new QGridLayout();

    hungerDisplay = new QLabel();
    hungerDisplay->setAlignment(Qt::AlignCenter);
    hungerDisplay->setWordWrap(true);
    hungerDisplay->setStyleSheet(
        "QLabel { background-color: rgba(0,0,0,160); border-radius: 6px;"
        "padding: 6px; color: mistyrose; }");
    updateHungerDisplay();

    appleBtn = new QPushButton("🍎 Apple");
    boneBtn  = new QPushButton("🦴 Bone");
    drinkBtn = new QPushButton("🥤 A Drink");
    pizzaBtn = new QPushButton("🍕 Pizza");
    backBtn  = new QPushButton("Back to Care Hub!");

    backBtn->setIcon(QIcon(":/images/Assets/left.png"));

    actionsGrid->addWidget(appleBtn, 0, 0, Qt::AlignCenter);
    actionsGrid->addWidget(boneBtn,  0, 1, Qt::AlignCenter);
    actionsGrid->addWidget(drinkBtn, 1, 0, Qt::AlignCenter);
    actionsGrid->addWidget(pizzaBtn, 1, 1, Qt::AlignCenter);
    actionsBox->setLayout(actionsGrid);

    connect(appleBtn, SIGNAL(clicked()), this, SLOT(apple()));
    connect(boneBtn,  SIGNAL(clicked()), this, SLOT(bone()));
    connect(drinkBtn, SIGNAL(clicked()), this, SLOT(drink()));
    connect(pizzaBtn, SIGNAL(clicked()), this, SLOT(pizza()));

    layout->addStretch();
    layout->addWidget(hungerDisplay);
    layout->addWidget(actionsBox);
    layout->addWidget(backBtn);
    this->setLayout(layout);

    actionsBox->setStyleSheet(
        "QGroupBox { background-color: rgba(0,0,0,155); border-radius: 8px;"
        "color: mistyrose; margin-top: 30px; }"
        "QGroupBox::title { color: mistyrose; subcontrol-origin: margin;"
        "subcontrol-position: top center; padding: 0 4px; }");

    backBtn->setStyleSheet(R"(
        QPushButton { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4850DB, stop:1 #4A71DB);
            border: 2px inset #FBA8FF; border-radius: 10px;
            padding: 4px; font: bold; color: mistyrose; }
        QPushButton:pressed { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4A71DB, stop:1 #4850DB); })");
}

// paintEvent is called every time Qt redraws the widget —
// always has the correct size by the time it fires
void Feed::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    if (!m_bg.isNull())
        painter.drawPixmap(0, 0, width(), height(), m_bg);
}

void Feed::updateHungerDisplay() {
    hungerDisplay->setText(
        QString("Hunger: %1 / 100").arg(player->getPet().hunger()));
}

void Feed::applyHungerAction(int boost, const QString &message) {
    PiPet pet = player->getPet();
    if (pet.hunger() >= 100) {
        hungerDisplay->setText("Your pet is already full!");
        return;
    }
    int newVal = qMin(100, pet.hunger() + boost);
    pet.set_hunger(newVal);
    player->setPet(pet);
    hungerDisplay->setText(
        QString("%1  |  Hunger: %2 / 100").arg(message).arg(newVal));
}

void Feed::apple()  { applyHungerAction(10, "Apple!"); }
void Feed::bone()   { applyHungerAction(10, "Bone!"); }
void Feed::drink()  { applyHungerAction(5,  "A Drink!"); }
void Feed::pizza()  { applyHungerAction(5,  "Pizza!"); }
