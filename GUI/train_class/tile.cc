/*
 * Tile is a single button cell in the PiPatterns 4x4 matrix.
 * It can be highlighted to show the pattern and flashes green or red on player input.
 *
 * Author: Sasha C. Guerrero
 */

#include "tile.h"

static const char *kDefaultStyle = R"(
    QPushButton {
        background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1,
            stop:0 #e55bde, stop:1 #2d34fc);
        border-radius: 0px;
        border: 4px inset mistyrose;
    }
    QPushButton:pressed {
        background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1,
            stop:0 #2d34fc, stop:1 #e55bde);
    }
)";

static const char *kHighlightedStyle = R"(
    QPushButton {
        background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1,
            stop:0 #FFD700, stop:1 #FFA500);
        border-radius: 0px;
        border: 4px outset #FFFF88;
    }
)";

Tile::Tile(QPushButton *parent) : QPushButton(parent) {
    setFixedSize(90, 90);
    setStyleSheet(kDefaultStyle);
}

// Switches between the idle gradient and the gold highlight used during pattern playback.
void Tile::setHighlighted(bool highlighted) {
    m_state = highlighted;
    setStyleSheet(highlighted ? kHighlightedStyle : kDefaultStyle);
}

// Briefly flashes green for correct or red for incorrect, then returns to idle.
void Tile::flashFeedback(bool correct) {
    QString style = correct
        ? "QPushButton { background-color: #00CC44; border: 4px outset #88FF88; border-radius: 0px; }"
        : "QPushButton { background-color: #CC2222; border: 4px outset #FF8888; border-radius: 0px; }";
    setStyleSheet(style);
    QTimer::singleShot(350, this, [this]() { setHighlighted(false); });
}

bool Tile::getState() {
    return m_state;
}
