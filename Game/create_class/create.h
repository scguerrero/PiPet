/*
 * Create class specification file. This is page 1 of the game, where the Player creates their piPet.
 * We create a tabbed view of the pages for choosing the pet's name, species, and color palette.
 * Pet creation is only available for new savefiles. Players have only one opportunity to
 * choose their pet's characteristics.
 * Author(s): Sasha C. Guerrero
 * Created: 2/16/2026
 * Last Edited: 2/19/2026
 */
#ifndef CREATE_H
#define CREATE_H
#include <QtWidgets>
#include <QStringList>

class Create : public QWidget
{
    Q_OBJECT
public:
    explicit Create (QWidget *parent = nullptr);
private:
	QVBoxLayout *box;
	QTabWidget *tabs;
	QWidget *name, *species, *palette;
	QRadioButton *b_axolotl, *b_dog, *b_cat, *b_classic, *b_vivid, *b_pastel;
	QStringList str_names = {"Axolvyn","Brixlore","Cyphrel","Duskmourn",
	"Eclynth","Frostael","Glimmvex","Hexalon","Irisvane","Jorethis",
	"Kryndel","Lumivex","Myrthalon","Noctrel","Ombryx","Pyxelorn",
	"Quilvash","Runethis","Spectrivon","Thalvex","Umbryss",
	"Vexlorin","Wyrmvael","Xyndrel","Ysolthex","Zyndarix"};
	QListView *name_list;
	QPushButton *done;
    signals:
    private slots:
        //void openModeWidget(); // open the game mode selection widget
};

#endif // CREATE_H
