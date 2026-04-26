/*
 * Tile is a single button cell in the PiPatterns 4x4 matrix.
 * It can be highlighted to show the pattern and flashes green or red on player input.
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
