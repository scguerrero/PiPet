/*
 * Lootbox displays mystery rewards grouped by rarity.
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

    Inventory m_copper;
    Inventory m_silver;
    Inventory m_gold;
    std::vector<Item> m_rewardItems;

    QList<QLabel*> m_copperSlots;
    QList<QLabel*> m_silverSlots;
    QList<QLabel*> m_goldSlots;

    Item::Rarity m_lastWonTier  = Item::Copper;
    int          m_lastWonIndex = -1;

    QVBoxLayout *m_layout;
    QLabel      *m_title;
    QGroupBox   *m_copperGroup;
    QGroupBox   *m_silverGroup;
    QGroupBox   *m_goldGroup;
    QPushButton *m_openBtn;

    QFrame  *m_resultFrame;
    QLabel  *m_resultIcon;
    QLabel  *m_resultName;
    QLabel  *m_resultFlavor;
    QLabel  *m_resultDupe;

};

#endif // LOOTBOX_H
