/*
 * In "Affection" mode, the Player can perform actions that increase the PiPet's happiness.
 *
 * Author(s): Tanya Magurupira
 */

#include "affection.h"

Affection::Affection(Player *player, QWidget *parent)
    : QWidget{parent}, player(player)
{
    layout      = new QVBoxLayout();
    actionsBox  = new QGroupBox("Affection Actions");
    actionsGrid = new QGridLayout();

    //live happiness display
    happinessDisplay = new QLabel();
    happinessDisplay->setAlignment(Qt::AlignCenter);
    happinessDisplay->setWordWrap(true);
    updateHappinessDisplay();

    //Buttons
    cuddleBtn   = new QPushButton("Cuddle");
    playBtn     = new QPushButton("Play");
    petsBtn     = new QPushButton("Give Pets");
    treatBtn    = new QPushButton("Give Treat");
    backBtn     = new QPushButton("Back to Care Hub!");

    //Icons for buttons
    QIcon left_icon(":/images/Assets/left.png");
    backBtn->setIcon(left_icon);

    // grid Layout
    actionsGrid->addWidget(cuddleBtn,   0, 0, Qt::AlignCenter);
    actionsGrid->addWidget(playBtn,     0, 1, Qt::AlignCenter);
    actionsGrid->addWidget(petsBtn,     1, 0, Qt::AlignCenter);
    actionsGrid->addWidget(treatBtn,    1, 1, Qt::AlignCenter);
    actionsBox->setLayout(actionsGrid);

    //connects
    connect(cuddleBtn,  SIGNAL(clicked()), this, SLOT(cuddle()));
    connect(playBtn,    SIGNAL(clicked()), this, SLOT(play()));
    connect(petsBtn,    SIGNAL(clicked()), this, SLOT(givePets()));
    connect(treatBtn,   SIGNAL(clicked()), this, SLOT(giveTreat()));

    // Construction layout
    layout->addWidget(happinessDisplay);
    layout->addWidget(actionsBox);
    layout->addWidget(backBtn);
    this->setLayout(layout);

    backBtn->setStyleSheet(R"(
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

void Affection::updateHappinessDisplay()
{
    happinessDisplay->setText(QString("Happiness: %1/100").arg(player->pet.happiness()));
}

void Affection::applyAffectionAction(int boost, const QString &message)
{
    if (player->pet.happiness() >= 100){
        happinessDisplay->setText("Your pet is already very happy!");
        return;
    }
    player->pet.increase_happiness(boost);
    updateHappinessDisplay();
    happinessDisplay->setText(QString("%1 | Happiness: %2/100")
                                  .arg(message).arg(player->pet.happiness()));

}

void Affection::cuddle()    { applyAffectionAction(10, "Cuddled!"); }
void Affection::play()      { applyAffectionAction(10, "Playtime!"); }
void Affection::givePets() { applyAffectionAction(5,  "Pets given!"); }
void Affection::giveTreat() { applyAffectionAction(5,  "Treat given!"); }
