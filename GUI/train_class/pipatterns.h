/*
 * PiPatterns is a minigame that improves the PiPet's intelligence.
 * The PiPet must memorize a given pattern of tiles and recreate it from memory to earn lootboxes.
 *
 * Author: Sasha C. Guerrero
 */

#ifndef PIPATTERNS_H
#define PIPATTERNS_H
#include <QtWidgets>
#include "matrix.h"

class PiPatterns : public QWidget {
    Q_OBJECT
public:
    explicit PiPatterns(QWidget *parent = nullptr);

    // Matrix of tiles
    Matrix *matrix;

    // Vertically-arrange widgets
    QVBoxLayout *layout;
    QLabel *logo;
    QPushButton *b_back;

};

#endif // PIPATTERNS_H
