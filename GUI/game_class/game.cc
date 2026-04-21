/*
 * game.cc - Top-level game widget implementation.
 * Author(s): Luke Cerwin, Sasha Guerrero
 */
#include "game.h"
#include <QTimer>
#include <QDialog>
#include <QVBoxLayout>
#include <QScrollArea>

// 30 minutes in milliseconds
static constexpr int kInactivityMs = 30 * 60 * 1000;

Game::Game(QWidget *parent) : QWidget{parent} {
    this->setWindowTitle("PIPET");

    pet    = new PiPet();
    player = new Player(*pet);

    layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    this->setLayout(layout);

    pages = new QStackedWidget();
    layout->addWidget(pages);

    start  = new Start();
    create = new Create();
    mode   = new Mode(player);
    feed   = new Feed(player, currentPetType);
    groom  = new Groom(player, currentPetType);
    sleep  = new Sleep(player, currentPetType);
    train  = new Train(pet);
    battle = new Battle();
    gear   = new Gear(player, currentPetType);

    pages->addWidget(start);   // 0
    pages->addWidget(create);  // 1
    pages->addWidget(mode);    // 2
    pages->addWidget(feed);    // 3
    pages->addWidget(groom);   // 4
    pages->addWidget(sleep);   // 5
    pages->addWidget(train);   // 6
    pages->addWidget(battle);  // 7
    pages->addWidget(gear);    // 8

    // ── Bottom utility bar ────────────────────────────────────────────────
    utilityWidget = new QWidget(this);
    utility_bar   = new QHBoxLayout(utilityWidget);
    utility_bar->setContentsMargins(4, 4, 4, 4);
    utility_bar->setSpacing(6);

    b_home         = new QPushButton("🏠 HOME",         utilityWidget);
    b_achievements = new QPushButton("🏆 Achievements", utilityWidget);

    b_home->setIcon(QIcon(":/images/Assets/home.png"));

    utility_bar->addWidget(b_home);
    utility_bar->addWidget(b_achievements);
    layout->addWidget(utilityWidget);

    // ── Save button on Mode screen top-left ───────────────────────────────
    b_save_mode = new QPushButton("SAVE", mode);
    b_save_mode->setIcon(QIcon(":/images/Assets/save.png"));
    b_save_mode->setGeometry(8, 8, 80, 36);
    b_save_mode->hide();

    // ── Inactivity timer ──────────────────────────────────────────────────
    m_inactivityTimer = new QTimer(this);
    m_inactivityTimer->setSingleShot(true);
    m_inactivityTimer->setInterval(kInactivityMs);
    connect(m_inactivityTimer, &QTimer::timeout,
            this, &Game::onInactivityTriggered);
    m_inactivityTimer->start();

    // ── Connections ───────────────────────────────────────────────────────
    if (new_game)
        connect(start->b_start, SIGNAL(clicked()), this, SLOT(open_create()));
    else
        connect(start->b_start, SIGNAL(clicked()), this, SLOT(open_mode()));

    connect(b_save_mode,    SIGNAL(clicked()), this, SLOT(saveGame()));
    connect(b_home,         SIGNAL(clicked()), this, SLOT(open_mode()));
    connect(b_achievements, SIGNAL(clicked()), this, SLOT(showAchievementsScreen()));

    connect(create->b_done, SIGNAL(clicked()), this, SLOT(onCreateDone()));

    mode->feedBubble->installEventFilter(this);
    mode->groomBubble->installEventFilter(this);
    mode->sleepBubble->installEventFilter(this);

    connect(mode, &Mode::petAgedUp,     this, &Game::onAgeChanged);
    connect(mode, &Mode::temperTantrum, this, [&](){ auto u = player->achievements.onTemperTantrum(true,true); showAchievementPopup(u); });

    connect(mode->b_train,   SIGNAL(clicked()), this, SLOT(open_train()));
    connect(mode->b_battle,  SIGNAL(clicked()), this, SLOT(open_battle()));
    connect(mode->b_gear,    SIGNAL(clicked()), this, SLOT(open_gear()));

    connect(train->b_back,    SIGNAL(clicked()), this, SLOT(open_mode()));
    connect(gear, &Gear::hatEquipped, this, [this](const QString &hatKey) {
        PiPet p = player->getPet();
        p.set_hat(hatKey);
        player->setPet(p);
        mode->refreshDisplay();
        if (hatKey == "crown")
            onCrownHatEquipped();
    });

    setUtilityStyle(*b_save_mode);
    setUtilityStyle(*b_home);
    setUtilityStyle(*b_achievements);

    showUtilityBar(false);
}

