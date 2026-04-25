/*
 * game.cc - Top-level game widget implementation.
 * Achievements button moved to Gear screen.
 * Bottom bar simplified to Home only at normal size.
 * Author(s): Luke Cerwin
 */
#include "game.h"
#include <QTimer>
#include <QDialog>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QDir>
#include <QFileInfo>

static constexpr int kInactivityMs = 30 * 60 * 1000;
static constexpr int kMarathonMs   = 2  * 60 * 60 * 1000;

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
    train = new Train(pet, player);
    battle = new Battle();
    gear   = new Gear(player, currentPetType);
    lootbox = new Lootbox(player);

    pages->addWidget(start);   // 0
    pages->addWidget(create);  // 1
    pages->addWidget(mode);    // 2
    pages->addWidget(feed);    // 3
    pages->addWidget(groom);   // 4
    pages->addWidget(sleep);   // 5
    pages->addWidget(train);   // 6
    pages->addWidget(battle);  // 7
    pages->addWidget(gear);    // 8
    pages->addWidget(lootbox); // 9

    //  Bottom bar — Home only, normal size
    utilityWidget = new QWidget(this);
    utility_bar   = new QHBoxLayout(utilityWidget);
    utility_bar->setContentsMargins(4, 4, 4, 4);
    utility_bar->setSpacing(6);

    b_home = new QPushButton("HOME", utilityWidget);
    b_home->setIcon(QIcon(":/images/Assets/home.png"));
    utility_bar->addWidget(b_home);
    layout->addWidget(utilityWidget);

    //  Save button on Mode screen top-left
    b_save_mode = new QPushButton("SAVE", mode);
    b_save_mode->setIcon(QIcon(":/images/Assets/save.png"));
    b_save_mode->setGeometry(8, 8, 80, 36);
    b_save_mode->hide();

    //  Timers
    m_inactivityTimer = new QTimer(this);
    m_inactivityTimer->setSingleShot(true);
    m_inactivityTimer->setInterval(kInactivityMs);
    connect(m_inactivityTimer, &QTimer::timeout,
            this, &Game::onInactivityTriggered);
    m_inactivityTimer->start();

    m_marathonTimer = new QTimer(this);
    m_marathonTimer->setSingleShot(true);
    m_marathonTimer->setInterval(kMarathonMs);
    connect(m_marathonTimer, &QTimer::timeout,
            this, &Game::onMarathonTriggered);
    m_marathonTimer->start();

    //  Connections
    // Start button routing is wired in loadGame() once we know whether a
    // save file exists — connecting here would always fire open_create()
    // because new_game hasn't been resolved yet.
    connect(start->b_start, SIGNAL(clicked()), this, SLOT(open_create()));

    connect(b_save_mode, SIGNAL(clicked()), this, SLOT(saveGame()));
    connect(b_home,      SIGNAL(clicked()), this, SLOT(open_mode()));

    connect(create->b_done, SIGNAL(clicked()), this, SLOT(onCreateDone()));

    mode->feedBubble->installEventFilter(this);
    mode->groomBubble->installEventFilter(this);
    mode->sleepBubble->installEventFilter(this);

    // Mode signals
    connect(mode, &Mode::petAgedUp,     this, &Game::onAgeChanged);
    connect(mode, &Mode::temperTantrum, this, [this]() {
        if (!m_achievementsEnabled) return;
        auto u = player->achievements.onTemperTantrum(true, true);
        showAchievementPopup(u);
    });

    connect(mode->b_train,   SIGNAL(clicked()), this, SLOT(open_train()));
    connect(mode->b_battle,  SIGNAL(clicked()), this, SLOT(open_battle()));
    connect(mode->b_gear,    SIGNAL(clicked()), this, SLOT(open_gear()));

    connect(train->b_back,   SIGNAL(clicked()), this, SLOT(open_mode()));
    connect(train, &Train::lootboxEarned, this, [this](const QString &source) {
        lootbox->awardLootbox();
        QLabel *toast = new QLabel(QString("You won a lootbox from %1!\nVisit Gear → Lootbox to open it.").arg(source), this);
        toast->setAlignment(Qt::AlignCenter);
        toast->setWordWrap(true);
        toast->setStyleSheet(R"(
            QLabel {
                background-color: rgba(30,10,60,230);
                border: 2px solid #FBA8FF;
                border-radius: 12px;
                padding: 10px;
            }
        )");
        toast->setFixedWidth(300);
        toast->adjustSize();
        toast->setGeometry((width() - 300) / 2, 120, 300, toast->height());
        toast->raise();
        toast->show();
        QTimer::singleShot(3500, toast, &QLabel::deleteLater);
    });

    // Lootbox: when Train awards a hat, unlock it in the save data and
    // make it selectable in Gear — no screen navigation required.
    connect(train, &Train::hatUnlocked, this, [this](const QString &hatKey) {
        PiPet p = player->getPet();
        p.unlockHat(hatKey);          // persists to save file
        player->setPet(p);
        gear->unlockHat(hatKey);      // refreshes the hat card in Gear UI

    });

    // Battle achievement signals
    connect(battle, &Battle::battleWon,   this, &Game::onBattleWon);
    // Sleep achievement signals
    connect(sleep, &Sleep::tuckInUsed,       this, &Game::onTuckIn);
    connect(sleep, &Sleep::bedTimeStoryUsed, this, &Game::onBedTimeStory);

    // Gear — hat equipped + achievements button
    connect(gear, &Gear::hatEquipped, this, [this](const QString &hatKey) {
        PiPet p = player->getPet();
        p.set_hat(hatKey);
        player->setPet(p);
        mode->refreshDisplay();
        if (hatKey == "crown")
            onCrownHatEquipped();
    });
    connect(gear->b_achievements, &QPushButton::clicked,
            this, &Game::showAchievementsScreen);
    connect(gear->b_lootboxes, &QPushButton::clicked,
            this, &Game::open_lootbox);

    connect(lootbox, &Lootbox::hatUnlocked, this, [this](const QString &hatKey) {
        PiPet p = player->getPet();
        if (!p.isHatUnlocked(hatKey)) {
            p.unlockHat(hatKey);
            player->setPet(p);
            gear->unlockHat(hatKey);
        }
    });

    setUtilityStyle(*b_save_mode);
    setUtilityStyle(*b_home);

    showUtilityBar(false);
}

