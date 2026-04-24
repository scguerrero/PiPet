/*
 * Inventory holds a collection of Item objects.
 *
 * Author: Sasha C. Guerrero
 */

#ifndef INVENTORY_H
#define INVENTORY_H
#include <QWidget>
#include <vector>
#include "item.h"
using namespace std;

class Inventory : public QWidget {
    Q_OBJECT
public:
    explicit Inventory(QWidget *parent = nullptr);

    vector<Item> getItems();
    /*void setItems(vector<Item> items);*/

    void addItem(Item item);
    Item grabItemAt(int index);

private:
    vector<Item> m_items;
};

#endif // INVENTORY_H
