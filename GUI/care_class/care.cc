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

Care::Care(QWidget *parent)
    : QWidget{parent}
{
    // Initialize pet and player
    PiPet pet;
    Player player(pet);

    // Top-level layout that will hold stacked widget
    top_layout = new QVBoxLayout();
    hub = new QWidget();
    groom = new Groom(&player);
    affection = new Affection(&player);
    pages = new QStackedWidget();

    top_layout->addWidget(pages);
    pages->addWidget(hub);
    pages->addWidget(groom);
    pages->addWidget(affection);

    // Initialize grids, box, and layout
    layout = new QVBoxLayout();
    grid = new QGridLayout();
    careGrid = new QGridLayout();
    careBox = new QGroupBox();
    conditionBox = new QGroupBox();
    b_back = new QPushButton("BACK");

    // Text labels
    hunger_label = new QLabel("Hunger");
    energy_label = new QLabel("Energy");
    strength_label = new QLabel("Strength");
    hygiene_label = new QLabel("Hygiene");
    intelligence_label = new QLabel("Intelligence");
    happiness_label = new QLabel("Happiness");
    age_days_label = new QLabel("Days Old");
    age_group_label = new QLabel("Age Group");
    age_days = new QLabel("0");
    age_group = new QLabel("Baby");

    // Progress bars
    hunger_bar = new QProgressBar();
    energy_bar = new QProgressBar();
    strength_bar = new QProgressBar();
    hygiene_bar = new QProgressBar();
    intelligence_bar = new QProgressBar();
    happiness_bar = new QProgressBar();

    // Buttons for care actions
    b_feed = new QPushButton("Feed");
    b_groom = new QPushButton("Groom");
    b_sleep = new QPushButton("Send to Sleep");
    b_affection = new QPushButton("Give Affection");

    // Add grids to layout
    conditionBox->setLayout(grid);
    careBox->setLayout(careGrid);
    layout->addWidget(conditionBox);
    layout->addWidget(careBox);
    layout->addWidget(b_back);

    // Add labels and bars to grid
    grid->addWidget(hunger_label, 0, 0, Qt::AlignLeft);
    grid->addWidget(energy_label, 1, 0, Qt::AlignLeft);
    grid->addWidget(strength_label, 2, 0, Qt::AlignLeft);
    grid->addWidget(hygiene_label, 3, 0, Qt::AlignLeft);
    grid->addWidget(intelligence_label, 4, 0, Qt::AlignLeft);
    grid->addWidget(happiness_label, 5, 0, Qt::AlignLeft);
    grid->addWidget(age_days_label, 6, 0, Qt::AlignLeft);
    grid->addWidget(age_group_label, 7, 0, Qt::AlignLeft);
    grid->addWidget(hunger_bar, 0, 1, Qt::AlignLeft);
    grid->addWidget(energy_bar, 1, 1, Qt::AlignLeft);
    grid->addWidget(strength_bar, 2, 1, Qt::AlignLeft);
    grid->addWidget(hygiene_bar, 3, 1, Qt::AlignLeft);
    grid->addWidget(intelligence_bar, 4, 1, Qt::AlignLeft);
    grid->addWidget(happiness_bar, 5, 1, Qt::AlignLeft);
    grid->addWidget(age_days, 6, 1, Qt::AlignLeft);
    grid->addWidget(age_group, 7, 1, Qt::AlignLeft);
    conditionBox->setTitle("Condition");

    // Add buttons to careGrid
    careGrid->addWidget(b_feed, 0, 0, Qt::AlignCenter);
    careGrid->addWidget(b_groom, 1, 0, Qt::AlignCenter);
    careGrid->addWidget(b_sleep, 0, 1, Qt::AlignCenter);
    careGrid->addWidget(b_affection, 1, 1, Qt::AlignCenter);
    careBox->setTitle("Care Actions");

    hub->setLayout(layout);

    this->setLayout(top_layout);

    connect(b_groom, SIGNAL( clicked() ), this, SLOT( groomPet() )); // Open widget for grooming pet
    connect(groom->backBtn, SIGNAL( clicked() ), this, SLOT( returnToHub() )); // Go back to hub after grooming

    connect(b_affection, SIGNAL( clicked() ), this, SLOT( givePetAffection() )); // Open widget for giving affection to pet
    connect(affection->backBtn, SIGNAL( clicked() ), this, SLOT( returnToHub() )); // Go back to hub after giving affection
}

void Care::returnToHub() {
    pages->setCurrentIndex(0);
}

void Care::feedPet() // Increase hunger
{
    // index 1 of stacked widget
}

void Care::groomPet() // Increase hygiene
{
    pages->setCurrentIndex(1); // index 2 of stacked widget
}

void Care::sendPetToSleep() // Increase energy
{
    // index 3 of stacked widget
}

void Care::givePetAffection() // Increase happiness
{
    pages->setCurrentIndex(2); // index 4 of stacked widget
}
