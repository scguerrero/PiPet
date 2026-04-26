/*
 * Main hub for minigames. Includes implementations from tanya and sasha
 *
 * Author(s): Luke Cerwin & Sasha C. Guerrero
 */
#ifndef TRAIN_H
#define TRAIN_H
#include <QtWidgets>
#include "pipatterns.h"
#include "mindReader.h"
#include "pidash.h"
#include "../../Pet/PiPet.h"
#include "../../Player/Player.h"

class Train : public QWidget
{
    Q_OBJECT
public:
    explicit Train(PiPet* pet, Player* player, QWidget *parent = nullptr);
    QPushButton *b_back;

signals:
    void hatUnlocked(const QString &hatKey);
    void lootboxEarned(const QString &source);

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    QPixmap m_bg;
    QVBoxLayout *main_layout;
    PiPet*  m_pet;
    Player* m_player;

    QStackedWidget *stack;
    QWidget    *trainHub;
    PiPatterns *pipatterns   = nullptr;
    piDash     *m_trackRush  = nullptr;
    mindReader *m_mindReader = nullptr;

    QVBoxLayout *layout;
    QPushButton *b_minigame1, *b_minigame2, *b_minigame3;
    QLabel *logo_pipatterns, *logo_pidash, *logo_mindReader;

public slots:
    void setUtilityStyle(QPushButton &button);
    void openTrainHub();
    void openPiPatterns();
    void openPiDash();
    void openmindReader();
    void onPiPatternsFinished(int finalScore, int xpEarned, bool perfect);
    void onTrackRushFinished(int finalScore, int xpEarned);
    void onMindReaderFinished(int finalScore, int xpEarned); // FIX: removed stray period
    void refreshMindReader();
    void tryAwardLootbox(int score, int xpEarned, const QString &source);
};

#endif // TRAIN_H
