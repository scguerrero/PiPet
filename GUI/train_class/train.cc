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
    // QStackedWidget contains TrainHub, PiPatterns, and PiDash.
    // mindReader is added lazily on first openmindReader() call.
    stack      = new QStackedWidget();
    trainHub   = new QWidget();
    pipatterns = new PiPatterns(player);
    stack->addWidget(trainHub);    // index 0
    stack->addWidget(pipatterns);  // index 1

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
    QImage *img0 = new QImage(":/images/Assets/PiPatterns.png");
    QPixmap pxmap0 = QPixmap::fromImage(img0->scaled(275, 275, Qt::KeepAspectRatio));
    logo_pipatterns->setPixmap(pxmap0);
    logo_pipatterns->setAlignment(Qt::AlignCenter);

    // PiDash logo
    logo_pidash = new QLabel();
    QImage *img1 = new QImage(":/images/Assets/pidash.png");
    QPixmap pxmap1 = QPixmap::fromImage(img1->scaled(200, 200, Qt::KeepAspectRatio));
    logo_pidash->setPixmap(pxmap1);
    logo_pidash->setAlignment(Qt::AlignCenter);

    // mindReader logo (swap mindReader.png for your real asset when ready)
    logo_mindReader = new QLabel();
    QImage *img2 = new QImage(":/images/Assets/MindReader.png");
    QPixmap pxmap2 = QPixmap::fromImage(img2->scaled(250, 250, Qt::KeepAspectRatio));
    logo_mindReader->setPixmap(pxmap2);
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
    setUtilityStyle(*pipatterns->b_back);

    // Connect minigame1 button to PiPatterns
    connect(b_minigame1, SIGNAL(clicked()), this, SLOT(openPiPatterns()));

    // Connect PiPatterns back button to Train Hub
    connect(pipatterns->b_back, SIGNAL(clicked()), this, SLOT(openTrainHub()));

    // Connect PiPatterns game result to stat gains + lootbox award
    connect(pipatterns, &PiPatterns::gameFinished,
            this, &Train::onPiPatternsFinished);

    // Connect minigame2 button to PiDash
    connect(b_minigame2, SIGNAL(clicked()), this, SLOT(openPiDash()));

    // Connect minigame3 button to mindReader
    connect(b_minigame3, SIGNAL(clicked()), this, SLOT(openmindReader()));

    // ══════════════════════════════════════════════════════════════════════
    //  LOOTBOX HOOK
    // ──────────────────────────────────────────────────────────────────────
    // After a minigame finishes (onMindReaderFinished, onTrackRushFinished,
    // or a dedicated lootbox button here in the TrainHub), call:
    //
    //   tryAwardLootbox(score, xpEarned);
    //
    // That function (stub below) decides whether to award a hat, calls
    //   m_player->getPet().unlockHat("crown");   // or "cowboy"
    //   emit hatUnlocked("crown");
    // and game.cc picks it up to refresh Gear.
    //
    // Suggested placement options:
    //   A) At the end of onMindReaderFinished() / onTrackRushFinished()
    //   B) As a dedicated QPushButton "Open Lootbox" in the TrainHub layout
    //      (add the button between logo_mindReader and b_minigame3 above)
    //
    // The two hats tracked in the save file are "crown" and "cowboy".
    // Any key passed to unlockHat() that matches a kHats entry in gear.cc
    // will automatically become selectable in the Gear screen.
    // ══════════════════════════════════════════════════════════════════════
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
    stack->setCurrentIndex(1);
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


void Train::onPiPatternsFinished(int finalScore, int xpEarned)
{
    int intelligenceGain = qMax(1, xpEarned  / 5);
    int happinessGain    = qMax(1, finalScore / 25);
    int hungerGain       = qMax(1, finalScore / 40);
    m_pet->increase_intelligence(intelligenceGain);
    m_pet->increase_happiness(happinessGain);
    m_pet->increase_hunger(hungerGain);
    qDebug() << "[PiPatterns] score:" << finalScore << "xp:" << xpEarned;

    tryAwardLootbox(finalScore, xpEarned);
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
    int strengthGain  = qMax(1, xpEarned  / 5);
    int attackGain    = xpEarned / 10;
    m_pet->increase_happiness(happinessGain);
    m_pet->increase_energy(energyGain);
    m_pet->increase_strength(strengthGain);
    m_pet->increase_attack(attackGain);
    qDebug() << "[PiDash] score:" << finalScore << "xp:" << xpEarned;
    emit lootboxEarned();
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

    m_pet->increase_intelligence(intelligenceGain);
    m_pet->increase_happiness(happinessGain);
    m_pet->increase_hunger(hungerGain);
// Sasha!
    // Do NOT navigate away here — the result panel is still on screen.
    // The player uses the Back button (already connected in openmindReader)
    // to return to the hub whenever they're ready.
    qDebug() << "[mindReader] score:" << finalScore << "xp:" << xpEarned
             << "→ intelligence +" << intelligenceGain
             << "happiness +"      << happinessGain
             << "hunger +"         << hungerGain;

    tryAwardLootbox(finalScore, xpEarned);
}

// ══════════════════════════════════════════════════════════════════════════
//  tryAwardLootbox — STUB for your partner to implement
// ──────────────────────────────────────────────────────────────────────────
//  Rules to implement inside this function:
//    1. Decide if the player earns a lootbox roll (e.g. score threshold,
//       random chance, or a dedicated button press — see the LOOTBOX HOOK
//       comment in the constructor for placement options).
//    2. Pick a hat to award. The two save-tracked hats are "crown" and
//       "cowboy".  Other hat keys ("santa", "wizard") can be added here too.
//    3. Skip the award if the player already has that hat:
//         if (m_player->getPet().isHatUnlocked(hatKey)) return;
//    4. Unlock it and fire the signal so game.cc can refresh Gear:
//         PiPet p = m_player->getPet();
//         p.unlockHat(hatKey);
//         m_player->setPet(p);
//         emit hatUnlocked(hatKey);
// ══════════════════════════════════════════════════════════════════════════
void Train::tryAwardLootbox(int /*score*/, int xpEarned)
{
    if (xpEarned > 0)
        emit lootboxEarned();
}