//  Utility bar

void Game::showUtilityBar(bool show) {
    utilityWidget->setVisible(show);
}

void Game::showHomeOnly(bool activeStyle) {
    showUtilityBar(true);
    b_home->show();
    if (activeStyle)
        setUtilityStyle(*b_home);
    else
        b_home->setStyleSheet(R"(
            QPushButton {
                background-color: rgba(60,60,60,180);
                border: 2px inset #888888; border-radius: 10px;
                padding: 4px;
            })");
}

//  Event filter

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
    if (!m_achievementsEnabled) return;
    showAchievementPopup(player->achievements.onInactive());
}

void Game::onMarathonTriggered() {
    if (!m_achievementsEnabled) return;
    showAchievementPopup(player->achievements.onMarathonSession());
}

//  Navigation

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
    player->updateDaysOld();
    mode->resetHintFlag();
    mode->refreshDisplay();
    pages->setCurrentIndex(2);
    onVeteranCheck();
}

void Game::open_feed() {
    showHomeOnly(true);
    b_save_mode->hide();
    feed->refreshCharacter();      // sync hat gif before showing screen
    feed->updateHungerDisplay();
    pages->setCurrentIndex(3);
}

void Game::open_groom() {
    showHomeOnly(true);
    b_save_mode->hide();
    groom->refreshCharacter();     // sync hat gif before showing screen
    groom->updateHygieneDisplay();
    pages->setCurrentIndex(4);
}

void Game::open_sleep() {
    showHomeOnly(true);
    b_save_mode->hide();
    sleep->refreshCharacter();     // sync hat gif before showing screen
    sleep->updateSleepDisplay();
    pages->setCurrentIndex(5);
}

