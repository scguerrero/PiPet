/*
 * feed.cc - Feed screen drag and drop.
 * Author(s): Luke Cewin & Sasha Guerrero
 */
#include "feed.h"
#include <QPainter>
#include <QRandomGenerator>

static constexpr int kSpriteSize = 160;
static constexpr int kHitRadius  = kSpriteSize / 2;

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
        m_offset = e->pos();
        setCursor(Qt::ClosedHandCursor);
        raise();
    }
}

void FoodItem::mouseMoveEvent(QMouseEvent *e) {
    if (m_dragging)
        move(parentWidget()->mapFromGlobal(e->globalPos()) - m_offset);
}

void FoodItem::mouseReleaseEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton && m_dragging) {
        m_dragging = false;
        setCursor(Qt::OpenHandCursor);
        emit dropped(this, e->globalPos());
    }
}

bool FoodItem::event(QEvent *e) {
    switch (e->type()) {

    case QEvent::TouchBegin: {
        auto *te  = static_cast<QTouchEvent *>(e);
        auto pts = te->touchPoints();
        if (!pts.isEmpty()) {
            m_dragging = true;
            m_offset = e->pos();
            setCursor(Qt::ClosedHandCursor);
            raise();
        }
        return true;
    }

    case QEvent::TouchUpdate: {
        auto *te  = static_cast<QTouchEvent *>(e);
        auto pts = te->touchPoints();
        if (m_dragging && !pts.isEmpty()) {
            QPoint globalPos = pts.first().screenPos().toPoint();
            QPoint newPos    = parentWidget()->mapFromGlobal(globalPos) - m_offset;
            move(newPos);
        }
        return true;
    }

    case QEvent::TouchEnd: {
        auto *te  = static_cast<QTouchEvent *>(e);
        auto pts = te->touchPoints();
        if (m_dragging) {
            m_dragging = false;
            setCursor(Qt::OpenHandCursor);
            QPoint globalPos = pts.isEmpty()
                                   ? mapToGlobal(rect().center())
                                   : pts.first().screenPos().toPoint();
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

    // ── Food tray group box (visual backdrop only — icons are free children) ─
    actionsBox = new QGroupBox("Drag A food to Feed your PiPet!", this);
    actionsBox->setStyleSheet(
        "QGroupBox { background-color: rgba(0,0,0,155); border-radius: 8px;"
        "color: mistyrose; margin-top: 30px; }"
        "QGroupBox::title { color: mistyrose; subcontrol-origin: margin;"
        "subcontrol-position: top center; padding: 0 4px; }");
    actionsBox->lower();

    m_crumbTimer = new QTimer(this);
    m_crumbTimer->setInterval(30);
    connect(m_crumbTimer, &QTimer::timeout, this, &Feed::tickCrumbs);
}

void Feed::showEvent(QShowEvent *e) {
    QWidget::showEvent(e);
    QResizeEvent re(size(), size());
    resizeEvent(&re);
}

QPoint Feed::spriteCenter() const {
    return character->geometry().center();
}

QRect Feed::characterHitbox() const {
    QPoint c = spriteCenter();
    return QRect(c.x() - kHitRadius, c.y() - kHitRadius,
                 kHitRadius * 2,     kHitRadius * 2);
}

void Feed::resizeEvent(QResizeEvent *e) {
    QWidget::resizeEvent(e);
    int w = width(), h = height();
    int petY = 200;
    int petX = (w - kSpriteSize) / 2;
    character->setGeometry(petX, petY, kSpriteSize, kSpriteSize);
    // Hunger display sits just above the actionsBox
    hungerDisplay->setGeometry((w - 300) / 2, h - 170, 300, 38);
    // actionsBox stretches full width with 8px side margins
    actionsBox->setGeometry(8, h - 130, w - 16, 122);
    placeIcons();
}

void Feed::placeIcons() {
    int w = width(), h = height();
    int iconW = 64, spacing = 18;
    int totalW = 4 * iconW + 3 * spacing;
    int startX = (w - totalW) / 2;
    int y = h - 88;   // centered inside actionsBox
    QList<FoodItem*> icons = {appleItem, boneItem, drinkItem, pizzaItem};
    for (int i = 0; i < icons.size(); i++) {
        int x = startX + i * (iconW + spacing);
        icons[i]->move(x, y);
        icons[i]->homePos = QPoint(x, y);
        icons[i]->raise();
    }
    actionsBox->lower();
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

    // Hint ring — drawn at exact character center matching the hit zone
    QPoint sc = spriteCenter();
    p.setPen(QPen(QColor(255, 100, 30, 90), 3, Qt::DashLine));
    p.setBrush(Qt::NoBrush);
    p.drawEllipse(QPointF(sc), static_cast<qreal>(kHitRadius),
                               static_cast<qreal>(kHitRadius));
}

void Feed::onFoodDropped(FoodItem *icon, QPoint globalPos) {
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
        QString("Hunger: %1 / 100").arg(player->getPet().hunger()));
}

void Feed::applyHungerAction(int boost, const QString &message) {
    PiPet pet = player->getPet();
    if (pet.hunger() >= 100) { hungerDisplay->setText("Your pet is already full!"); return; }
    int newVal = qMin(100, pet.hunger() + boost);
    pet.set_hunger(newVal);
    player->setPet(pet);
    spawnCrumbs(spriteCenter());
    hungerDisplay->setText(QString("%1  Hunger: %2 / 100").arg(message).arg(newVal));
}

// ── Hat-aware character refresh ───────────────────────────────────────────
// Called by game.cc each time the feed screen is opened so the equipped
// hat (if any) is always visible on the character sprite.

void Feed::refreshCharacter() {
    QString hat = player->getPet().hat();
    if (hat.isEmpty()) {
        character->syncWithPlayer(*player, petType);
        return;
    }

    QString folder, prefix;
    QString type = player->getPet().pet_type();
    if      (type == "ElectricAxolotl") { folder = "axolotl";   prefix = "axolotl";   }
    else if (type == "SeelCat")         { folder = "seelcat";   prefix = "seelcat";   }
    else                                { folder = "dragondog"; prefix = "dragondog"; }

    QString stage = player->getPet().age_group();
    QString infix = (stage == "Teen") ? "teen_" : (stage == "Adult") ? "adult_" : "";
    QString path  = QString(":/images/Sprites/pets/%1/%2_%3%4.gif")
                        .arg(folder, prefix, infix, hat);

    QMovie *movie = new QMovie(path, QByteArray(), character);
    if (movie->isValid()) {
        QLabel *disp = character->findChild<QLabel *>();
        if (disp) {
            if (disp->movie()) disp->movie()->stop();
            disp->setMovie(movie);
            movie->start();
            return;
        }
    }
    delete movie;
    character->syncWithPlayer(*player, petType);
}
