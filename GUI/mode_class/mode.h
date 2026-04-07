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

#ifndef MODE_H
#define MODE_H
#include <QtWidgets>
#include "../clock_class/clock.h"

class Mode : public QWidget
{
    Q_OBJECT
    public:
        // Constructor
        explicit Mode(QWidget *parent = nullptr);

        // Timekeeper tracks time elapsed in seconds
        Clock *timekeeper;

        // Show current time and elapsed time of the game session
        QLabel *clock_time; // Display system clock time
        QLabel *elapsed_time; // Display seconds elapsed during game session

        // Time utilities
        QTime *time; // Get system clock time
        QTimer *timer; // Count time in milliseconds

        // Buttons for Care, Train, Battle, and Gear gamemodes
        QPushButton *b_care, *b_train, *b_battle, *b_gear;

    public slots:
        // Update current clock time and time elapsed every 1 second
        void updateClock();

    private:
        // Vertically-arrange widgets inside Mode
        QVBoxLayout *layout;


};

#endif
