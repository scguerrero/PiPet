/*
 * Mode class specification file.
 * From here the Player can select which game mode they want to play:
 *     Care mode for feeding and grooming the pet,
 *     Train mode for choosing a mini-game,
 *     Battle mode for playing single-player games versus a bot opponent, and
 *     Gear mode for choosing equipment/accessories for the pet.
 *
 * Author(s): Sasha C. Guerrero
 */

#include "mode.h"

Mode::Mode(QWidget *parent) : QWidget{parent} {
    // Vertically-arrange widgets inside Mode
    layout = new QVBoxLayout();
    this->setLayout(layout);

    // Timekeeper tracks time elapsed in seconds
    timekeeper = new Clock();

    // Show current time and elapsed time of the game session
    clock_time = new QLabel(); // Display system clock time
    elapsed_time = new QLabel(); // Display seconds elapsed during game session

    // Add time labels to layout
    layout->addWidget(clock_time);
    layout->addWidget(elapsed_time);

    // Time utilities
    time = new QTime(); // Get system clock time
    timer = new QTimer(); // Count time in milliseconds

    // Set initial values for system clock and elapsed time
    clock_time->setText(time->currentTime().toString("hh:mm:ss AP")); // hour-minute-second AM, PM format
    elapsed_time->setText( QString::number(timekeeper->elapsed_time()) );

    // Update clock_time and elapsed_time every 1 second
    connect(timer, SIGNAL( timeout() ), this, SLOT( updateClock() ));
    timer->start(1000); // 1000ms = 1s intervals

    // Buttons lead to their corresponding gamemode
    b_care = new QPushButton("Care");
    b_train = new QPushButton("Train");
    b_battle = new QPushButton("Battle");
    b_gear = new QPushButton("Gear");

    // Add buttons to layout
    layout->addWidget(b_care);
    layout->addWidget(b_train);
    layout->addWidget(b_battle);
    layout->addWidget(b_gear);
}

void Mode::updateClock() {
    // Update current time on clock
    clock_time->setText( time->currentTime().toString("hh:mm:ss AP") );

    // Update elapsed time in seconds
    timekeeper->increment_elapsed_time(1);
    elapsed_time->setText( QString::number(timekeeper->elapsed_time()) );
}
