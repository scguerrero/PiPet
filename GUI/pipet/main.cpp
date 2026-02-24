/*
 * Main file for Qt application.
 * Author(s): Sasha C. Guerrero
 * Created: 2/9/2026
 * Last Edited: 2/18/2026
 */

#include "window.h"
#include <QApplication>

int main(int argc, char**argv)
{
    // Initialize application and window
    QApplication app(argc, argv);

    // Set style properties
    app.setStyleSheet("QWidget { font-size: 18px; font-family: monospace; background: darkseagreen; color: darkslategreen; letter-spacing: 1px }");

    // Initialize main window
    Window window;

    // Fullscreen necessary for RaspPi
    window.showFullScreen();

    // Non-fullscreen mode for testing
    //window.show();
    //window.setFixedSize(480,640);

    return app.exec(); // launches event loop
}
