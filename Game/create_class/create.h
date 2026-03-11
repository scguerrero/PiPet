/*
 * Create class specification file.
 * This page of the game is for the Player to create their piPet.
 * We create a tabbed view of the pages for choosing the pet's name and species.
 * Pet creation is only available for new savefiles.
 * Author(s): Sasha C. Guerrero
 * Created: 2/16/2026
 * Last Edited: 3/10/2026
 */
#ifndef CREATE_H
#define CREATE_H
#include <QtWidgets>

class Create : public QWidget {
	Q_OBJECT
	public:
		explicit Create(QWidget *parent = nullptr);
		QPushButton *b_done; // For navigation to next page
	private:
		QVBoxLayout *layout; // Vertically-arrange widgets inside Create
		QTabWidget *tabs; // Will hold name and species widgets
		QWidget *name, *species; // Child widgets of tabs

		// name's child widgets
		QVBoxLayout *l_name; // subwidget layouts are prefixed by l_ to differentiate from top-level layout
		QLabel *name_instruction; // Tell player to choose a name
		QListWidget *name_list; // Name options for the pet
		QStringList str_names = {"Axolvyn","Brixlore","Cyphrel","Duskmourn",
			"Eclynth","Frostael","Glimmvex","Hexalon","Irisvane","Jorethis",
			"Kryndel","Lumivex","Myrthalon","Noctrel","Ombryx","Pyxelorn",
			"Quilvash","Runethis","Spectrivon","Thalvex","Umbryss",
			"Vexlorin","Wyrmvael","Xyndrel","Ysolthex","Zyndarix"};

		// species's child widgets
		QVBoxLayout *l_species; // l_species's children: species_instruction, box_species, and box_choose
		QLabel *species_instruction;
//		QGroupBox *box_species; // box_species's children: grid_species
//		QGridLayout *grid_species; // arrange widgets on a row/column grid
		QGroupBox *box_buttons; // Radio buttons will go inside this box
		QVBoxLayout *l_buttons;
		QRadioButton *b_axolotl, *b_cat, *b_dog; // Player may choose 1 species only using radio buttons
};

#endif
