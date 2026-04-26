/*
 * Lootbox displays mystery rewards grouped by rarity tier.
 * Items are revealed slot-by-slot as the player opens lootboxes earned from minigames.
 * Gold-tier wins emit hatUnlocked() so the Gear screen can make the hat available.
 *
 * Author: Sasha C. Guerrero
 */

#ifndef LOOTBOX_H
#define LOOTBOX_H
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QFrame>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include "inventory.h"
#include "../../Player/Player.h"

class Lootbox : public QWidget {
    Q_OBJECT
public:
    explicit Lootbox(Player *player, QWidget *parent = nullptr);
    int computeReward();

signals:
    void hatUnlocked(const QString &hatKey);

public slots:
    void awardLootbox();
    void restoreFromPlayer();

protected:
    void paintEvent(QPaintEvent *e) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onOpen();

private:
    Player  *player;
    QPixmap  m_bg;
    int      m_pendingLootboxes = 0;

    // One inventory per rarity tier
    Inventory m_copper;
    Inventory m_silver;
    Inventory m_gold;
    std::vector<Item> m_rewardItems;

    QList<QLabel*> m_copperSlots;
    QList<QLabel*> m_silverSlots;
    QList<QLabel*> m_goldSlots;

    // Tracks which slot to reveal after a roll
    Item::Rarity m_lastWonTier  = Item::Copper;
    int          m_lastWonIndex = -1;

    QVBoxLayout *m_layout;
    QLabel      *m_title;
    QGroupBox   *m_copperGroup;
    QGroupBox   *m_silverGroup;
    QGroupBox   *m_goldGroup;
    QPushButton *m_openBtn;

    // Result panel shown after each open
    QFrame  *m_resultFrame;
    QLabel  *m_resultIcon;
    QLabel  *m_resultName;
    QLabel  *m_resultFlavor;
};

#endif // LOOTBOX_H
