/*
 * Lootbox displays mystery rewards grouped by rarity.
 *
 * Author: Sasha C. Guerrero
 */

#include "lootbox.h"

Lootbox::Lootbox(Player *player, QWidget *parent) : QWidget(parent) {
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

    m_gold.addItem(Item("Cowboy",  ":/images/Sprites/pets/icons/cowboy_hat.png", "This town ain't big enough for the two of us."));
    m_gold.addItem(Item("Crown",   ":/images/Sprites/pets/icons/crown_hat.png",  "All hail the PiPet royalty."));
    m_gold.addItem(Item("Santa",   ":/images/Sprites/pets/icons/santa_hat.png",  "Ho ho ho! Have you been good this year?"));
    m_gold.addItem(Item("Wizard",  ":/images/Sprites/pets/icons/wizard_hat.png", "You're a wizard, PiPet."));
}
