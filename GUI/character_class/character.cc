/*
 * character.cc - Character display widget implementation.
 * Author(s): Luke Cerwin
 */
#include "character.h"

Character::Character(QWidget *parent)
    : QWidget{parent}
{
    display = new QLabel(this);
    display->setAlignment(Qt::AlignCenter);
    display->setScaledContents(true);
    display->setStyleSheet("QLabel { background-color: transparent; }");
    // Movies are loaded on demand in setPetType() / syncWithPlayer().
}

void Character::resizeEvent(QResizeEvent *e) {
    QWidget::resizeEvent(e);
    // Keep display label filling the full widget — GIF scales to fit
    display->setGeometry(0, 0, width(), height());
}

void Character::loadMoviesForType(PetType type) {
    unloadMovies();
    auto make = [this](const char *path) {
        return new QMovie(path, QByteArray(), this);
    };
    if (type == DragonDog) {
        m_idle_baby  = make(":/images/Sprites/pets/dragondog/dragondog_idle.gif");
        m_idle_teen  = make(":/images/Sprites/pets/dragondog/purpledragondog_idle.gif");
        m_idle_adult = make(":/images/Sprites/pets/dragondog/dragondog_adult_idle.gif");
        m_sleepy     = make(":/images/Sprites/pets/dragondog/purpledragondog_sleep.gif");
    } else if (type == ElectricAxolotl) {
        m_idle_baby  = make(":/images/Sprites/pets/axolotl/axolotl_idle.gif");
        m_idle_teen  = make(":/images/Sprites/pets/axolotl/axolotl_teen_idle.gif");
        m_idle_adult = make(":/images/Sprites/pets/axolotl/axolotl_adult_idle.gif");
        m_sleepy     = make(":/images/Sprites/pets/axolotl/electric_axolotl_sleep.gif");
    } else {
        m_idle_baby  = make(":/images/Sprites/pets/seelcat/seelcat_idle.gif");
        m_idle_teen  = make(":/images/Sprites/pets/seelcat/seelcat_teen_idle.gif");
        m_idle_adult = make(":/images/Sprites/pets/seelcat/seelcat_adult_idle.gif");
        m_sleepy     = make(":/images/Sprites/pets/seelcat/seelcat_sleep.gif");
    }
    m_loadedType = type;
}

void Character::unloadMovies() {
    if (display->movie()) {
        display->movie()->stop();
        display->setMovie(nullptr);
    }
    delete m_idle_baby;  m_idle_baby  = nullptr;
    delete m_idle_teen;  m_idle_teen  = nullptr;
    delete m_idle_adult; m_idle_adult = nullptr;
    delete m_sleepy;     m_sleepy     = nullptr;
}

void Character::setPetType(PetType type) {
    if (type != m_loadedType)
        loadMoviesForType(type);
    currentType = type;
    playIdle();
}

void Character::setStageFromString(const QString &age_group) {
    currentStage = age_group;
    playIdle();
}

void Character::updateEmotionFromStats(int energyLevel, int hungerLevel) {
    Q_UNUSED(hungerLevel);
    currentEmotion = (energyLevel < 25) ? Sleepy : Normal;
    playIdle();
}

void Character::syncWithPlayer(const Player &player, PetType type) {
    if (type != m_loadedType)
        loadMoviesForType(type);
    currentType = type;
    setStageFromString(player.getPet().age_group());
    updateEmotionFromStats(player.getPet().energy(), player.getPet().hunger());
}

void Character::playIdle() {
    switchTo(currentMovie());
}

QMovie *Character::currentMovie() {
    if (currentEmotion == Sleepy) return m_sleepy;
    if (currentStage == "Teen")  return m_idle_teen;
    if (currentStage == "Adult") return m_idle_adult;
    return m_idle_baby;
}

void Character::switchTo(QMovie *movie) {
    if (!movie) return;
    if (display->movie() == movie) return;
    if (display->movie()) display->movie()->stop();
    display->setMovie(movie);
    movie->start();
    // No dynamic resize — display fills widget via resizeEvent
}

QRect Character::spriteRect() const {
    return QRect(0, 0, width(), height());
}

QPoint Character::spriteCenter() const {
    return mapToParent(QPoint(width() / 2, height() / 2));
}
