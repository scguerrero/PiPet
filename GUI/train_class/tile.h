/*
 * Tile is a matrix element that makes up the tile-matrix for PiPatterns.
 *
 * Author: Sasha C. Guerrero
 */

#ifndef TILE_H
#define TILE_H
#include <QPushButton>
#include <QTimer>

class Tile : public QPushButton {
    Q_OBJECT
public:
    explicit Tile(QPushButton *parent = nullptr);

    void setHighlighted(bool highlighted);
    void flashFeedback(bool correct);
    bool getState();

private:
    bool m_state = false;
};

#endif // TILE_H
