/*
 * Lootbox displays mystery rewards grouped by rarity.
 *
 * Author: Sasha C. Guerrero
 */

#include "lootbox.h"
#include <QRandomGenerator>
#include <QDialog>
#include <QMouseEvent>
#include <QEvent>

static void populateGroupSlots(QGroupBox *group, Inventory &inv, QList<QLabel*> &out) {
    auto *row = new QHBoxLayout(group);
    row->setAlignment(Qt::AlignCenter);
    row->setSpacing(8);
    int count = static_cast<int>(inv.getItems().size());
    for (int i = 0; i < count; ++i) {
        auto *slot = new QLabel("?");
        slot->setAlignment(Qt::AlignCenter);
        slot->setFixedSize(36, 36);
        slot->setStyleSheet(
            "QLabel { background-color: #0247A7;"
            "border: 2px solid mistyrose; border-radius: 4px; }");
        row->addWidget(slot);
        out.append(slot);
    }
}

Lootbox::Lootbox(Player *player, QWidget *parent) : QWidget(parent), player(player) {
    m_bg.load(":/images/Backgrounds/trainbg.png");

    m_title = new QLabel("Mystery Rewards!");
    m_title->setAlignment(Qt::AlignCenter);
    m_title->setStyleSheet(
        "QLabel { background-color: #0247A7;"
        "border: 2px solid mistyrose; border-radius: 4px; padding: 4px; }");

    m_copperGroup = new QGroupBox("Copper");
    m_silverGroup = new QGroupBox("Silver");
    m_goldGroup   = new QGroupBox("Gold");

    QString groupBoxBase = R"(
        QGroupBox {
            background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #4850DB, stop:1 #FBA8FF);
            border: 2px solid mistyrose;
            border-radius: 10px;
            margin-top: 1.5ex;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 0 4px;
        }
    )";
    m_copperGroup->setStyleSheet(groupBoxBase + "QGroupBox::title { color: #eda15a; }");
    m_silverGroup->setStyleSheet(groupBoxBase + "QGroupBox::title { color: #C0C0C0; }");
    m_goldGroup->setStyleSheet(groupBoxBase   + "QGroupBox::title { color: #FFD700; }");

    // Populate item pools
    m_copper.addItem(Item("Hourglass Almost Empty",      ":/images/Sprites/pets/icons/hourglass.png",  "The sand runs down at exactly the rate you expect it to. That's the terrifying part.",                                                                                                 Item::Copper));
    m_copper.addItem(Item("Noise-Cancelling Headphones", ":/images/Sprites/pets/icons/headphones.png", "Blocks out everything except your own thoughts. Whether that's a feature or a bug depends entirely on your thoughts.",                                                                Item::Copper));
    m_copper.addItem(Item("Charger (Vehicle)",           ":/images/Sprites/pets/icons/charger.png",    "Zero to sixty in a time that depends on whether you meant the car or the cable. Both will leave you waiting longer than expected.",                                                   Item::Copper));
    m_copper.addItem(Item("Suspicious Cookie",           ":/images/Sprites/pets/icons/cookie.png",     "By accepting this cookie, you agree to let it remember everything about you forever. It's chocolate chip. It's always been watching.",                                               Item::Copper));
    m_copper.addItem(Item("Crypto Coin",                 ":/images/Sprites/pets/icons/coin.png",       "Worth exactly what someone else believes it's worth. Currently: disputed. The blockchain remembers, even if your portfolio doesn't.",                                                 Item::Copper));

    m_silver.addItem(Item("Conch Shell",            ":/images/Sprites/pets/icons/shell.png",      "Hold it to your ear and you'll hear the ocean. Hold it to your terminal and it'll hear you. Supports bash, zsh, and the soft crash of waves.", Item::Silver));
    m_silver.addItem(Item("Ruby Gem",               ":/images/Sprites/pets/icons/rubygem.png",    "A flawless stone, red as a raised exception. It does everything you ask, interpreted line by line, without complaint. Mostly.",                Item::Silver));
    m_silver.addItem(Item("Java Coffee",            ":/images/Sprites/pets/icons/javacoffee.png", "Strongly typed. Verbose. Takes forever to get going in the morning, but once it's running, it runs everywhere.",                               Item::Silver));
    m_silver.addItem(Item("String Spool of Thread", ":/images/Sprites/pets/icons/string.png",     "Immutable, as far as you can tell. Technically it's just a sequence of characters. Technically so is everything you've ever said.",            Item::Silver));
    m_silver.addItem(Item("Master Key",             ":/images/Sprites/pets/icons/key.png",        "Opens everything in theory. In practice, it has never been tested. The previous owner said the same thing.",                                   Item::Silver));

    m_gold.addItem(Item("Cowboy", ":/images/Sprites/pets/icons/cowboy_hat.png", "This town ain't big enough for the two of us.", Item::Gold));
    m_gold.addItem(Item("Crown",  ":/images/Sprites/pets/icons/crown_hat.png",  "All hail the PiPet royalty.",                    Item::Gold));
    m_gold.addItem(Item("Santa",  ":/images/Sprites/pets/icons/santa_hat.png",  "Ho ho ho! Have you been good this year?",         Item::Gold));
    m_gold.addItem(Item("Wizard", ":/images/Sprites/pets/icons/wizard_hat.png", "You're a wizard, PiPet.",                         Item::Gold));

    // Fill group boxes with mystery slots
    populateGroupSlots(m_copperGroup, m_copper, m_copperSlots);
    populateGroupSlots(m_silverGroup, m_silver, m_silverSlots);
    populateGroupSlots(m_goldGroup,   m_gold,   m_goldSlots);

    // Open button
    m_openBtn = new QPushButton("Open!");
    m_openBtn->setStyleSheet(R"(
        QPushButton {
            background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #0247A7, stop:1 #1B264F);
            border: 2px solid mistyrose;
            border-radius: 10px;
            padding: 8px;
        }
        QPushButton:pressed {
            background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #1B264F, stop:1 #0247A7);
        }
        QPushButton:disabled {
            background-color: rgba(60,60,80,180);
            border: 2px solid #555;
        }
    )");
    m_openBtn->setEnabled(false);
    connect(m_openBtn, &QPushButton::clicked, this, &Lootbox::onOpen);

    // Result frame — hidden until first open
    m_resultFrame = new QFrame();
    m_resultFrame->setVisible(false);

    auto *resultLayout = new QHBoxLayout(m_resultFrame);
    resultLayout->setSpacing(12);
    resultLayout->setContentsMargins(8, 8, 8, 8);

    m_resultIcon = new QLabel();
    m_resultIcon->setAlignment(Qt::AlignCenter);
    m_resultIcon->setFixedSize(64, 64);
    m_resultIcon->setStyleSheet("QLabel { border: none; background: transparent; }");

    auto *textCol  = new QVBoxLayout();
    m_resultName   = new QLabel();
    m_resultName->setWordWrap(true);
    m_resultFlavor = new QLabel();
    m_resultFlavor->setWordWrap(true);
    m_resultFlavor->setStyleSheet(
        "QLabel { border: none; background: transparent; }");

    m_resultDupe = new QLabel("You didn't win anything new!");
    m_resultDupe->setWordWrap(true);
    m_resultDupe->setStyleSheet(
        "QLabel { font-style: italic; border: none; background: transparent; }");
    m_resultDupe->setVisible(false);

    textCol->addWidget(m_resultName);
    textCol->addWidget(m_resultFlavor);
    textCol->addWidget(m_resultDupe);
    textCol->addStretch();

    resultLayout->addWidget(m_resultIcon);
    resultLayout->addLayout(textCol, 1);

    // Main layout
    m_layout = new QVBoxLayout();
    m_layout->setContentsMargins(12, 12, 12, 12);
    m_layout->setSpacing(8);
    m_layout->addWidget(m_title);
    m_layout->addWidget(m_copperGroup);
    m_layout->addWidget(m_silverGroup);
    m_layout->addWidget(m_goldGroup);
    m_layout->addWidget(m_openBtn);
    m_layout->addWidget(m_resultFrame);
    m_layout->addStretch();
    setLayout(m_layout);
}

