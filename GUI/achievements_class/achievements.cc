/*
 * Achievements.cc - Achievement tracking implementation.
 * Author(s): Luke Cerwin
 */
#include "achievements.h"

Achievements::Achievements() {
    // Define all achievements in order
    m_achievements = {
        // Battle
        { "battle_1",       "Not Too Shabby I",    "Win 1 battle",                          false },
        { "battle_10",      "Not Too Shabby II",   "Win 10 battles",                        false },
        { "battle_100",     "Not Too Shabby III",  "Win 100 battles",                       false },
        // Care — Feed
        { "bone_seelcat",   "What Are You Doing?", "Feed a Bone to a SeelCat",              false },
        // Care — Sleep
        { "beauty_sleep",   "Beauty Sleep",         "Use Tuck Into Bed",                    false },
        { "bookworm",       "Bookworm",             "Use Bed Time Story 10 times",           false },
        // Progression
        { "growing_up",     "Growing Up",           "Reach the Teen stage",                  false },
        { "proud_parent",   "Proud Parent",         "Bring your pet to Adult stage",         false },
        { "veteran",        "Veteran",              "Keep your pet alive for 7 days",        false },
        // Session
        { "marathon",       "Marathon Session",     "Play for 2 hours straight",             false },
        // Misc
        { "inactive_30",    "Are You There?",       "Be inactive for 30 minutes",            false },
        { "temper_tantrum", "Temper Tantrum",       "Pet is sleeping AND hungry at once",    false },
        { "crown_hat",      "Here Yee Here Yee",   "Obtain the Crown Hat",                  false },
    };
}

//  Internal unlock helper
QString Achievements::unlock(const QString &key) {
    for (Achievement &a : m_achievements) {
        if (a.key == key && !a.unlocked) {
            a.unlocked = true;
            return a.title; // newly unlocked
        }
    }
    return QString(); // already had it or not found
}

//  Check methods for actual achievements

QList<QString> Achievements::onBattleWon(int totalWins) {
    QList<QString> u;
    if (totalWins >= 1)   { auto t = unlock("battle_1");   if (!t.isEmpty()) u << t; }
    if (totalWins >= 10)  { auto t = unlock("battle_10");  if (!t.isEmpty()) u << t; }
    if (totalWins >= 100) { auto t = unlock("battle_100"); if (!t.isEmpty()) u << t; }
    return u;
}

QList<QString> Achievements::onFedBone(const QString &petType) {
    QList<QString> u;
    if (petType == "SeelCat") {
        auto t = unlock("bone_seelcat");
        if (!t.isEmpty()) u << t;
    }
    return u;
}

QList<QString> Achievements::onInactive() {
    QList<QString> u;
    auto t = unlock("inactive_30");
    if (!t.isEmpty()) u << t;
    return u;
}

QList<QString> Achievements::onAgeChanged(const QString &ageGroup) {
    QList<QString> u;
    if (ageGroup == "Teen") {
        auto t = unlock("growing_up");
        if (!t.isEmpty()) u << t;
    }
    if (ageGroup == "Adult") {
        auto t = unlock("proud_parent");
        if (!t.isEmpty()) u << t;
    }
    return u;
}

QList<QString> Achievements::onTemperTantrum(bool sleeping, bool angry) {
    QList<QString> u;
    if (sleeping && angry) {
        auto t = unlock("temper_tantrum");
        if (!t.isEmpty()) u << t;
    }
    return u;
}

QList<QString> Achievements::onCrownHatEquipped() {
    QList<QString> u;
    auto t = unlock("crown_hat");
    if (!t.isEmpty()) u << t;
    return u;
}

QList<QString> Achievements::onTuckIn() {
    QList<QString> u;
    auto t = unlock("beauty_sleep");
    if (!t.isEmpty()) u << t;
    return u;
}

QList<QString> Achievements::onBedTimeStory(int totalStories) {
    QList<QString> u;
    if (totalStories >= 10) {
        auto t = unlock("bookworm");
        if (!t.isEmpty()) u << t;
    }
    return u;
}

QList<QString> Achievements::onDaysOld(int days) {
    QList<QString> u;
    if (days >= 7) {
        auto t = unlock("veteran");
        if (!t.isEmpty()) u << t;
    }
    return u;
}

QList<QString> Achievements::onMarathonSession() {
    QList<QString> u;
    auto t = unlock("marathon");
    if (!t.isEmpty()) u << t;
    return u;
}

QJsonObject Achievements::toJson() const {
    QJsonObject json;
    for (const Achievement &a : m_achievements)
        json[a.key] = a.unlocked;
    return json;
}

void Achievements::fromJson(const QJsonObject &json) {
    for (Achievement &a : m_achievements)
        if (json.contains(a.key))
            a.unlocked = json[a.key].toBool();
}
