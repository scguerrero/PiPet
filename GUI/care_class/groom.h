/*
 * groom.h - Groom screen with two-spot drag mechanic.
 * Uses standard QWidget mouse events — same as rest of codebase.
 * Author(s): Luke Cerwin Sasha Guerro
 */
#ifndef GROOM_H
#define GROOM_H
#include <QtWidgets>
#include <QPixmap>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QTimer>
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
    explicit Groom(Player *player, Character::PetType petType,  // ADD: petType
                   QWidget *parent = nullptr);
    void updateHygieneDisplay();
    QPushButton *backBtn;
protected:
    void paintEvent (QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;
private slots:
    void onToolDropped(GroomItem *tool, QPoint globalPos);
    void resetSpots();
private:
    Player             *player;
    Character::PetType  petType;                                // ADD: petType member
    QPixmap  m_bg;
    Character *character;
    GroomItem *brushTool;
    GroomItem *batheTool;
    GroomItem *trimTool;
    GroomItem *teethTool;
    void placeTools();
    QLabel *hygieneDisplay;
    QLabel *hintLabel;
    GroomItem *activeTool = nullptr;
    bool       topDone    = false;
    bool       bottomDone = false;
    QRect topSpot()    const;
    QRect bottomSpot() const;
    QTimer *m_resetTimer;
    void applyGroomAction(const QString &message);
};
#endif // GROOM_H
