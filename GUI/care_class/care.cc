/*
 * care.cc - Care hub implementation.
 * Removed: Give Affection tab
 * Added: Character GIF on hub, 3 care actions only
 * Author(s): Sasha C. Guerrero, Tanya Magurupira
 */
#include "care.h"
#include "../../Player/Player.h"

Care::Care(Player* player, Character::PetType petType, QWidget *parent)
    : QWidget(parent), player(player), petType(petType)
{
    top_layout = new QVBoxLayout(this);

    // Pages: hub=0, groom=1, feed=2, sleep=3
    hub   = new QWidget(this);
    groom = new Groom(player, this);                   // Groom takes (player, parent)
    feed  = new Feed(player, petType, this);           // Feed takes (player, petType, parent)
    sleep = new Sleep(player, petType, this);          // Sleep takes (player, petType, parent)

    pages = new QStackedWidget();
    pages->addWidget(hub);    // 0
    pages->addWidget(groom);  // 1
    pages->addWidget(feed);   // 2
    pages->addWidget(sleep);  // 3

    // ── Hub layout ─────────────────────────────────────────────────────────
    layout       = new QVBoxLayout(hub);
    grid         = new QGridLayout();
    careGrid     = new QGridLayout();
    careBox      = new QGroupBox("Care Actions", hub);
    conditionBox = new QGroupBox("Condition", hub);

    b_back = new QPushButton("BACK", hub);
    b_back->setIcon(QIcon(":/images/Assets/left.png"));

    // ── Character GIF on hub (small, top center) ───────────────────────────
    character = new Character(hub);
    character->setFixedSize(120, 120);
    character->syncWithPlayer(*player, petType);

    // ── Stat labels ────────────────────────────────────────────────────────
    hunger_label       = new QLabel("Hunger",       hub);
    energy_label       = new QLabel("Energy",       hub);
    strength_label     = new QLabel("Strength",     hub);
    hygiene_label      = new QLabel("Hygiene",      hub);
    intelligence_label = new QLabel("Intelligence", hub);
    happiness_label    = new QLabel("Happiness",    hub);
    age_days_label     = new QLabel("Days Old",     hub);
    age_group_label    = new QLabel("Age Group",    hub);
    age_days           = new QLabel("0",            hub);
    age_group          = new QLabel("Baby",         hub);

    // ── Progress bars ──────────────────────────────────────────────────────
    hunger_bar       = new QProgressBar(hub);
    energy_bar       = new QProgressBar(hub);
    strength_bar     = new QProgressBar(hub);
    hygiene_bar      = new QProgressBar(hub);
    intelligence_bar = new QProgressBar(hub);
    happiness_bar    = new QProgressBar(hub);

    for (QProgressBar *bar : {hunger_bar, energy_bar, strength_bar,
                              hygiene_bar, intelligence_bar, happiness_bar}) {
        bar->setRange(0, 100);
        bar->setTextVisible(true);
    }

    // ── Care buttons — 3 only (no affection) ──────────────────────────────
    b_feed  = new QPushButton("Feed",          hub);
    b_groom = new QPushButton("Groom",         hub);
    b_sleep = new QPushButton("Send to Sleep", hub);

    // ── Assemble layouts ───────────────────────────────────────────────────
    conditionBox->setLayout(grid);
    careBox->setLayout(careGrid);

    // Character GIF centered at top of hub
    layout->addWidget(character, 0, Qt::AlignCenter);
    layout->addWidget(conditionBox);
    layout->addWidget(careBox);
    layout->addWidget(b_back);

    // Stat grid
    grid->addWidget(hunger_label,       0, 0); grid->addWidget(hunger_bar,       0, 1);
    grid->addWidget(energy_label,       1, 0); grid->addWidget(energy_bar,       1, 1);
    grid->addWidget(strength_label,     2, 0); grid->addWidget(strength_bar,     2, 1);
    grid->addWidget(hygiene_label,      3, 0); grid->addWidget(hygiene_bar,      3, 1);
    grid->addWidget(intelligence_label, 4, 0); grid->addWidget(intelligence_bar, 4, 1);
    grid->addWidget(happiness_label,    5, 0); grid->addWidget(happiness_bar,    5, 1);
    grid->addWidget(age_days_label,     6, 0); grid->addWidget(age_days,         6, 1);
    grid->addWidget(age_group_label,    7, 0); grid->addWidget(age_group,        7, 1);

    // Care action buttons — 3 in a row
    careGrid->addWidget(b_feed,  0, 0);
    careGrid->addWidget(b_groom, 0, 1);
    careGrid->addWidget(b_sleep, 0, 2);

    top_layout->addWidget(pages);

    // ── Navigation ─────────────────────────────────────────────────────────
    connect(b_feed,         SIGNAL(clicked()), this, SLOT(feedPet()));
    connect(b_groom,        SIGNAL(clicked()), this, SLOT(groomPet()));
    connect(b_sleep,        SIGNAL(clicked()), this, SLOT(sendPetToSleep()));
    connect(feed->backBtn,  SIGNAL(clicked()), this, SLOT(returnToHub()));
    connect(groom->backBtn, SIGNAL(clicked()), this, SLOT(returnToHub()));
    connect(sleep->backBtn, SIGNAL(clicked()), this, SLOT(returnToHub()));

    updateStats();

    this->setStyleSheet("QLabel { background-color: transparent; }");

    b_back->setStyleSheet(R"(
        QPushButton { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4850DB, stop:1 #4A71DB);
            border: 2px inset #FBA8FF; border-radius: 10px;
            padding: 4px; font: bold; }
        QPushButton:pressed { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4A71DB, stop:1 #4850DB); })");
}

void Care::updateStats() {
    PiPet pet = player->getPet();
    hunger_bar->setValue(pet.hunger());
    energy_bar->setValue(pet.energy());
    strength_bar->setValue(pet.strength());
    hygiene_bar->setValue(pet.hygiene());
    intelligence_bar->setValue(pet.intelligence());
    happiness_bar->setValue(pet.happiness());
    age_days->setText(QString::number(pet.days_old()));
    age_group->setText(pet.age_group());

    // Keep character GIF in sync with current pet state
    character->syncWithPlayer(*player, petType);
}

void Care::returnToHub() {
    updateStats();
    pages->setCurrentIndex(0);
}

void Care::groomPet()       { pages->setCurrentIndex(1); }
void Care::feedPet()        { pages->setCurrentIndex(2); }
void Care::sendPetToSleep() { pages->setCurrentIndex(3); }