int Lootbox::computeReward() {
    m_rewardItems.clear();
    m_lastWonIndex = -1;

    auto pickFrom = [&](Inventory &inv, Item::Rarity tier) {
        int size = static_cast<int>(inv.getItems().size());
        if (size == 0) return;
        int idx = QRandomGenerator::global()->bounded(size);
        m_rewardItems.push_back(inv.grabItemAt(idx));
        m_lastWonTier  = tier;
        m_lastWonIndex = idx;
    };

    int roll = QRandomGenerator::global()->bounded(100);
    if (roll < 70)
        pickFrom(m_copper, Item::Copper);
    else if (roll < 90)
        pickFrom(m_silver, Item::Silver);
    else
        pickFrom(m_gold,   Item::Gold);

    return static_cast<int>(m_rewardItems.size());
}

void Lootbox::awardLootbox() {
    m_pendingLootboxes++;
    player->pendingLootboxes = m_pendingLootboxes;
    PiPet p = player->getPet();
    p.increase_happiness(25);
    player->setPet(p);
    m_openBtn->setEnabled(true);
    m_openBtn->setText(m_pendingLootboxes > 1
        ? QString("Open! (%1)").arg(m_pendingLootboxes)
        : "Open!");
}

void Lootbox::restoreFromPlayer() {
    auto revealMatching = [&](QList<QLabel*> &labels, Inventory &inv) {
        auto items = inv.getItems();
        for (int i = 0; i < labels.size() && i < static_cast<int>(items.size()); ++i) {
            if (player->wonLootboxItems.contains(items[i].getName())) {
                QPixmap px(items[i].getFilepath());
                if (!px.isNull())
                    labels[i]->setPixmap(px.scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                labels[i]->setToolTip(items[i].getName());
                labels[i]->setStyleSheet("QLabel { border: none; background: transparent; }");
                labels[i]->setProperty("item_name",   items[i].getName());
                labels[i]->setProperty("item_flavor", items[i].getFlavortext());
                labels[i]->setProperty("item_path",   items[i].getFilepath());
                labels[i]->setProperty("item_rarity", static_cast<int>(items[i].getRarity()));
                labels[i]->setCursor(Qt::PointingHandCursor);
                labels[i]->installEventFilter(this);
            }
        }
    };
    revealMatching(m_copperSlots, m_copper);
    revealMatching(m_silverSlots, m_silver);
    revealMatching(m_goldSlots,   m_gold);

    m_pendingLootboxes = player->pendingLootboxes;
    if (m_pendingLootboxes > 0) {
        m_openBtn->setEnabled(true);
        m_openBtn->setText(m_pendingLootboxes > 1
            ? QString("Open! (%1)").arg(m_pendingLootboxes)
            : "Open!");
    }
}

void Lootbox::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);
    QPainter p(this);
    if (!m_bg.isNull())
        p.drawPixmap(0, 0, width(), height(), m_bg);
}

