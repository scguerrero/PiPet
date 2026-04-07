/*
 * The PiPet program is structured as a stack of pages that can be navigated via buttons.
 *
 * Saving/loading data functions are based on this example from the Qt Documentation:
 * https://doc.qt.io/qt-6/qtcore-serialization-savegame-example.html
 *
 * Author(s): Sasha C. Guerrero
 */

#ifndef GAME_H
#define GAME_H
#include "../start_class/start.h"
#include "../create_class/create.h"
#include "../mode_class/mode.h"
#include "../care_class/care.h"
#include "../train_class/train.h"
#include "../battle_class/battle.h"

class Game : public QWidget {
	Q_OBJECT
	public:
        // Constructor
        explicit Game (QWidget *parent = nullptr);

        // Saving/loading data based on example from Qt Documentation: https://doc.qt.io/qt-6/qtcore-serialization-savegame-example.html
        QJsonObject toJson() const; // Convert PiPet and Player to JSON
        void read(const QJsonObject &json); // Read and load in JSON data

        // Game has PiPet and Player objects
        PiPet *pet;
        Player *player;

    public slots:
        // Saving/loading data based on example from Qt Documentation: https://doc.qt.io/qt-6/qtcore-serialization-savegame-example.html
        bool loadGame(); // Read data from JSON file
        bool saveGame(); // Write data to JSON file

        // Assign special stylesheet to utility bar buttons to distinguish them as important
        void setUtilityStyle(QPushButton &button);

    private:
        // Flag indicating if we need to create a new savefile
        bool new_game = true;

        // Highest level layout
        QVBoxLayout *top_layout;

        // Vertically-arrange widgets inside Game
        QVBoxLayout *layout;

        // Game is structured as a "stack" of "pages" where each page is a different gamemode
        QStackedWidget *pages;

        // The different gamemodes, or pages of the game
		Start *start; // page 0: Title page
		Create *create; // page 1: Choose pet's characteristics
        Mode *mode; // page 2: Select a game mode
        Care *care; // page 3: Care for the pet
        Train *train; // page 4: Play mini-games to "train" the pet
        Battle *battle; // page 5: Play single- or multi-player battle
        //Gear *gear; // page 6: Pet can equip gear that effect stats

        // Horizontal utility bar at bottom of screen
        QHBoxLayout *utility_bar; // Horizontal row of buttons
        QPushButton *b_save; // Save the game
        QPushButton *b_home; // Go back to "Start" page
		QPushButton *b_quit; // Close the running application

	private slots:
        // Page navigation is done by changing which page index we are at
        void open_start(); // go to page "Start"
		void open_create(); // go to page "Create"
        void open_mode(); // go to page "Mode"
        void open_care(); // go to page "Care"
        void open_train(); // go to page "Train"
        void open_battle(); // go to page "Battle"
        //void open_gear(); // go to page "Gear"
};

#endif