void Game::open_train() {
    showHomeOnly(true);
    b_save_mode->hide();
    train->refreshMindReader();
    pages->setCurrentIndex(6);
}

void Game::open_battle() {
    showHomeOnly(true);
    b_save_mode->hide();
    battle->setPlayerInfo(player, currentPetType);  // sync sprite + hat
    pages->setCurrentIndex(7);
}

void Game::open_gear() {
    showHomeOnly(true);
    b_save_mode->hide();
    gear->refresh(currentPetType);
    pages->setCurrentIndex(8);
}

void Game::open_lootbox() {
    showHomeOnly(true);
    b_save_mode->hide();
    pages->setCurrentIndex(9);
}

//  Achievement trigger slots

void Game::onBattleWon() {
    QList<QString> unlocked = player->achievements.onBattleWon(++player->battleWins);
    showAchievementPopup(unlocked);

    lootbox->awardLootbox();
    QLabel *lbToast = new QLabel("You won a lootbox from Battle!\nVisit Gear → Lootbox to open it.", this);
    lbToast->setAlignment(Qt::AlignCenter);
    lbToast->setWordWrap(true);
    lbToast->setStyleSheet(R"(
        QLabel {
            background-color: rgba(30,10,60,230);
            border: 2px solid #FBA8FF;
            border-radius: 12px;
            padding: 10px;
        }
    )");
    lbToast->setFixedWidth(300);
    lbToast->adjustSize();
    lbToast->setGeometry((width() - 300) / 2, 120, 300, lbToast->height());
    lbToast->raise();
    lbToast->show();
    QTimer::singleShot(3500, lbToast, &QLabel::deleteLater);

    if (unlocked.contains("Not Too Shabby II") && !player->getPet().isHatUnlocked("cowboy")) {
        PiPet p = player->getPet();
        p.unlockHat("cowboy");
        player->setPet(p);
        gear->unlockHat("cowboy");  // removes the lock card in Gear immediately

        QLabel *toast = new QLabel("Cowboy hat unlocked!\nEarned with \"Not Too Shabby II\"", this);
        toast->setAlignment(Qt::AlignCenter);
        toast->setWordWrap(true);
        toast->setStyleSheet(R"(
            QLabel {
                background-color: rgba(30,10,60,230);
                border: 2px solid #FBA8FF;
                border-radius: 12px;
                padding: 10px;
            }
        )");
        toast->setFixedWidth(300);
        toast->adjustSize();
        toast->setGeometry((width() - 300) / 2, 120, 300, toast->height());
        toast->raise();
        toast->show();
        QTimer::singleShot(3500, toast, &QLabel::deleteLater);
    }
}

void Game::onFedBone() {
    showAchievementPopup(
        player->achievements.onFedBone(player->getPet().pet_type()));
}

void Game::onAgeChanged(const QString &ageGroup) {
    showAchievementPopup(player->achievements.onAgeChanged(ageGroup));
}

void Game::onCrownHatEquipped() {
    showAchievementPopup(player->achievements.onCrownHatEquipped());
}

void Game::onTuckIn() {
    showAchievementPopup(player->achievements.onTuckIn());
}

void Game::onBedTimeStory(int totalUsed) {
    showAchievementPopup(player->achievements.onBedTimeStory(totalUsed));
}

void Game::onVeteranCheck() {
    showAchievementPopup(
        player->achievements.onDaysOld(player->getPet().days_old()));
}

//  Achievement popup toast

