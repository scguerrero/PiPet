/*
 * mindReader.h - mindReader minigame widget.
 *
 * Author(s): Luke Cerwin
 */

#ifndef GUESSER_H
#define GUESSER_H

#include <QtWidgets>
#include "../character_class/character.h"
#include "../../Player/Player.h"

//mindReader
class mindReader : public QWidget
{
    Q_OBJECT

public:
    explicit mindReader(Player *player,
                       Character::PetType petType = Character::DragonDog,
                       QWidget *parent = nullptr);
    QPushButton *b_back;
    void refreshCharacter();
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
signals:
    void gameFinished(int finalScore, int xpEarned);

private slots:
    void onGuess(int guess);
    void startNewRound();      // pick a new secret number
    void endSession();         // tally score

private:
    Player              *m_player;
    Character::PetType   m_petType;

    int  m_secretNumber   = 1;   // 1, 2, or 3 — randomised each round
    int  m_score          = 0;
    int  m_roundsPlayed   = 0;
    int  m_streak         = 0;
    bool m_sessionFinished = false;
    static constexpr int kMaxRounds = 10;  // session length
    QPixmap             m_bg;

    //Widgets
    Character   *m_character;
    QMovie      *m_hatMovie = nullptr;

    // Info panel
    QGroupBox *m_infoBox;
    QLabel    *m_scoreLabel;
    QLabel    *m_streakLabel;
    QLabel    *m_roundsLabel;

    // Feedback area
    QLabel *m_feedbackLabel;
    QLabel *m_thinkingLabel;

    // Guess buttons
    QPushButton *m_btn1, *m_btn2, *m_btn3;
    QWidget     *m_btnRow;

    QWidget     *m_resultPanel;
    QLabel      *m_resultLabel;
    QPushButton *m_playAgainBtn;

    void buildUi();
    void applyButtonStyle(QPushButton *btn, const QString &extraColor = "");
    void applyUtilityStyle(QPushButton *btn);
    void updateInfoPanel();
    void setGuessButtonsEnabled(bool enabled);
    void showResultPanel(bool visible);
    void applyHatSprite();
};

#endif // mindReader_H
