/*
 * groom.cc - Groom screen two-spot drag mechanic.
 * Author(s): Tanya Magurupira
 */
#include "groom.h"
#include <QPainter>

// ── GroomTool ─────────────────────────────────────────────────────────────

GroomTool::GroomTool(const QString &iconPath, const QString &name,
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
    setStyleSheet("QLabel { background-color: rgba(0,0,0,130);"
                  "border-radius: 10px; color: white; font-size: 11px; }");
}

void GroomTool::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        m_dragging = true;
        m_offset   = e->position().toPoint();
        raise();
    }
}

void GroomTool::mouseMoveEvent(QMouseEvent *e) {
    if (m_dragging) {
        QPoint newPos = parentWidget()->mapFromGlobal(
                            e->globalPosition().toPoint()) - m_offset;
        move(newPos);
    }
}

void GroomTool::mouseReleaseEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton && m_dragging) {
        m_dragging = false;
        emit dropped(this, e->globalPosition().toPoint());
    }
}

// ── Groom ─────────────────────────────────────────────────────────────────

Groom::Groom(Player *player, QWidget *parent)
    : QWidget{parent}, player(player)
{
    m_bg.load(":/images/Backgrounds/bathroom_16bit.jpg");

    character = new Character(this);
    character->setFixedSize(150, 150);
    character->syncWithPlayer(*player, Character::DragonDog);

    hygieneDisplay = new QLabel(this);
    hygieneDisplay->setAlignment(Qt::AlignCenter);
    hygieneDisplay->setWordWrap(true);
    hygieneDisplay->setStyleSheet(
        "QLabel { background-color: rgba(0,0,0,170); border-radius: 8px;"
        "padding: 6px; color: mistyrose; font-size: 15px; }");
    hygieneDisplay->setFixedWidth(300);
    updateHygieneDisplay();

    hintLabel = new QLabel("Drag a tool to spot 1 then spot 2!", this);
    hintLabel->setAlignment(Qt::AlignCenter);
    hintLabel->setStyleSheet(
        "QLabel { background-color: rgba(0,0,0,140); border-radius: 6px;"
        "padding: 4px; color: #ffd700; font-size: 13px; }");
    hintLabel->setFixedWidth(300);

    brushTool = new GroomTool(":/images/Sprites/pets/icons/comb.png",       "Brush",       this);
    batheTool = new GroomTool(":/images/Sprites/pets/icons/soap.png",       "Bathe",       this);
    trimTool  = new GroomTool(":/images/Sprites/pets/icons/showerhead.png", "Trim Nails",  this);
    teethTool = new GroomTool(":/images/Sprites/pets/icons/comb.png",       "Brush Teeth", this);

    connect(brushTool, &GroomTool::dropped, this, &Groom::onToolDropped);
    connect(batheTool, &GroomTool::dropped, this, &Groom::onToolDropped);
    connect(trimTool,  &GroomTool::dropped, this, &Groom::onToolDropped);
    connect(teethTool, &GroomTool::dropped, this, &Groom::onToolDropped);

    backBtn = new QPushButton("Back to Care Hub!", this);
    backBtn->setIcon(QIcon(":/images/Assets/left.png"));
    backBtn->setStyleSheet(R"(
        QPushButton { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4850DB, stop:1 #4A71DB);
            border: 2px inset #FBA8FF; border-radius: 10px;
            padding: 8px; font: bold; color: mistyrose; }
        QPushButton:pressed { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4A71DB, stop:1 #4850DB); })");

    m_resetTimer = new QTimer(this);
    m_resetTimer->setSingleShot(true);
    m_resetTimer->setInterval(1800);
    connect(m_resetTimer, &QTimer::timeout, this, &Groom::resetSpots);
}

void Groom::resizeEvent(QResizeEvent *e) {
    QWidget::resizeEvent(e);
    int w = width(), h = height();
    int petSize = 150;
    character->setGeometry((w - petSize) / 2, 55, petSize, petSize);
    hygieneDisplay->setGeometry((w - 300) / 2, 55 + petSize + 6,  300, 38);
    hintLabel->setGeometry     ((w - 300) / 2, 55 + petSize + 50, 300, 30);
    backBtn->setGeometry((w - 220) / 2, h - 55, 220, 40);
    placeTools();
}

void Groom::placeTools() {
    int w = width(), h = height();
    int iconW = 64, spacing = 18;
    int totalW = 4 * iconW + 3 * spacing;
    int startX = (w - totalW) / 2;
    int y = h - 115;
    QList<GroomTool*> tools = {brushTool, batheTool, trimTool, teethTool};
    for (int i = 0; i < tools.size(); i++) {
        int x = startX + i * (iconW + spacing);
        tools[i]->move(x, y);
        tools[i]->homePos = QPoint(x, y);
        tools[i]->raise();
    }
    backBtn->raise();
}

void Groom::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);
    QPainter p(this);
    if (!m_bg.isNull())
        p.drawPixmap(0, 0, width(), height(), m_bg);

    // Top spot
    QRect ts = topSpot();
    QColor tc = topDone ? QColor(80, 255, 120, 200) : QColor(255, 220, 50, 140);
    p.setPen(QPen(tc, 4, Qt::DashLine));
    p.setBrush(QColor(tc.red(), tc.green(), tc.blue(), 55));
    p.drawEllipse(ts);
    p.setPen(QColor(255, 255, 200, 220));
    p.setFont(QFont("monospace", 11, QFont::Bold));
    p.drawText(ts, Qt::AlignCenter, topDone ? "✓" : "1");

    // Bottom spot
    QRect bs = bottomSpot();
    QColor bc = bottomDone ? QColor(80, 255, 120, 200) : QColor(255, 220, 50, 140);
    p.setPen(QPen(bc, 4, Qt::DashLine));
    p.setBrush(QColor(bc.red(), bc.green(), bc.blue(), 55));
    p.drawEllipse(bs);
    p.setPen(QColor(255, 255, 200, 220));
    p.drawText(bs, Qt::AlignCenter, bottomDone ? "✓" : "2");
}

QRect Groom::topSpot() const {
    QRect pet = character->geometry();
    return QRect(pet.center().x() - 26, pet.top() + 22 - 26, 52, 52);
}

QRect Groom::bottomSpot() const {
    QRect pet = character->geometry();
    return QRect(pet.center().x() - 26, pet.bottom() - 22 - 26, 52, 52);
}

void Groom::onToolDropped(GroomTool *tool, QPoint globalPos) {
    QPoint local = mapFromGlobal(globalPos);

    if (activeTool && activeTool != tool) {
        topDone = bottomDone = false;
        activeTool = nullptr;
    }
    activeTool = tool;

    if (topSpot().contains(local) && !topDone) {
        topDone = true;
        hintLabel->setText("Spot 1 done! Now hit spot 2!");
    } else if (bottomSpot().contains(local) && !bottomDone) {
        bottomDone = true;
        hintLabel->setText("Spot 2 done! Now hit spot 1!");
    }

    if (topDone && bottomDone) {
        applyGroomAction(tool->toolName + "!");
        topDone = bottomDone = false;
        activeTool = nullptr;
        m_resetTimer->start();
        hintLabel->setText("Drag a tool to spot 1 then spot 2!");
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
