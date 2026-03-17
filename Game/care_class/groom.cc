#include "groom.h"

Groom::Groom(Player *player, QWidget *parent)
    :QWidget{parent}, player(player)
{

    layout       = new QVBoxLayout();
    actionsBox   = new QGroupBox("Grooming Actions");
    actionsGrid  = new QGridLayout();

    // Live hygiene label
    hygieneDisplay = new QLabel();
    hygieneDisplay->setAlignment(Qt::AlignCenter);
    updateHygieneDisplay();

    // Buttons
    brushBtn        = new QPushButton("Brush");
    batheBtn        = new QPushButton("Bathe");
    trimBtn         = new QPushButton("Trim Nails");
    cleanEarsBtn    = new QPushButton("Clean Ears");
    backBtn         = new QPushButton("Back");

    // Layout
    actionsGrid->addWidget(brushBtn,        0, 0, Qt::AlignCenter);
    actionsGrid->addWidget(batheBtn,        0, 1, Qt::AlignCenter);
    actionsGrid->addWidget(trimBtn,         1, 0, Qt::AlignCenter);
    actionsGrid->addWidget(cleanEarsBtn,    1, 1,  Qt::AlignCenter);
    actionsBox->setLayout(actionsGrid);

    // Connect buttons
    connect(brushBtn,       SIGNAL(clicked()), this, SLOT(brush()));
    connect(batheBtn,       SIGNAL(clicked()), this, SLOT(bathe()));
    connect(trimBtn,        SIGNAL(clicked()), this, SLOT(trimNails()));
    connect(cleanEarsBtn,   SIGNAL(clicked()), this, SLOT(cleanEars()));


    // Construction layout
    layout->addWidget(hygieneDisplay);
    layout->addWidget(actionsBox);
    layout->addWidget(backBtn);
    this->setLayout(layout);
}

void Groom::updateHygieneDisplay()
{
    int hygiene = player->pet.getHygiene();
    hygieneDisplay->setText(QString("Hygiene: %1 / 100").arg(hygiene));
}

void Groom::applyGroomAction(int boost, const QString &message)
{
    int hygiene = player->pet.getHygiene();
    if (hygiene >= 100) {
        hygieneDisplay->setText("Your pet is already clean!");
        return;
    }
    player->pet.increaseHygiene(boost);
    updateHygieneDisplay();
    hygieneDisplay->setText(QString("%1  |  Hygiene: %2 / 100")
                                .arg(message)
                                .arg(player->pet.getHygiene()));
}

void Groom::brush()       { applyGroomAction(5,  "Brushed!"); }
void Groom::bathe()       { applyGroomAction(5, "Bath time!"); }
void Groom::trimNails()   { applyGroomAction(5,  "Nails trimmed!"); }
void Groom::cleanEars()   { applyGroomAction(5,  "Ears cleaned!"); }
