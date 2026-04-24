/*
 * start.cc - Start/title screen
 * Author(s): Luke Cerwin
 */
#include "start.h"
#include <QPainter>

Start::Start(QWidget *parent) : QWidget{parent} {
    m_bg.load(":/images/Backgrounds/title_screen.png");

    layout  = new QVBoxLayout();
    b_start = new QPushButton("START");

    // Use a spacer instead of a QLabel
    layout->addSpacing(400);
    layout->addStretch();
    layout->addWidget(b_start);

    this->setLayout(layout);
    // Start Button
    b_start->setStyleSheet(R"(
        QPushButton {
            background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #4850DB, stop:1 #4A71DB);
            border: 2px inset #FBA8FF;
            border-radius: 10px;
            padding: 4px;
            font: bold;
            color: mistyrose;
        }
        QPushButton:pressed {
            background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #4A71DB, stop:1 #4850DB);
        }
    )");
}

void Start::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    if (!m_bg.isNull())
        painter.drawPixmap(0, 0, width(), height(), m_bg);
}
