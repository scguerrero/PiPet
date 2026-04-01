#include "sleep.h"

Sleep::Sleep(Player *player, QWidget *parent)
    : QWidget{parent}, player(player)
{
    layout      = new QVBoxLayout();
    actionsBox  = new QGroupBox("Sleep Actions");
    actionsGrid = new QGridLayout();

    //live sleep display
    sleepDisplay = new QLabel();
    sleepDisplay->setAlignment(Qt::AlignCenter);
    sleepDisplay->setWordWrap(true);
    updateSleepDisplay();

    //Buttons
    brushTeethBtn   = new QPushButton("🪥 Brush Teeth");
    wearPjsBtn     = new QPushButton("👖 Wear Pjs");
    readBookBtn     = new QPushButton("📖Bed Time Story");
    tuckInBtn    = new QPushButton("🛌 Tuck In Bed");
    backBtn     = new QPushButton("Back to menu");

    // grid Layout
    actionsGrid->addWidget(brushTeethBtn,   0, 0, Qt::AlignCenter);
    actionsGrid->addWidget(wearPjsBtn,     0, 1, Qt::AlignCenter);
    actionsGrid->addWidget(readBookBtn,     1, 0, Qt::AlignCenter);
    actionsGrid->addWidget(tuckInBtn,    1, 1, Qt::AlignCenter);
    actionsBox->setLayout(actionsGrid);

    //connects
    connect(brushTeethBtn,  SIGNAL(clicked()), this, SLOT(brushTeeth()));
    connect(wearPjsBtn,    SIGNAL(clicked()), this, SLOT(wearPjs()));
    connect(readBookBtn,    SIGNAL(clicked()), this, SLOT(readBook()));
    connect(tuckInBtn,   SIGNAL(clicked()), this, SLOT(tuckIn()));

    // Construction layout
    layout->addWidget(sleepDisplay);
    layout->addWidget(actionsBox);
    layout->addWidget(backBtn);
    this->setLayout(layout);

}

void Sleep::updateSleepDisplay()
{
    int sleep = player->getPet().getEnergy();
    sleepDisplay->setText(QString("Sleep: %1/100").arg(sleep));
}

void Sleep::applySleepAction(int boost, const QString &message)
{
    piPet &pet = player->getPet();

    if (pet.getEnergy() >= 100){
        sleepDisplay->setText("Your pet is Asleep!");
        return;
    }

    pet.increaseEnergy(boost);
    updateSleepDisplay();
    sleepDisplay->setText(QString("%1 | Sleep: %2/100")
                               .arg(message).arg(pet.getEnergy()));

}

void Sleep::brushTeeth()    { applySleepAction(10, "Brush Teeth!"); }
void Sleep::wearPjs()      { applySleepAction(10, "Wear Pjs!"); }
void Sleep::readBook() { applySleepAction(5,  "Bed Time Story!"); }
void Sleep::tuckIn() { applySleepAction(5,  "Tuck In Bed!"); }
