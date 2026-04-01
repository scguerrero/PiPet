#ifndef SLEEP_H
#define SLEEP_H

#include <QtWidgets>
#include "../../Player/Player.h"

class Sleep : public QWidget
{
    Q_OBJECT
public:

public:
    explicit Sleep(Player *player, QWidget *parent = nullptr);
    void updateSleepDisplay();
    QPushButton *backBtn;

signals:

private slots:
    void brushTeeth();
    void wearPjs();
    void readBook();
    void tuckIn();

private:
    Player *player;

    QVBoxLayout *layout;
    QGroupBox *actionsBox;
    QGridLayout *actionsGrid;

    QPushButton *brushTeethBtn;
    QPushButton *wearPjsBtn;
    QPushButton *readBookBtn;
    QPushButton *tuckInBtn;

    QLabel *sleepDisplay; // shows current sleep live

    void applySleepAction(int boost, const QString &message);

};

#endif // SLEEP_H
