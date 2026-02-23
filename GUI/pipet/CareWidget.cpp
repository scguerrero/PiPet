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
    // initialize grid and child widgets
    grid = new QGridLayout();

    // labels
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

    // progress bars
    hunger_bar = new QProgressBar();
    energy_bar = new QProgressBar();
    strength_bar = new QProgressBar();
    hygiene_bar = new QProgressBar();
    intelligence_bar = new QProgressBar();
    happiness_bar = new QProgressBar();

    // add grid to layout
    this->setLayout(grid);

    // add children to grid
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
}
