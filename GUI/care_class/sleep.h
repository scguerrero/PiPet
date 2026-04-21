/*
 * sleep.h - Sleep screen with character GIF.
 * Cuddle replaces Brush Teeth.
 * Author(s): Luke Cewin & Sasha Guerrero
  */
#ifndef SLEEP_H
#define SLEEP_H

#include <QtWidgets>
#include <QPixmap>
#include <QPaintEvent>
#include "../../Player/Player.h"
#include "../character_class/character.h"

class Sleep : public QWidget
{
    Q_OBJECT
public:
    explicit Sleep(Player *player, Character::PetType petType,
                   QWidget *parent = nullptr);
    void updateSleepDisplay();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void cuddle();
    void wearPjs();
    void readBook();
    void tuckIn();

private:
    Player             *player;
    Character::PetType  petType;
    QPixmap             m_bg;
    Character          *character;

    QVBoxLayout *layout;
    QGroupBox   *actionsBox;
    QGridLayout *actionsGrid;

    QPushButton *cuddleBtn;
    QPushButton *wearPjsBtn;
    QPushButton *readBookBtn;
    QPushButton *tuckInBtn;

    QLabel *sleepDisplay;

    void applySleepAction(int boost, const QString &message);
};

#endif // SLEEP_H
