/*
 * Gear mode is where the player can collect items for their PiPet.
 *
 * Author(s): Luke Cewin & Sasha Guerrero
 */

#ifndef GEAR_H
#define GEAR_H
#include <QtWidgets>
#include "item.h"

class Gear : public QWidget {
    Q_OBJECT
public:
    explicit Gear(QWidget *parent = nullptr);
    QVBoxLayout *layout;
    QLabel *logo;
    QPushButton *b_back;
    Item *item;
};

#endif // GEAR_H