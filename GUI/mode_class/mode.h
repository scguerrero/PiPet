/*
 * mode.h - Mode hub screen.
 * Shows character GIF, live stat bars, clock, and mode navigation buttons.
 * Stats decay every 10 seconds. Emotion switches automatically from stats.
 *
 * Author(s): Sasha C. Guerrero, [Your name]
 */

#ifndef MODE_H
#define MODE_H
#include <QtWidgets>
#include "../clock_class/clock.h"
#include "../character_class/character.h"
#include "../../Player/Player.h"

class Mode : public QWidget
{
    Q_OBJECT
public:
    explicit Mode(Player *player, QWidget *parent = nullptr);

    // Called from game.cc after player picks species on Create screen
    void setPetType(Character::PetType type);

    // Call whenever Mode becomes visible to refresh stats + GIF
    void refreshDisplay();

    // Navigation buttons
    QPushButton *b_care, *b_train, *b_battle, *b_gear;

    // Clock (kept public to match original interface)
    Clock  *timekeeper;
    QLabel *clock_time;
    QLabel *elapsed_time;
    QTime  *time;
    QTimer *timer;

public slots:
    void updateClock(); // fires every second

private:
    Player    *player;
    Character *character;
    Character::PetType petType = Character::DragonDog;

    QVBoxLayout *layout;

    // Stat bars shown on hub
    QGroupBox    *statsBox;
    QGridLayout  *statsGrid;
    QLabel       *hunger_label, *energy_label, *happiness_label;
    QProgressBar *hunger_bar,   *energy_bar,   *happiness_bar;

    // Stat decay
    static constexpr int DECAY_INTERVAL_SECS = 10;
    int secondsSinceDecay = 0;

    void decayStats();
    void updateStatBars();
};

#endif