void Game::showAchievementPopup(const QList<QString> &titles) {
    if (titles.isEmpty()) return;
    int offset = 0;
    for (const QString &title : titles) {
        QLabel *toast = new QLabel(this);
        toast->setText(QString("Achievement Unlocked!\n\"%1\"").arg(title));
        toast->setAlignment(Qt::AlignCenter);
        toast->setWordWrap(true);
        toast->setStyleSheet(R"(
            QLabel {
                background-color: rgba(30,10,60,230);
                border: 2px solid #FBA8FF;
                border-radius: 12px;
                padding: 10px;
            }
        )");
        toast->setFixedWidth(300);
        toast->adjustSize();
        toast->setGeometry((width() - 300) / 2, 60 + offset, 300, toast->height());
        toast->raise();
        toast->show();
        offset += toast->height() + 8;
        QTimer::singleShot(3000, toast, &QLabel::deleteLater);

        lootbox->awardLootbox();
    }

    QLabel *lbToast = new QLabel("You won a lootbox from an Achievement!\nVisit Gear → Lootbox to open it.", this);
    lbToast->setAlignment(Qt::AlignCenter);
    lbToast->setWordWrap(true);
    lbToast->setStyleSheet(R"(
        QLabel {
            background-color: rgba(30,10,60,230);
            border: 2px solid #FBA8FF;
            border-radius: 12px;
            padding: 10px;
        }
    )");
    lbToast->setFixedWidth(300);
    lbToast->adjustSize();
    lbToast->setGeometry((width() - 300) / 2, 60 + offset, 300, lbToast->height());
    lbToast->raise();
    lbToast->show();
    QTimer::singleShot(3500, lbToast, &QLabel::deleteLater);
}

//  Achievements screen

