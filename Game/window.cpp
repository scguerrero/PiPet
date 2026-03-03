/*
 * Window class implementation file.
 * Author(s): Sasha C. Guerrero
 * Created: 2/9/2026
 * Last Edited: 2/19/2026
 */
#include "window.h"

// Window is a subclass of QWidget
Window::Window(QWidget *parent)
    : QWidget{parent}
{
    // instantiate label, layouts, and buttons
    title = new QLabel("PIPET");
    top = new QVBoxLayout();
    start_layout = new QVBoxLayout();
    start = new QPushButton("Start");
    quit = new QPushButton("Quit");

    // add widgets to layouts
    start_layout->addWidget(title);
    start_layout->addWidget(start);
    // nest start_layout inside top
    top->addLayout(start_layout);
    top->addWidget(quit);

    // when quit button is clicked, quit application
    connect(quit, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
    // when start button is clicked, add create widget to start_layout
    connect(start, SIGNAL(clicked()), this, SLOT(openCreateWidget()) );

    // style properties
    title->setStyleSheet("font-size: 32px; font-weight: bold");
    title->setAlignment(Qt::AlignHCenter);
    quit->setStyleSheet("background: darkslategreen; color: darkseagreen");

    // window properties
    this->setLayout(top);
    this->setWindowTitle("PIPET");
    this->setContentsMargins(50,50,50,50);
}

void Window::openCreateWidget()
{
    create = new CreateWidget();
    // code fragment from Qt docs for deleting all widgets inside a layout: https://doc.qt.io/qt-6/qlayout.html#takeAt
    // delete all widgets from start_layout
    while ((deleteable = start_layout->takeAt(0)) != nullptr)
    {
        delete deleteable->widget();
        delete deleteable;
    }
    // add new widget to cleared layout
    start_layout->addWidget(create);
}

