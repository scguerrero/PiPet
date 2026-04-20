/*
 * Players can collect Item objects for their PiPet.
 * Item objects can be viewed in Gear mode.
 * Players must open a Lootbox to acquire an Item.
 *
 * Author: Sasha C. Guerrero
 */

#ifndef ITEM_H
#define ITEM_H
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>

class Item : public QGroupBox {
    Q_OBJECT
private:
    QString m_name = ""; // Item name
    QString m_imagepath = ""; // Path to Item image in resources.qrc
    QString m_flavortext = ""; // Item description

public:
    // Constructor with name, imagepath, and flavortext arguments
    explicit Item(QString, QString, QString, QGroupBox *parent = nullptr);

    // Vertical layout
    QVBoxLayout *layout;
    QLabel *name;
    QLabel *image;
    QLabel *flavortext;

    // Getters
    QString get_name();
    QString get_imagepath();
    QString get_flavortext();

    // Setters
    void set_name(QString);
    void set_imagepath(QString);
    void set_flavortext(QString);
};

#endif // ITEM_H