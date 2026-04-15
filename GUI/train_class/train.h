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
#include "minigame_2.h"
#include "minigame_3.h"


class Train : public QWidget
{
    Q_OBJECT
public:
    explicit Train(PiPet* pet, QWidget *parent = nullptr);
    QPushButton *b_back;

private slots:
    void openMenuPage(); // Page 0 is the Menu Page
    void openPiPatterns(); // Page 1 is PiPatterns

    void onMiniGame2(); // triggered by the Minigame 2 button
    void onTrackRushFinished(int finalScole, int xpEarned);
    void onMiniGame3();                                      // ← NEW
    void onSkySnackFinished(int finalScore, int xpEarned);   // ← NEW

private:
    PiPet* m_pet = nullptr;

    QStackedWidget *m_stack; // page 0 = menu, page 1 = minigame
    QWidget  *m_menuPage; // holds the button layout

    QVBoxLayout *layout; // buttons leading to each mini-game will go in this layout
    QPushButton *b_minigame1, *b_minigame2, *b_minigame3;

    PiPatterns *pipatterns;// minigame widget 1
    minigame_2* m_trackRush = nullptr; // the game widget
    minigame_3* m_skySnack  = nullptr;  // ← NEW
};

#endif // TRAIN_H
