/*
 * feed.h - Feed screen header.
 * Author(s): Tanya Magurupira
 */
#ifndef FEED_H
#define FEED_H

#include <QtWidgets>
#include <QPixmap>
#include <QPaintEvent>
#include "../../Player/Player.h"

class Feed : public QWidget
{
    Q_OBJECT
public:
    explicit Feed(Player *player, QWidget *parent = nullptr);
    void updateHungerDisplay();
    QPushButton *backBtn;

protected:
    void paintEvent(QPaintEvent *event) override; // draws background

private slots:
    void apple();
    void bone();
    void drink();
    void pizza();

private:
    Player  *player;
    QPixmap  m_bg;  // background image

    QVBoxLayout *layout;
    QGroupBox   *actionsBox;
    QGridLayout *actionsGrid;

    QPushButton *appleBtn;
    QPushButton *boneBtn;
    QPushButton *drinkBtn;
    QPushButton *pizzaBtn;

    QLabel *hungerDisplay;

    void applyHungerAction(int boost, const QString &message);
};

#endif // FEED_H
