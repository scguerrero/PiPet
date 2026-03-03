/*
 * Main file for the PIPET game.
 * Author(s): Sasha C. Guerrero
 * Created: 2/9/2026
 * Last Edited: 3/3/2026
 */

#include "game_class/game.h"
#include <QApplication>
#include <string>

int main(int argc, char**argv)
{
    QApplication app(argc, argv); // Need a QApplication object to house all game components
    
    app.setStyleSheet( // Set colors/font of the game
		R"(QWidget { font-size: 18px; font-family: monospace; background: darkseagreen;
		color: darkslategreen; letter-spacing: 1px })"
	);

    Game game; // Our first widget is the game window
    game.showFullScreen();

    return app.exec(); // Open the QApplication
}
