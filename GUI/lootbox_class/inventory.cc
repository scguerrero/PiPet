/*
 * Inventory holds a collection of Item objects.
 *
 * Author: Sasha C. Guerrero
 */

#include "inventory.h"
using namespace std;

Inventory::Inventory(QWidget *parent) : QWidget(parent) {}

vector<Item> Inventory::getItems() {
    return m_items;
}

/*void Inventory::setItems(vector<Item> items) {
    m_items = items;
}*/

void Inventory::addItem(Item item) {
    m_items.push_back(item);
}

Item Inventory::grabItemAt(int index) {
    return m_items[index];
}
