/*
 * create.cc - Pet creation screen.
 * No tabs — gallery IS the species selector.
 * Author(s): Sasha C. Guerrero
 */

#include "create.h"
#include <QPainter>

Create::Create(QWidget *parent) : QWidget{parent} {
    m_bg.load(":/images/Backgrounds/character_screen.jpg"); // character select background

    layout = new QVBoxLayout(this);
    layout->setSpacing(8);
    layout->setContentsMargins(16, 16, 16, 16);

    // ── Gallery ───────────────────────────────────────────────────────────
    QHBoxLayout *galleryRow = new QHBoxLayout();

    b_left  = new QPushButton(this);
    b_right = new QPushButton(this);
    b_left->setIcon(QIcon(":/images/Assets/left.png"));
    b_right->setIcon(QIcon(":/images/Assets/right.png"));
    b_left->setFixedSize(44, 44);
    b_right->setFixedSize(44, 44);

    petImage = new QLabel(this);
    petImage->setAlignment(Qt::AlignCenter);
    petImage->setFixedSize(200, 200);

    galleryRow->addWidget(b_left);
    galleryRow->addWidget(petImage);
    galleryRow->addWidget(b_right);
    layout->addLayout(galleryRow);

    // ── Pet name label ────────────────────────────────────────────────────
    petName = new QLabel(this);
    petName->setAlignment(Qt::AlignCenter);
    petName->setStyleSheet(
        "QLabel { background-color: rgba(0,0,0,160); border-radius: 8px;"
        "padding: 6px; color: #ffd700; font-size: 18px; font-weight: bold; }");
    layout->addWidget(petName);

    // ── Pet description ───────────────────────────────────────────────────
    petDescription = new QLabel(this);
    petDescription->setAlignment(Qt::AlignCenter);
    petDescription->setWordWrap(true);
    petDescription->setStyleSheet(
        "QLabel { background-color: rgba(0,0,0,140); border-radius: 8px;"
        "padding: 6px; color: mistyrose; font-size: 14px; }");
    layout->addWidget(petDescription);

    // ── Hidden radio buttons (game.cc reads these to get species choice) ──
    b_axolotl = new QRadioButton(this);
    b_cat      = new QRadioButton(this);
    b_dog      = new QRadioButton(this);
    b_axolotl->hide();
    b_cat->hide();
    b_dog->hide();

    // ── Name list ─────────────────────────────────────────────────────────
    QLabel *nameHeader = new QLabel("Choose a name for your PiPet:", this);
    nameHeader->setAlignment(Qt::AlignCenter);
    nameHeader->setStyleSheet(
        "QLabel { background-color: rgba(0,0,0,140); border-radius: 6px;"
        "padding: 4px; color: mistyrose; font-size: 15px; }");
    layout->addWidget(nameHeader);

    name_list = new QListWidget(this);
    name_list->setViewMode(QListView::ListMode);
    name_list->setFlow(QListView::TopToBottom);
    name_list->setFixedHeight(140);
    for (int i = 0; i < 26; i++) {
        QListWidgetItem *item = new QListWidgetItem(str_names[i]);
        name_list->addItem(item);
    }
    layout->addWidget(name_list);

    // ── DONE button ───────────────────────────────────────────────────────
    b_done = new QPushButton("DONE", this);
    b_done->setEnabled(false);
    b_done->setToolTip("Choose a name first!");
    layout->addWidget(b_done);

    // ── Connections ───────────────────────────────────────────────────────
    connect(b_left,    SIGNAL(clicked()), this, SLOT(left_gallery()));
    connect(b_right,   SIGNAL(clicked()), this, SLOT(right_gallery()));
    connect(name_list, SIGNAL(itemClicked(QListWidgetItem*)),
            this,      SLOT(checkDoneEligibility()));

    // ── Styles ────────────────────────────────────────────────────────────
    b_done->setStyleSheet(R"(
        QPushButton { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4850DB, stop:1 #4A71DB);
            border: 2px inset #FBA8FF; border-radius: 10px;
            padding: 6px; font: bold; color: mistyrose; }
        QPushButton:pressed { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4A71DB, stop:1 #4850DB); }
        QPushButton:disabled { background-color: #444; color: #888; }
    )");

    QString btnStyle = R"(
        QPushButton { background-color: rgba(0,0,0,140);
            border: 2px inset #FBA8FF; border-radius: 8px; padding: 4px; }
        QPushButton:pressed { background-color: rgba(72,80,219,180); }
    )";
    b_left->setStyleSheet(btnStyle);
    b_right->setStyleSheet(btnStyle);

    // Show first pet on load
    updateGallery();
}

void Create::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    if (!m_bg.isNull())
        painter.drawPixmap(0, 0, width(), height(), m_bg);
}

void Create::updateGallery() {
    QStringList imagePaths = {
        ":/images/Sprites/pets/axolotl/axolotl_idle.gif",
        ":/images/Sprites/pets/dragondog/dragondog_idle.gif",
        ":/images/Sprites/pets/seelcat/seelcat_idle.gif"
    };

    QImage img(imagePaths[m_galleryIndex]);
    QPixmap px = QPixmap::fromImage(img);
    petImage->setPixmap(px.scaled(200, 200, Qt::KeepAspectRatio,
                                  Qt::SmoothTransformation));

    petName->setText(petNames[m_galleryIndex]);
    petDescription->setText(petDescriptions[m_galleryIndex]);

    b_axolotl->setChecked(m_galleryIndex == 0);
    b_dog->setChecked    (m_galleryIndex == 1);
    b_cat->setChecked    (m_galleryIndex == 2);
}

void Create::left_gallery() {
    m_galleryIndex = (m_galleryIndex == 0) ? 2 : m_galleryIndex - 1;
    updateGallery();
}

void Create::right_gallery() {
    m_galleryIndex = (m_galleryIndex == 2) ? 0 : m_galleryIndex + 1;
    updateGallery();
}

void Create::checkDoneEligibility() {
    if (name_list->currentItem())
        name_chosen = true;
    b_done->setEnabled(name_chosen);
    if (name_chosen)
        b_done->setToolTip("");
}
