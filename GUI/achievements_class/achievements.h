/*
 * Achievements.h - Tracks all player achievements.
 * Saves/loads with Player via JSON.
 * Author(s): Luke Cerwin
 */
#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

#include <QString>
#include <QJsonObject>
#include <QList>

struct Achievement {
    QString key;          // unique ID used for JSON
    QString title;        // display name
    QString description;  // what the player needs to do
    bool    unlocked = false;
};

class Achievements
{
public:
    Achievements();

    // Check methods — call these after relevant actions
    // Returns list of newly unlocked achievement titles (empty if none)
    QList<QString> onBattleWon(int totalWins);
    QList<QString> onFedBone(const QString &petType);
    QList<QString> onInactive();                     // call after 30 min timer
    QList<QString> onAgeChanged(const QString &ageGroup);
    QList<QString> onTemperTantrum(bool sleeping, bool angry);
    QList<QString> onCrownHatEquipped();
    QList<QString> onTuckIn();
    QList<QString> onBedTimeStory(int totalStories);
    QList<QString> onDaysOld(int days);
    QList<QString> onMarathonSession();

    // Gear screen display
    const QList<Achievement> &all() const { return m_achievements; }

    // JSON Save File
    QJsonObject toJson()  const;
    void fromJson(const QJsonObject &json);

private:
    QList<Achievement> m_achievements;
    QString unlock(const QString &key);
};

#endif
