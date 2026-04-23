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
#include "guesser.h"
#include "../../Pet/PiPet.h"
#include "../../Player/Player.h"

class Train : public QWidget
{
    Q_OBJECT
public:
    // Player* is required so mindReader (and any future minigame) can read
    // live stat and hat data.  PiPet* is kept for minigames that still use
    // it directly (PiDash etc.).
    explicit Train(PiPet* pet, Player* player, QWidget *parent = nullptr);
    QPushButton *b_back;

private:
    QVBoxLayout *main_layout;
    PiPet*  m_pet;
    Player* m_player;

    // QStackedWidget contains TrainHub, PiPatterns, PiDash, and mindReader
    QStackedWidget *stack;
    QWidget    *trainHub;
    PiPatterns *pipatterns;             // Mini-game 1
    mindReader *m_mindReader = nullptr; // Mini-game 3 (lazy — created on first open)

    // TrainHub widgets ----------------------------------------------
    QVBoxLayout *layout;
    QPushButton *b_minigame1, *b_minigame2, *b_minigame3;
    QLabel *logo_pipatterns, *logo_pidash, *logo_mindReader;

public slots:
    void setUtilityStyle(QPushButton &button);
    void openTrainHub();
    void openPiPatterns();
    void openPiDash();
    void openmindReader();
    void onTrackRushFinished(int finalScore, int xpEarned);
    void onMindReaderFinished(int finalScore, int xpEarned);
};

#endif // TRAIN_H
