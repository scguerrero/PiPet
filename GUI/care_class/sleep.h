/*
 * sleep.h - Sleep screen with character GIF.
 * Cuddle replaces Brush Teeth.
 * Author(s): Luke Cerwin
 */
#ifndef SLEEP_H
#define SLEEP_H

#include <QtWidgets>
#include <QPixmap>
#include <QPaintEvent>
#include "../../Player/Player.h"
#include "../character_class/character.h"

class Sleep : public QWidget
{
    Q_OBJECT
public:
    explicit Sleep(Player *player, Character::PetType petType,
                   QWidget *parent = nullptr);
    void updateSleepDisplay();

    // Re-syncs the character sprite to the current pet type and equipped hat.
    // Call this from game.cc each time the sleep screen is opened.
    void refreshCharacter();

signals:
    void tuckInUsed();                    // Beauty Sleep achievement
    void bedTimeStoryUsed(int totalUsed); // Bookworm achievement

protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent  (QShowEvent   *e) override;

private slots:
    void cuddle();
    void wearPjs();
    void readBook();
    void tuckIn();

private:
    Player             *player;
    Character::PetType  petType;
    QPixmap             m_bg;
    Character          *character;

    QVBoxLayout *layout;
    QGroupBox   *actionsBox;
    QGridLayout *actionsGrid;
    QLabel  *infoHelper;
    QTimer  *m_infoTimer;

    QPushButton *cuddleBtn;
    QPushButton *wearPjsBtn;
    QPushButton *readBookBtn;
    QPushButton *tuckInBtn;

    QLabel *sleepDisplay;

    QMovie *m_hatMovie          = nullptr;
    int     m_bedTimeStoryCount = 0;

    void applySleepAction(int boost, const QString &message);
};

#endif // SLEEP_H
