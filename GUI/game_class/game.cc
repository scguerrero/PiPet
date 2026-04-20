/*
 * game.cc - Top-level game widget implementation.
 * Author(s): Luke Cewin & Sasha Guerrero
 */
#include "game.h"

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
    gear   = new Gear();

    pages->addWidget(start);   // 0
    pages->addWidget(create);  // 1
    pages->addWidget(mode);    // 2
    pages->addWidget(feed);    // 3
    pages->addWidget(groom);   // 4
    pages->addWidget(sleep);   // 5
    pages->addWidget(train);   // 6
    pages->addWidget(battle);  // 7
    pages->addWidget(gear);    // 8

    // Utility bar
    utility_bar = new QHBoxLayout();
    b_save = new QPushButton("SAVE");
    b_home = new QPushButton("HOME");
    b_quit = new QPushButton("QUIT");
    b_save->setIcon(QIcon(":/images/Assets/save.png"));
    b_home->setIcon(QIcon(":/images/Assets/home.png"));
    b_quit->setIcon(QIcon(":/images/Assets/quit.png"));
    utility_bar->addWidget(b_save);
    utility_bar->addWidget(b_home);
    utility_bar->addWidget(b_quit);
    layout->addLayout(utility_bar);

    // Start navigation
    if (new_game)
        connect(start->b_start, SIGNAL(clicked()), this, SLOT(open_create()));
    else
        connect(start->b_start, SIGNAL(clicked()), this, SLOT(open_mode()));

    connect(b_save, SIGNAL(clicked()), this, SLOT(saveGame()));
    connect(b_home, SIGNAL(clicked()), this, SLOT(open_mode()));  // HOME → mode screen
    connect(b_quit, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));

    connect(create->b_done, SIGNAL(clicked()), this, SLOT(onCreateDone()));

    // Mode bubble clicks → go directly to care sub-screen
    mode->feedBubble->installEventFilter(this);
    mode->groomBubble->installEventFilter(this);
    mode->sleepBubble->installEventFilter(this);

    // Mode other buttons
    connect(mode->b_train,  SIGNAL(clicked()), this, SLOT(open_train()));
    connect(mode->b_battle, SIGNAL(clicked()), this, SLOT(open_battle()));
    connect(mode->b_gear,   SIGNAL(clicked()), this, SLOT(open_gear()));

    // Back buttons → return to Mode
    connect(feed->backBtn,  &QPushButton::clicked, this, &Game::open_mode);
    connect(groom->backBtn, &QPushButton::clicked, this, &Game::open_mode);
    connect(sleep->backBtn, &QPushButton::clicked, this, &Game::open_mode);
    connect(train->b_back,  SIGNAL(clicked()), this, SLOT(open_mode()));
    connect(battle->btnBack,SIGNAL(clicked()), this, SLOT(open_mode()));
    connect(gear->b_back,   SIGNAL(clicked()), this, SLOT(open_mode()));

    setUtilityStyle(*b_save);
    setUtilityStyle(*b_home);
    setUtilityStyle(*b_quit);
}

// Event filter catches bubble label taps/clicks
bool Game::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonRelease) {
        if (obj == mode->feedBubble)  { open_feed();  return true; }
        if (obj == mode->groomBubble) { open_groom(); return true; }
        if (obj == mode->sleepBubble) { open_sleep(); return true; }
    }
    return QWidget::eventFilter(obj, event);
}

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

    // Rebuild Feed, Groom, Sleep with new pet type
    pages->removeWidget(feed);  delete feed;
    pages->removeWidget(groom); delete groom;
    pages->removeWidget(sleep); delete sleep;

    feed  = new Feed(player, currentPetType);
    groom = new Groom(player, currentPetType);
    sleep = new Sleep(player, currentPetType);

    pages->insertWidget(3, feed);
    pages->insertWidget(4, groom);
    pages->insertWidget(5, sleep);

    connect(feed->backBtn,  &QPushButton::clicked, this, &Game::open_mode);
    connect(groom->backBtn, &QPushButton::clicked, this, &Game::open_mode);
    connect(sleep->backBtn, &QPushButton::clicked, this, &Game::open_mode);

    // Re-install event filter on mode bubbles
    mode->feedBubble->installEventFilter(this);
    mode->groomBubble->installEventFilter(this);
    mode->sleepBubble->installEventFilter(this);

    open_mode();
}

void Game::open_mode()   { mode->refreshDisplay(); pages->setCurrentIndex(2); }
void Game::open_feed()   { feed->updateHungerDisplay();   pages->setCurrentIndex(3); }
void Game::open_groom()  { groom->updateHygieneDisplay(); pages->setCurrentIndex(4); }
void Game::open_sleep()  { sleep->updateSleepDisplay();   pages->setCurrentIndex(5); }
void Game::open_start()  { pages->setCurrentIndex(0); }
void Game::open_create() { pages->setCurrentIndex(1); }
void Game::open_train()  { pages->setCurrentIndex(6); }
void Game::open_battle() { pages->setCurrentIndex(7); }
void Game::open_gear()   { pages->setCurrentIndex(8); }

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
            padding: 4px; font: bold; }
        QPushButton:pressed { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4A71DB, stop:1 #4850DB); })");
}