void Game::showAchievementsScreen() {
    QDialog dlg(this);
    dlg.setWindowTitle("Achievements");
    dlg.setFixedSize(400, 520);
    dlg.setStyleSheet(
        "QDialog { background-color: #120828; }"
        "QLabel  { background-color: transparent; }"
        "QPushButton { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "  stop:0 #4850DB,stop:1 #4A71DB); border:2px inset #FBA8FF;"
        "  border-radius:10px; padding:4px; }");

    QVBoxLayout *dlgLayout = new QVBoxLayout(&dlg);
    dlgLayout->setContentsMargins(16, 16, 16, 16);
    dlgLayout->setSpacing(8);

    QLabel *header = new QLabel("Achievements", &dlg);
    header->setAlignment(Qt::AlignCenter);
    header->setStyleSheet("QLabel { color:#ffd700; }");
    dlgLayout->addWidget(header);

    int unlocked = 0;
    for (const Achievement &a : player->achievements.all())
        if (a.unlocked) unlocked++;
    QLabel *progress = new QLabel(
        QString("%1 / %2 unlocked")
            .arg(unlocked).arg(player->achievements.all().size()), &dlg);
    progress->setAlignment(Qt::AlignCenter);
    progress->setStyleSheet("QLabel { color:#aaa; }");
    dlgLayout->addWidget(progress);

    QScrollArea *scroll = new QScrollArea(&dlg);
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("QScrollArea { border: none; background: #1a0a2e; } QWidget#qt_scrollarea_viewport { background: #1a0a2e; }");

    QWidget     *listWidget = new QWidget();
    listWidget->setStyleSheet("background: transparent;");
    QVBoxLayout *listLayout = new QVBoxLayout(listWidget);
    listLayout->setSpacing(6);
    listLayout->setContentsMargins(4, 4, 4, 4);

    for (const Achievement &a : player->achievements.all()) {
        QLabel *row = new QLabel(listWidget);
        row->setWordWrap(true);
        if (a.unlocked) {
            row->setText(QString("<b>%1</b><br>"
                                 "<span>%2</span>")
                             .arg(a.title, a.description));
            row->setStyleSheet(
                "QLabel { background-color: rgba(72,80,219,120);"
                "border: 1px solid #ffd700; border-radius: 8px;"
                "padding: 8px; color: #FFE4E1; }");
        } else {
            row->setText(QString("<b style='color:#555;'>%1</b><br>"
                                 "<span>%2</span>")
                             .arg(a.title, a.description));
            row->setStyleSheet(
                "QLabel { background-color: rgba(0,0,0,100);"
                "border: 1px solid #333; border-radius: 8px;"
                "padding: 8px; color: #555; }");
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

//  onCreateDone

void Game::onCreateDone() {
    m_achievementsEnabled = true;
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

    rebuildCareScreens();
    open_mode();
}

//  Save / Load

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
    // Use a stable, OS-appropriate writable location instead of a bare
    // relative path — the CWD at runtime is not reliable on Linux/macOS.
    QString savePath = QDir::homePath() + "/Desktop/player.json";
    QDir().mkpath(QDir::homePath() + "/Desktop");
    QDir().mkpath(QFileInfo(savePath).absolutePath()); // ensure directory exists

    QFile loadFile(savePath);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file — treating as new game.");
        // Start button already defaults to open_create(); no rewire needed.
        return false;
    }
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    read(loadDoc.object());
    m_achievementsEnabled = true;

    // Save file found and loaded — rewire Start button to go to Mode, not Create.
    disconnect(start->b_start, SIGNAL(clicked()), this, SLOT(open_create()));
    connect   (start->b_start, SIGNAL(clicked()), this, SLOT(open_mode()));

    QString typeStr = player->getPet().pet_type();
    if      (typeStr == "ElectricAxolotl") currentPetType = Character::ElectricAxolotl;
    else if (typeStr == "SeelCat")         currentPetType = Character::SeelCat;
    else                                   currentPetType = Character::DragonDog;

    mode->setPetType(currentPetType);
    gear->refresh(currentPetType);

    rebuildCareScreens();

    for (const QString &hatKey : player->getPet().unlockedHats())
        gear->unlockHat(hatKey);

    QString savedHat = player->getPet().hat();
    if (!savedHat.isEmpty())
        gear->restoreHat(savedHat);

    lootbox->restoreFromPlayer();

    return true;
}

bool Game::saveGame() {
    QString savePath = QDir::homePath() + "/Desktop/player.json";
    QDir().mkpath(QDir::homePath() + "/Desktop");
    QFile saveFile(savePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    saveFile.write(QJsonDocument(toJson()).toJson());

    // Show a non-blocking toast — same style as the hunger hint in Mode
    QLabel *toast = new QLabel("Save successful!", this);
    toast->setAlignment(Qt::AlignCenter);
    toast->setStyleSheet(R"(
        QLabel {
            background-color: rgba(30,10,60,230);
            border: 2px solid #FBA8FF;
            border-radius: 12px;
            padding: 10px;
        }
    )");
    toast->setFixedWidth(260);
    toast->adjustSize();
    toast->setGeometry((width() - 260) / 2, 60, 260, toast->height());
    toast->raise();
    toast->show();
    QTimer::singleShot(2000, toast, &QLabel::deleteLater);

    return true;
}

// rebuildCareScreens
// Destroys and recreates Feed, Groom, and Sleep with the current petType.
// Called from both onCreateDone() and loadGame() so the correct sprite is
// always used regardless of whether this is a new game or a loaded save.

void Game::rebuildCareScreens() {
    pages->removeWidget(feed);  delete feed;
    pages->removeWidget(groom); delete groom;
    pages->removeWidget(sleep); delete sleep;

    feed  = new Feed(player, currentPetType);
    groom = new Groom(player, currentPetType);
    sleep = new Sleep(player, currentPetType);

    pages->insertWidget(3, feed);
    pages->insertWidget(4, groom);
    pages->insertWidget(5, sleep);

    connect(sleep, &Sleep::tuckInUsed,       this, &Game::onTuckIn);
    connect(sleep, &Sleep::bedTimeStoryUsed, this, &Game::onBedTimeStory);

    mode->feedBubble->installEventFilter(this);
    mode->groomBubble->installEventFilter(this);
    mode->sleepBubble->installEventFilter(this);
}

void Game::setUtilityStyle(QPushButton &button) {
    button.setStyleSheet(R"(
        QPushButton { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4850DB, stop:1 #4A71DB);
            border: 2px inset #FBA8FF; border-radius: 10px;
            padding: 4px; }
        QPushButton:pressed { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4A71DB, stop:1 #4850DB); })");
}
