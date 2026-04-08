/*
 * create.h - Pet creation screen.
 * Gallery replaces the species tab entirely.
 * Arrows cycle pet image + show name/description automatically.
 * DONE button unlocks once a name is chosen (species auto-selected by gallery).
 * Author(s): Sasha C. Guerrero
 */

#ifndef CREATE_H
#define CREATE_H
#include <QtWidgets>

class Create : public QWidget {
    Q_OBJECT
public:
    explicit Create(QWidget *parent = nullptr);
    QPushButton *b_done;

    // Public so game.cc can read which species is selected
    QRadioButton *b_axolotl, *b_cat, *b_dog;

    // Public so game.cc can read the chosen name
    QListWidget *name_list;

protected:
    void paintEvent(QPaintEvent *event) override;

public slots:
    void left_gallery();
    void right_gallery();
    void checkDoneEligibility();

private:
    QPixmap m_bg;

    // Gallery index: 0=axolotl, 1=dragondog, 2=seelcat
    int m_galleryIndex = 0;

    void updateGallery(); // updates image, name, description, and auto-selects radio

    // Gallery widgets
    QLabel *petImage;
    QLabel *petName;
    QLabel *petDescription;
    QPushButton *b_left;
    QPushButton *b_right;

    // Name list
    QLabel      *name_instruction;
    QScrollArea *scroll_name;
    QWidget     *nameWidget;
    QVBoxLayout *l_name;

    QStringList str_names = {
        "Axolvyn","Brixlore","Cyphrel","Duskmourn",
        "Eclynth","Frostael","Glimmvex","Hexalon","Irisvane","Jorethis",
        "Kryndel","Lumivex","Myrthalon","Noctrel","Ombryx","Pyxelorn",
        "Quilvash","Runethis","Spectrivon","Thalvex","Umbryss",
        "Vexlorin","Wyrmvael","Xyndrel","Ysolthex","Zyndarix"
    };

    // Pet info per index
    QStringList petNames = {
        "Electric Axolotl",
        "Dragon Dog",
        "Seel Cat"
    };

    QStringList petDescriptions = {
        "An amphibian with powers of electricity.\nStrong attack, agile in battle.",
        "A scaly, fire-breathing, winged dog.\nBalanced attack and defense.",
        "A semiaquatic cat with a powerful tailfin.\nStrong defense, resilient fighter."
    };

    QVBoxLayout *layout;

    bool name_chosen    = false;
    bool species_chosen = true; // always true — gallery auto-selects
};

#endif
