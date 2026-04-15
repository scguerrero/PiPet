/*
 * Tile is a matrix element that makes up the tile-matrix for PiPatterns.
 *
 * Author: Sasha C. Guerrero
 */

#include "tile.h"

Tile::Tile(QPushButton *parent) : QPushButton(parent) {

    // Square tiles
    this->setFixedSize(90,90);

    // Stylesheet for default state and pressed state
    this->setStyleSheet(R"(
    QPushButton {
        background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #e55bde, stop:1 #2d34fc);
        border-radius: 0px;
        border: 4px inset mistyrose
    }
    QPushButton:pressed {
        background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #2d34fc, stop:1 #e55bde);
    }
    )");

}

void Tile::changeState(bool state) {
    m_state = state;
}

bool Tile::getState() {
    return m_state;
}