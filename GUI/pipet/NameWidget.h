/*
 * NameWidget class specification file.
 * This page of the game is for the Player to choose a name for their piPet from a given bank of names.
 * Author(s): Sasha C. Guerrero
 * Created: 2/16/2026
 * Last Edited: 2/18/2026
 */

#ifndef NAMEWIDGET_H
#define NAMEWIDGET_H
#include <QtWidgets>
class NameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NameWidget(QWidget *parent = nullptr);
private:
    QLabel *title;
    QPushButton *done;
    QVBoxLayout *menu;
signals:
};
#endif // NAMEWIDGET_H