void Lootbox::onOpen() {
    if (m_pendingLootboxes <= 0) return;
    if (computeReward() == 0) return;
    m_pendingLootboxes--;
    Item won = m_rewardItems[0];

    QPixmap px(won.getFilepath());
    if (px.isNull())
        m_resultIcon->clear();
    else
        m_resultIcon->setPixmap(px.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QString rarityColor;
    switch (won.getRarity()) {
        case Item::Gold:   rarityColor = "#FFD700"; break;
        case Item::Silver: rarityColor = "#C0C0C0"; break;
        default:           rarityColor = "#eda15a"; break;
    }

    m_resultName->setText(won.getName());
    m_resultName->setStyleSheet(
        QString("QLabel { font-weight: bold; color: %1;"
                "border: none; background: transparent; }").arg(rarityColor));
    m_resultFlavor->setText(won.getFlavortext());

    m_resultFrame->setStyleSheet(
        QString("QFrame { background-color: rgba(18,8,40,200);"
                "border: 2px solid %1; border-radius: 12px; padding: 6px; }").arg(rarityColor));

    m_resultFrame->setVisible(true);

    bool isDupe = player->wonLootboxItems.contains(won.getName());
    m_resultDupe->setVisible(isDupe);

    // Persist the won item and updated pending count to player
    if (!isDupe)
        player->wonLootboxItems.append(won.getName());
    player->pendingLootboxes = m_pendingLootboxes;

    // Reveal only the winning slot; all others stay as "?"
    QList<QLabel*> *winningLabels = nullptr;
    if (m_lastWonTier == Item::Copper)      winningLabels = &m_copperSlots;
    else if (m_lastWonTier == Item::Silver) winningLabels = &m_silverSlots;
    else                                    winningLabels = &m_goldSlots;

    if (winningLabels && m_lastWonIndex >= 0 && m_lastWonIndex < winningLabels->size()) {
        QLabel *slot = (*winningLabels)[m_lastWonIndex];
        QPixmap spx(won.getFilepath());
        if (!spx.isNull())
            slot->setPixmap(spx.scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        slot->setToolTip(won.getName());
        slot->setStyleSheet("QLabel { border: none; background: transparent; }");
        slot->setProperty("item_name",   won.getName());
        slot->setProperty("item_flavor", won.getFlavortext());
        slot->setProperty("item_path",   won.getFilepath());
        slot->setProperty("item_rarity", static_cast<int>(won.getRarity()));
        slot->setCursor(Qt::PointingHandCursor);
        slot->installEventFilter(this);
    }

    if (won.getRarity() == Item::Gold)
        emit hatUnlocked(won.getName().toLower());

    if (m_pendingLootboxes <= 0) {
        m_openBtn->setEnabled(false);
        m_openBtn->setText("Open!");
    } else {
        m_openBtn->setText(m_pendingLootboxes > 1
            ? QString("Open! (%1)").arg(m_pendingLootboxes)
            : "Open!");
    }
}

bool Lootbox::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() != QEvent::MouseButtonPress)
        return QWidget::eventFilter(obj, event);

    QLabel *slot = qobject_cast<QLabel*>(obj);
    if (!slot) return QWidget::eventFilter(obj, event);

    QString name   = slot->property("item_name").toString();
    QString flavor = slot->property("item_flavor").toString();
    QString path   = slot->property("item_path").toString();
    int     rarity = slot->property("item_rarity").toInt();
    if (name.isEmpty()) return QWidget::eventFilter(obj, event);

    QString borderColor;
    switch (static_cast<Item::Rarity>(rarity)) {
        case Item::Gold:   borderColor = "#FFD700"; break;
        case Item::Silver: borderColor = "#C0C0C0"; break;
        default:           borderColor = "#eda15a"; break;
    }

    QDialog dlg(this);
    dlg.setWindowTitle(name);
    dlg.setModal(true);
    dlg.setFixedWidth(300);
    dlg.setStyleSheet(
        QString("QDialog { background-color: #120828; border: 2px solid %1; border-radius: 12px; }").arg(borderColor));

    auto *layout = new QVBoxLayout(&dlg);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(10);

    QLabel *icon = new QLabel(&dlg);
    icon->setAlignment(Qt::AlignCenter);
    QPixmap px(path);
    if (!px.isNull())
        icon->setPixmap(px.scaled(72, 72, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    layout->addWidget(icon);

    QLabel *nameLabel = new QLabel(name, &dlg);
    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setWordWrap(true);
    nameLabel->setStyleSheet(
        QString("QLabel { color: %1; background: transparent; border: none; }").arg(borderColor));
    layout->addWidget(nameLabel);

    QLabel *flavorLabel = new QLabel(flavor, &dlg);
    flavorLabel->setAlignment(Qt::AlignCenter);
    flavorLabel->setWordWrap(true);
    flavorLabel->setStyleSheet(
        "QLabel { font-style: italic; background: transparent; border: none; }");
    layout->addWidget(flavorLabel);

    QPushButton *closeBtn = new QPushButton("Close", &dlg);
    closeBtn->setStyleSheet(
        QString("QPushButton { background-color: rgba(72,50,180,200);"
                "border: 1px solid %1; border-radius: 6px; padding: 6px; }"
                "QPushButton:pressed { background-color: rgba(100,70,220,200); }").arg(borderColor));
    connect(closeBtn, &QPushButton::clicked, &dlg, &QDialog::accept);
    layout->addWidget(closeBtn);

    dlg.exec();
    return true;
}
