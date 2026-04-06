/* Author: Tanya Magurupira */
#include "feed.h"

Feed::Feed(Player *player, QWidget *parent)
    : QWidget{parent}, player(player)
{
    layout      = new QVBoxLayout();
    actionsBox  = new QGroupBox("Feed Actions");
    actionsGrid = new QGridLayout();

    //live hunger display
    hungerDisplay = new QLabel();
    hungerDisplay->setAlignment(Qt::AlignCenter);
    hungerDisplay->setWordWrap(true);
    updateHungerDisplay();

    //Buttons
    appleBtn   = new QPushButton("🍎 Apple");
    boneBtn     = new QPushButton("🦴 Bone ");
    drinkBtn     = new QPushButton("🥤 A Drink");
    pizzaBtn    = new QPushButton("🍕 Eat pizza");
    backBtn     = new QPushButton("⬅️ Back to Care Hub!");

    // grid Layout
    actionsGrid->addWidget(appleBtn,   0, 0, Qt::AlignCenter);
    actionsGrid->addWidget(boneBtn,     0, 1, Qt::AlignCenter);
    actionsGrid->addWidget(drinkBtn,     1, 0, Qt::AlignCenter);
    actionsGrid->addWidget(pizzaBtn,    1, 1, Qt::AlignCenter);
    actionsBox->setLayout(actionsGrid);

    //connects
    connect(appleBtn,  SIGNAL(clicked()), this, SLOT(apple()));
    connect(boneBtn,    SIGNAL(clicked()), this, SLOT(bone()));
    connect(drinkBtn,    SIGNAL(clicked()), this, SLOT(drink()));
    connect(pizzaBtn,   SIGNAL(clicked()), this, SLOT(pizza()));

    // Construction layout
    layout->addWidget(hungerDisplay);
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

void Feed::updateHungerDisplay()
{
    int hunger = player->getPet().hunger();
    hungerDisplay->setText(QString("Hunger: %1/100").arg(hunger));
}

void Feed::applyHungerAction(int boost, const QString &message)
{
    PiPet pet = player->getPet();

    if (pet.energy() >= 100){
        hungerDisplay->setText("Your pet is full!");
        return;
    }

    pet.increase_hunger(boost);
    updateHungerDisplay();
    hungerDisplay->setText(QString("%1 | Hunger: %2/100")
                               .arg(message).arg(pet.energy()));

}

void Feed::apple()    { applyHungerAction(10, "Apple!"); }
void Feed::bone()      { applyHungerAction(10, "Bone!"); }
void Feed::drink() { applyHungerAction(5,  "A Drink!"); }
void Feed::pizza() { applyHungerAction(5,  "Pizza!"); }
