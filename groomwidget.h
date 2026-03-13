#ifndef GROOMWIDGET_H
#define GROOMWIDGET_H

#include <QtWidgets>
#include "../../Player/Player.h"

class GroomWidget : public QDialog
{
    Q_OBJECT
public:
    explicit GroomWidget(Player *player, QWidget *parent = nullptr);

private slots:
    void brush();
    void bathe();
    void brushTeeth();
    void trimNails();
    void cleanEars();

private:
    Player *player;

    QVBoxLayout *layout;
    QGroupBox *actionsBox;
    QGridLayout *actionsGrid;

    QPushButton *brushBtn;
    QPushButton *batheBtn;
    QPushButton *brushTeethBtn;
    QPushButton *trimBtn;
    QPushButton *cleanEarsBtn;
    QPushButton *doneBtn;

    QLabel *hygieneDisplay; // shows current hygiene live

    void updateHygieneDisplay();
    void applyGroomAction(int boost, const QString &message);

};

#endif // GROOMWIDGET_H
