/*
 * game.cc - Top-level game widget implementation.
 * Author(s): Sasha C. Guerrero
 */
#include "game.h"

Game::Game(QWidget *parent) : QWidget{parent} {
    this->setWindowTitle("PIPET");
    this->setContentsMargins(15, 15, 15, 15);

    pet    = new PiPet();
    player = new Player(*pet);

    layout = new QVBoxLayout();
    this->setLayout(layout);

    pages = new QStackedWidget();
    layout->addWidget(pages);

    start  = new Start();
    create = new Create();
    mode   = new Mode(player);
    // Care needs petType — starts with default DragonDog,
    // rebuilt in onCreateDone() once the player makes a choice
    care   = new Care(player, currentPetType);
    train  = new Train();
    battle = new Battle();

    pages->addWidget(start);   // 0
    pages->addWidget(create);  // 1
    pages->addWidget(mode);    // 2
    pages->addWidget(care);    // 3
    pages->addWidget(train);   // 4
    pages->addWidget(battle);  // 5

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

    // Navigation
    if (new_game)
        connect(start->b_start, SIGNAL(clicked()), this, SLOT(open_create()));
    else
        connect(start->b_start, SIGNAL(clicked()), this, SLOT(open_mode()));

    connect(b_save, SIGNAL(clicked()), this,                     SLOT(saveGame()));
    connect(b_home, SIGNAL(clicked()), this,                     SLOT(open_start()));
    connect(b_quit, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));

    connect(create->b_done, SIGNAL(clicked()), this, SLOT(onCreateDone()));

    connect(mode->b_care,    SIGNAL(clicked()), this, SLOT(open_care()));
    connect(care->b_back,    SIGNAL(clicked()), this, SLOT(open_mode()));

    connect(mode->b_train,   SIGNAL(clicked()), this, SLOT(open_train()));
    connect(train->b_back,   SIGNAL(clicked()), this, SLOT(open_mode()));

    connect(mode->b_battle,  SIGNAL(clicked()), this, SLOT(open_battle()));
    connect(battle->btnBack, SIGNAL(clicked()), this, SLOT(open_mode()));

    setUtilityStyle(*b_save);
    setUtilityStyle(*b_home);
    setUtilityStyle(*b_quit);
}

void Game::onCreateDone() {
    // Read species selection
    if (create->b_axolotl->isChecked())
        currentPetType = Character::ElectricAxolotl;
    else if (create->b_cat->isChecked())
        currentPetType = Character::SeelCat;
    else
        currentPetType = Character::DragonDog;

    // Set pet name
    QListWidgetItem *item = create->name_list->currentItem();
    if (item)
        player->pet.set_name(item->text());

    // Tell Mode which pet to show
    mode->setPetType(currentPetType);

    // Rebuild Care with the correct petType now that we know it
    // Remove old care page, replace with new one that has the right pet
    pages->removeWidget(care);
    delete care;
    care = new Care(player, currentPetType);
    pages->insertWidget(3, care);

    // Reconnect care navigation
    connect(mode->b_care,  SIGNAL(clicked()), this, SLOT(open_care()));
    connect(care->b_back,  SIGNAL(clicked()), this, SLOT(open_mode()));

    open_mode();
}

void Game::open_mode() {
    mode->refreshDisplay();
    pages->setCurrentIndex(2);
}

void Game::open_care() {
    care->updateStats();
    pages->setCurrentIndex(3);
}

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
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    read(loadDoc.object());
    return true;
}

bool Game::saveGame() {
    QFile saveFile("player.json");
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    QMessageBox msg(this);
    msg.setText("Save successful!");
    msg.exec();
    saveFile.write(QJsonDocument(toJson()).toJson());
    return true;
}

void Game::open_start()  { pages->setCurrentIndex(0); }
void Game::open_create() { pages->setCurrentIndex(1); }
void Game::open_train()  { pages->setCurrentIndex(4); }
void Game::open_battle() { pages->setCurrentIndex(5); }

void Game::setUtilityStyle(QPushButton &button) {
    button.setStyleSheet(R"(
        QPushButton { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4850DB, stop:1 #4A71DB);
            border: 2px inset #FBA8FF; border-radius: 10px;
            padding: 4px; font: bold; }
        QPushButton:pressed { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4A71DB, stop:1 #4850DB); })");
}
