/*
 * TrainWidget class specification file.
 * In Train mode, the Player can play mini-games with their pet to increase
 * the pet's attributes.
 * Author(s): Sasha C. Guerrero
 * Created: 2/23/2026
 * Last Edited: 2/23/2026
 */
#ifndef TRAINWIDGET_H
#define TRAINWIDGET_H
#include <QtWidgets>

class TrainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TrainWidget(QWidget *parent = nullptr);
private:
    QGridLayout *grid; // buttons leading to each mini-game will go in this grid
    QPushButton *minigame1, *minigame2, *minigame3;

    // minigame widget 1
    // minigame widget 2
    // minigame widget 3
};

#endif // TRAINWIDGET_H
