/*
 * affection.cc - Affection screen.
 * Background: affection_stage_16bit.jpg via paintEvent
 * Author(s): Tanya Magurupira
 */
#include "affection.h"
#include <QPainter>

Affection::Affection(Player *player, QWidget *parent)
    : QWidget{parent}, player(player)
{
    m_bg.load(":/images/Backgrounds/affection_stage_16bit.jpg");

    layout      = new QVBoxLayout();
    actionsBox  = new QGroupBox("Affection Actions");
    actionsGrid = new QGridLayout();

    happinessDisplay = new QLabel();
    happinessDisplay->setAlignment(Qt::AlignCenter);
    happinessDisplay->setWordWrap(true);
    happinessDisplay->setStyleSheet(
        "QLabel { background-color: rgba(0,0,0,160); border-radius: 6px;"
        "padding: 6px; color: mistyrose; }");
    updateHappinessDisplay();

    cuddleBtn = new QPushButton("Cuddle");
    playBtn   = new QPushButton("Play");
    petsBtn   = new QPushButton("Give Pets");
    treatBtn  = new QPushButton("Give Treat");
    backBtn   = new QPushButton("Back to Care Hub!");

    backBtn->setIcon(QIcon(":/images/Assets/left.png"));

    actionsGrid->addWidget(cuddleBtn, 0, 0, Qt::AlignCenter);
    actionsGrid->addWidget(playBtn,   0, 1, Qt::AlignCenter);
    actionsGrid->addWidget(petsBtn,   1, 0, Qt::AlignCenter);
    actionsGrid->addWidget(treatBtn,  1, 1, Qt::AlignCenter);
    actionsBox->setLayout(actionsGrid);

    connect(cuddleBtn, SIGNAL(clicked()), this, SLOT(cuddle()));
    connect(playBtn,   SIGNAL(clicked()), this, SLOT(play()));
    connect(petsBtn,   SIGNAL(clicked()), this, SLOT(givePets()));
    connect(treatBtn,  SIGNAL(clicked()), this, SLOT(giveTreat()));

    layout->addStretch();
    layout->addWidget(happinessDisplay);
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

void Affection::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    if (!m_bg.isNull())
        painter.drawPixmap(0, 0, width(), height(), m_bg);
}

void Affection::updateHappinessDisplay() {
    happinessDisplay->setText(
        QString("Happiness: %1 / 100").arg(player->pet.happiness()));
}

void Affection::applyAffectionAction(int boost, const QString &message) {
    if (player->pet.happiness() >= 100) {
        happinessDisplay->setText("Your pet is already very happy!");
        return;
    }
    player->pet.increase_happiness(boost);
    happinessDisplay->setText(
        QString("%1  |  Happiness: %2 / 100").arg(message).arg(player->pet.happiness()));
}

void Affection::cuddle()    { applyAffectionAction(10, "Cuddled!"); }
void Affection::play()      { applyAffectionAction(10, "Playtime!"); }
void Affection::givePets()  { applyAffectionAction(5,  "Pets given!"); }
void Affection::giveTreat() { applyAffectionAction(5,  "Treat given!"); }
