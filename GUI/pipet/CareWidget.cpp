/*
 * CareWidget class implementation file.
 * In Care mode, the Player can feed, groom, and give affection to their pet.
 * They can also view their pet's Condition, or wellness attributes.
 * Author(s): Sasha C. Guerrero
 * Created: 2/20/2026
 * Last Edited: 2/20/2026
 */
#include "CareWidget.h"

CareWidget::CareWidget(QWidget *parent)
    : QWidget{parent}
{
    // Initialize grids, box, and layout
    layout = new QVBoxLayout();
    grid = new QGridLayout();
    careGrid = new QGridLayout();
    careBox = new QGroupBox();
    conditionBox = new QGroupBox();

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
    feed = new QPushButton("Feed");
    groom = new QPushButton("Groom");
    sleep = new QPushButton("Send to Sleep");
    affection = new QPushButton("Give Affection");

    // Add grids to layout
    conditionBox->setLayout(grid);
    careBox->setLayout(careGrid);
    layout->addWidget(conditionBox);
    layout->addWidget(careBox);
    this->setLayout(layout);

    // Add children to grid
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

    // Add children to careGrid
    careGrid->addWidget(feed, 0, 0, Qt::AlignCenter);
    careGrid->addWidget(groom, 1, 0, Qt::AlignCenter);
    careGrid->addWidget(sleep, 0, 1, Qt::AlignCenter);
    careGrid->addWidget(affection, 1, 1, Qt::AlignCenter);
    careBox->setTitle("Care Actions");
}


void CareWidget::feedPet()
{
    // increase hunger
}


void CareWidget::groomPet()
{
    // increase hygiene
}

void CareWidget::sendPetToSleep()
{
    // increase energy
}

void CareWidget::givePetAffection()
{
    // increase affection
}
