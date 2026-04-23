/*
 * mindReader.h - mindReader minigame widget.
 *
 * Tamagotchi-style number guessing game: the pet "thinks" of a number 1–3,
 * and the player must guess which one. Stats and sprite are pulled live from
 * the Player/PiPet so they always match the Mode screen.
 *
 * Signals:
 *   gameFinished(int finalScore, int xpEarned)  — emitted when the session ends.
 *
 * Integration with Train:
 *   Replace every reference to piCatcher / PiCatcher with mindReader / mindReader
 *   in train.h and train.cc (see comments there).  The slot
 *   Train::onSkySnackFinished() becomes Train::onmindReaderFinished() and is
 *   already compatible with the same (finalScore, xpEarned) signature.
 *
 * Author(s): <your name here>
 */

#ifndef GUESSER_H
#define GUESSER_H

#include <QtWidgets>
#include "../character_class/character.h"
#include "../../Player/Player.h"

// ─────────────────────────────────────────────────────────────────────────────
//  mindReader
// ─────────────────────────────────────────────────────────────────────────────
class mindReader : public QWidget
{
    Q_OBJECT

public:
    // Construct with a live Player pointer (same pattern as Feed/Groom/Sleep).
    // petType is forwarded to the embedded Character widget so the correct
    // sprite + hat appear automatically.
    explicit mindReader(Player *player,
                       Character::PetType petType = Character::DragonDog,
                       QWidget *parent = nullptr);

    // Back button is public so Train can wire it to openTrainHub(), exactly as
    // pipatterns->b_back is wired today.
    QPushButton *b_back;

    // Call this whenever the player equips a new hat or the pet ages up so the
    // embedded sprite stays in sync with the Mode screen.
    void refreshCharacter();

signals:
    // Emitted once per session (when the player clicks "Done" or runs out of
    // guesses).  Connect to Train::onmindReaderFinished(int,int).
    void gameFinished(int finalScore, int xpEarned);

private slots:
    void onGuess(int guess);   // called by the three number buttons
    void startNewRound();      // pick a new secret number, reset per-round UI
    void endSession();         // tally score, emit gameFinished, return to hub

private:
    // ── Data ─────────────────────────────────────────────────────────────────
    Player              *m_player;
    Character::PetType   m_petType;

    int  m_secretNumber   = 1;   // 1, 2, or 3 — randomised each round
    int  m_score          = 0;   // correct guesses this session
    int  m_roundsPlayed   = 0;   // total guesses so far
    int  m_streak         = 0;   // current consecutive-correct streak
    static constexpr int kMaxRounds = 10;  // session length

    // ── Widgets ──────────────────────────────────────────────────────────────
    Character   *m_character;    // live pet sprite (with hat)

    // Stats panel — mirrors the "Condition" GroupBox in Mode
    QGroupBox    *m_statsBox;
    QGridLayout  *m_statsGrid;
    QLabel       *m_hungerLabel, *m_energyLabel, *m_happinessLabel;
    QProgressBar *m_hungerBar,   *m_energyBar,   *m_happinessBar;

    // Info panel (score, streak, rounds left)
    QGroupBox *m_infoBox;
    QLabel    *m_scoreLabel;
    QLabel    *m_streakLabel;
    QLabel    *m_roundsLabel;

    // Feedback area
    QLabel *m_feedbackLabel;
    QLabel *m_thinkingLabel;   // "Your pet is thinking of a number…"

    // Guess buttons
    QPushButton *m_btn1, *m_btn2, *m_btn3;
    QWidget     *m_btnRow;

    // Session-end panel (shown after kMaxRounds)
    QWidget     *m_resultPanel;
    QLabel      *m_resultLabel;
    QPushButton *m_playAgainBtn;

    // ── Helpers ──────────────────────────────────────────────────────────────
    void buildUi();
    void applyButtonStyle(QPushButton *btn, const QString &extraColor = "");
    void applyUtilityStyle(QPushButton *btn);
    void updateStatBars();
    void updateInfoPanel();
    void setGuessButtonsEnabled(bool enabled);
    void showResultPanel(bool visible);

    // Hat-aware sprite refresh — same logic as Mode::refreshDisplay()
    QMovie  *m_cachedHatMovie = nullptr;
    QString  m_cachedHatPath  = "";
    void     applyHatSprite();
};

#endif // mindReader_H
