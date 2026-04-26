/*
 * PiPatterns is a minigame that trains the PiPet's intelligence.
 * The player watches a 5-tile sequence light up on a 4x4 matrix, then recreates it from memory.
 * A perfect round awards a lootbox. Score accumulates across rounds within a session.
 *
 * Author: Sasha C. Guerrero
 */

#ifndef PIPATTERNS_H
#define PIPATTERNS_H
#include <QtWidgets>
#include <QTimer>
#include "matrix.h"
#include "../../Player/Player.h"

class PiPatterns : public QWidget {
    Q_OBJECT
public:
    explicit PiPatterns(Player *player, QWidget *parent = nullptr);

    Matrix      *matrix;
    QPushButton *b_back;   // kept public -- train.cc connects to openTrainHub()

signals:
    void gameFinished(int roundScore, int xpEarned, bool perfect);

private slots:
    void startGame();
    void showNextPatternTile();
    void onTileClicked(int index);
    void finishRound();

private:
    enum class Difficulty { Bit, Nibble, Byte };
    enum class GameState  { Idle, Showing, Input, Result };

    Player     *m_player;
    Difficulty  m_difficulty   = Difficulty::Bit;
    GameState   m_state        = GameState::Idle;

    QList<int>  m_pattern;
    int         m_showStep     = 0;
    int         m_playerStep   = 0;
    int         m_score        = 0;   // cumulative session score (display only)
    int         m_roundScore   = 0;   // current round delta emitted via gameFinished
    int         m_roundCorrect = 0;

    QStackedWidget *m_stack;
    QPushButton    *b_play;
    QPushButton    *b_backFromMatrix;

    QLabel      *m_statusLabel;
    QLabel      *m_scoreLabel;
    QPushButton *m_startBtn;
    QPushButton *m_diffBit, *m_diffNibble, *m_diffByte;

    QTimer *m_patternTimer;

    int  pointsPerTile() const;
    void generatePattern();
    void resetMatrix();
    void setDifficulty(Difficulty d);
    void updateDiffButtons();
};

#endif // PIPATTERNS_H
