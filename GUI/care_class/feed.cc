/*
 * feed.cc - Feed screen drag and drop.
 * Author(s): Luke Cewin & Sasha Guerrero
 */
#include "feed.h"
#include <QPainter>
#include <QRandomGenerator>

// ── Sprite geometry ───────────────────────────────────────────────────────
// All pet GIFs are exactly 160×160 px and the character widget matches that
// size, so geometry().center() is always the true visual centre regardless
// of which pet is loaded.  One constant radius drives both the hit-test and
// the eating-bubble ring so they are always in sync.
static constexpr int kSpriteSize   = 160;   // must match character->setFixedSize
static constexpr int kHitRadius    = kSpriteSize / 2;   // 80 px — full sprite circle

// ── FoodItem ──────────────────────────────────────────────────────────────

FoodItem::FoodItem(const QString &iconPath, const QString &name,
                   int boost, QWidget *parent)
    : QLabel(parent), foodName(name), hungerBoost(boost)
{
    QPixmap px(iconPath);
    if (!px.isNull())
        setPixmap(px.scaled(56, 56, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else
        setText(name);
    setFixedSize(64, 64);
    setAlignment(Qt::AlignCenter);
    setCursor(Qt::OpenHandCursor);
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    setStyleSheet("QLabel { background-color: rgba(0,0,0,130);"
                  "border-radius: 10px; color: white; font-size: 11px; }");
    setAttribute(Qt::WA_AcceptTouchEvents);
    grabGesture(Qt::TapAndHoldGesture);
}

void FoodItem::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        m_dragging = true;
        m_offset   = e->position().toPoint();
        setCursor(Qt::ClosedHandCursor);
        raise();
    }
}

void FoodItem::mouseMoveEvent(QMouseEvent *e) {
    if (m_dragging) {
        QPoint globalPos = e->globalPosition().toPoint();
        QPoint newPos    = parentWidget()->mapFromGlobal(globalPos) - m_offset;
        move(newPos);
    }
}

void FoodItem::mouseReleaseEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton && m_dragging) {
        m_dragging = false;
        setCursor(Qt::OpenHandCursor);
        emit dropped(this, e->globalPosition().toPoint());
    }
}

bool FoodItem::event(QEvent *e) {
    switch (e->type()) {

    case QEvent::TouchBegin: {
        auto *te  = static_cast<QTouchEvent *>(e);
        auto  pts = te->points();
        if (!pts.isEmpty()) {
            m_dragging = true;
            m_offset   = pts.first().position().toPoint();
            setCursor(Qt::ClosedHandCursor);
            raise();
        }
        return true;
    }

    case QEvent::TouchUpdate: {
        auto *te  = static_cast<QTouchEvent *>(e);
        auto  pts = te->points();
        if (m_dragging && !pts.isEmpty()) {
            QPoint globalPos = pts.first().globalPosition().toPoint();
            QPoint newPos    = parentWidget()->mapFromGlobal(globalPos) - m_offset;
            move(newPos);
        }
        return true;
    }

    case QEvent::TouchEnd: {
        auto *te  = static_cast<QTouchEvent *>(e);
        auto  pts = te->points();
        if (m_dragging) {
            m_dragging = false;
            setCursor(Qt::OpenHandCursor);
            QPoint globalPos = pts.isEmpty()
                                   ? mapToGlobal(rect().center())
                                   : pts.first().globalPosition().toPoint();
            emit dropped(this, globalPos);
        }
        return true;
    }

    default:
        return QLabel::event(e);
    }
}

// ── Feed ──────────────────────────────────────────────────────────────────

