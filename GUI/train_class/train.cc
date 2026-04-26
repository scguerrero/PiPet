/*
 * Train class implementation file.
 * In Train mode, the Player can play mini-games with their pet to increase the pet's attributes.
 *
 * Author(s): Sasha C. Guerrero & Luke Cerwin
 */
#include "train.h"

Train::Train(PiPet* pet, Player* player, QWidget *parent)
    : QWidget{parent}, m_pet(pet), m_player(player)
{
    m_bg.load(":/images/Backgrounds/trainbg.png");
    stack    = new QStackedWidget();
    trainHub = new QWidget();
    stack->addWidget(trainHub); // index 0; minigames added lazily on first open

    // Top-level layout
    main_layout = new QVBoxLayout();
    this->setLayout(main_layout);
    main_layout->addWidget(stack);

    // Train Hub widgets ------------------------------------------
    layout = new QVBoxLayout();
    trainHub->setLayout(layout);
    b_minigame1 = new QPushButton("Play PiPatterns!");
    b_minigame2 = new QPushButton("Play PiDash!");
    b_minigame3 = new QPushButton("Play MindReader!");

    // PiPatterns logo
    logo_pipatterns = new QLabel();
    logo_pipatterns->setPixmap(QPixmap(":/images/Assets/PiPatterns.png")
                                   .scaled(275, 275, Qt::KeepAspectRatio, Qt::FastTransformation));
    logo_pipatterns->setAlignment(Qt::AlignCenter);

    // PiDash logo
    logo_pidash = new QLabel();
    logo_pidash->setPixmap(QPixmap(":/images/Assets/pidash.png")
                               .scaled(200, 200, Qt::KeepAspectRatio, Qt::FastTransformation));
    logo_pidash->setAlignment(Qt::AlignCenter);

    // mindReader logo
    logo_mindReader = new QLabel();
    logo_mindReader->setPixmap(QPixmap(":/images/Assets/MindReader.png")
                                   .scaled(300, 300, Qt::KeepAspectRatio, Qt::FastTransformation));
    logo_mindReader->setAlignment(Qt::AlignCenter);

    // Construct b_back BEFORE any setUtilityStyle call that dereferences it
    b_back = new QPushButton("Back");

    // Add logos and buttons to layout
    layout->addWidget(logo_pipatterns);
    layout->addWidget(b_minigame1);
    layout->addWidget(logo_pidash);
    layout->addWidget(b_minigame2);
    layout->addWidget(logo_mindReader);
    layout->addWidget(b_minigame3);

    // Button stylesheets
    setUtilityStyle(*b_back);

    connect(b_minigame1, SIGNAL(clicked()), this, SLOT(openPiPatterns()));
    connect(b_minigame2, SIGNAL(clicked()), this, SLOT(openPiDash()));

    // Connect minigame3 button to mindReader
    connect(b_minigame3, SIGNAL(clicked()), this, SLOT(openmindReader()));
}

void Train::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);
    QPainter p(this);
    if (!m_bg.isNull())
        p.drawPixmap(0, 0, width(), height(), m_bg);
}

void Train::openTrainHub() {
    stack->setCurrentIndex(0);
}

void Train::openPiPatterns() {
    if (!pipatterns) {
        pipatterns = new PiPatterns(m_player);
        stack->addWidget(pipatterns);
        setUtilityStyle(*pipatterns->b_back);
        connect(pipatterns->b_back, SIGNAL(clicked()), this, SLOT(openTrainHub()));
        connect(pipatterns, &PiPatterns::gameFinished,
                this, &Train::onPiPatternsFinished);
    }
    stack->setCurrentWidget(pipatterns);
}

