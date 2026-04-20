/*
 * character.cpp - Character display widget implementation.
 * Author(s): Luke Cerwin Sasha Guerrero
 */
#include "character.h"

Character::Character(QWidget *parent)
    : QWidget{parent}
{
    display = new QLabel(this);
    display->setAlignment(Qt::AlignCenter);
    display->setFixedSize(160, 160);
    display->setScaledContents(true);
    display->setStyleSheet("QLabel { background-color: transparent; }");

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

    // One-shot: resize widget to GIF's actual frame size once first frame is ready
    connect(movie, &QMovie::frameChanged, this, [this, movie](int) {
        QSize gifSize = movie->currentPixmap().size();
        if (gifSize.isValid()) {
            display->setGeometry(0, 0, gifSize.width(), gifSize.height());
            setFixedSize(gifSize);
        }
        disconnect(movie, &QMovie::frameChanged, this, nullptr);        
    });
}
QRect Character::spriteRect() const {
    if (!display || display->pixmap(Qt::ReturnByValue).isNull())
        return rect();

    QSize spriteSize = display->pixmap(Qt::ReturnByValue).size();
    spriteSize.scale(size(), Qt::KeepAspectRatio);

    QPoint topLeft(
        (width()  - spriteSize.width())  / 2,
        (height() - spriteSize.height()) / 2
        );

    return QRect(topLeft, spriteSize);
}
QPoint Character::spriteCenter() const {
    return mapToParent(spriteRect().center());
}


