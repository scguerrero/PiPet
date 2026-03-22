#ifndef AFFECTION_H
#define AFFECTION_H

#include <QtWidgets>
#include "../../Player/Player.h"

class Affection : public QWidget
{
    Q_OBJECT
public:
    explicit Affection(Player *player, QWidget *parent = nullptr);
    void updateHappinessDisplay();
    QPushButton *backBtn;

signals:

private slots:
    void cuddle();
    void play();
    void givePets();
    void giveTreat();

private:
    Player *player;

    QVBoxLayout *layout;
    QGroupBox *actionsBox;
    QGridLayout *actionsGrid;

    QPushButton *cuddleBtn;
    QPushButton *playBtn;
    QPushButton *petsBtn;
    QPushButton *treatBtn;

    QLabel *happinessDisplay; // shows current happiness live

    void applyAffectionAction(int boost, const QString &message);

};

#endif // AFFECTION_H
