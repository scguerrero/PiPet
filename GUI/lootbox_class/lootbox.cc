/*
 * Lootbox displays mystery rewards grouped by rarity.
 *
 * Author: Sasha C. Guerrero
 */

#include "lootbox.h"
#include <QRandomGenerator>

Lootbox::Lootbox(Player *player, QWidget *parent) : QWidget(parent), player(player) {
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("Lootbox { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
                  "stop:0 #4850DB, stop:1 #4A71DB); }");

    m_title = new QLabel("Mystery Rewards!");
    m_title->setAlignment(Qt::AlignCenter);
    m_title->setStyleSheet(
        "QLabel { font-size: 22px; font-weight: bold; color: mistyrose;"
        "border: 2px solid mistyrose; border-radius: 6px; padding: 6px; }");
    m_copperGroup = new QGroupBox("Copper");
    m_silverGroup = new QGroupBox("Silver");
    m_goldGroup = new QGroupBox("Gold");

    QString groupBoxBase = R"(
        QGroupBox {
            background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #4850DB, stop:1 #FBA8FF);
            border: 2px solid rgba(251,168,255,150);
            border-radius: 10px;
            margin-top: 1.5ex;
            font: bold;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 0 4px;
        }
    )";

    m_copperGroup->setStyleSheet(groupBoxBase + "QGroupBox::title { color: #eda15a; }");
    m_silverGroup->setStyleSheet(groupBoxBase + "QGroupBox::title { color: #C0C0C0; }");
    m_goldGroup->setStyleSheet(groupBoxBase   + "QGroupBox::title { color: #FFD700; }");

    m_layout = new QVBoxLayout();
    m_layout->addWidget(m_title);
    m_layout->addWidget(m_copperGroup);
    m_layout->addWidget(m_silverGroup);
    m_layout->addWidget(m_goldGroup);

    setLayout(m_layout);

    m_gold.addItem(Item("Cowboy",  ":/images/Sprites/pets/icons/cowboy_hat.png", "This town ain't big enough for the two of us.", Item::Gold));
    m_gold.addItem(Item("Crown",   ":/images/Sprites/pets/icons/crown_hat.png",  "All hail the PiPet royalty.",                    Item::Gold));
    m_gold.addItem(Item("Santa",   ":/images/Sprites/pets/icons/santa_hat.png",  "Ho ho ho! Have you been good this year?",         Item::Gold));
    m_gold.addItem(Item("Wizard",  ":/images/Sprites/pets/icons/wizard_hat.png", "You're a wizard, PiPet.",                         Item::Gold));

    m_silver.addItem(Item("Conch Shell",           ":/images/Sprites/pets/icons/shell.png",      "Hold it to your ear and you'll hear the ocean. Hold it to your terminal and it'll hear you. Supports bash, zsh, and the soft crash of waves.", Item::Silver));
    m_silver.addItem(Item("Ruby Gem",              ":/images/Sprites/pets/icons/rubygem.png",     "A flawless stone, red as a raised exception. It does everything you ask, interpreted line by line, without complaint. Mostly.",                Item::Silver));
    m_silver.addItem(Item("Java Coffee",           ":/images/Sprites/pets/icons/javacoffee.png",  "Strongly typed. Verbose. Takes forever to get going in the morning, but once it's running, it runs everywhere.",                               Item::Silver));
    m_silver.addItem(Item("String Spool of Thread",":/images/Sprites/pets/icons/string.png",      "Immutable, as far as you can tell. Technically it's just a sequence of characters. Technically so is everything you've ever said.",            Item::Silver));
    m_silver.addItem(Item("Master Key",            ":/images/Sprites/pets/icons/key.png",         "Opens everything in theory. In practice, it has never been tested. The previous owner said the same thing.",                                   Item::Silver));
}

int Lootbox::computeReward() {
    // x in [0.000, 1.750], y in {0, 1, 2}
    double x       = QRandomGenerator::global()->bounded(1751) / 1000.0;
    int    y       = QRandomGenerator::global()->bounded(3);
    double product = x * y;

    m_rewardItems.clear();

    auto pickFrom = [&](Inventory &inv) {
        int size = static_cast<int>(inv.getItems().size());
        if (size == 0) return;
        int idx = QRandomGenerator::global()->bounded(size);
        m_rewardItems.push_back(inv.grabItemAt(idx));
    };

    if (product == 0.0) {
        return 0;
    } else if (product <= 1.0) {
        for (int i = 0; i < y; i++) pickFrom(m_copper);
    } else if (product <= 2.0) {
        for (int i = 0; i < y; i++) pickFrom(m_silver);
    } else if (product < 3.0) {
        PiPet pet = player->getPet();
        pet.increase_happiness(25);
        player->setPet(pet);
        for (int i = 0; i < y; i++) pickFrom(m_silver);
    } else {
        for (int i = 0; i < y; i++) pickFrom(m_gold);
    }

    return y;
}
