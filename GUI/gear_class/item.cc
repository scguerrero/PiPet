/*
 * Players can collect Item objects for their PiPet.
 * Item objects can be viewed in Gear mode.
 * Players must open a Lootbox to acquire an Item.
 *
 * Author: Sasha C. Guerrero
 */

#include "item.h"

// Argument abbreviations --> n: name, ip: imagepath, ft: flavortext
Item::Item(QString n, QString ip, QString ft, QGroupBox *parent) : QGroupBox(parent)
{
    // Initialize member variables
    m_name = n;
    m_imagepath = ip;
    m_flavortext = ft;

    // Item image
    image = new QLabel();
    QImage *img = new QImage(get_imagepath());
    QPixmap px = QPixmap::fromImage(img->scaled(100,100,Qt::KeepAspectRatio));
    image->setPixmap(px);

    // Vertical layout, name, and flavortext
    layout = new QVBoxLayout();
    name = new QLabel(get_name());
    name->setAlignment(Qt::AlignCenter);
    flavortext = new QLabel(get_flavortext());

    // Add widgets to layout with center-alignment
    layout->addWidget(name);
    layout->addWidget(image);
    layout->addWidget(flavortext);
    this->setLayout(layout);

    // Style
    this->setStyleSheet(R"(
    QWidget {
        background-color: transparent;
    }
    QGroupBox {
        background-color: #030ba3;
        border: 2px inset #fba8ff;

    }
    )");
}

QString Item::get_name() {
    return m_name;
}

QString Item::get_imagepath() {
    return m_imagepath;
}

QString Item::get_flavortext() {
    return m_flavortext;
}

void Item::set_name(QString n) {
    m_name = n;
}

void Item::set_imagepath(QString ip) {
    m_imagepath = ip;
}

void Item::set_flavortext(QString ft) {
    m_flavortext = ft;
}