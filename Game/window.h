/*
 * Window class specification file.
 * Author(s): Sasha C. Guerrero
 * Created: 2/9/2026
 * Last Edited: 2/19/2026
 */

#ifndef WINDOW_H
#define WINDOW_H
#include <QtWidgets>
#include "CreateWidget.h"

class Window : public QWidget
{
Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);
private:
    QLabel *title; // title text
    QVBoxLayout *top; // toplevel layout
    QVBoxLayout *start_layout; // layout containing start button
    QPushButton *start; // start button
    QPushButton *quit; // quit button
    CreateWidget *create; // widget for pet creation
    QLayoutItem *deleteable; // holds layout item to be deleted
signals:
private slots:
    void openCreateWidget(); // swap current layout for pet creation layout
};


#endif // WINDOW_H
