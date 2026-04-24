/*
 * Item represents a collectible reward with a name, flavor text, image, and rarity.
 *
 * Author: Sasha C. Guerrero
 */

#ifndef ITEM_H
#define ITEM_H
#include <QString>

class Item {
public:
    enum Rarity { Copper, Silver, Gold };

    Item();
    Item(QString name, QString filepath, QString flavortext, Rarity rarity);
    Item(const Item &other);

    QString getName();
    void setName(QString name);

    QString getFlavortext();
    void setFlavortext(QString flavortext);

    QString getFilepath();
    void setFilepath(QString filepath);

    Rarity getRarity();
    void setRarity(Rarity rarity);

private:
    QString m_name;
    QString m_flavortext;
    QString m_filepath;
    Rarity m_rarity;
};

#endif // ITEM_H
