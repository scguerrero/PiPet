/*
 * Inventory holds a collection of Item objects for a single rarity tier.
 * Used by Lootbox to separate Copper, Silver, and Gold item pools.
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
    void addItem(Item item);
    Item grabItemAt(int index);

private:
    vector<Item> m_items;
};

#endif // INVENTORY_H
