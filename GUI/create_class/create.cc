/*
 * create.cc
 * Author(s): Sasha C. Guerrero
 * Fixed: DONE button now starts disabled and only enables once both a name
 *        AND a species have been selected. Previously had no validation.
 */

#include "create.h"
using namespace std;

Create::Create(QWidget *parent) : QWidget{parent} {

    // Name widget ---------------------------------------------------------------
    name = new QWidget();
    scroll_name = new QScrollArea();
    scroll_name->setWidgetResizable(true);
    l_name = new QVBoxLayout();
    name_instruction = new QLabel("Your PiPet requires a name! Choose 1 name from the list.");
    name_instruction->setWordWrap(true);

    name_list = new QListWidget();
    name_list->setViewMode(QListView::ListMode);
    name_list->setFlow(QListView::TopToBottom);
    for (int i = 0; i < 26; i++) {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(str_names[i]);
        name_list->addItem(item);
    }

    name->setLayout(l_name);
    l_name->addWidget(name_instruction);
    l_name->addWidget(name_list);
    scroll_name->setWidget(name);

    // Species widget ------------------------------------------------------------
    species = new QWidget();
    scroll_species = new QScrollArea();
    scroll_species->setWidgetResizable(true);
    l_species = new QVBoxLayout();
    species_instruction = new QLabel(R"(Your PiPet can be 1 of 3 species:
        <ul>
            <li><em>Electric Axolotl</em>: An amphibian with powers of electricity.</li>
            <li><em>Seal Cat</em>: A semiaquatic cat with a powerful tailfin.</li>
            <li><em>Dragon Dog</em>: A scaly, fire-breathing, and winged dog.</li>
        </ul>)");
    species_instruction->setTextFormat(Qt::RichText);
    species_instruction->setWordWrap(true);
    species->setLayout(l_species);
    l_species->addWidget(species_instruction);
    scroll_species->setWidget(species);

    box_buttons = new QGroupBox("Choose 1");
    l_buttons = new QVBoxLayout();
    box_buttons->setLayout(l_buttons);
    l_species->addWidget(box_buttons);

    b_axolotl = new QRadioButton("Electric Axolotl");
    b_cat     = new QRadioButton("Seal Cat");
    b_dog     = new QRadioButton("Dragon Dog");
    l_buttons->addWidget(b_axolotl);
    l_buttons->addWidget(b_cat);
    l_buttons->addWidget(b_dog);

    // Create widget -------------------------------------------------------------
    layout = new QVBoxLayout();
    tabs   = new QTabWidget();
    b_done = new QPushButton("DONE");

    // FIX: start disabled — only enable once both name and species are chosen
    b_done->setEnabled(false);
    b_done->setToolTip("Choose a name and a species first!");

    tabs->addTab(scroll_name,    "Name");
    tabs->addTab(scroll_species, "Species");

    // Gallery widget ------------------------------------------------------------
    QWidget    *gallery = new QWidget();
    QHBoxLayout *row    = new QHBoxLayout();
    QPushButton *b_left = new QPushButton();
    pictures            = new QStackedWidget();
    QPushButton *b_right = new QPushButton();
    gallery->setLayout(row);

    QIcon left_icon(":/images/Assets/left.png");
    QIcon right_icon(":/images/Assets/right.png");
    b_left->setIcon(left_icon);
    b_right->setIcon(right_icon);

    // Axolotl
    QLabel  *axolotl = new QLabel();
    QImage  *img0    = new QImage(":/images/Sprites/pets/axolotl/axolotl_idle.gif");
    QPixmap  pxmap0  = QPixmap::fromImage(*img0);
    axolotl->setPixmap(pxmap0.scaled(200, 200, Qt::KeepAspectRatio));
    axolotl->setAlignment(Qt::AlignCenter);
    pictures->addWidget(axolotl);

    // DragonDog
    QLabel  *dog  = new QLabel();
    QImage  *img1 = new QImage(":/images/Sprites/pets/dragondog/dragondog_idle.gif");
    QPixmap  pxmap1 = QPixmap::fromImage(*img1);
    dog->setPixmap(pxmap1.scaled(200, 200, Qt::KeepAspectRatio));
    dog->setAlignment(Qt::AlignCenter);
    pictures->addWidget(dog);

    // SeelCat
    QLabel  *cat  = new QLabel();
    QImage  *img2 = new QImage(":/images/Sprites/pets/seelcat/seelcat_idle.gif");
    QPixmap  pxmap2 = QPixmap::fromImage(*img2);
    cat->setPixmap(pxmap2.scaled(200, 200, Qt::KeepAspectRatio));
    cat->setAlignment(Qt::AlignCenter);
    pictures->addWidget(cat);

    connect(b_left,  SIGNAL(clicked()), this, SLOT(left_gallery()));
    connect(b_right, SIGNAL(clicked()), this, SLOT(right_gallery()));

    row->addWidget(b_left);
    row->addWidget(pictures);
    row->addWidget(b_right);

    this->setLayout(layout);
    layout->addWidget(gallery);
    layout->addWidget(tabs);
    layout->addWidget(b_done);

    // Validation: check eligibility whenever name or species changes
    connect(name_list, SIGNAL(itemClicked(QListWidgetItem*)),
            this,      SLOT(checkDoneEligibility()));
    connect(b_axolotl, SIGNAL(clicked()), this, SLOT(checkDoneEligibility()));
    connect(b_cat,     SIGNAL(clicked()), this, SLOT(checkDoneEligibility()));
    connect(b_dog,     SIGNAL(clicked()), this, SLOT(checkDoneEligibility()));

    b_done->setStyleSheet(R"(
        QPushButton { background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #4850DB, stop: 1 #4A71DB);
        border: 2px inset #FBA8FF; border-radius: 10px; padding: 4px; font: bold; }
        QPushButton:pressed { background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #4A71DB, stop: 1 #4850DB); }
        QPushButton:disabled { background-color: #555; color: #999; }
    )");
}

void Create::checkDoneEligibility() {
    if (name_list->currentItem())   name_chosen    = true;
    if (b_axolotl->isChecked() || b_cat->isChecked() || b_dog->isChecked())
        species_chosen = true;
    b_done->setEnabled(name_chosen && species_chosen);
    if (name_chosen && species_chosen)
        b_done->setToolTip(""); // clear the tooltip once ready
}

void Create::left_gallery() {
    int current = pictures->currentIndex();
    if (current == 0) pictures->setCurrentIndex(2);
    else pictures->setCurrentIndex(current - 1);
}

void Create::right_gallery() {
    int current = pictures->currentIndex();
    if (current == 2) pictures->setCurrentIndex(0);
    else pictures->setCurrentIndex(current + 1);
}
