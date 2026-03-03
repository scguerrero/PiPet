/*
 * Game class specification file. The game is structured as pages that can be navigated via buttons.
 * Author(s): Sasha C. Guerrero
 * Created: 2/9/2026
 * Last Edited: 3/3/2026
 */

#ifndef GAME_H
#define GAME_H
#include <QtWidgets>
#include "../start_class/start.h"
//#include "Create.h"
//#include "Mode.h"
//#include "Care.h"
//#include "Train.h"
//#include "Battle.h"
//#include "Gear.h"

class Game : public QWidget
{
Q_OBJECT
public:
    explicit Game (QWidget *parent = nullptr);
private:
	QVBoxLayout *layout; // This widget needs a top-level layout
	QStackedWidget *pages; // This widget will hold all pages of the game 
	Start *page_start; // Starting page	
signals:
private slots:
};

#endif // GAME_H
