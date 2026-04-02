/* Tanya Magurupira */
#ifndef FEED_H
#define FEED_H

#include <QtWidgets>
#include "../../Player/Player.h"

class Feed : public QWidget
{
    Q_OBJECT
public:

public:
    explicit Feed(Player *player, QWidget *parent = nullptr);
    void updateHungerDisplay();
    QPushButton *backBtn;

signals:

private slots:
    void apple();
    void bone();
    void drink();
    void pizza();

private:
    Player *player;

    QVBoxLayout *layout;
    QGroupBox *actionsBox;
    QGridLayout *actionsGrid;

    QPushButton *appleBtn;
    QPushButton *boneBtn;
    QPushButton *drinkBtn;
    QPushButton *pizzaBtn;

    QLabel *hungerDisplay; // shows current happiness live

    void applyHungerAction(int boost, const QString &message);

};

#endif // FEED_H
