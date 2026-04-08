/*
 * character.cpp - Character display widget implementation.
 * Author(s): [Your name]
 */

#include "character.h"

Character::Character(QWidget *parent)
    : QWidget{parent}
{
    display = new QLabel(this);
    display->setGeometry(0, 0, 200, 200);
    display->setScaledContents(true);
    display->setAlignment(Qt::AlignCenter);

    // DragonDog
    dd_baby_normal  = new QMovie(":/images/Sprites/pets/dragondog/dragondog_idle.gif",       QByteArray(), this);
    dd_teen_normal  = new QMovie(":/images/Sprites/pets/dragondog/purpledragondog_idle.gif",  QByteArray(), this);
    dd_adult_normal = new QMovie(":/images/Sprites/pets/dragondog/dragondog_adult_idle.gif",  QByteArray(), this);
    dd_sleepy       = new QMovie(":/images/Sprites/pets/dragondog/purpledragondog_sleep.gif", QByteArray(), this);

    // ElectricAxolotl
    ax_baby_normal  = new QMovie(":/images/Sprites/pets/axolotl/axolotl_idle.gif",           QByteArray(), this);
    ax_teen_normal  = new QMovie(":/images/Sprites/pets/axolotl/axolotl_teen_idle.gif",      QByteArray(), this);
    ax_adult_normal = new QMovie(":/images/Sprites/pets/axolotl/axolotl_adult_idle.gif",     QByteArray(), this);
    ax_sleepy       = new QMovie(":/images/Sprites/pets/axolotl/electric_axolotl_sleep.gif", QByteArray(), this);

    // SeelCat
    sc_baby_normal  = new QMovie(":/images/Sprites/pets/seelcat/seelcat_idle.gif",           QByteArray(), this);
    sc_teen_normal  = new QMovie(":/images/Sprites/pets/seelcat/seelcat_teen_idle.gif",      QByteArray(), this);
    sc_adult_normal = new QMovie(":/images/Sprites/pets/seelcat/seelcat_adult_idle.gif",     QByteArray(), this);
    sc_sleepy       = new QMovie(":/images/Sprites/pets/seelcat/seelcat_sleep.gif",          QByteArray(), this);

    playIdle();
}

void Character::setPetType(PetType type) {
    currentType = type;
    playIdle();
}

void Character::setStageFromString(const QString &age_group) {
    currentStage = age_group;
    playIdle();
}

void Character::updateEmotionFromStats(int energyLevel, int hungerLevel) {
    Q_UNUSED(hungerLevel);
    currentEmotion = (energyLevel < 30) ? Sleepy : Normal;
    playIdle();
}

void Character::syncWithPlayer(const Player &player, PetType type) {
    currentType = type;
    setStageFromString(player.getPet().age_group());
    updateEmotionFromStats(player.getPet().energy(), player.getPet().hunger());
}

void Character::playIdle() {
    switchTo(currentMovie());
}

QMovie *Character::currentMovie() {
    if (currentEmotion == Sleepy) {
        if (currentType == DragonDog)       return dd_sleepy;
        if (currentType == ElectricAxolotl) return ax_sleepy;
        return sc_sleepy;
    }
    if (currentType == DragonDog) {
        if (currentStage == "Teen")  return dd_teen_normal;
        if (currentStage == "Adult") return dd_adult_normal;
        return dd_baby_normal;
    }
    if (currentType == ElectricAxolotl) {
        if (currentStage == "Teen")  return ax_teen_normal;
        if (currentStage == "Adult") return ax_adult_normal;
        return ax_baby_normal;
    }
    // SeelCat
    if (currentStage == "Teen")  return sc_teen_normal;
    if (currentStage == "Adult") return sc_adult_normal;
    return sc_baby_normal;
}

void Character::switchTo(QMovie *movie) {
    if (!movie) return;
    if (display->movie() == movie) return;
    if (display->movie()) display->movie()->stop();
    display->setMovie(movie);
    movie->start();
}
