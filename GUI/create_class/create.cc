/*
 * Create class implementation file.
 * This page of the game is for the Player to create their piPet.
 * We create a tabbed view of the pages for choosing the pet's name and species.
 * Pet creation is only available for new savefiles.
 * Author(s): Sasha C. Guerrero
 * Created: 2/16/2026
 * Last Edited: 3/10/2026
 */
#include "create.h"

Create::Create(QWidget *parent) : QWidget{parent} {
	// Name widget --------------------------------------------------------------
	name = new QWidget();
    scroll_name = new QScrollArea(); // Enable scrolling if widgets don't fit in visible area of screen
    scroll_name->setWidgetResizable(true);
    l_name = new QVBoxLayout(); // The name widget's main layout
	name_instruction = new QLabel("Your PiPet requires a name! You must choose 1 name from the list.");
    name_instruction->setWordWrap(true); // If text is too long, wrap around to the next line

	// Make the list of names
	name_list = new QListWidget(); 
	name_list->setViewMode(QListView::ListMode); // List style
	name_list->setFlow(QListView::TopToBottom);
	for (int i=0; i<26; i++) {
		QListWidgetItem* item = new QListWidgetItem();
		item->setText(str_names[i]);
		name_list->addItem(item);
	}

	// Assigning layout and widgets to name
	name->setLayout(l_name); // name widget gets layout l_name
	l_name->addWidget(name_instruction); // l_name contains instructions for the player
	l_name->addWidget(name_list); // l_name contains the list of names
    scroll_name->setWidget(name); // Enable scrolling if widgets don't fit in visible area of screen

	// Species widget ----------------------------------------------------------
	species = new QWidget();
    scroll_species = new QScrollArea(); // Enable scrolling if widgets don't fit in visible area of screen
    scroll_species->setWidgetResizable(true);
	l_species = new QVBoxLayout();
	species_instruction = new QLabel(R"(Your PiPet can be 1 of 3 species:
		<ul>
			<li><em>Electric Axolotl</em>: An amphibian with powers of electricity.</li>
			<li><em>Seal Cat</em>: A semiaquatic cat with a powerful tailfin.</li>
			<li><em>Dragon Dog</em>: A scaly, fire-breathing, and winged dog.</li>
		</ul>
		)");
	species_instruction->setTextFormat(Qt::RichText); // Parse HTML tags
    species_instruction->setWordWrap(true); // If text is too long, wrap around to the next line
	species->setLayout(l_species);
	l_species->addWidget(species_instruction);
    scroll_species->setWidget(species); // Enable scrolling if widgets don't fit in visible area of screen
	
	// Make the button box where the player can mark their decision
    box_buttons = new QGroupBox("Choose 1");
	l_buttons = new QVBoxLayout();
	box_buttons->setLayout(l_buttons);
	l_species->addWidget(box_buttons);

	// Add buttons
	b_axolotl = new QRadioButton("Electric Axolotl");
	b_cat = new QRadioButton("Seal Cat");
	b_dog = new QRadioButton("Dragon Dog");
	l_buttons->addWidget(b_axolotl);
	l_buttons->addWidget(b_cat);
	l_buttons->addWidget(b_dog);

	// Create widget -----------------------------------------------------------
	layout = new QVBoxLayout(); // Create widget's main layout
	tabs = new QTabWidget(); // Tab widget holding name and species tabs
	b_done = new QPushButton("DONE"); // Player will click 'DONE' after choosing pet's name and species 

	// Populate tab widget
    tabs->addTab(scroll_name, "Name");
    tabs->addTab(scroll_species, "Species");
	
	// Populate main layout
	this->setLayout(layout);
	layout->addWidget(tabs);
	layout->addWidget(b_done);
}
