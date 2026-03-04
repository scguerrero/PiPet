/*
 * Start class specification file.
 * This is the first page the player sees in the game. They can start/close the game from here.
 * Author(s): Sasha C. Guerrero
 * Created: 3/3/2026
 * Last Edited: 3/3/2026
 */

#ifndef START_H
#define START_H
#include <QtWidgets>

class Start : public QWidget
{
Q_OBJECT
public:
    explicit Start (QWidget *parent = nullptr);
private:
	QVBoxLayout *box; // Box will hold title and buttons
	QLabel *title; // Title text
	QPushButton *b_start, *b_quit; // Buttons are  prefixed with b_ to differentiate them from their corresponding pages
signals:
private slots:
};

#endif // START_H
