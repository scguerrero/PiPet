/*
 * character.h - Character display widget for PiPet.
 * Displays the correct GIF based on pet type, age group, and emotion.
 * Emotion is driven automatically from stat levels.
 *
 * Author(s): [Your name]
 */

#ifndef CHARACTER_H
#define CHARACTER_H

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include "../../Player/Player.h"

class Character : public QWidget
{
    Q_OBJECT

public:
    enum PetType   { DragonDog, ElectricAxolotl, SeelCat };
    enum Emotion   { Normal, Sleepy };

    explicit Character(QWidget *parent = nullptr);

    void setPetType(PetType type);
    void setStageFromString(const QString &age_group); // "Baby","Teen","Adult"
    void updateEmotionFromStats(int energyLevel, int hungerLevel);
    void syncWithPlayer(const Player &player, PetType type);

public slots:
    void playIdle();

private:
    QLabel *display;

    // DragonDog GIFs
    QMovie *dd_baby_normal;
    QMovie *dd_teen_normal;
    QMovie *dd_adult_normal;
    QMovie *dd_sleepy;

    // ElectricAxolotl GIFs
    QMovie *ax_baby_normal;
    QMovie *ax_teen_normal;
    QMovie *ax_adult_normal;
    QMovie *ax_sleepy;

    // SeelCat GIFs
    QMovie *sc_baby_normal;
    QMovie *sc_teen_normal;
    QMovie *sc_adult_normal;
    QMovie *sc_sleepy;

    PetType currentType    = DragonDog;
    Emotion currentEmotion = Normal;
    QString currentStage   = "Baby";

    QMovie *currentMovie();
    void    switchTo(QMovie *movie);
};

#endif // CHARACTER_H
