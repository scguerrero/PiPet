#ifndef GROOMWIDGET_H
#define GROOMWIDGET_H

#include <QtWidgets>
#include "../../Player/Player.h"

class GroomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GroomWidget(Player *player, QWidget *parent = nullptr);
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

#endif // GROOMWIDGET_H

