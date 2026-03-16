/*
 * Game class specification file. The game is structured as pages that can be navigated via buttons.
 * Author(s): Sasha C. Guerrero
 * Created: 2/9/2026
 * Last Edited: 3/7/2026
 */

#ifndef GAME_H
#define GAME_H
#include "../start_class/start.h"
#include "../create_class/create.h"
#include "../mode_class/mode.h"
#include "../care_class/care.h"

class Game : public QWidget {
	Q_OBJECT
	public:
        explicit Game (QWidget *parent = nullptr);
	private:
        bool new_game = true;
		QVBoxLayout *layout; // Vertically-arrange widgets inside Game
		QStackedWidget *pages; // This holds each "page" of Game
		Start *start; // page 0: Title page
		Create *create; // page 1: Choose pet's characteristics
        Mode *mode; // page 2: Select a game mode
        Care *care; // page 3: Care for the pet
        /*Train *train; // page 4: Play mini-games to "train" the pet
		Battle *battle; // page 5: Play single- or multi-player battle
		Gear *gear; // page 6: Pet can equip gear that effect stats
		*/
		QPushButton *b_quit; // Close the running application
	private slots:
		void open_create(); // go to page "Create"
        void open_mode(); // go to page "Mode"
        void open_care(); // go to page "Care"
//		void open_train(); // go to page "Train"
//		void open_battle(); // go to page "Battle"
//		void open_gear(); // go to page "Gear"
};

#endif
