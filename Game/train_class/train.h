/*
 * Train class specification file.
 * In Train mode, the Player can play mini-games with their pet to increase the pet's attributes.
 * Author(s): Sasha C. Guerrero
 * Created: 2/23/2026
 * Last Edited: 3/16/2026
 */
#ifndef TRAIN_H
#define TRAIN_H
#include <QtWidgets>
//include minigame 1
//include minigame 2
//include minigame 3

class Train : public QWidget
{
    Q_OBJECT
public:
    explicit Train(QWidget *parent = nullptr);
    QPushButton *b_back;
private:
    QVBoxLayout *layout; // buttons leading to each mini-game will go in this layout
    QPushButton *b_minigame1, *b_minigame2, *b_minigame3;

    // minigame widget 1
    // minigame widget 2
    // minigame widget 3
};

#endif // TRAIN_H
