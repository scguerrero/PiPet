#ifndef GROOM_H
#define GROOM_H

#include <QtWidgets>
#include "../../Player/Player.h"

class Groom : public QWidget
{
    Q_OBJECT
public:
    explicit Groom(Player *player, QWidget *parent = nullptr);
    void updateHygieneDisplay();
    QPushButton *backBtn;

signals:

private slots:
    void brush();
    void bathe();
    void trimNails();
    void cleanEars();

private:
    Player *player;

    QVBoxLayout *layout;
    QGroupBox *actionsBox;
    QGridLayout *actionsGrid;

    QPushButton *brushBtn;
    QPushButton *batheBtn;
    QPushButton *trimBtn;
    QPushButton *cleanEarsBtn;

    QLabel *hygieneDisplay; // shows current hygiene live

    void applyGroomAction(int boost, const QString &message);

};

#endif // GROOM_H
