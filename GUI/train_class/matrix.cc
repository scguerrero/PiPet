/*
 * Matrix is a vector of Tile objects that are displayed on screen as a matrix.
 *
 * Author: Sasha C. Guerrero
 */

#include "matrix.h"

Matrix::Matrix(QWidget *parent) : QWidget(parent) {
    // 4x4 grid
    grid = new QGridLayout();
    this->setLayout(grid);

    // Initialize matrix elements
    for (int i=0; i<16; i++) {
        Tile *tile = new Tile();
        elements.push_back(tile);
    }

    // Add elements to grid layout
    for (int k = 0; k < 16; k++) {
        grid->addWidget(elements.at(k), k / 4, k % 4);
    }

}