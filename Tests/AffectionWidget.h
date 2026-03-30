#ifndef AFFECTIONWIDGET_H
#define AFFECTIONWIDGET_H

#include <QtWidgets>
#include "../../Player/Player.h"

class AffectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AffectionWidget(Player *player, QWidget *parent = nullptr);
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

#endif // AFFECTIONWIDGET_H
