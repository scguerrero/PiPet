/*
 * affection.h - Affection screen header.
 * Author(s): Tanya Magurupira
 */
#ifndef AFFECTION_H
#define AFFECTION_H

#include <QtWidgets>
#include <QPixmap>
#include <QPaintEvent>
#include "../../Player/Player.h"

class Affection : public QWidget
{
    Q_OBJECT
public:
    explicit Affection(Player *player, QWidget *parent = nullptr);
    void updateHappinessDisplay();
    QPushButton *backBtn;

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void cuddle();
    void play();
    void givePets();
    void giveTreat();

private:
    Player  *player;
    QPixmap  m_bg;

    QVBoxLayout *layout;
    QGroupBox   *actionsBox;
    QGridLayout *actionsGrid;

    QPushButton *cuddleBtn;
    QPushButton *playBtn;
    QPushButton *petsBtn;
    QPushButton *treatBtn;

    QLabel *happinessDisplay;

    void applyAffectionAction(int boost, const QString &message);
};

#endif // AFFECTION_H
