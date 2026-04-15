/*
 * Tile is a matrix element that makes up the tile-matrix for PiPatterns.
 *
 * Author: Sasha C. Guerrero
 */

#ifndef TILE_H
#define TILE_H
#include <QPushButton>

class Tile : public QPushButton {
    Q_OBJECT
public:
    explicit Tile(QPushButton *parent = nullptr);
    void changeState(bool);
    bool getState();
private:
    bool m_state = false; // State indicates if the tile is highlighted.
};

#endif // TILE_H
