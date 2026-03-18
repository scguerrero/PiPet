#include "AffectionWidget.h"

AffectionWidget::AffectionWidget(Player *player, QWidget *parent)
    : QWidget{parent}, player(player)
{
    layout      = new QVBoxLayout();
    actionsBox  = new QGroupBox("Affection Actions");
    actionsGrid = new QGridLayout();

    //live happiness display
    happinessDisplay = new QLabel();
    happinessDisplay->setAlignment(Qt::AlignCenter);
    updateHappinessDisplay();

    //Buttons
    cuddleBtn   = new QPushButton("Cuddle");
    playBtn     = new QPushButton("Play");
    petsBtn     = new QPushButton("Give Pets");
    treatBtn    = new QPushButton("Give Treat");
    backBtn     = new QPushButton("Back");
    backBtn->setStyleSheet("background: darkslategreen; color: darkseagreen");

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

}

void AffectionWidget::updateHappinessDisplay()
{
    int happiness = player->getPetRef().getHappiness();
    happinessDisplay->setText(QString("Happiness: %1/100").arg(happiness));
}

void AffectionWidget::applyAffectionAction(int boost, const QString &message)
{
    piPet& pet = player->getPetRef();
    if (pet.getHappiness() >= 100){
        happinessDisplay->setText("Your pet is already very happy!");
        return;
    }
    pet.increaseHappiness(boost);
    updateHappinessDisplay();
    happinessDisplay->setText(QString("%1 | Happiness: %2/100")
                                    .arg(message).arg(pet.getHappiness()));

}

void AffectionWidget::cuddle()    { applyAffectionAction(10, "Cuddled!"); }
void AffectionWidget::play()      { applyAffectionAction(10, "Playtime!"); }
void AffectionWidget::givePets() { applyAffectionAction(5,  "Pets given!"); }
void AffectionWidget::giveTreat() { applyAffectionAction(5,  "Treat given!"); }