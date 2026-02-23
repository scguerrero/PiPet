/*
 * CareWidget class specification file.
 * In Care mode, the Player can feed, groom, and give affection to their pet.
 * They can also view their pet's Condition, or wellness attributes.
 * Author(s): Sasha C. Guerrero
 * Created: 2/19/2026
 * Last Edited: 2/20/2026
 */
#ifndef CAREWIDGET_H
#define CAREWIDGET_H
#include <QtWidgets>

class CareWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CareWidget(QWidget *parent = nullptr);
private:
    QGridLayout *grid; // Text labels and progress bars will go inside grid.

    // Text labels for the pet's hunger, energy, strength, hygiene, intelligence, happiness, days old, and age group.
    QLabel *hunger_label, *energy_label, *strength_label,
        *hygiene_label, *intelligence_label, *happiness_label,
        *age_days_label, *age_group_label, *age_days, *age_group;

    // Progress bars corresponding to the pet's attributes above.
    QProgressBar *hunger_bar, *energy_bar, *strength_bar,
        *hygiene_bar, *intelligence_bar, *happiness_bar;
};

#endif // CAREWIDGET_H
