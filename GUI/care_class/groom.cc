/*
 * groom.cc - Groom screen two-spot drag mechanic.
 * Fixed: bottomSpot() was calculating outside visible area.
 * Author(s): Luke Cerwin
 */
#include "groom.h"
#include <QPainter>
#include <QGroupBox>
static constexpr int kSpriteSize = 160;

// - GroomItem
GroomItem::GroomItem(const QString &iconPath, const QString &name,
                     QWidget *parent)
    : QLabel(parent), toolName(name)
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

void GroomItem::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        m_dragging = true;
        m_offset   = e->pos();
        setCursor(Qt::ClosedHandCursor);
        raise();
    }
}

void GroomItem::mouseMoveEvent(QMouseEvent *e) {
    if (m_dragging) {
        QPoint globalPos = e->globalPos();
        QPoint newPos    = parentWidget()->mapFromGlobal(globalPos) - m_offset;
        move(newPos);
    }
}

void GroomItem::mouseReleaseEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton && m_dragging) {
        m_dragging = false;
        setCursor(Qt::OpenHandCursor);
        emit dropped(this, e->globalPos());
    }
}

bool GroomItem::event(QEvent *e) {
    switch (e->type()) {

    case QEvent::TouchBegin: {
        auto *te  = static_cast<QTouchEvent *>(e);
        auto  pts = te->touchPoints();
        if (!pts.isEmpty()) {
            m_dragging = true;
            m_offset   = pts.first().pos().toPoint();
            setCursor(Qt::ClosedHandCursor);
            raise();
        }
        return true;   // must return true or TouchUpdate won't fire
    }

    case QEvent::TouchUpdate: {
        auto *te  = static_cast<QTouchEvent *>(e);
        auto  pts = te->touchPoints();
        if (m_dragging && !pts.isEmpty()) {
            QPoint globalPos = pts.first().screenPos().toPoint();
            QPoint newPos    = parentWidget()->mapFromGlobal(globalPos) - m_offset;
            move(newPos);
        }
        return true;
    }

    case QEvent::TouchEnd: {
        auto *te  = static_cast<QTouchEvent *>(e);
        auto  pts = te->touchPoints();
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

// - Groom
Groom::Groom(Player *player, Character::PetType petType, QWidget *parent)
    : QWidget{parent}, player(player), petType(petType)
{
    m_bg.load(":/images/Backgrounds/bathroom_16bit.png");

    character = new Character(this);
    character->setFixedSize(kSpriteSize, kSpriteSize);
    character->syncWithPlayer(*player, petType);

    //Hygiene display - sits above the actionsBox
    hygieneDisplay = new QLabel(this);
    hygieneDisplay->setAlignment(Qt::AlignCenter);
    hygieneDisplay->setWordWrap(true);
    hygieneDisplay->setStyleSheet(
        "QLabel { background-color: rgba(0,0,0,170); border-radius: 8px;"
        "padding: 6px; color: mistyrose; font-size: 15px; }");
    updateHygieneDisplay();

    // - Info helper: shown on open, hides after 3 seconds
    infoHelper = new QLabel(this);
    infoHelper->setAlignment(Qt::AlignCenter);
    infoHelper->setWordWrap(true);
    infoHelper->setStyleSheet(
        "QLabel { background-color: rgba(0,0,0,170); border-radius: 8px;"
        "padding: 6px; color: mistyrose; font-size: 15px; }");
    infoHelper->setFixedWidth(300);
    infoHelper->setText("Drag a tool onto spots 1 & 2 to groom your pet!");
    infoHelper->hide();

    m_infoTimer = new QTimer(this);
    m_infoTimer->setSingleShot(true);
    connect(m_infoTimer, &QTimer::timeout, infoHelper, &QLabel::hide);

    //Hint label: pops up below the character, hidden by default
    hintLabel = new QLabel(this);
    hintLabel->setAlignment(Qt::AlignCenter);
    hintLabel->setStyleSheet(
        "QLabel { background-color: rgba(0,0,0,140); border-radius: 6px;"
        "padding: 4px; color: #ffd700; font-size: 13px; }");
    hintLabel->setFixedWidth(300);
    hintLabel->hide();

    //Hint auto-hide timer (3 seconds)
    m_hintTimer = new QTimer(this);
    m_hintTimer->setSingleShot(true);
    m_hintTimer->setInterval(3000);
    connect(m_hintTimer, &QTimer::timeout, this, [this]() {
        hintLabel->hide();
    });

    brushTool = new GroomItem(":/images/Sprites/pets/icons/comb.png",
                              "Brush",       this);
    batheTool = new GroomItem(":/images/Sprites/pets/icons/soap.png",
                              "Scrub Scrub!",       this);
    trimTool  = new GroomItem(":/images/Sprites/pets/icons/showerhead.png",
                              "Rinsed off!",            this);
    teethTool = new GroomItem(":/images/Sprites/pets/icons/toothbrush.png",
                              "Brush Teeth", this);

    connect(brushTool, &GroomItem::dropped, this, &Groom::onToolDropped);
    connect(batheTool, &GroomItem::dropped, this, &Groom::onToolDropped);
    connect(trimTool,  &GroomItem::dropped, this, &Groom::onToolDropped);
    connect(teethTool, &GroomItem::dropped, this, &Groom::onToolDropped);

    m_resetTimer = new QTimer(this);
    m_resetTimer->setSingleShot(true);
    m_resetTimer->setInterval(1800);
    connect(m_resetTimer, &QTimer::timeout, this, &Groom::resetSpots);
    actionsBox = new QGroupBox("■‿■", this);
    actionsBox->setStyleSheet(
        "QGroupBox { background-color: rgba(0,0,0,155); border-radius: 8px;"
        "color: mistyrose; margin-top: 30px; }"
        "QGroupBox::title { color: mistyrose; subcontrol-origin: margin;"
        "subcontrol-position: top center; padding: 0 4px; }");
    actionsBox->lower();
}

void Groom::showEvent(QShowEvent *e) {
    QWidget::showEvent(e);

    // Position info helper near the top center
    infoHelper->setGeometry((width() - 300) / 2, 20, 300, 50);
    infoHelper->show();
    infoHelper->raise();
    m_infoTimer->start(3000);
}

void Groom::resizeEvent(QResizeEvent *e) {
    QWidget::resizeEvent(e);
    int w = width(), h = height();
    int petY = 250;
    int petX = (w - kSpriteSize) / 2;
    // FIX: set character geometry here so topSpot/bottomSpot are always correct
    character->setGeometry(petX, petY, kSpriteSize, kSpriteSize);
    hintLabel->setGeometry((w - 300) / 2, 80, 300, 30);
    hygieneDisplay->setGeometry((w - 360) / 2, h - 170, 360, 38);
    actionsBox->setGeometry(8, h - 130, w - 16, 122);
    infoHelper->setGeometry((w - 300) / 2, 20, 300, 50);
    placeTools();
}

void Groom::placeTools() {
    int w = width(), h = height();
    int iconW = 64, spacing = 18;
    int totalW = 4 * iconW + 3 * spacing;
    int startX = (w - totalW) / 2;
    int y = h - 88;   // centered inside actionsBox

    QList<GroomItem*> tools = {brushTool, batheTool, trimTool, teethTool};
    for (int i = 0; i < tools.size(); i++) {
        int x = startX + i * (iconW + spacing);
        tools[i]->move(x, y);
        tools[i]->homePos = QPoint(x, y);
        tools[i]->raise();
    }
    actionsBox->lower();
}

void Groom::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);
    QPainter p(this);
    if (!m_bg.isNull())
        p.drawPixmap(0, 0, width(), height(), m_bg);

    // Top spot
    QRect ts = topSpot();
    QColor tc = topDone ? QColor(80, 255, 120, 200)
                        : QColor(255, 220, 50,  160);
    p.setPen(QPen(tc, 4, Qt::DashLine));
    p.setBrush(QColor(tc.red(), tc.green(), tc.blue(), 60));
    p.drawEllipse(ts);
    p.setPen(QColor(255, 255, 200, 230));
    p.setFont(QFont("monospace", 12, QFont::Bold));
    p.drawText(ts, Qt::AlignCenter, topDone ? "✓" : "1");

    // Bottom spot
    QRect bs = bottomSpot();
    QColor bc = bottomDone ? QColor(80, 300, 120, 200)
                           : QColor(255, 220, 50, 160);
    p.setPen(QPen(bc, 4, Qt::DashLine));
    p.setBrush(QColor(bc.red(), bc.green(), bc.blue(), 60));
    p.drawEllipse(bs);
    p.setPen(QColor(255, 255, 200, 230));
    p.setFont(QFont("monospace", 12, QFont::Bold));
    p.drawText(bs, Qt::AlignCenter, bottomDone ? "✓" : "2");
}

// FIX: spots are now calculated relative to character geometry,
QRect Groom::topSpot() const {
    QRect pet = character->geometry();
    int cx = pet.center().x();
    int cy = pet.top() + (pet.height() / 4);
    return QRect(cx - 28, cy - 28, 56, 56);
}

QRect Groom::bottomSpot() const {
    QRect pet = character->geometry();
    int cx = pet.center().x();
    int cy = pet.top() + (pet.height() * 3 / 4);
    return QRect(cx - 28, cy - 28, 56, 56);
}

void Groom::showHint(const QString &text) {
    hintLabel->setText(text);
    hintLabel->show();
    hintLabel->raise();
    m_hintTimer->start();
}

void Groom::onToolDropped(GroomItem *tool, QPoint globalPos) {
    QPoint local = mapFromGlobal(globalPos);

    if (activeTool && activeTool != tool) {
        topDone = bottomDone = false;
        activeTool = nullptr;
    }
    activeTool = tool;

    if (!topDone && topSpot().contains(local)) {
        topDone = true;
        showHint("Spot 1 done!  Now hit Spot 2!");
    } else if (!bottomDone && bottomSpot().contains(local)) {
        bottomDone = true;
        showHint("Spot 2 done!  Now hit Spot 1!");
    }

    if (topDone && bottomDone) {
        applyGroomAction(tool->toolName + "!");
        topDone = bottomDone = false;
        activeTool = nullptr;
        m_resetTimer->start();
        // No hint shown here — hygieneDisplay already gives feedback
        hintLabel->hide();
        m_hintTimer->stop();
    }

    tool->move(tool->homePos);
    update();
}

void Groom::resetSpots() {
    topDone = bottomDone = false;
    activeTool = nullptr;
    update();
}

void Groom::updateHygieneDisplay() {
    hygieneDisplay->setText(
        QString("Hygiene: %1 / 100").arg(player->pet.hygiene()));
}

void Groom::applyGroomAction(const QString &message) {
    if (player->pet.hygiene() >= 100) {
        hygieneDisplay->setText("Your pet is already clean!");
        return;
    }
    player->pet.increase_hygiene(5);
    hygieneDisplay->setText(
        QString("%1  Hygiene: %2 / 100").arg(message).arg(player->pet.hygiene()));
}

// Hat-aware character refresh
// Called by game.cc each time the groom screen is opened so the equipped

void Groom::refreshCharacter() {
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
