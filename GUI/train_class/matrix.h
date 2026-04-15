/*
 * Matrix is a vector of Tile objects that are displayed on screen as a matrix.
 *
 * Author: Sasha C. Guerrero
 */

#ifndef MATRIX_H
#define MATRIX_H
#include <QtWidgets>
#include <vector>
#include "tile.h"
using namespace std;

class Matrix : public QWidget {
    Q_OBJECT
public:
    explicit Matrix (QWidget *parent = nullptr);

    // 4x4 grid
    QGridLayout *grid;

    // Vector of matrix elements, where each element is a Tile object
    vector<Tile*> elements;
};

#endif // MATRIX_H