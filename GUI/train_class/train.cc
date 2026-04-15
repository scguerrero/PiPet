/*
 * Train class specification file.
 * In Train mode, the Player can play mini-games with their pet to increase the pet's attributes.
 * PiPatterns is by Sasha. PiDash and SkySnack are by Tanya.
 *
 * Author(s): Sasha C. Guerrero, Tanya Magurupira
 */
#include "train.h"

Train::Train(PiPet* pet, QWidget *parent)
    : QWidget{parent}, m_pet(pet)
{
    m_stack    = new QStackedWidget(this);
    m_menuPage = new QWidget();
    pipatterns = new PiPatterns();

    layout = new QVBoxLayout();
    b_minigame1 = new QPushButton("Minigame 1");
    b_minigame2 = new QPushButton("Minigame 2");
    b_minigame3 = new QPushButton("Minigame 3");
    b_back = new QPushButton("BACK");

    layout->addWidget(b_minigame1);
    layout->addWidget(b_minigame2);
    layout->addWidget(b_minigame3);
    layout->addWidget(b_back);
    m_menuPage->setLayout(layout);

    m_stack->addWidget(m_menuPage);   // index 0
    m_stack->addWidget(pipatterns);   // index 1

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->addWidget(m_stack);
    this->setLayout(outerLayout);

    QIcon left_icon(":/images/Assets/left.png");
    b_back->setIcon(left_icon);

    connect(pipatterns->b_back, SIGNAL(clicked()), this, SLOT(openMenuPage()));
    connect(b_minigame1, SIGNAL(clicked()), this, SLOT(openPiPatterns()));
    //connect(b_minigame2, &QPushButton::clicked, this, &Train::onMiniGame2);
    //connect(b_minigame3, &QPushButton::clicked, this, &Train::onMiniGame3);

    b_back->setStyleSheet(R"(
        QPushButton { background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #4850DB, stop: 1 #4A71DB);
        border: 2px inset #FBA8FF;
        border-radius: 10px;
        padding: 4px;
        font: bold; }
        QPushButton:pressed {
        background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #4A71DB, stop: 1 #4850DB);
        }
        )");
}

void Train::openMenuPage() {
    m_stack->setCurrentIndex(0);
}

// Minigame 1 - PiPatterns is at index 1 of the QStackedWidget
void Train::openPiPatterns() {
    m_stack->setCurrentIndex(1);
}

/*
// ══════════════════════════════════════════════════════════════
//  Minigame 2 — PiDash (runner)
// ══════════════════════════════════════════════════════════════

void Train::onMiniGame2()
{
    if (!m_trackRush) {
        m_trackRush = new minigame_2(m_pet, this, 800, 530);
        m_stack->addWidget(m_trackRush);

        connect(m_trackRush, &minigame_2::gameFinished,
                this,         &Train::onTrackRushFinished);
        connect(m_trackRush->btnBack, &QPushButton::clicked,
                this, [this]() { m_stack->setCurrentWidget(m_menuPage); });
    }
    m_stack->setCurrentWidget(m_trackRush);
    m_trackRush->setFocus();
}

void Train::onTrackRushFinished(int finalScore, int xpEarned)
{
    int happinessGain = qMax(1, finalScore / 20);
    int energyGain    = qMax(1, finalScore / 25);
    int strengthGain  = qMax(1, xpEarned  / 5);
    int attackGain    = xpEarned / 10;

    m_pet->increase_happiness(happinessGain);
    m_pet->increase_energy(energyGain);
    m_pet->increase_strength(strengthGain);
    m_pet->increase_attack(attackGain);

    m_stack->setCurrentWidget(m_menuPage);

    qDebug() << "[PiDash] score:" << finalScore << "xp:" << xpEarned
             << "→ happiness +" << happinessGain
             << "energy +"      << energyGain
             << "strength +"    << strengthGain
             << "attack +"      << attackGain;
}

// ══════════════════════════════════════════════════════════════
//  Minigame 3 — Sky Snack (food catcher)               ← NEW
// ══════════════════════════════════════════════════════════════

void Train::onMiniGame3()
{
    if (!m_skySnack) {
        m_skySnack = new minigame_3(m_pet, this, 800, 520);
        m_stack->addWidget(m_skySnack);

        connect(m_skySnack, &minigame_3::gameFinished,
                this,        &Train::onSkySnackFinished);
        connect(m_skySnack->btnBack, &QPushButton::clicked,
                this, [this]() { m_stack->setCurrentWidget(m_menuPage); });
    }
    m_stack->setCurrentWidget(m_skySnack);
    m_skySnack->setFocus();
}

void Train::onSkySnackFinished(int finalScore, int xpEarned)
{
    // Food game → rewards hunger (feeding!) + happiness + intelligence
    int hungerGain       = qMax(1, finalScore / 15);
    int happinessGain    = qMax(1, finalScore / 25);
    int intelligenceGain = qMax(1, xpEarned  / 5);

    m_pet->increase_hunger(hungerGain);
    m_pet->increase_happiness(happinessGain);
    m_pet->increase_intelligence(intelligenceGain);

    m_stack->setCurrentWidget(m_menuPage);

    qDebug() << "[PiCatcher] score:" << finalScore << "xp:" << xpEarned
             << "→ hunger +"       << hungerGain
             << "happiness +"      << happinessGain
             << "intelligence +"   << intelligenceGain;
}
*/