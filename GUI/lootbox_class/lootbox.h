/*
 * Lootbox displays mystery rewards grouped by rarity.
 *
 * Author: Sasha C. Guerrero
 */

#ifndef LOOTBOX_H
#define LOOTBOX_H
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include "inventory.h"
#include "../../Player/Player.h"

class Lootbox : public QWidget {
    Q_OBJECT
public:
    explicit Lootbox(Player *player, QWidget *parent = nullptr);

private:
    Player *player;
    Inventory m_copper;
    Inventory m_silver;
    Inventory m_gold;

    QVBoxLayout *m_layout;
    QLabel *m_title;
    QGroupBox *m_copperGroup;
    QGroupBox *m_silverGroup;
    QGroupBox *m_goldGroup;
};

#endif // LOOTBOX_H
