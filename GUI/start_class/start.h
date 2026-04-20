/*
 * start.h - Start/title screen header.
 * Author(s): Luke Cewin & Sasha Guerrero
 */

#ifndef START_H
#define START_H
#include <QtWidgets>
#include <QPixmap>
#include <QPaintEvent>

class Start : public QWidget {
    Q_OBJECT
public:
    explicit Start(QWidget *parent = nullptr);
    QPushButton *b_start;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVBoxLayout *layout;
    QLabel      *title;
    QPixmap      m_bg;
};

#endif
