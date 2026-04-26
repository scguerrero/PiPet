/*
 * character.h - Character display widget for PiPet.
 * Displays the correct GIF based on pet type, age group, and emotion.
 * Emotion is driven automatically from stat levels.
 *
 * Author(s): Luke Cerwin
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
    QRect spriteRect() const;
    QPoint spriteCenter() const;

public slots:
    void playIdle();
protected:
    void resizeEvent(QResizeEvent *e) override;
private:
    QLabel *display;

    // Only the 4 movies for the currently-loaded pet type are alive at once.
    QMovie *m_idle_baby  = nullptr;
    QMovie *m_idle_teen  = nullptr;
    QMovie *m_idle_adult = nullptr;
    QMovie *m_sleepy     = nullptr;
    PetType m_loadedType = (PetType)-1;

    PetType currentType    = DragonDog;
    Emotion currentEmotion = Normal;
    QString currentStage   = "Baby";

    void    loadMoviesForType(PetType type);
    void    unloadMovies();
    QMovie *currentMovie();
    void    switchTo(QMovie *movie);
};

#endif // CHARACTER_H
