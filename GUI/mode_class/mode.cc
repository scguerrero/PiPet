/*
 * mode.cc - Mode hub implementation.
 * Author(s): Sasha C. Guerrero, [Your name]
 */

#include "mode.h"

Mode::Mode(Player *player, QWidget *parent)
    : QWidget{parent}, player(player)
{
    layout = new QVBoxLayout();
    this->setLayout(layout);

    // ── Clock ──────────────────────────────────────────────────────────────
    timekeeper   = new Clock();
    time         = new QTime();
    timer        = new QTimer();
    clock_time   = new QLabel(time->currentTime().toString("hh:mm:ss AP"));
    elapsed_time = new QLabel("Time played: 0s");
    clock_time->setAlignment(Qt::AlignCenter);
    elapsed_time->setAlignment(Qt::AlignCenter);
    layout->addWidget(clock_time);
    layout->addWidget(elapsed_time);

    // ── Character GIF ──────────────────────────────────────────────────────
    character = new Character(this);
    character->setFixedSize(200, 200);
    layout->addWidget(character, 0, Qt::AlignCenter);

    // ── Stat bars ──────────────────────────────────────────────────────────
    statsBox  = new QGroupBox("Condition", this);
    statsGrid = new QGridLayout();
    statsBox->setLayout(statsGrid);

    hunger_label    = new QLabel("Hunger");
    energy_label    = new QLabel("Energy");
    happiness_label = new QLabel("Happiness");
    hunger_bar      = new QProgressBar();
    energy_bar      = new QProgressBar();
    happiness_bar   = new QProgressBar();

    for (QProgressBar *bar : {hunger_bar, energy_bar, happiness_bar}) {
        bar->setRange(0, 100);
        bar->setTextVisible(true);
    }

    statsGrid->addWidget(hunger_label,    0, 0);
    statsGrid->addWidget(hunger_bar,      0, 1);
    statsGrid->addWidget(energy_label,    1, 0);
    statsGrid->addWidget(energy_bar,      1, 1);
    statsGrid->addWidget(happiness_label, 2, 0);
    statsGrid->addWidget(happiness_bar,   2, 1);
    layout->addWidget(statsBox);

    // ── Mode buttons ───────────────────────────────────────────────────────
    b_care   = new QPushButton("Care");
    b_train  = new QPushButton("Train");
    b_battle = new QPushButton("Battle");
    b_gear   = new QPushButton("Gear");
    layout->addWidget(b_care);
    layout->addWidget(b_train);
    layout->addWidget(b_battle);
    layout->addWidget(b_gear);

    // ── Timer ──────────────────────────────────────────────────────────────
    connect(timer, SIGNAL(timeout()), this, SLOT(updateClock()));
    timer->start(1000);

    refreshDisplay();
}

void Mode::setPetType(Character::PetType type) {
    petType = type;
    character->setPetType(type);
    refreshDisplay();
}

void Mode::refreshDisplay() {
    updateStatBars();
    character->syncWithPlayer(*player, petType);
}

void Mode::updateClock() {
    clock_time->setText(time->currentTime().toString("hh:mm:ss AP"));

    timekeeper->increment_elapsed_time(1);
    elapsed_time->setText(QString("Time played: %1s")
                              .arg(timekeeper->elapsed_time()));

    secondsSinceDecay++;
    if (secondsSinceDecay >= DECAY_INTERVAL_SECS) {
        secondsSinceDecay = 0;
        decayStats();
    }
}

void Mode::decayStats() {
    PiPet pet = player->getPet();
    pet.set_hunger   (qMax(0, pet.hunger()    - 1));
    pet.set_energy   (qMax(0, pet.energy()    - 1));
    pet.set_happiness(qMax(0, pet.happiness() - 1));
    player->setPet(pet);

    updateStatBars();
    character->syncWithPlayer(*player, petType);
}

void Mode::updateStatBars() {
    PiPet pet = player->getPet();
    hunger_bar->setValue(pet.hunger());
    energy_bar->setValue(pet.energy());
    happiness_bar->setValue(pet.happiness());
}
