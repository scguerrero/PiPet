/*
 * Inventory holds a collection of Item objects.
 *
 * Author: Sasha C. Guerrero
 */

#ifndef INVENTORY_H
#define INVENTORY_H
#include <vector>
#include "item.h"
using namespace std;

class Inventory {
public:
    Inventory();

    vector<Item> getItems();
    /*void setItems(vector<Item> items);*/

    void addItem(Item item);
    Item grabItemAt(int index);

private:
    vector<Item> m_items;
};

#endif // INVENTORY_H
