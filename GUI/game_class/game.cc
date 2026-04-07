/*
 * The PiPet program is structured as a stack of pages that can be navigated via buttons.
 *
 * Saving/loading data functions are based on this example from the Qt Documentation:
 * https://doc.qt.io/qt-6/qtcore-serialization-savegame-example.html
 *
 * Author(s): Sasha C. Guerrero
 */
#include "game.h"

Game::Game(QWidget *parent) : QWidget{parent} {
    // Set title of running program
    this->setWindowTitle("PIPET");

    // Set margin size between the edges of the screen and the Qt widgets
    this->setContentsMargins(15,15,15,15);

    // Game has PiPet and Player objects
    pet = new PiPet();
    player = new Player(*pet);

    // Vertically-arrange widgets inside Game
    layout = new QVBoxLayout();
    this->setLayout(layout);

    // Game is structured as a "stack" of "pages" where each page is a different gamemode
	pages = new QStackedWidget();
    layout->addWidget(pages);

    // The different gamemodes, or pages of the game
	start = new Start();
	create = new Create();
    mode = new Mode();
    care = new Care(player);
    train = new Train();
    battle = new Battle();
    //gear = new Gear();

    // Add each gamemode to the pages widget
    pages->addWidget(start);// page 0
    pages->addWidget(create); // page 1
    pages->addWidget(mode); // page 2
    pages->addWidget(care); // page 3
    pages->addWidget(train); // page 4
    pages->addWidget(battle); // page 5
    //pages->addWidget(gear); // page 6

    // Utility bar widgets
    utility_bar = new QHBoxLayout();
    b_save = new QPushButton("SAVE");
    b_home = new QPushButton("HOME");
	b_quit = new QPushButton("QUIT");

    // Utility bar icons
    QIcon save_icon(":/images/Assets/save.png");
    QIcon home_icon(":/images/Assets/home.png");
    QIcon quit_icon(":/images/Assets/quit.png");
    b_save->setIcon(save_icon);
    b_home->setIcon(home_icon);
    b_quit->setIcon(quit_icon);

    // Arrange buttons inside utility bar
    utility_bar->addWidget(b_save);
    utility_bar->addWidget(b_home);
    utility_bar->addWidget(b_quit);
    layout->addLayout(utility_bar);

    // For a new game, the first step is to create a pet
    if (new_game) {
		connect(start->b_start, SIGNAL( clicked() ), this, SLOT( open_create() ));
    // When loading a previous game, send them to the mode-selection page
    } else {
        connect(start->b_start, SIGNAL( clicked() ), this, SLOT( open_mode() ));
    }

    // Utility bar functionality
    connect(b_save, SIGNAL(clicked()), this, SLOT(saveGame())); // The Save button writes current data to a JSON file
    connect(b_home, SIGNAL( clicked() ), this, SLOT( open_start() )); // The Home button goes back to the Start page
    connect(b_quit, SIGNAL( clicked() ), QApplication::instance(), SLOT( quit() )); // The Quit button terminates running program

    // Navigation from Mode to Create
    connect(create->b_done, SIGNAL( clicked() ), this, SLOT( open_mode() ));

    // Navigation from Mode to Care, Care to Mode
    connect(mode->b_care, SIGNAL( clicked() ), this, SLOT( open_care() ));
    connect(care->b_back, SIGNAL( clicked() ), this, SLOT( open_mode() ));

    // Navigation from Mode to Train, Train to Mode
    connect(mode->b_train, SIGNAL( clicked() ), this, SLOT( open_train() ));
    connect(train->b_back, SIGNAL( clicked() ), this, SLOT( open_mode() ));

    // Navigation from Mode to Battle, Battle to Mode
    connect(mode->b_battle, SIGNAL( clicked() ), this, SLOT( open_battle() ));
    connect(battle->btnBack, SIGNAL( clicked() ), this, SLOT( open_mode() ));

    // The utility bar buttons are a different color to distinguish them as important
    setUtilityStyle(*b_save);
    setUtilityStyle(*b_home);
    setUtilityStyle(*b_quit);
}

// Write data from Player object to a JSON object
// Saving/loading data based on example from Qt Documentation: https://doc.qt.io/qt-6/qtcore-serialization-savegame-example.html
QJsonObject Game::toJson() const {
    QJsonObject json;
    json["Player"] = player->toJson();
    return json;
}

// Read data from JSON object to create a Player object
// Saving/loading data based on example from Qt Documentation: https://doc.qt.io/qt-6/qtcore-serialization-savegame-example.html
void Game::read(const QJsonObject &json) {
    if (const QJsonValue v = json["Player"]; v.isObject())
        *player = Player::fromJSON(v.toObject());
}

// Read data from JSON file "player.json"
// Saving/loading data based on example from Qt Documentation: https://doc.qt.io/qt-6/qtcore-serialization-savegame-example.html
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

// Write data to JSON file "player.json"
// Saving/loading data based on example from Qt Documentation: https://doc.qt.io/qt-6/qtcore-serialization-savegame-example.html
bool Game::saveGame() {
    QFile saveFile("player.json"); // Filename

    if (!saveFile.open(QIODevice::WriteOnly)) { // Issue warning if file can't be opened
        qWarning("Couldn't open save file.");
        return false;
    }

    QMessageBox msg(this);
    msg.setText("Save successful!");
    msg.exec();

    QJsonObject gameObject = toJson(); // Convert Player object to a JSON object
    saveFile.write(QJsonDocument(gameObject).toJson()); // Write JSON to file player.json
    return true;
}

// The "Start" page is at index 0
void Game::open_start() {
    pages->setCurrentIndex(0);
}

// The "Create" page is at index 1
void Game::open_create() {
	pages->setCurrentIndex(1);
}

// The "Mode" page is at index 2
void Game::open_mode() {
    pages->setCurrentIndex(2);
}

// The "Care" page is at index 3
void Game::open_care() {
    pages->setCurrentIndex(3);
}

// The "Train" page is at index 4
void Game::open_train() {
    pages->setCurrentIndex(4);
}

// The "Battle" page is at index 5
void Game::open_battle() {
    pages->setCurrentIndex(5);
}

// Assign special stylesheet to utility bar buttons to distinguish them as important
void Game::setUtilityStyle(QPushButton &button) {
    button.setStyleSheet(R"(
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
