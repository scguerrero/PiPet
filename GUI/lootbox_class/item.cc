/*
 * Item represents a collectible reward with a name, flavor text, image, and rarity.
 *
 * Author: Sasha C. Guerrero
 */

#include "item.h"

Item::Item(QWidget *parent) : QWidget(parent) {}

Item::Item(QString name, QString filepath, QString flavortext, Rarity rarity, QWidget *parent)
    : QWidget(parent), m_name(name), m_filepath(filepath), m_flavortext(flavortext), m_rarity(rarity) {}

Item::Item(const Item &other) : QWidget(nullptr) {
    m_name = other.m_name;
    m_flavortext = other.m_flavortext;
    m_filepath = other.m_filepath;
    m_rarity = other.m_rarity;
}

QString Item::getName() {
    return m_name;
}

void Item::setName(QString name) {
    m_name = name;
}

QString Item::getFlavortext() {
    return m_flavortext;
}

void Item::setFlavortext(QString flavortext) {
    m_flavortext = flavortext;
}

QString Item::getFilepath() {
    return m_filepath;
}

void Item::setFilepath(QString filepath) {
    m_filepath = filepath;
}

Item::Rarity Item::getRarity() {
    return m_rarity;
}

void Item::setRarity(Rarity rarity) {
    m_rarity = rarity;
}
