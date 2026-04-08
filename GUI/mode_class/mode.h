/*
 * mode.h - Mode hub screen.
 * Clock moved to About/Settings page.
 * Author(s): Sasha C. Guerrero
 */
#ifndef MODE_H
#define MODE_H
#include <QtWidgets>
#include <QPaintEvent>
#include <QPixmap>
#include "../clock_class/clock.h"
#include "../character_class/character.h"
#include "../../Player/Player.h"

class Mode : public QWidget
{
    Q_OBJECT
public:
    explicit Mode(Player *player, QWidget *parent = nullptr);

    void setPetType(Character::PetType type);
    void refreshDisplay();

    // Navigation buttons
    QPushButton *b_care, *b_train, *b_battle, *b_gear;

    // Bubble labels — public so game.cc can install event filters
    QLabel *feedBubble;
    QLabel *groomBubble;
    QLabel *sleepBubble;

protected:
    void paintEvent (QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

public slots:
    void updateClock();
    void openAbout();

private:
    Player    *player;
    Character *character;
    Character::PetType petType = Character::DragonDog;

    QPixmap m_bg;
    QPixmap m_kitchenPx, m_bathroomPx, m_bedroomPx;

    // Pet name above character
    QLabel *petNameLabel;

    // Anger mark
    QLabel  *angerMark;
    QPixmap  m_angerPx;

    // Settings button top-right
    QPushButton *b_settings;

    // Stat bars
    QGroupBox    *statsBox;
    QGridLayout  *statsGrid;
    QLabel       *hunger_label, *energy_label, *happiness_label;
    QProgressBar *hunger_bar,   *energy_bar,   *happiness_bar;

    // Clock — kept private, used only in About page
    Clock  *timekeeper;
    QTimer *timer;
    QTime  *time;

    // Stat decay
    static constexpr int DECAY_INTERVAL_SECS = 10;
    int secondsSinceDecay = 0;

    void decayStats();
    void updateStatBars();
    void updateIndicators();
    void layoutWidgets();
    void drawBubble(QPainter &p, QRect rect,
                    const QPixmap &bg, const QString &label);
};

#endif
