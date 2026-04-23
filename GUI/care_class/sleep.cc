/*
 * sleep.cc - Sleep screen with character GIF.
 * Cuddle replaces Brush Teeth.
 * Author(s): Luke Cewin & Sasha Guerrero
 */
#include "sleep.h"
#include <QPainter>

Sleep::Sleep(Player *player, Character::PetType petType, QWidget *parent)
    : QWidget{parent}, player(player), petType(petType)
{
    m_bg.load(":/images/Backgrounds/bedroom_16bit.png");

    layout      = new QVBoxLayout();
    actionsBox  = new QGroupBox("■‿■");
    actionsGrid = new QGridLayout();

    // ── Character GIF ─────────────────────────────────────────────────────
    character = new Character(this);
    character->setFixedSize(160, 160);
    character->syncWithPlayer(*player, petType);

    sleepDisplay = new QLabel();
    sleepDisplay->setAlignment(Qt::AlignCenter);
    sleepDisplay->setWordWrap(true);
    sleepDisplay->setStyleSheet(
        "QLabel { background-color: rgba(0,0,0,160); border-radius: 6px;"
        "padding: 6px; color: mistyrose; }");
    updateSleepDisplay();

    // ── Info helper — shown on open, hides after 3 s ──────────────────────
    infoHelper = new QLabel(this);
    infoHelper->setAlignment(Qt::AlignCenter);
    infoHelper->setWordWrap(true);
    infoHelper->setStyleSheet(
        "QLabel { background-color: rgba(0,0,0,170); border-radius: 8px;"
        "padding: 6px; color: mistyrose; font-size: 15px; }");
    infoHelper->setFixedWidth(300);
    infoHelper->setText("Use the actions below to help your pet rest!");
    infoHelper->hide();

    m_infoTimer = new QTimer(this);
    m_infoTimer->setSingleShot(true);
    connect(m_infoTimer, &QTimer::timeout, infoHelper, &QLabel::hide);

    cuddleBtn   = new QPushButton("🤗 Cuddle Pet");
    wearPjsBtn  = new QPushButton("👖 Wear Pjs");
    readBookBtn = new QPushButton("📖 Bed Time Story");
    tuckInBtn   = new QPushButton("🛌 Tuck In Bed");

    actionsGrid->addWidget(cuddleBtn,   0, 0, Qt::AlignCenter);
    actionsGrid->addWidget(wearPjsBtn,  0, 1, Qt::AlignCenter);
    actionsGrid->addWidget(readBookBtn, 1, 0, Qt::AlignCenter);
    actionsGrid->addWidget(tuckInBtn,   1, 1, Qt::AlignCenter);
    actionsBox->setLayout(actionsGrid);

    connect(cuddleBtn,   SIGNAL(clicked()), this, SLOT(cuddle()));
    connect(wearPjsBtn,  SIGNAL(clicked()), this, SLOT(wearPjs()));
    connect(readBookBtn, SIGNAL(clicked()), this, SLOT(readBook()));
    connect(tuckInBtn,   SIGNAL(clicked()), this, SLOT(tuckIn()));

    actionsBox->setStyleSheet(
        "QGroupBox { background-color: rgba(0,0,0,155); border-radius: 8px;"
        "color: mistyrose; margin-top: 30px; }"
        "QGroupBox::title { color: mistyrose; subcontrol-origin: margin;"
        "subcontrol-position: top center; padding: 0 4px; }");

    // Character GIF at top center, buttons below
    layout->addSpacing(150);
    layout->addWidget(character, 0, Qt::AlignCenter);
    layout->addStretch();
    layout->addWidget(sleepDisplay);
    layout->addWidget(actionsBox);
    this->setLayout(layout);
}

void Sleep::showEvent(QShowEvent *e) {
    QWidget::showEvent(e);

    // Position info helper near the top center
    infoHelper->setGeometry((width() - 300) / 2, 20, 300, 50);
    infoHelper->show();
    infoHelper->raise();
    m_infoTimer->start(3000);
}

void Sleep::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    if (!m_bg.isNull())
        painter.drawPixmap(0, 0, width(), height(), m_bg);
}

void Sleep::updateSleepDisplay() {
    sleepDisplay->setText(
        QString("Energy: %1 / 100").arg(player->getPet().energy()));
}

void Sleep::applySleepAction(int boost, const QString &message) {
    PiPet pet = player->getPet();
    if (pet.energy() >= 100) {
        sleepDisplay->setText("Your pet is fully rested!");
        return;
    }
    int newVal = qMin(100, pet.energy() + boost);
    pet.set_energy(newVal);
    player->setPet(pet);
    sleepDisplay->setText(
        QString("%1  |  Energy: %2 / 100").arg(message).arg(newVal));
}

void Sleep::cuddle()  { applySleepAction(5,  "Aww so cute!"); }
void Sleep::wearPjs() { applySleepAction(5,  "Wear Pjs!"); }

void Sleep::readBook() {
    applySleepAction(10, "Bed Time Story!");
    m_bedTimeStoryCount++;
    emit bedTimeStoryUsed(m_bedTimeStoryCount); // → Bookworm
}

void Sleep::tuckIn() {
    applySleepAction(20, "Tucked In!");
    emit tuckInUsed(); // → Beauty Sleep
}

// ── Hat-aware character refresh ───────────────────────────────────────────
// Called by game.cc each time the sleep screen is opened so the equipped
// hat (if any) is always visible on the character sprite.

void Sleep::refreshCharacter() {
    QString hat = player->getPet().hat();
    if (hat.isEmpty()) {
        character->syncWithPlayer(*player, petType);
        return;
    }

    QString folder, prefix;
    QString type = player->getPet().pet_type();
    if      (type == "ElectricAxolotl") { folder = "axolotl";   prefix = "axolotl";   }
    else if (type == "SeelCat")         { folder = "seelcat";   prefix = "seelcat";   }
    else                                { folder = "dragondog"; prefix = "dragondog"; }

    QString stage = player->getPet().age_group();
    QString infix = (stage == "Teen") ? "teen_" : (stage == "Adult") ? "adult_" : "";
    QString path  = QString(":/images/Sprites/pets/%1/%2_%3%4.gif")
                        .arg(folder, prefix, infix, hat);

    QMovie *movie = new QMovie(path, QByteArray(), character);
    if (movie->isValid()) {
        QLabel *disp = character->findChild<QLabel *>();
        if (disp) {
            if (disp->movie()) disp->movie()->stop();
            disp->setMovie(movie);
            movie->start();
            return;
        }
    }
    delete movie;
    character->syncWithPlayer(*player, petType);
}