void Train::setUtilityStyle(QPushButton &button) {
    button.setStyleSheet(R"(
        QPushButton { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4850DB, stop:1 #4A71DB);
            border: 2px inset #FBA8FF; border-radius: 10px;
            padding: 4px; font: bold; }
        QPushButton:pressed { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4A71DB, stop:1 #4850DB); })");
}


void Train::onPiPatternsFinished(int finalScore, int xpEarned, bool perfect)
{
    int intelligenceGain = qMax(1, xpEarned  / 5);
    if (perfect) intelligenceGain += 25;
    m_pet->increase_intelligence(intelligenceGain);
    m_pet->set_happiness(qMax(0, m_pet->happiness() + finalScore / 25));
    tryAwardLootbox(finalScore, xpEarned, "PiPatterns");
}

// ══════════════════════════════════════════════════════════════
//  Minigame 2 — PiDash (Tanya)
// ══════════════════════════════════════════════════════════════

void Train::openPiDash() {
    if (!m_trackRush) {
        // Use m_player->getPet().pet_type() — same source as openmindReader() —
        // so both minigames resolve the pet type from the same object.
        // Using m_pet->pet_type() caused a mismatch when the Player's pet and
        // the raw PiPet* pointer were out of sync.
        QString petTypeStr = m_player->getPet().pet_type();
        Character::PetType petTypeEnum = Character::DragonDog;
        if      (petTypeStr == "ElectricAxolotl") petTypeEnum = Character::ElectricAxolotl;
        else if (petTypeStr == "SeelCat")         petTypeEnum = Character::SeelCat;

        m_trackRush = new piDash(
            m_pet,
            m_player,
            petTypeEnum,
            this,
            450, 400
            );
        stack->addWidget(m_trackRush);
        connect(m_trackRush, &piDash::gameFinished,
                this,         &Train::onTrackRushFinished);
        connect(m_trackRush->btnBack, &QPushButton::clicked,
                this, [this]() { stack->setCurrentWidget(trainHub); });
    }

    m_trackRush->refreshCharacter();
    stack->setCurrentWidget(m_trackRush);
    m_trackRush->setFocus();
}

void Train::onTrackRushFinished(int finalScore, int xpEarned)
{
    int happinessGain = qMax(1, finalScore / 20);
    int energyGain    = qMax(1, finalScore / 25);
    int strengthGain  = qMax(1, xpEarned  / 5) + (xpEarned > 10 ? 25 : 0);
    int attackGain    = xpEarned / 10;
    m_pet->increase_happiness(happinessGain);
    m_pet->increase_energy(energyGain);
    m_pet->increase_strength(strengthGain);
    m_pet->increase_attack(attackGain);
    if (xpEarned > 0 && finalScore > 10)
        emit lootboxEarned("PiDash");
}


void Train::refreshMindReader()
{
    if (m_mindReader)
        m_mindReader->refreshCharacter();
}

//  Minigame 3 - mindReader (number guessing)

void Train::openmindReader()
{
    if (!m_mindReader) {
        QString petTypeStr = m_player->getPet().pet_type();
        Character::PetType petType = Character::DragonDog;
        if      (petTypeStr == "ElectricAxolotl") petType = Character::ElectricAxolotl;
        else if (petTypeStr == "SeelCat")         petType = Character::SeelCat;

        m_mindReader = new mindReader(m_player, petType, this);
        stack->addWidget(m_mindReader);

        connect(m_mindReader, &mindReader::gameFinished,
                this,          &Train::onMindReaderFinished);
        connect(m_mindReader->b_back, &QPushButton::clicked,
                this, [this]() { stack->setCurrentWidget(trainHub); });
    }

    // Sync sprite and hat to whatever the player has equipped right now.
    m_mindReader->refreshCharacter();

    stack->setCurrentWidget(m_mindReader);
    m_mindReader->setFocus();
}

void Train::onMindReaderFinished(int finalScore, int xpEarned)
{
    int intelligenceGain = qMax(1, xpEarned  / 5);
    int happinessGain    = qMax(1, finalScore / 25);
    int hungerGain       = qMax(1, finalScore / 40);
    if (finalScore >= 50) intelligenceGain += 25;

    m_pet->increase_intelligence(intelligenceGain);
    m_pet->increase_happiness(happinessGain);
    m_pet->increase_hunger(hungerGain);
    tryAwardLootbox(finalScore, xpEarned, "MindReader");
}

void Train::tryAwardLootbox(int /*score*/, int xpEarned, const QString &source)
{
    if (xpEarned > 0)
        emit lootboxEarned(source);
}
