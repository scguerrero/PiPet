/*
 * Gear mode — stage screen where the player dresses their PiPet with hats.
 *
 * Layout (bottom-up, matching gearStage_16bit.png):
 *   - Full-window background: gearStage_16bit.png
 *   - Character widget sitting on the pedestal (blue-square region)
 *   - Horizontal drag-scroll hat strip along the bottom (yellow-line region)
 *   - Particle effect overlay label (transparent, full-window)
 *   - Back button bottom-left
 *
 * Hat GIF naming convention assumed:
 *   :/images/Sprites/pets/{folder}/{prefix}_{stageInfix}{hat}.gif
 *   e.g. seelcat/seelcat_teen_santa.gif
 *        dragondog/dragondog_adult_cowboy.gif
 *        axolotl/axolotl_wizard.gif  (baby has no stage infix)
 *
 * Author(s): Luke Cewin & Sasha Guerrero
 */

#ifndef GEAR_H
#define GEAR_H

#include <QtWidgets>
#include <QMovie>
#include "../character_class/character.h"
#include "../../Player/Player.h"

// ── Hat strip item ────────────────────────────────────────────────────────
class HatCard : public QLabel {
    Q_OBJECT
public:
    explicit HatCard(const QString &hatKey,   // "cowboy" | "crown" | "santa" | "wizard"
                     const QString &iconPath,
                     QWidget *parent = nullptr);
    QString hatKey() const { return m_key; }
    void setSelected(bool sel);

signals:
    void clicked(const QString &hatKey);

protected:
    void mousePressEvent(QMouseEvent *) override;

private:
    QString m_key;
    bool    m_selected = false;
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

    QPushButton *backBtn;

signals:
    void hatEquipped(const QString &hatKey); // emitted after the particle burst

protected:
    void resizeEvent(QResizeEvent *e) override;
    void paintEvent (QPaintEvent  *e) override;

private slots:
    void onHatSelected(const QString &hatKey);

private:
    Player            *m_player;
    Character::PetType m_petType  = Character::DragonDog;
    QString            m_stage    = "Baby";   // "Baby" | "Teen" | "Adult"
    QString            m_equippedHat = "";    // "" = no hat / idle gif

    // Background
    QPixmap m_bg;

    // Pet on pedestal
    Character *m_character;

    // Hat strip (inside a scroll area)
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

    // Build a GIF path from current state + hatKey
    QString gifPath(const QString &hatKey) const;
};

#endif // GEAR_H
