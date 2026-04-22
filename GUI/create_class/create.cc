/*
 * create.cc - Pet creation screen with animated GIF gallery.
 * Author(s): Sasha C. Guerrero, Luke Cerwin
 */
#include "create.h"
#include <QPainter>

Create::Create(QWidget *parent) : QWidget{parent} {
    m_bg.load(":/images/Backgrounds/character_screen.png");

    layout = new QVBoxLayout(this);
    layout->setSpacing(8);
    layout->setContentsMargins(16, 16, 16, 16);

    // ── Gallery row ───────────────────────────────────────────────────────
    QHBoxLayout *galleryRow = new QHBoxLayout();

    b_left  = new QPushButton(this);
    b_right = new QPushButton(this);
    b_left->setIcon(QIcon(":/images/Assets/left.png"));
    b_right->setIcon(QIcon(":/images/Assets/right.png"));
    b_left->setFixedSize(44, 44);
    b_right->setFixedSize(44, 44);

    // petImage uses QMovie for animated GIF
    // transparent background — no blue box
    petImage = new QLabel(this);
    petImage->setAlignment(Qt::AlignCenter);
    petImage->setFixedSize(180, 180);
    petImage->setScaledContents(false);
    petImage->setStyleSheet("QLabel { background-color: transparent; }");

    galleryRow->addWidget(b_left,   0, Qt::AlignVCenter);
    galleryRow->addWidget(petImage, 0, Qt::AlignCenter);
    galleryRow->addWidget(b_right,  0, Qt::AlignVCenter);
    layout->addLayout(galleryRow);

    // ── Pet name ──────────────────────────────────────────────────────────
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
        "padding: 6px; color: mistyrose; font-size: 13px; }");
    layout->addWidget(petDescription);

    // ── Hidden radio buttons (game.cc reads these) ────────────────────────
    b_axolotl = new QRadioButton(this);
    b_cat      = new QRadioButton(this);
    b_dog      = new QRadioButton(this);
    b_axolotl->hide();
    b_cat->hide();
    b_dog->hide();

    // ── Name header ───────────────────────────────────────────────────────
    nameHeader = new QLabel("Choose a name for your PiPet:", this);
    nameHeader->setAlignment(Qt::AlignCenter);
    nameHeader->setStyleSheet(
        "QLabel { background-color: rgba(0,0,0,140); border-radius: 6px;"
        "padding: 4px; color: mistyrose; font-size: 14px; }");
    layout->addWidget(nameHeader);

    // ── Name list ─────────────────────────────────────────────────────────
    name_list = new QListWidget(this);
    name_list->setViewMode(QListView::ListMode);
    name_list->setFlow(QListView::TopToBottom);
    name_list->setFixedHeight(130);
    name_list->setStyleSheet(
        "QListWidget { background-color: rgba(0,0,0,150);"
        "border: 2px solid #FBA8FF; border-radius: 6px; color: mistyrose; }"
        "QListWidget::item:selected { background-color: rgba(72,80,219,200); }");
    for (int i = 0; i < 26; i++)
        name_list->addItem(new QListWidgetItem(str_names[i]));
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

    // ── Button styles ─────────────────────────────────────────────────────
    b_done->setStyleSheet(R"(
        QPushButton { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4850DB, stop:1 #4A71DB);
            border: 2px inset #FBA8FF; border-radius: 10px;
            padding: 6px; font: bold; color: mistyrose; }
        QPushButton:pressed { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4A71DB, stop:1 #4850DB); }
        QPushButton:disabled { background-color: #444; color: #888; }
    )");

    QString arrowStyle = R"(
        QPushButton { background-color: rgba(0,0,0,140);
            border: 2px inset #FBA8FF; border-radius: 8px; padding: 4px; }
        QPushButton:pressed { background-color: rgba(72,80,219,180); }
    )";
    b_left->setStyleSheet(arrowStyle);
    b_right->setStyleSheet(arrowStyle);

    updateGallery();
}

void Create::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    if (!m_bg.isNull())
        painter.drawPixmap(0, 0, width(), height(), m_bg);
}

void Create::updateGallery() {
    // Stop and clear old movie
    if (currentMovie) {
        currentMovie->stop();
        delete currentMovie;
        currentMovie = nullptr;
    }

    // Load new animated GIF via QMovie
    currentMovie = new QMovie(gifPaths[m_galleryIndex], QByteArray(), this);

    // Scale GIF to fit 180x180 keeping aspect ratio
    currentMovie->setScaledSize(QSize(180, 180));

    petImage->setMovie(currentMovie);
    currentMovie->start();

    petName->setText(petNames[m_galleryIndex]);
    petDescription->setText(petDescriptions[m_galleryIndex]);

    // Auto-select radio button
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
    if (name_list->currentItem()) name_chosen = true;
    b_done->setEnabled(name_chosen);
    if (name_chosen) b_done->setToolTip("");

}
