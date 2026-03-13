#include "groomwidget.h"

GroomWidget::GroomWidget(Player *player, QWidget *parent)
    :QDialog{parent}, player(player)
{
    setWindowTitle("Groom Your Pet");
    setModal(true); // blocks CareWidget until closed

    layout       = new QVBoxLayout();
    actionsBox   = new QGroupBox("Grooming Actions");
    actionsGrid  = new QGridLayout();

    // Live hygiene label
    hygieneDisplay = new QLabel();
    updateHygieneDisplay();
    hygieneDisplay->setAlignment(Qt::AlignCenter);

    // Buttons
    brushBtn        = new QPushButton("Brush Fur");
    batheBtn        = new QPushButton("Bathe");
    brushTeethBtn   = new QPushButton("Brush Teeth");
    trimBtn         = new QPushButton("Trim Nails");
    cleanEarsBtn    = new QPushButton("Clean Ears");
    doneBtn         = new QPushButton("Done");
    doneBtn->setStyleSheet("background: darkslategreen; color: darkseagreen");

    // Layout
    actionsGrid->addWidget(brushBtn,        0, 0, Qt::AlignCenter);
    actionsGrid->addWidget(batheBtn,        0, 1, Qt::AlignCenter);
    actionsGrid->addWidget(brushTeethBtn,   1, 0, Qt::AlignCenter);
    actionsGrid->addWidget(trimBtn,         1, 1, Qt::AlignCenter);
    actionsGrid->addWidget(cleanEarsBtn,    2, 0, 1, 2,  Qt::AlignCenter);
    actionsBox->setLayout(actionsGrid);

    layout->addWidget(hygieneDisplay);
    layout->addWidget(actionsBox);
    layout->addWidget(doneBtn);
    this->setLayout(layout);

    // Connect buttons
    connect(brushBtn,       SIGNAL(clicked()), this, SLOT(brushFur()));
    connect(batheBtn,       SIGNAL(clicked()), this, SLOT(bathe()));
    connect(brushTeethBtn,  SIGNAL(clicked()), this, SLOT(brushTeeth()));
    connect(trimBtn,        SIGNAL(clicked()), this, SLOT(trimNails()));
    connect(cleanEarsBtn,   SIGNAL(clicked()), this, SLOT(cleanEars()));
    connect(doneBtn,        SIGNAL(clicked()), this, SLOT(accept())); // closes dialog

}

void GroomWidget::updateHygieneDisplay()
{
    int hygiene = player->getPetRef().getHygiene();
    hygieneDisplay->setText(QString("Hygiene: %1 / 100").arg(hygiene));
}

void GroomWidget::applyGroomAction(int boost, const QString &message)
{
    piPet& pet = player->getPetRef();
    if (pet.getHygiene() >= 100) {
        hygieneDisplay->setText("Your pet is already clean!");
        return;
    }
    pet.increaseHygiene(boost);
    updateHygieneDisplay();
    hygieneDisplay->setText(QString("%1  |  Hygiene: %2 / 100")
                                .arg(message)
                                .arg(pet.getHygiene()));
}

void GroomWidget::brush()    { applyGroomAction(5,  "Brushed fur!"); }
void GroomWidget::bathe()       { applyGroomAction(5, "Bath time!"); }
void GroomWidget::brushTeeth()  { applyGroomAction(5, "Teeth Brushed!"); }
void GroomWidget::trimNails()   { applyGroomAction(5,  "Nails trimmed!"); }
void GroomWidget::cleanEars()   { applyGroomAction(5,  "Ears cleaned!"); }