Feed::Feed(Player *player, Character::PetType petType, QWidget *parent)
    : QWidget{parent}, player(player), petType(petType)
{
    m_bg.load(":/images/Backgrounds/kitchen_16bit.png");

    character = new Character(this);
    // kSpriteSize drives both the widget size and the hit geometry —
    // change the constant once and everything stays in sync.
    character->setFixedSize(kSpriteSize, kSpriteSize);
    character->syncWithPlayer(*player, petType);

    hungerDisplay = new QLabel(this);
    hungerDisplay->setAlignment(Qt::AlignCenter);
    hungerDisplay->setWordWrap(true);
    hungerDisplay->setStyleSheet(
        "QLabel { background-color: rgba(0,0,0,170); border-radius: 8px;"
        "padding: 6px; color: mistyrose; font-size: 15px; }");
    hungerDisplay->setFixedWidth(300);
    updateHungerDisplay();

    appleItem = new FoodItem(":/images/Sprites/pets/icons/apple.png",    "Apple", 10, this);
    boneItem  = new FoodItem(":/images/Sprites/pets/icons/dogtreat.png", "Bone",  10, this);
    drinkItem = new FoodItem(":/images/Sprites/pets/icons/drink.png",    "Drink",  5, this);
    pizzaItem = new FoodItem(":/images/Sprites/pets/icons/pizza.png",    "Pizza",  5, this);

    connect(appleItem, &FoodItem::dropped, this, &Feed::onFoodDropped);
    connect(boneItem,  &FoodItem::dropped, this, &Feed::onFoodDropped);
    connect(drinkItem, &FoodItem::dropped, this, &Feed::onFoodDropped);
    connect(pizzaItem, &FoodItem::dropped, this, &Feed::onFoodDropped);

    backBtn = new QPushButton("Back to Care Hub!", this);
    backBtn->setIcon(QIcon(":/images/Assets/left.png"));
    backBtn->setStyleSheet(R"(
        QPushButton { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4850DB, stop:1 #4A71DB);
            border: 2px inset #FBA8FF; border-radius: 10px;
            padding: 8px; font: bold; color: mistyrose; }
        QPushButton:pressed { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4A71DB, stop:1 #4850DB); })");

    m_crumbTimer = new QTimer(this);
    m_crumbTimer->setInterval(30);
    connect(m_crumbTimer, &QTimer::timeout, this, &Feed::tickCrumbs);
    if (width() > 0 && height() > 0)
        resizeEvent(nullptr);
}

// ── spriteCenter ──────────────────────────────────────────────────────────
// Single source of truth for the visual centre of the pet sprite.
// Because every GIF is kSpriteSize × kSpriteSize and the widget matches,
// geometry().center() is always exact — no per-asset tweaking needed.
QPoint Feed::spriteCenter() const {
    qDebug() << "character geometry:" << character->geometry();
    qDebug() << "spriteCenter:" << character->geometry().center();
    return character->geometry().center();
}

// ── characterHitbox ───────────────────────────────────────────────────────
// Circular hit zone built from spriteCenter() + kHitRadius.
// Represented as a QRect whose inscribed circle has radius kHitRadius.
QRect Feed::characterHitbox() const {
    QPoint c = spriteCenter();
    return QRect(c.x() - kHitRadius, c.y() - kHitRadius,
                 kHitRadius * 2,     kHitRadius * 2);
}

void Feed::resizeEvent(QResizeEvent *e) {
    QWidget::resizeEvent(e);
    int w = width(), h = height();
    // Centre the sprite widget; top edge at y=55
    character->setGeometry((w - kSpriteSize) / 2, 55, kSpriteSize, kSpriteSize);
    hungerDisplay->setGeometry((w - 300) / 2, 55 + kSpriteSize + 8, 300, 38);
    backBtn->setGeometry((w - 220) / 2, h - 55, 220, 40);
    placeIcons();
}

void Feed::placeIcons() {
    int w = width(), h = height();
    int iconW = 64, spacing = 18;
    int totalW = 4 * iconW + 3 * spacing;
    int startX = (w - totalW) / 2;
    int y = h - 115;
    QList<FoodItem*> icons = {appleItem, boneItem, drinkItem, pizzaItem};
    for (int i = 0; i < icons.size(); i++) {
        int x = startX + i * (iconW + spacing);
        icons[i]->move(x, y);
        icons[i]->homePos = QPoint(x, y);
        icons[i]->raise();
    }
    backBtn->raise();
}

void Feed::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);
    QPainter p(this);
    if (!m_bg.isNull())
        p.drawPixmap(0, 0, width(), height(), m_bg);

    for (const Crumb &c : m_crumbs) {
        QColor col(210, 160, 80, qMin(255, c.life * 14));
        p.setBrush(col);
        p.setPen(Qt::NoPen);
        p.drawEllipse(QPointF(c.pos), 4, 4);
    }

    // Hint ring — drawn as a circle centred exactly on spriteCenter()
    // with radius kHitRadius so it matches the hit zone perfectly.
    QPoint  sc = spriteCenter();
    QPointF cf(sc);
    p.setPen(QPen(QColor(255, 100, 30, 90), 3, Qt::DashLine));
    p.setBrush(Qt::NoBrush);
    p.drawEllipse(cf, static_cast<qreal>(kHitRadius),
                      static_cast<qreal>(kHitRadius));
}

void Feed::onFoodDropped(FoodItem *icon, QPoint globalPos) {
    // Hit-test: check whether the drop point falls inside the circular zone.
    // Using QRect::contains is a square approximation; the circle check below
    // is exact and matches the ring drawn in paintEvent.
    QPoint local = mapFromGlobal(globalPos);
    QPoint sc    = spriteCenter();
    int dx = local.x() - sc.x();
    int dy = local.y() - sc.y();
    if (dx * dx + dy * dy <= kHitRadius * kHitRadius)
        applyHungerAction(icon->hungerBoost, icon->foodName + "!");
    icon->move(icon->homePos);
    update();
}

void Feed::spawnCrumbs(QPoint center) {
    for (int i = 0; i < 16; i++) {
        Crumb c;
        c.pos  = center;
        double angle = QRandomGenerator::global()->bounded(360) * M_PI / 180.0;
        double speed = QRandomGenerator::global()->bounded(3, 10);
        c.vel  = QPointF(qCos(angle) * speed, qSin(angle) * speed);
        c.life = QRandomGenerator::global()->bounded(14, 24);
        m_crumbs.append(c);
    }
    if (!m_crumbTimer->isActive()) m_crumbTimer->start();
}

void Feed::tickCrumbs() {
    for (int i = m_crumbs.size() - 1; i >= 0; i--) {
        m_crumbs[i].pos  += m_crumbs[i].vel;
        m_crumbs[i].vel.setY(m_crumbs[i].vel.y() + 0.5);
        if (--m_crumbs[i].life <= 0) m_crumbs.removeAt(i);
    }
    if (m_crumbs.isEmpty()) m_crumbTimer->stop();
    update();
}

void Feed::updateHungerDisplay() {
    hungerDisplay->setText(
        QString("Hunger: %1 / 100 ")
            .arg(player->getPet().hunger()));
}

void Feed::applyHungerAction(int boost, const QString &message) {
    PiPet pet = player->getPet();
    if (pet.hunger() >= 100) { hungerDisplay->setText("Your pet is already full!"); return; }
    int newVal = qMin(100, pet.hunger() + boost);
    pet.set_hunger(newVal);
    player->setPet(pet);
    spawnCrumbs(spriteCenter());   // always use the shared helper
    hungerDisplay->setText(QString("%1  Hunger: %2 / 100").arg(message).arg(newVal));
}
