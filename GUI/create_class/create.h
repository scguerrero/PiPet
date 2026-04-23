/*
 * create.h - Pet creation screen.
 * Gallery shows animated GIF, no blue box, character_screen.jpg background.
 * Author(s): Luke Cerwin
 */
#ifndef CREATE_H
#define CREATE_H
#include <QtWidgets>
#include <QMovie>
#include <QPaintEvent>
#include <QPixmap>

class Create : public QWidget {
    Q_OBJECT
public:
    explicit Create(QWidget *parent = nullptr);
    QPushButton  *b_done;
    QRadioButton *b_axolotl, *b_cat, *b_dog;
    QListWidget  *name_list;

protected:
    void paintEvent(QPaintEvent *event) override;

public slots:
    void left_gallery();
    void right_gallery();
    void checkDoneEligibility();

private:
    QPixmap m_bg;
    int     m_galleryIndex = 0;

    void updateGallery();

    QLabel       *petImage;
    QMovie       *currentMovie = nullptr;
    QLabel       *petName;
    QLabel       *petDescription;
    QPushButton  *b_left;
    QPushButton  *b_right;

    QLabel      *nameHeader;
    QVBoxLayout *layout;

    bool name_chosen = false;

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

    QStringList gifPaths = {
        ":/images/Sprites/pets/axolotl/axolotl_idle.gif",
        ":/images/Sprites/pets/dragondog/dragondog_idle.gif",
        ":/images/Sprites/pets/seelcat/seelcat_idle.gif"
    };

    QStringList str_names = {
        "Axolvyn","Brixlore","Cyphrel","Duskmourn",
        "Eclynth","Frostael","Glimmvex","Hexalon","Irisvane","Jorethis",
        "Kryndel","Lumivex","Myrthalon","Noctrel","Ombryx","Pyxelorn",
        "Quilvash","Runethis","Spectrivon","Thalvex","Umbryss",
        "Vexlorin","Wyrmvael","Xyndrel","Ysolthex","Zyndarix"
    };
};

#endif
