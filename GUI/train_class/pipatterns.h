/*
 * PiPatterns is a minigame that improves the PiPet's intelligence.
 *
 * Author: Sasha C. Guerrero
 */

#ifndef PIPATTERNS_H
#define PIPATTERNS_H
#include <QtWidgets>

class PiPatterns : public QWidget
{
    Q_OBJECT
public:
    explicit PiPatterns(QWidget *parent = nullptr);
    QVBoxLayout *layout;
    QLabel *logo;
    QPushButton *b_back;

};

#endif // PIPATTERNS_H
