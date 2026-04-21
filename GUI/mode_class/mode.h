/*
 * mode.h - Mode hub screen.
 * Author(s): Sasha C. Guerrero, Luke Cerwin
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

    QPushButton *b_care, *b_train, *b_battle, *b_gear;
    QLabel *feedBubble, *groomBubble, *sleepBubble;

signals:
    void petAgedUp(const QString &newAgeGroup); // game.cc → onAgeChanged()
    void temperTantrum();                        // game.cc → achievements check

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

    QPixmap m_bg, m_kitchenPx, m_bathroomPx, m_bedroomPx;

    QLabel      *petNameLabel, *angerMark;
    QPixmap      m_angerPx;
    QPushButton *b_settings;

    QGroupBox    *statsBox;
    QGridLayout  *statsGrid;
    QLabel       *hunger_label, *energy_label, *happiness_label;
    QProgressBar *hunger_bar,   *energy_bar,   *happiness_bar;

    Clock  *timekeeper;
    QTimer *timer;
    QTime  *time;

    static constexpr int DECAY_INTERVAL_SECS = 10;
    int     secondsSinceDecay = 0;
    QString m_lastAgeGroup    = "Baby";

    void decayStats();
    void updateStatBars();
    void updateIndicators();
    void layoutWidgets();
    void drawBubble(QPainter &p, QRect rect,
                    const QPixmap &bg, const QString &label, bool dimmed);
};

#endif
