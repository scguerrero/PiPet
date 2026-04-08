/*
 * create.h
 * Author(s): Sasha C. Guerrero
 * Fixed: b_axolotl, b_cat, b_dog moved to public so game.cc can read
 *        which species the player chose in onCreateDone().
 *        Added name_list to public so game.cc can read the chosen name.
 *        Added validation: DONE button only activates once name AND species chosen.
 */

#ifndef CREATE_H
#define CREATE_H
#include <QtWidgets>

class Create : public QWidget {
    Q_OBJECT
public:
    explicit Create(QWidget *parent = nullptr);
    QPushButton *b_done;

    // Made public so game.cc can read which species was chosen
    QRadioButton *b_axolotl, *b_cat, *b_dog;

    // Made public so game.cc can read the chosen name
    QListWidget *name_list;

public slots:
    void left_gallery();
    void right_gallery();
    void checkDoneEligibility(); // enables b_done only when both chosen

private:
    bool name_chosen    = false;
    bool species_chosen = false;

    QVBoxLayout *layout;
    QTabWidget  *tabs;
    QWidget     *name, *species;

    QScrollArea  *scroll_name;
    QVBoxLayout  *l_name;
    QLabel       *name_instruction;
    QStringList   str_names = {
        "Axolvyn","Brixlore","Cyphrel","Duskmourn",
        "Eclynth","Frostael","Glimmvex","Hexalon","Irisvane","Jorethis",
        "Kryndel","Lumivex","Myrthalon","Noctrel","Ombryx","Pyxelorn",
        "Quilvash","Runethis","Spectrivon","Thalvex","Umbryss",
        "Vexlorin","Wyrmvael","Xyndrel","Ysolthex","Zyndarix"
    };

    QScrollArea *scroll_species;
    QVBoxLayout *l_species;
    QLabel      *species_instruction;
    QGroupBox   *box_buttons;
    QVBoxLayout *l_buttons;

    QStackedWidget *pictures;
};

#endif
