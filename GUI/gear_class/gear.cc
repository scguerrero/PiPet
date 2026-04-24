/*
 * Gear mode — stage screen implementation. Draggable sequences for hat decorations and achievements page
 *
 * Author(s): Luke Cerwin
 */

#include "gear.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QTimer>
#include <QScroller>


//  HatCard
HatCard::HatCard(const QString &hatKey, const QString &iconPath, QWidget *parent)
    : QLabel(parent), m_key(hatKey), m_iconPath(iconPath)
{
    setFixedSize(110, 110);
    setAlignment(Qt::AlignCenter);
    setScaledContents(false);

    loadIcon();

    setCursor(Qt::PointingHandCursor);
    applyStyle();
}

void HatCard::loadIcon() {
    if (m_iconPath.isEmpty()) { setText("\u2715"); return; }  // ✕
    QImage img(m_iconPath);
    if (!img.isNull()) {
        QPixmap px = QPixmap::fromImage(
            img.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        setPixmap(px);
        setText("");
    } else {
        setText(m_key);
    }
}

void HatCard::setSelected(bool sel) {
    m_selected = sel;
    applyStyle();
}

void HatCard::setLocked(bool locked) {
    m_locked = locked;
    applyStyle();
}

void HatCard::applyStyle() {
    if (m_locked) {
        setStyleSheet(R"(
            QLabel {
                background-color: rgba(20,20,20,180);
                border: 2px solid rgba(100,100,100,100);
                border-radius: 12px;
                color: #555;
                font-size: 22px;
            }
        )");
        // Overlay
        setText("🔒");
        return;
    }
    loadIcon();
    if (m_selected) {
        setStyleSheet(R"(
            QLabel {
                background-color: rgba(72,80,219,200);
                border: 2px solid #FBA8FF;
                border-radius: 12px;
            }
        )");
    } else {
        setStyleSheet(R"(
            QLabel {
                background-color: rgba(0,0,0,140);
                border: 2px solid rgba(251,168,255,80);
                border-radius: 12px;
            }
            QLabel:hover {
                background-color: rgba(72,80,219,120);
                border: 2px solid #FBA8FF;
            }
        )");
    }
}

void HatCard::mousePressEvent(QMouseEvent *) {
    if (m_locked) {
        emit lockedTapped(m_key);
        return;
    }
    emit clicked(m_key);
}


//  Gear
struct HatDef { QString key; QString icon; bool lootbox; };
static const QList<HatDef> kHats = {
    { "cowboy", ":/images/Sprites/pets/icons/cowboy_hat.png", true },
    { "crown",  ":/images/Sprites/pets/icons/crown_hat.png",  true },
    { "santa",  ":/images/Sprites/pets/icons/santa_hat.png",  true },
    { "wizard", ":/images/Sprites/pets/icons/wizard_hat.png", true },
};

// Pet folder/prefix lookup (mirrors character.cpp naming)
static QString petFolder(Character::PetType t) {
    switch (t) {
        case Character::ElectricAxolotl: return "axolotl/axolotl";
        case Character::SeelCat:         return "seelcat/seelcat";
        default:                         return "dragondog/dragondog";
    }
}

// Stage infix: Baby "", Teen "teen_", Adult "adult_"
static QString stageInfix(const QString &stage) {
    if (stage == "Teen")  return "teen_";
    if (stage == "Adult") return "adult_";
    return "";
}

//  Constructor
Gear::Gear(Player *player, Character::PetType petType, QWidget *parent)
    : QWidget(parent), m_player(player), m_petType(petType)
{
    m_stage = player->getPet().age_group();

    m_bg.load(":/images/Backgrounds/gearStage_16bit.png");

    //  Achievements button — top-left, same geometry as b_save_mode
    b_achievements = new QPushButton("Achievements", this);
    b_achievements->setIcon(QIcon()); // text only, no icon needed
    b_achievements->setGeometry(8, 8, 160, 36);
    b_achievements->setStyleSheet(R"(
        QPushButton { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4850DB, stop:1 #4A71DB);
            border: 2px inset #FBA8FF; border-radius: 10px;
            padding: 4px; font: bold; color: mistyrose; }
        QPushButton:pressed { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4A71DB, stop:1 #4850DB); })");
    // connected externally from game.cc

    b_lootboxes = new QPushButton("Lootboxes", this);
    b_lootboxes->setStyleSheet(R"(
        QPushButton { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4850DB, stop:1 #4A71DB);
            border: 2px inset #FBA8FF; border-radius: 10px;
            padding: 4px; font: bold; color: mistyrose; }
        QPushButton:pressed { background-color: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4A71DB, stop:1 #4850DB); })");
    // connected externally from game.cc

    m_character = new Character(this);
    m_character->syncWithPlayer(*m_player, m_petType);

    // - Info helper: shown on open, hides after 3 seconds
    infoHelper = new QLabel(this);
    infoHelper->setAlignment(Qt::AlignCenter);
    infoHelper->setWordWrap(true);
    infoHelper->setStyleSheet(
        "QLabel { background-color: rgba(0,0,0,170); border-radius: 8px;"
        "padding: 6px; color: mistyrose; font-size: 15px; }");
    infoHelper->setFixedWidth(300);
    infoHelper->setText("Tap a hat below to dress up your pet!");
    infoHelper->hide();

    m_infoTimer = new QTimer(this);
    m_infoTimer->setSingleShot(true);
    connect(m_infoTimer, &QTimer::timeout, infoHelper, &QLabel::hide);
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setWidgetResizable(false);
    m_scrollArea->setStyleSheet(R"(
        QScrollArea { background: transparent; border: none; }
        QScrollArea > QWidget > QWidget { background: transparent; }
    )");

    // Enable touch AND left-mouse
    QScroller::grabGesture(m_scrollArea->viewport(), QScroller::TouchGesture);
    QScroller::grabGesture(m_scrollArea->viewport(), QScroller::LeftMouseButtonGesture);


    m_stripWidget = new QWidget();
    m_stripWidget->setStyleSheet("background: transparent;");
    m_stripLayout = new QHBoxLayout(m_stripWidget);
    m_stripLayout->setContentsMargins(8, 8, 8, 8);
    m_stripLayout->setSpacing(22);

    for (auto &hd : kHats) {
        HatCard *card = new HatCard(hd.key, hd.icon, m_stripWidget);
        m_hatCards.append(card);
        m_stripLayout->addWidget(card);
        if (hd.lootbox && !m_player->getPet().isHatUnlocked(hd.key)) {
            card->setLocked(true);
        }

        connect(card, &HatCard::clicked,      this, &Gear::onHatSelected);
        connect(card, &HatCard::lockedTapped, this, [this](const QString &hatKey) {
            // Capitalise the hat name for a friendlier display
            QString displayName = hatKey;
            if (!displayName.isEmpty())
                displayName[0] = displayName[0].toUpper();

            infoHelper->setText(
                QString("The %1 hat is locked!\nUnlock it in the Minigames!.")
                    .arg(displayName));
            infoHelper->show();
            infoHelper->raise();
            // Reset the auto-hide timer — keeps the bubble up for 3 s from this tap
            m_infoTimer->start(3000);
        });
    }
    // Add a "No hat" card at the front
    {
        HatCard *noneCard = new HatCard("", "", m_stripWidget);
        noneCard->setText("✕");
        noneCard->setStyleSheet(R"(
            QLabel {
                background-color: rgba(0,0,0,140);
                border: 2px solid rgba(251,168,255,80);
                border-radius: 12px;
                color: mistyrose;
                font-size: 22px;
                font-weight: bold;
            }
        )");
        m_stripLayout->insertWidget(0, noneCard);
        m_hatCards.prepend(noneCard);
        connect(noneCard, &HatCard::clicked, this, &Gear::onHatSelected);
    }

    m_stripWidget->setFixedHeight(126);
    m_scrollArea->setWidget(m_stripWidget);
    // Particle Effects
    m_particleOverlay = new QLabel(this);
    m_particleOverlay->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_particleOverlay->setStyleSheet("background: transparent;");
    m_particleOverlay->hide();

    m_particleTimer = new QTimer(this);
    m_particleTimer->setInterval(30);
    connect(m_particleTimer, &QTimer::timeout, this, &Gear::tickParticles);

    layoutWidgets();
}

//  showEvent
void Gear::showEvent(QShowEvent *e) {
    QWidget::showEvent(e);
    infoHelper->setText("Tap a hat below to dress up your pet!");
    infoHelper->setGeometry((width() - 300) / 2, 60, 300, 50);
    infoHelper->show();
    infoHelper->raise();
    m_infoTimer->start(3000);
}

//  Geometry helpers
QRect Gear::pedestalCharRect() const {
    int cw = 160, ch = 160;
    int cx = 160;
    int cy = 190;
    return QRect(cx, cy, cw, ch);
}

QRect Gear::stripRect() const {
    int h = height();
    int stripH = 126, margin = 8;
    int visibleW = 3 * (110 + 22) + 55 + 16; // 3.5 cards across ~480px "hard math lol"
    return QRect(margin, h - stripH - margin, visibleW, stripH);
}

void Gear::layoutWidgets() {
    int w = width(), h = height();

    b_lootboxes->setGeometry(w - 160 - 8, 8, 160, 36);
    m_character->setGeometry(pedestalCharRect());
    // Info Helper Card
    infoHelper->setGeometry((w - 300) / 2, 20, 300, 50);

    // Hat strip
    QRect sr = stripRect();
    m_scrollArea->setGeometry(sr);
    int totalW = m_hatCards.size() * (110 + 22) + 16;
    m_stripWidget->setFixedWidth(totalW);
    m_particleOverlay->setGeometry(0, 0, w, h); //Whole window
}

void Gear::resizeEvent(QResizeEvent *e) {
    QWidget::resizeEvent(e);
    layoutWidgets();
}

//  background + particle burst
void Gear::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);
    QPainter p(this);

    if (!m_bg.isNull())
        p.drawPixmap(0, 0, width(), height(), m_bg);
    if (!m_particles.isEmpty()) {
        for (int i = 0; i < m_particles.size(); ++i) {
            QColor c = m_particleColors.value(i, QColor(0xFB, 0xA8, 0xFF));
            c.setAlpha(qMax(0, 255 - m_particleTick * 8));
            p.setBrush(c);
            p.setPen(Qt::NoPen);
            p.drawEllipse(m_particles[i]);
        }
    }
}

//  Hat selection
void Gear::onHatSelected(const QString &hatKey) {
    m_equippedHat = hatKey;

    // Update selection highlight
    for (HatCard *card : qAsConst(m_hatCards))
        card->setSelected(card->hatKey() == hatKey);

    // Swap GIF
    loadHatGif(hatKey);

    // Particle burst
    spawnParticles();

    emit hatEquipped(hatKey);
}

QString Gear::gifPath(const QString &hatKey) const {
    QString folder = petFolder(m_petType);
    QString infix  = stageInfix(m_stage);

    if (hatKey.isEmpty()) {
        if (m_stage == "Teen" || m_stage == "Adult")
            return QString(":/images/Sprites/pets/%1_%2idle.gif").arg(folder, infix);
        return QString(":/images/Sprites/pets/%1_idle.gif").arg(folder);
    }
    // Hat GIF: e.g. seelcat/seelcat_teen_santa.gif
    return QString(":/images/Sprites/pets/%1_%2%3.gif")
               .arg(folder, infix, hatKey);
}

void Gear::loadHatGif(const QString &hatKey) {
    QString path = gifPath(hatKey);
    QMovie *movie = new QMovie(path, QByteArray(), m_character);
    if (!movie->isValid()) {
        // Fallback: sync normally (idle gif via Character)
        delete movie;
        m_character->syncWithPlayer(*m_player, m_petType);
        return;
    }
    // Access the internal QLabel via findChild (it's named "display" in character.cpp)
    QLabel *disp = m_character->findChild<QLabel *>();
    if (disp) {
        if (disp->movie()) disp->movie()->stop();
        disp->setMovie(movie);
        movie->start();
    } else {
        delete movie;
        m_character->syncWithPlayer(*m_player, m_petType);
    }
}

//  Particles
static const QList<QColor> kParticleColors = {
    QColor(0xFB, 0xA8, 0xFF), QColor(0xFF, 0xD7, 0x00),
    QColor(0x48, 0x50, 0xDB), QColor(0xFF, 0xFF, 0xFF),
    QColor(0xFF, 0x8A, 0xFF), QColor(0xB0, 0xCF, 0xFF),
};

void Gear::spawnParticles() {
    m_particles.clear();
    m_particleColors.clear();
    m_particleTick = 0;

    QRect pr = pedestalCharRect();
    int cx = pr.center().x();
    int cy = pr.center().y();

    auto *rng = QRandomGenerator::global();
    for (int i = 0; i < 28; ++i) {
        int size = rng->bounded(6, 16);
        int dx   = rng->bounded(-pr.width()  / 2, pr.width()  / 2);
        int dy   = rng->bounded(-pr.height() / 2, pr.height() / 2);
        m_particles.append(QRect(cx + dx - size/2, cy + dy - size/2, size, size));
        m_particleColors.append(kParticleColors[rng->bounded(kParticleColors.size())]);
    }

    m_particleOverlay->show();
    m_particleTimer->start();
}

void Gear::tickParticles() {
    m_particleTick++;

    // Drift particles outward
    QRect pr = pedestalCharRect();
    int cx = pr.center().x();
    int cy = pr.center().y();
    auto *rng = QRandomGenerator::global();
    for (int i = 0; i < m_particles.size(); ++i) {
        QRect &r = m_particles[i];
        int dx = (r.center().x() - cx) / 4 + rng->bounded(-2, 3);
        int dy = (r.center().y() - cy) / 4 + rng->bounded(-3, 1) - 1;
        r.translate(dx, dy);
    }

    update(); // triggers paintEvent

    if (m_particleTick > 20) {
        m_particleTimer->stop();
        m_particles.clear();
        m_particleOverlay->hide();
        update();
    }
}

//  Public API
void Gear::refresh(Character::PetType petType) {
    m_petType = petType;
    m_stage   = m_player->getPet().age_group();

    if (m_equippedHat.isEmpty())
        m_character->syncWithPlayer(*m_player, m_petType);
    else
        loadHatGif(m_equippedHat);

    layoutWidgets();
}

// Restore a hat silently (no particle burst) — used after loadGame()
void Gear::restoreHat(const QString &hatKey) {
    m_equippedHat = hatKey;
    for (HatCard *card : qAsConst(m_hatCards))
        card->setSelected(card->hatKey() == hatKey);
    loadHatGif(hatKey);
}

// Called by game.cc when a hat is earned (battle achievement or lootbox).
void Gear::unlockHat(const QString &hatKey) {
    for (HatCard *card : qAsConst(m_hatCards)) {
        if (card->hatKey() == hatKey) {
            card->setLocked(false);   // restores icon to unlocked
            break;
        }
    }
}
