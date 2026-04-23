/*
 * Gear mode — stage screen where the player dresses their PiPet with hats.
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

// ── Hat strip item ────────────────────────────────────────────────────────
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

// ── Main Gear widget ──────────────────────────────────────────────────────
class Gear : public QWidget {
    Q_OBJECT
public:
    explicit Gear(Player *player,
                  Character::PetType petType,
                  QWidget *parent = nullptr);

    // Call this whenever the player's pet type or stage changes externally
    void refresh(Character::PetType petType);

    // Call after loadGame() to restore a previously equipped hat without a particle burst
    void restoreHat(const QString &hatKey);
    void unlockHat(const QString &hatKey);

signals:
    void hatEquipped(const QString &hatKey); // emitted after the particle burst

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
private:

    Character   *m_character;

    QScrollArea *m_scrollArea;
    QWidget     *m_stripWidget;
    QHBoxLayout *m_stripLayout;
    QList<HatCard *> m_hatCards;

    // Particle overlay (full-window transparent label, shown briefly)
    QLabel  *m_particleOverlay;
    QTimer  *m_particleTimer;
    QList<QRect>  m_particles;
    QList<QColor> m_particleColors;
    int     m_particleTick = 0;

    // ── Geometry helpers ──────────────────────────────────────────────────
    // Pedestal rect (matches the blue-square region in the reference image)
    QRect pedestalCharRect() const;
    // Hat strip rect  (matches the yellow-line region)
    QRect stripRect()        const;

    void layoutWidgets();
    void loadHatGif(const QString &hatKey); // sets Character GIF
    void spawnParticles();
    void tickParticles();
    QString gifPath(const QString &hatKey) const;
};

#endif // GEAR_H
