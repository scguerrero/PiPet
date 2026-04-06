/*
 * Game class implementation file.
 * Author(s): Sasha C. Guerrero
 * Source: https://doc.qt.io/qt-6/qtcore-serialization-savegame-example.html
 * Created: 2/9/2026
 * Last Edited: 3/14/2026
 */
#include "game.h"

Game::Game(QWidget *parent) : QWidget{parent} {

    //Create a PiPet and a Player
    pet = new PiPet();
    player = new Player(*pet);

    layout = new QVBoxLayout();
	pages = new QStackedWidget();
	start = new Start();
	create = new Create();
    mode = new Mode();
    care = new Care(player);
    train = new Train();
    battle = new Battle();
    //gear = new Gear();

    // Utility bar widgets
    utility_bar = new QHBoxLayout();
    b_save = new QPushButton("SAVE");
    b_home = new QPushButton("🏠 HOME");
	b_quit = new QPushButton("QUIT");

    // Arrange buttons inside utility bar
    utility_bar->addWidget(b_save);
    utility_bar->addWidget(b_home);
    utility_bar->addWidget(b_quit);

    // Icons for buttons
    b_save->setIcon(QIcon::fromTheme("document-save"));
    b_quit->setIcon(QIcon::fromTheme("application-exit"));

	this->setWindowTitle("PIPET"); // The title of the non-fullscreened application window
    this->setContentsMargins(15,15,15,15); // 30px margins
	
	this->setLayout(layout); // Vertically-arrange widgets inside Game
	layout->addWidget(pages); // Children of layout: pages and quit button
    //layout->addWidget(b_quit); // For testing purposes, we want the ability to quit any time

    top_layout = new QVBoxLayout();
    layout->addLayout(top_layout);
    layout->addLayout(utility_bar);

	pages->addWidget(start);// Children of pages: Start, Create, Mode, Care, Train, Battle, Gear
    pages->addWidget(create); // index 1
    pages->addWidget(mode); // index 2
    pages->addWidget(care); // 3
    pages->addWidget(train); // 4
    pages->addWidget(battle); // 5
    //pages->addWidget(gear); // 6
	
    if (new_game) { // For a new game, the first step is to create a pet
		connect(start->b_start, SIGNAL( clicked() ), this, SLOT( open_create() ));
    } else { // When loading a previous game, send them to the mode-selection page
        connect(start->b_start, SIGNAL( clicked() ), this, SLOT( open_mode() ));
    }

    // The Save button writes current data to a JSON file
    connect(b_save, SIGNAL(clicked()), this, SLOT(saveGame()));

    // The Home button goes back to the Start page
    connect(b_home, SIGNAL( clicked() ), this, SLOT( open_start() ));

    // The Quit button terminates running program
    connect(b_quit, SIGNAL( clicked() ), QApplication::instance(), SLOT( quit() ));

    // Navigation from Mode to Create
    connect(create->b_done, SIGNAL( clicked() ), this, SLOT( open_mode() ));

    // Navigation from Mode tot Care, Care to Mode
    connect(mode->b_care, SIGNAL( clicked() ), this, SLOT( open_care() ));
    connect(care->b_back, SIGNAL( clicked() ), this, SLOT( open_mode() ));

    // Navigation from Mode to Train, Train to Mode
    connect(mode->b_train, SIGNAL( clicked() ), this, SLOT( open_train() ));
    connect(train->b_back, SIGNAL( clicked() ), this, SLOT( open_mode() ));

    // Navigation from Mode to Battle, Battle to Mode
    connect(mode->b_battle, SIGNAL( clicked() ), this, SLOT( open_battle() ));
    connect(battle->btnBack, SIGNAL( clicked() ), this, SLOT( open_mode() ));

    // Styling widgets
    b_save->setStyleSheet(R"(
        QPushButton { background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #4850DB, stop: 1 #4A71DB);
        border: 2px inset #FBA8FF;
        border-radius: 10px;
        padding: 4px;
        font: bold; }
        QPushButton:pressed {
        background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #4A71DB, stop: 1 #4850DB);
        }
        )");
    b_home->setStyleSheet(R"(
        QPushButton { background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #4850DB, stop: 1 #4A71DB);
        border: 2px inset #FBA8FF;
        border-radius: 10px;
        padding: 4px;
        font: bold; }
        QPushButton:pressed {
        background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #4A71DB, stop: 1 #4850DB);
        }
        )");
    b_quit->setStyleSheet(R"(
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
    QFile loadFile("player.json"); // Filename

    if (!loadFile.open(QIODevice::ReadOnly)) { // Issue warning if file can't be opened
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll(); // Read player.json as bytes
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData)); // Convert bytes to Qt JSON document
    read(loadDoc.object()); // Read the JSON document

    return true;
}

bool Game::saveGame() {
    QFile saveFile("player.json"); // Filename

    if (!saveFile.open(QIODevice::WriteOnly)) { // Issue warning if file can't be opened
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject gameObject = toJson(); // Convert Player object to a JSON object
    saveFile.write(QJsonDocument(gameObject).toJson()); // Write JSON to file player.json
    return true;
}

void Game::open_start() {
    pages->setCurrentIndex(0);
}

void Game::open_create() {
	pages->setCurrentIndex(1);
}

void Game::open_mode() {
    pages->setCurrentIndex(2);
}

void Game::open_care() {
    pages->setCurrentIndex(3);
}

void Game::open_train() {
    pages->setCurrentIndex(4);
}

void Game::open_battle() {
    pages->setCurrentIndex(5);
}
