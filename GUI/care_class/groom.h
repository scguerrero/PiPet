/*
 * groom.h - Groom screen header.
 * Author(s): Tanya Magurupira
 */
#ifndef GROOM_H
#define GROOM_H

#include <QtWidgets>
#include <QPixmap>
#include <QPaintEvent>
#include "../../Player/Player.h"

class Groom : public QWidget
{
    Q_OBJECT
public:
    explicit Groom(Player *player, QWidget *parent = nullptr);
    void updateHygieneDisplay();
    QPushButton *backBtn;

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void brush();
    void bathe();
    void trimNails();
    void cleanEars();

private:
    Player  *player;
    QPixmap  m_bg;

    QVBoxLayout *layout;
    QGroupBox   *actionsBox;
    QGridLayout *actionsGrid;

    QPushButton *brushBtn;
    QPushButton *batheBtn;
    QPushButton *trimBtn;
    QPushButton *cleanEarsBtn;

    QLabel *hygieneDisplay;

    void applyGroomAction(int boost, const QString &message);
};

#endif // GROOM_H
