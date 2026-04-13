/*
 * Gear mode is where the player can collect items for their PiPet.
 *
 * Author: Sasha C. Guerrero
 */

#ifndef GEAR_H
#define GEAR_H
#include <QtWidgets>

class Gear : public QWidget {
    Q_OBJECT
public:
    explicit Gear(QWidget *parent = nullptr);
    QVBoxLayout *layout;
    QLabel *logo;
    QPushButton *b_back;
};

#endif // GEAR_H