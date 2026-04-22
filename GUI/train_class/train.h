/*
 * Train class specification file.
 * In Train mode, the Player can play mini-games with their pet to increase the pet's attributes.
 *
 * Author(s): Sasha C. Guerrero, Tanya Magurupira
 */
#ifndef TRAIN_H
#define TRAIN_H
#include <QtWidgets>
#include "pipatterns.h"
#include "../../Pet/PiPet.h"

class Train : public QWidget
{
    Q_OBJECT
public:
    explicit Train(PiPet* pet, QWidget *parent = nullptr);
    QPushButton *b_back;
private:
    QVBoxLayout *main_layout;
    PiPet* m_pet;

    // QStackedWidget contains TrainHub, PiPatterns, PiDash, and PiCatcher
    QStackedWidget *stack;
    QWidget *trainHub;
    PiPatterns *pipatterns; // Mini-game 1

    // TrainHub widgets ----------------------------------------------
    QVBoxLayout *layout; // Vertically-arrange widgets
    QPushButton *b_minigame1, *b_minigame2, *b_minigame3; // Buttons leading to each minigame
    QLabel *logo_pipatterns, *logo_pidash, *logo_picatcher; // Logos for each minigame

public slots:
    void setUtilityStyle(QPushButton &button); // Button stylesheet
    void openTrainHub(); // Open Train Hub at index 0 of stack
    void openPiPatterns(); // Open minigame PiPatterns at index 1 of stack
    void openPiDash(); // open minigame PiDash at index 2 of stock
    void openPiCatcher(); // open minigame PiCatcher at index 3 of stock
    void onTrackRushFinished(int finalScore, int xpEarned); //
    void onSkySnackFinished(int finalScore, int xpEarned);

};

#endif // TRAIN_H
