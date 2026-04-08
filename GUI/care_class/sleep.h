/*
 * sleep.h - Sleep screen header.
 * Author(s): Tanya Magurupira
 */
#ifndef SLEEP_H
#define SLEEP_H

#include <QtWidgets>
#include <QPixmap>
#include <QPaintEvent>
#include "../../Player/Player.h"

class Sleep : public QWidget
{
    Q_OBJECT
public:
    explicit Sleep(Player *player, QWidget *parent = nullptr);
    void updateSleepDisplay();
    QPushButton *backBtn;

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void brushTeeth();
    void wearPjs();
    void readBook();
    void tuckIn();

private:
    Player  *player;
    QPixmap  m_bg;

    QVBoxLayout *layout;
    QGroupBox   *actionsBox;
    QGridLayout *actionsGrid;

    QPushButton *brushTeethBtn;
    QPushButton *wearPjsBtn;
    QPushButton *readBookBtn;
    QPushButton *tuckInBtn;

    QLabel *sleepDisplay;

    void applySleepAction(int boost, const QString &message);
};

#endif // SLEEP_H
