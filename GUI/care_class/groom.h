/*
 * groom.h - Groom screen with two-spot drag mechanic.
 * Uses standard QWidget mouse events — same as rest of codebase.
 * Author(s): Luke Cewin & Sasha Guerrero
 */
#ifndef GROOM_H
#define GROOM_H
#include <QtWidgets>
#include <QPixmap>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QTimer>
#include <QGroupBox>
#include "../../Player/Player.h"
#include "../character_class/character.h"

// ── Draggable Item ────────────────────────────────────────────────────────
class GroomItem : public QLabel {
    Q_OBJECT
public:
    GroomItem(const QString &iconPath, const QString &name,
              QWidget *parent = nullptr);
    QString toolName;
    QPoint  homePos;
protected:
    void mousePressEvent  (QMouseEvent *e) override;
    void mouseMoveEvent   (QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    bool event(QEvent *e) override;
signals:
    void dropped(GroomItem *self, QPoint globalPos);
private:
    QPoint m_offset;
    bool   m_dragging = false;
};

// ── Groom widget ──────────────────────────────────────────────────────────
class Groom : public QWidget
{
    Q_OBJECT
public:
    explicit Groom(Player *player, Character::PetType petType,
                   QWidget *parent = nullptr);
    void updateHygieneDisplay();

    // Re-syncs the character sprite to the current pet type and equipped hat.
    // Call this from game.cc each time the groom screen is opened.
    void refreshCharacter();

protected:
    void paintEvent (QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;
    void showEvent  (QShowEvent   *e) override;
private slots:
    void onToolDropped(GroomItem *tool, QPoint globalPos);
    void resetSpots();

private:
    Player             *player;
    Character::PetType  petType;
    QPixmap    m_bg;
    Character *character;
    GroomItem *brushTool;
    GroomItem *batheTool;
    GroomItem *trimTool;
    GroomItem *teethTool;
    void placeTools();
    QLabel     *hygieneDisplay;
    QLabel     *hintLabel;
    QLabel  *infoHelper;
    QTimer  *m_infoTimer;
    QGroupBox  *actionsBox;
    GroomItem  *activeTool = nullptr;
    bool        topDone    = false;
    bool        bottomDone = false;
    QRect topSpot()    const;
    QRect bottomSpot() const;
    QTimer *m_resetTimer;
    QTimer *m_hintTimer;
    void applyGroomAction(const QString &message);
    void showHint(const QString &text);
};

#endif // GROOM_H
