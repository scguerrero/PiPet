/*
 * Main file for the PIPET game.
 * Author(s): Sasha C. Guerrero
 * Created: 2/9/2026
 * Last Edited: 3/11/2026
 */

#include <QtWidgets>
#include <QStringList>
#include "game_class/game.h"
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char**argv)
{
    QCoreApplication::setLibraryPaths({"C:/Qt/6.11.0/mingw_64/plugins"});
    //qputenv("QT_QPA_PLATFORM", "xcb"); // Necessary for Raspberry Pi Zero 2 W compatibility
    QApplication app(argc, argv); // Need a QApplication object to house all game components
    
    app.setStyleSheet( // Set colors/font of the game
        R"(QWidget { font-size: 20px; font-family: monospace; background: navy; color: mistyrose; letter-spacing: 1px }

        QGroupBox { border: 2px solid mistyrose; border-radius: 10px; margin-top: 35px }
        
        QGroupBox::title { border: 2px solid mistyrose; border-radius: 3px; background-color: blue;
            padding: 2px; subcontrol-origin: margin; subcontrol-position: top center; }

        QRadioButton::indicator:unchecked { width: 20px; height: 20px; border-radius: 10px;
            border: 2px solid mistyrose; padding: 2px }
        
        QRadioButton::indicator:checked { width: 20px; height 20px; border-radius: 10px;
            background-color: mistyrose; padding: 2px }

        QListWidget { border: 2px solid mistyrose; border-radius: 10px; margin: 10px }

        QProgressBar { background: mistyrose }
        )"
	);

    Game game; // Our first widget is the game window
    game.showFullScreen();

    return app.exec(); // Open the QApplication
}
