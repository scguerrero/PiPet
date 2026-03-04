/*
 * CreateWidget class implementation file.
 * This page of the game is for the Player to create their piPet.
 * Author(s): Sasha C. Guerrero
 * Created: 2/16/2026
 * Last Edited: 2/19/2026
 */
#include "CreateWidget.h"

CreateWidget::CreateWidget(QWidget *parent)
    : QWidget{parent}
{
    // initialize tab widget and tab items
    menu = new QVBoxLayout();
    createTabs = new QTabWidget();
    name = new QWidget();
    species = new QWidget();
    palette = new QWidget();
    done = new QPushButton("Done");

    // when done button is clicked, close the CreateWidget
    connect(done, SIGNAL(clicked()), this, SLOT(openModeWidget()));
    // when the CreateWidget is closed, it is deleted
    //this->setAttribute(Qt::WA_DeleteOnClose);

    // add tab items to tab widget
    createTabs->addTab(name, "Name");
    createTabs->addTab(species, "Species");
    createTabs->addTab(palette, "Palette");
    menu->addWidget(createTabs);
    menu->addWidget(done);
    this->setLayout(menu);
}

void CreateWidget::openModeWidget() {
    mode = new ModeWidget();
    // delete all widgets from start_layout
    while ((deleteable = this->menu->takeAt(0)) != nullptr)
    {
        delete deleteable->widget();
        delete deleteable;
    }
    menu->addWidget(mode);
};
