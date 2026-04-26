/*
 * gear.h - Theater screen where the player dresses their PiPet with unlocked hats.
 * HatCard is a single selectable item in the horizontal hat strip.
 * Gear manages the full screen: character display, hat strip, and particle burst on equip.
 *
 * Author(s): Luke Cerwin
 */

#ifndef GEAR_H
#define GEAR_H

#include <QtWidgets>
#include <QMovie>
#include <QScroller>
#include "../character_class/character.h"
#include "../../Player/Player.h"

// Single hat option in the scrollable strip at the bottom of the screen.
class HatCard : public QLabel {
    Q_OBJECT
public:
    explicit HatCard(const QString &hatKey,
                     const QString &iconPath,
                     QWidget *parent = nullptr);
    QString hatKey() const { return m_key; }
    void setSelected(bool sel);
    void setLocked(bool locked);

signals:
    void clicked(const QString &hatKey);
    void lockedTapped(const QString &hatKey);

protected:
    void mousePressEvent(QMouseEvent *) override;

private:
    QString m_key;
    QString m_iconPath;
    bool    m_selected = false;
    bool    m_locked   = false;
    void    loadIcon();
    void    applyStyle();
};

// Main Theater widget showing the dressed pet and the scrollable hat strip.
class Gear : public QWidget {
    Q_OBJECT
public:
    explicit Gear(Player *player,
                  Character::PetType petType,
                  QWidget *parent = nullptr);

    // Call whenever the player's pet type or stage changes externally.
    void refresh(Character::PetType petType);

    // Call after loadGame() to restore a previously equipped hat without a particle burst.
    void restoreHat(const QString &hatKey);
    void unlockHat(const QString &hatKey);

signals:
    void hatEquipped(const QString &hatKey);

protected:
    void resizeEvent(QResizeEvent *e) override;
    void paintEvent (QPaintEvent  *e) override;
    void showEvent  (QShowEvent   *e) override;

private slots:
    void onHatSelected(const QString &hatKey);

private:
    Player            *m_player;
    Character::PetType m_petType  = Character::DragonDog;
    QString            m_stage    = "Baby";
    QString            m_equippedHat = "";
    QLabel  *infoHelper;
    QTimer  *m_infoTimer;
    QPixmap m_bg;

public:
    QPushButton *b_achievements;
    QPushButton *b_lootboxes;

private:
    Character   *m_character;

    QScrollArea *m_scrollArea;
    QWidget     *m_stripWidget;
    QHBoxLayout *m_stripLayout;
    QList<HatCard *> m_hatCards;

    // Particle overlay shown briefly after a hat is equipped
    QLabel  *m_particleOverlay;
    QTimer  *m_particleTimer;
    QList<QRect>  m_particles;
    QList<QColor> m_particleColors;
    int     m_particleTick = 0;

    QRect pedestalCharRect() const;
    QRect stripRect()        const;

    void layoutWidgets();
    void loadHatGif(const QString &hatKey);
    void spawnParticles();
    void tickParticles();
    QString gifPath(const QString &hatKey) const;
};

#endif // GEAR_H