// ── Utility bar ───────────────────────────────────────────────────────────

void Game::showUtilityBar(bool show) {
    utilityWidget->setVisible(show);
}

void Game::showHomeOnly(bool activeStyle) {
    showUtilityBar(true);
    b_home->show();
    b_achievements->show();
    if (activeStyle) {
        setUtilityStyle(*b_home);
    } else {
        b_home->setStyleSheet(R"(
            QPushButton {
                background-color: rgba(60,60,60,180);
                border: 2px inset #888888; border-radius: 10px;
                padding: 4px; font: bold; color: #888888;
            })");
    }
}

// ── Event filter — resets inactivity timer on any tap ────────────────────
bool Game::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonRelease ||
        event->type() == QEvent::TouchEnd) {
        resetInactivityTimer();
        if (obj == mode->feedBubble)  { open_feed();  return true; }
        if (obj == mode->groomBubble) { open_groom(); return true; }
        if (obj == mode->sleepBubble) { open_sleep(); return true; }
    }
    return QWidget::eventFilter(obj, event);
}

void Game::resetInactivityTimer() {
    m_inactivityTimer->start(kInactivityMs);
}

void Game::onInactivityTriggered() {
    auto unlocked = player->achievements.onInactive();
    showAchievementPopup(unlocked);
}

// ── Navigation ────────────────────────────────────────────────────────────

void Game::open_start() {
    showUtilityBar(false);
    b_save_mode->hide();
    pages->setCurrentIndex(0);
}

void Game::open_create() {
    showUtilityBar(false);
    b_save_mode->hide();
    pages->setCurrentIndex(1);
}

void Game::open_mode() {
    showHomeOnly(false);
    b_save_mode->show();
    b_save_mode->raise();
    mode->refreshDisplay();
    pages->setCurrentIndex(2);

    // Check Temper Tantrum whenever returning to mode
    PiPet pet = player->getPet();
    bool sleeping = (pet.energy() < 25);
    bool angry    = (pet.hunger() < 25);
    auto unlocked = player->achievements.onTemperTantrum(sleeping, angry);
    showAchievementPopup(unlocked);
}

void Game::open_feed() {
    showHomeOnly(true);
    b_save_mode->hide();
    feed->updateHungerDisplay();
    pages->setCurrentIndex(3);
}

void Game::open_groom() {
    showHomeOnly(true);
    b_save_mode->hide();
    groom->updateHygieneDisplay();
    pages->setCurrentIndex(4);
}

void Game::open_sleep() {
    showHomeOnly(true);
    b_save_mode->hide();
    sleep->updateSleepDisplay();
    pages->setCurrentIndex(5);
}

void Game::open_train() {
    showHomeOnly(true);
    b_save_mode->hide();
    pages->setCurrentIndex(6);
}

void Game::open_battle() {
    showHomeOnly(true);
    b_save_mode->hide();
    pages->setCurrentIndex(7);
}

void Game::open_gear() {
    showHomeOnly(true);
    b_save_mode->hide();
    gear->refresh(currentPetType);
    pages->setCurrentIndex(8);
}

// ── Achievement trigger points ────────────────────────────────────────────

void Game::onBattleWon() {
    player->battleWins++;
    auto unlocked = player->achievements.onBattleWon(player->battleWins);
    showAchievementPopup(unlocked);
}

void Game::onFedBone() {
    QString type = player->getPet().pet_type();
    auto unlocked = player->achievements.onFedBone(type);
    showAchievementPopup(unlocked);
}

