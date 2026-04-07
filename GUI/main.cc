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
	qputenv("QT_QPA_PLATFORM", "xcb"); // Necessary for Raspberry Pi Zero 2 W compatibility
    QApplication app(argc, argv); // Need a QApplication object to house all game components
    
    // Set colors/font of the game
    app.setStyleSheet(R"(
    QWidget {
        font-size: 18px;
        font-family: monospace;
        color: mistyrose;
        letter-spacing: 1px;
        background-color: #2230BA;
    }

    QPushButton {
        background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #1327BE, stop: 1 #172FE8);
        border: 2px inset #7C89DE;
        border-radius: 10px;
        padding: 4px;
    }

    QPushButton:pressed {
        background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #172FE8, stop: 1 #1327BE);
    }

    QGroupBox {
        border: 2px groove #172FE8;
        margin-top: 30px;
    }

    QGroupBox::title {
        subcontrol-origin: margin;
        subcontrol-position: top center;
        padding: 0 3px;
        background-color: #303FDB;
        border: 2px groove #172FE8;
        border-radius: 10px;
    }

    QRadioButton::indicator:unchecked {
        width: 20px; height: 20px;
        border-radius: 10px;
        border: 2px solid mistyrose;
        padding: 2px
    }

    QRadioButton::indicator:checked {
        width: 20px;
        height 20px;
        border-radius: 10px;
        background-color: mistyrose;
        padding: 2px;
    }

    QListWidget {
        border: 2px solid mistyrose;
        padding: 10px;
    }

    QProgressBar {
        border: 2px solid mistyrose;
        text-align: center;
        font: bold;
        background-color: #899AD1;
    }

    QProgressBar::chunk {
        background-color: #667CC4;
    }

    QScrollBar {
        background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #4850DB, stop: 1 #4A71DB);
        border: 1px solid #FBA8FF;
        width: 20px;
        margin: 22px 0 22px 0;
    }

    QScrollBar::handle {
        /*background: #B11DD6;*/
        background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #B11DD6, stop: 1 #C974D6);
        border: 1px solid #FBA8FF;
        min-height: 20px;
    }

    QScrollBar::add-line {
        border: 2px solid #FBA8FF;
        background: #B11DD6;
        height: 20px;
        subcontrol-position: bottom;
        subcontrol-origin: margin;
    }

    QScrollBar::sub-line {
        border: 2px solid #FBA8FF;
        background: #B11DD6;
        height: 20px;
        subcontrol-position: top;
        subcontrol-origin: margin;
    }

    QScrollBar::up-arrow, QScrollBar::down-arrow {
        border: 2px solid #FBA8FF;
        width: 3px;
        height: 3px;
        background: #FBA8FF;
    }

    QScrollBar::add-page, QScrollBar::sub-page {
        background: none;
    }
    )");

    Game game; // Our first widget is the game window
    game.loadGame(); // Load in previously saved data

    // Enable fullscreen
    game.showFullScreen();

    // For testing only: limit to 480 x 640
    //game.setFixedSize(480,640);

    // Open the game
    game.show();

    return app.exec(); // Open the QApplication
}
