/*
 * feed.h - Feed screen with drag-and-drop food onto character GIF.
 * Uses standard QWidget mouse events — same as rest of codebase.
 * Author(s): Luke Cerwin, Sasha Guerrero
 */
#ifndef FEED_H
#define FEED_H
#include <QtWidgets>
#include <QPixmap>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QTimer>
#include "../../Player/Player.h"
#include "../character_class/character.h"

struct Crumb {
    QPointF pos;
    QPointF vel;
    int     life;
};

// ── Draggable food icon ───────────────────────────────────────────────────
class FoodItem : public QLabel {
    Q_OBJECT
public:
    FoodItem(const QString &iconPath, const QString &name,
             int boost, QWidget *parent = nullptr);
    QString foodName;
    int     hungerBoost;
    QPoint  homePos;
protected:
    void mousePressEvent  (QMouseEvent *e) override;
    void mouseMoveEvent   (QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    bool event(QEvent *e) override;
signals:
    void dropped(FoodItem *self, QPoint globalPos);
private:
    QPoint m_offset;
    bool   m_dragging = false;
};

// ── Feed widget ───────────────────────────────────────────────────────────
class Feed : public QWidget
{
    Q_OBJECT
public:
    explicit Feed(Player *player, Character::PetType petType,
                  QWidget *parent = nullptr);
    void updateHungerDisplay();

    // Re-syncs the character sprite to the current pet type and equipped hat.
    // Call this from game.cc each time the feed screen is opened.
    void refreshCharacter();


protected:
    void paintEvent (QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;
    void showEvent  (QShowEvent   *e) override;

private slots:
    void onFoodDropped(FoodItem *icon, QPoint globalPos);
    void tickCrumbs();

private:
    Player             *player;
    Character::PetType  petType;
    QPixmap             m_bg;
    QLabel  *infoHelper;
    QTimer  *m_infoTimer;
    Character *character;
    QPoint spriteCenter()    const;
    QRect  characterHitbox() const;
    FoodItem *appleItem;
    FoodItem *boneItem;
    FoodItem *drinkItem;
    FoodItem *pizzaItem;
    void placeIcons();
    QGroupBox  *actionsBox;
    QLabel *hungerDisplay;
    QList<Crumb> m_crumbs;
    QTimer      *m_crumbTimer;
    void spawnCrumbs(QPoint center);
    void applyHungerAction(int boost, const QString &message);
};

#endif // FEED_H
