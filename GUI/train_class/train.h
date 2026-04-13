/*
 * Train class specification file.
 * In Train mode, the Player can play mini-games with their pet to increase the pet's attributes.
 *
 * Author(s): Sasha C. Guerrero
 */
#ifndef TRAIN_H
#define TRAIN_H
#include <QtWidgets>
#include "pipatterns.h"

class Train : public QWidget
{
    Q_OBJECT
public:
    explicit Train(QWidget *parent = nullptr);
    QPushButton *b_back;
private:
    // Top-level layout
    QVBoxLayout *main_layout;

    // QStackedWidget contains TrainHub, PiPatterns, and PiDash
    QStackedWidget *stack;
    QWidget *trainHub;
    PiPatterns *pipatterns; // Mini-game 1
    //PiDash *pidash; // Mini-game 2

    // TrainHub widgets ----------------------------------------------
    QVBoxLayout *layout; // Vertically-arrange widgets
    QPushButton *b_minigame1, *b_minigame2; // Buttons leading to each minigame
    QLabel *logo_pipatterns, *logo_pidash; // Logos for each minigame

public slots:
    void setUtilityStyle(QPushButton &button); // Button stylesheet
    void openTrainHub(); // Open Train Hub at index 0 of stack
    void openPiPatterns(); // Open minigame PiPatterns at index 1 of stack
    // Open minigame PiDash
};

#endif // TRAIN_H
