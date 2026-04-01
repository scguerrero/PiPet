/*
 * CareWidget class implementation file.
 * In Care mode, the Player can feed, groom, and give affection to their pet.
 * They can also view their pet's Condition, or wellness attributes.
 * Author(s): Sasha C. Guerrero
 * Created: 2/20/2026
 * Last Edited: 3/17/2026
 */
#include "care.h"
#include "../../Player/Player.h"

Care::Care(Player* player, QStackedWidget* stack, QWidget *parent)
    : QWidget(parent), player(player), stack(stack)
{
    // Top-level layout
    top_layout = new QVBoxLayout(this);

    // Create pages
    hub = new QWidget(this);
    groom = new Groom(player, this);
    affection = new Affection(player, this);
    feed = new Feed(player, this);
    sleep = new Sleep(player, this);

    pages = new QStackedWidget(this);
    pages->addWidget(hub);
    pages->addWidget(groom);
    pages->addWidget(affection);
    pages->addWidget(feed);
    pages->addWidget(sleep);

    // Main layout inside hub
    layout = new QVBoxLayout(hub);
    grid = new QGridLayout();
    careGrid = new QGridLayout();
    careBox = new QGroupBox("Care Actions", hub);
    conditionBox = new QGroupBox("Condition", hub);

    // Labels
    hunger_label = new QLabel("Hunger", hub);
    energy_label = new QLabel("Energy", hub);
    strength_label = new QLabel("Strength", hub);
    hygiene_label = new QLabel("Hygiene", hub);
    intelligence_label = new QLabel("Intelligence", hub);
    happiness_label = new QLabel("Happiness", hub);
    age_days_label = new QLabel("Days Old", hub);
    age_group_label = new QLabel("Age Group", hub);

    age_days = new QLabel("0", hub);
    age_group = new QLabel("Baby", hub);

    // Progress bars (0-100)
    hunger_bar = new QProgressBar(hub);
    energy_bar = new QProgressBar(hub);
    strength_bar = new QProgressBar(hub);
    hygiene_bar = new QProgressBar(hub);
    intelligence_bar = new QProgressBar(hub);
    happiness_bar = new QProgressBar(hub);

    // updates the stats on the progress bar to real time
    for (QProgressBar *bar : {hunger_bar, energy_bar, strength_bar,
                              hygiene_bar, intelligence_bar, happiness_bar}) {
        bar->setRange(0, 100);
        bar->setTextVisible(true);
    }

    // Buttons for care actions
    b_feed = new QPushButton("Feed", hub);
    b_groom = new QPushButton("Groom", hub);
    b_sleep = new QPushButton("Send to Sleep", hub);
    b_affection = new QPushButton("Give Affection", hub);

    // Layout assembly
    conditionBox->setLayout(grid);
    careBox->setLayout(careGrid);

    layout->addWidget(conditionBox);
    layout->addWidget(careBox);
    layout->addWidget(b_back);

    // Add labels and bars
    grid->addWidget(hunger_label, 0, 0);
    grid->addWidget(energy_label, 1, 0);
    grid->addWidget(strength_label, 2, 0);
    grid->addWidget(hygiene_label, 3, 0);
    grid->addWidget(intelligence_label, 4, 0);
    grid->addWidget(happiness_label, 5, 0);
    grid->addWidget(age_days_label, 6, 0);
    grid->addWidget(age_group_label, 7, 0);

    grid->addWidget(hunger_bar, 0, 1);
    grid->addWidget(energy_bar, 1, 1);
    grid->addWidget(strength_bar, 2, 1);
    grid->addWidget(hygiene_bar, 3, 1);
    grid->addWidget(intelligence_bar, 4, 1);
    grid->addWidget(happiness_bar, 5, 1);
    grid->addWidget(age_days, 6, 1);
    grid->addWidget(age_group, 7, 1);

    // Care actions
    careGrid->addWidget(b_feed, 0, 0);
    careGrid->addWidget(b_groom, 1, 0);
    careGrid->addWidget(b_sleep, 0, 1);
    careGrid->addWidget(b_affection, 1, 1);

    // Final layout
    top_layout->addWidget(pages);

    // Connections
    connect(b_feed, SIGNAL(clicked()), this, SLOT(feedPet()));
    connect(feed->backBtn, SIGNAL(clicked()), this, SLOT(returnToHub()));
    connect(b_groom, SIGNAL(clicked()), this, SLOT(groomPet()));
    connect(groom->backBtn, SIGNAL(clicked()), this, SLOT(returnToHub()));
    connect(b_affection, SIGNAL(clicked()), this, SLOT(givePetAffection()));
    connect(affection->backBtn, SIGNAL(clicked()), this, SLOT(returnToHub()));
    connect(b_sleep, SIGNAL(clicked()), this, SLOT(sendPetToSleep()));
    connect(sleep->backBtn, SIGNAL(clicked()), this, SLOT(returnToHub()));

    //load initial values
    updateStats();
}

void Care::updateStats(){
    // TODO: inmplement later
    //updateStats of the pet condition
    piPet &pet = player->getPet();

    hunger_bar->setValue(pet.getHunger());
    energy_bar->setValue(pet.getEnergy());
    strength_bar->setValue(pet.getStrength());
    hygiene_bar->setValue(pet.getHygiene());
    intelligence_bar->setValue(pet.getIntelligence());
    happiness_bar->setValue(pet.getHappiness());

    age_days->setText(QString::number(pet.getAgeDays()));

    //Map ageGroup int to a readable string
    QString group;
    switch (pet.getAgeGroup()){
    case 1: group = "Child"; break;
    case 2: group = "Teen"; break;
    case 3: group = "Adult"; break;
    default: group = "Unknown"; break;

    }
    age_group->setText(group);
}

void Care::returnToHub() {
    pages->setCurrentIndex(0);
}

void Care::groomPet() // Increase hygiene
{
    pages->setCurrentIndex(1); // index 1 of stacked widget

    // increase hygiene
    piPet &pet = player->getPet();
    if (pet.getHygiene() >= 100) return; // already clean
    player->groomPet(); // player grooms the pet
    updateStats();
}


void Care::givePetAffection() // Increase happiness
{
    pages->setCurrentIndex(2); // index 2 of stacked widget

    // increase affection
    piPet &pet = player->getPet();
    if (pet.getHappiness() >= 100) return; //already max happiness
    player->givePetAffection(); // player gives affection
    updateStats();
}

void Care::feedPet() // Increase hunger
{
    pages->setCurrentIndex(3);// index 3 of stacked widget

    // increase hunger
    piPet &pet = player->getPet();
    if (pet.getHunger() >= 100) return; // already full
    player->feedPet();
    updateStats();
}


void Care::sendPetToSleep() // Increase energy
{
    pages->setCurrentIndex(4);// index 4 of stacked widget

    // increase sleep
    piPet &pet = player->getPet();
    if (pet.getEnergy() >= 100) return; // already rested
    player->sendPetToSleep();
    updateStats();
}


