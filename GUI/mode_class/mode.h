/*
 * Mode class specification file.
 * From here the Player can select which game mode they want to play:
 * Care mode for feeding and grooming the pet, Train mode for choosing a mini-game,
 * Battle mode for playing single-player games versus a bot opponent, and
 * Gear mode for choosing equipment/accessories for the pet.
 * Author(s): Sasha C. Guerrero
 * Created: 2/19/2026
 * Last Edited: 3/14/2026
 */
#ifndef MODE_H
#define MODE_H
#include <QtWidgets>

class Mode : public QWidget
{
    Q_OBJECT
    public:
        explicit Mode(QWidget *parent = nullptr);
        QPushButton *b_care, *b_train, *b_battle, *b_gear; // Buttons lead to corresponding widget
    private:
        QVBoxLayout *layout; // Vertically-arrange widgets inside Mode

};

#endif