void Game::onMinigamePlayed(int gameIndex) {
    // bitmask: bit 0 = game 0, bit 1 = game 1, bit 2 = game 2
    player->minigamesPlayed |= (1 << gameIndex);
    int count = __builtin_popcount(player->minigamesPlayed);
    auto unlocked = player->achievements.onMinigamePlayed(count);
    showAchievementPopup(unlocked);
}

void Game::onAgeChanged(const QString &ageGroup) {
    auto unlocked = player->achievements.onAgeChanged(ageGroup);
    showAchievementPopup(unlocked);
}

void Game::onCrownHatEquipped() {
    auto unlocked = player->achievements.onCrownHatEquipped();
    showAchievementPopup(unlocked);
}

// ── Achievement popup ─────────────────────────────────────────────────────
// Shows a small non-blocking toast at the top of the screen for each unlock
void Game::showAchievementPopup(const QList<QString> &titles) {
    if (titles.isEmpty()) return;

    for (const QString &title : titles) {
        QLabel *toast = new QLabel(this);
        toast->setText(QString("🏆 Achievement Unlocked!\n\"%1\"").arg(title));
        toast->setAlignment(Qt::AlignCenter);
        toast->setWordWrap(true);
        toast->setStyleSheet(R"(
            QLabel {
                background-color: rgba(30,10,60,230);
                border: 2px solid #ffd700;
                border-radius: 12px;
                color: #ffd700;
                font-size: 15px;
                font-weight: bold;
                padding: 10px;
            }
        )");
        toast->setFixedWidth(300);
        toast->adjustSize();
        // Center horizontally, near top
        int tx = (width() - 300) / 2;
        toast->setGeometry(tx, 60, 300, toast->height());
        toast->raise();
        toast->show();

        // Auto-dismiss after 3 seconds
        QTimer::singleShot(3000, toast, &QLabel::deleteLater);
    }
}

// ── Achievements screen ───────────────────────────────────────────────────
void Game::showAchievementsScreen() {
    QDialog dlg(this);
    dlg.setWindowTitle("Achievements");
    dlg.setFixedSize(400, 520);
    dlg.setStyleSheet(
        "QDialog { background-color: #120828; }"
        "QLabel  { background-color: transparent; color: mistyrose; "
        "          font-family: monospace; font-size: 13px; }"
        "QPushButton { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "  stop:0 #4850DB,stop:1 #4A71DB); border:2px inset #FBA8FF;"
        "  border-radius:10px; padding:4px; font:bold; color:mistyrose; }");

    QVBoxLayout *dlgLayout = new QVBoxLayout(&dlg);
    dlgLayout->setContentsMargins(16, 16, 16, 16);
    dlgLayout->setSpacing(8);

    QLabel *header = new QLabel("🏆  Achievements", &dlg);
    header->setAlignment(Qt::AlignCenter);
    header->setStyleSheet(
        "QLabel { color:#ffd700; font-size:18px; font-weight:bold; }");
    dlgLayout->addWidget(header);

    // Scrollable list of achievements
    QScrollArea *scroll = new QScrollArea(&dlg);
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet(
        "QScrollArea { border: none; background: transparent; }");

    QWidget     *listWidget = new QWidget();
    QVBoxLayout *listLayout = new QVBoxLayout(listWidget);
    listLayout->setSpacing(6);
    listLayout->setContentsMargins(4, 4, 4, 4);

    for (const Achievement &a : player->achievements.all()) {
        QLabel *row = new QLabel(listWidget);
        row->setWordWrap(true);

        if (a.unlocked) {
            row->setText(QString("✅  <b>%1</b><br>"
                                 "<span style='color:#aaa;font-size:11px;'>%2</span>")
                             .arg(a.title, a.description));
            row->setStyleSheet(
                "QLabel { background-color: rgba(72,80,219,120);"
                "border: 1px solid #ffd700; border-radius: 8px;"
                "padding: 8px; color: mistyrose; }");
        } else {
            row->setText(QString("🔒  <b style='color:#666;'>%1</b><br>"
                                 "<span style='color:#555;font-size:11px;'>%2</span>")
                             .arg(a.title, a.description));
            row->setStyleSheet(
                "QLabel { background-color: rgba(0,0,0,100);"
                "border: 1px solid #333; border-radius: 8px;"
                "padding: 8px; color: #666; }");
        }
        listLayout->addWidget(row);
    }
    listLayout->addStretch();
    scroll->setWidget(listWidget);
    dlgLayout->addWidget(scroll, 1);

    QPushButton *closeBtn = new QPushButton("Close", &dlg);
    closeBtn->setFixedHeight(40);
    connect(closeBtn, &QPushButton::clicked, &dlg, &QDialog::accept);
    dlgLayout->addWidget(closeBtn);

    dlg.exec();
}

