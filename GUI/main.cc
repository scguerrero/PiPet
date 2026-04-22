/*
 * main.cc - Main file for the PIPET game.
 * Author(s): Luke Cewin & Sasha Guerrero
 */

#include <QtWidgets>
#include <QStringList>
#include "game_class/game.h"
#include <QApplication>
#include <QCoreApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QEventLoop>
#include <QPropertyAnimation>

int main(int argc, char**argv)
{
    // Correct Qt6 attribute name for touch-to-mouse synthesis
    QApplication::setAttribute(Qt::AA_SynthesizeMouseForUnhandledTouchEvents, false);

    qputenv("QT_QPA_PLATFORM", "xcb");
    QApplication app(argc, argv);

    app.setStyleSheet(R"(
    QWidget {
        font-size: 18px;
        font-family: monospace;
        color: mistyrose;
        letter-spacing: 1px;
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
        width: 20px; height: 20px;
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
        background-color: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #4850DB,stop:1 #4A71DB);
        border: 1px solid #FBA8FF;
        width: 20px;
        margin: 22px 0 22px 0;
    }
    QScrollBar::handle {
        background: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #B11DD6,stop:1 #C974D6);
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
        width: 3px; height: 3px;
        background: #FBA8FF;
    }
    QScrollBar::add-page, QScrollBar::sub-page {
        background: none;
    }
    )");
    // ── Boot splash ──────────────────────────────────────────────────────────
    QWidget splash;
    splash.setFixedSize(480, 640);
    splash.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    QLabel *splashImg = new QLabel(&splash);
    QPixmap splashPx(":/images/Backgrounds/bootscreen.png");
    splashImg->setPixmap(splashPx.scaled(480, 640, Qt::KeepAspectRatio,
                                         Qt::SmoothTransformation));
    splashImg->setGeometry(0, 0, 480, 640);
    splash.show();
    app.processEvents();

    // Load and show the game window while splash is still visible
    Game game;
    game.loadGame();
    game.setFixedSize(480, 640);
    game.show();
    app.processEvents();

    // Hold for 3 seconds then just close — game is already underneath
    QEventLoop loop;
    QTimer::singleShot(3000, &loop, &QEventLoop::quit);
    loop.exec();
    splash.close();

    return app.exec();
}