// ── onCreateDone ──────────────────────────────────────────────────────────
void Game::onCreateDone() {
    if (create->b_axolotl->isChecked())
        currentPetType = Character::ElectricAxolotl;
    else if (create->b_cat->isChecked())
        currentPetType = Character::SeelCat;
    else
        currentPetType = Character::DragonDog;

    QListWidgetItem *item = create->name_list->currentItem();
    if (item) player->pet.set_name(item->text());

    mode->setPetType(currentPetType);
    gear->refresh(currentPetType);

    {
        PiPet p = player->getPet();
        QString typeStr = (currentPetType == Character::ElectricAxolotl) ? "ElectricAxolotl"
                        : (currentPetType == Character::SeelCat)         ? "SeelCat"
                                                                         : "DragonDog";
        p.set_pet_type(typeStr);
        player->setPet(p);
    }

    pages->removeWidget(feed);  delete feed;
    pages->removeWidget(groom); delete groom;
    pages->removeWidget(sleep); delete sleep;

    feed  = new Feed(player, currentPetType);
    groom = new Groom(player, currentPetType);
    sleep = new Sleep(player, currentPetType);

    pages->insertWidget(3, feed);
    pages->insertWidget(4, groom);
    pages->insertWidget(5, sleep);

    mode->feedBubble->installEventFilter(this);
    mode->groomBubble->installEventFilter(this);
    mode->sleepBubble->installEventFilter(this);

    open_mode();
}

// ── Save / Load ───────────────────────────────────────────────────────────
QJsonObject Game::toJson() const {
    QJsonObject json;
    json["Player"] = player->toJson();
    return json;
}

void Game::read(const QJsonObject &json) {
    if (const QJsonValue v = json["Player"]; v.isObject())
        *player = Player::fromJSON(v.toObject());
}

bool Game::loadGame() {
    QFile loadFile("player.json");
    if (!loadFile.open(QIODevice::ReadOnly)) { qWarning("Couldn't open save file."); return false; }
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    read(loadDoc.object());

    QString typeStr = player->getPet().pet_type();
    if      (typeStr == "ElectricAxolotl") currentPetType = Character::ElectricAxolotl;
    else if (typeStr == "SeelCat")         currentPetType = Character::SeelCat;
    else                                   currentPetType = Character::DragonDog;

    mode->setPetType(currentPetType);
    gear->refresh(currentPetType);

    QString savedHat = player->getPet().hat();
    if (!savedHat.isEmpty())
        gear->restoreHat(savedHat);

    return true;
}

bool Game::saveGame() {
    QFile saveFile("player.json");
    if (!saveFile.open(QIODevice::WriteOnly)) { qWarning("Couldn't open save file."); return false; }
    QMessageBox msg(this);
    msg.setText("Save successful!");
    msg.exec();
    saveFile.write(QJsonDocument(toJson()).toJson());
    return true;
}

void Game::setUtilityStyle(QPushButton &button) {
    button.setStyleSheet(R"(
        QPushButton { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4850DB, stop:1 #4A71DB);
            border: 2px inset #FBA8FF; border-radius: 10px;
            padding: 4px; font: bold; color: mistyrose; }
        QPushButton:pressed { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4A71DB, stop:1 #4850DB); })");
}
