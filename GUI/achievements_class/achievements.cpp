/*
 * Achievements.cpp - Achievement tracking implementation.
 * Author(s): Sasha Guerrero
 */
#include "achievements.h"

Achievements::Achievements() {
    // Define all achievements in order
    m_achievements = {
        { "battle_1",       "Not Too Shabby I",    "Win 1 battle",              false },
        { "battle_10",      "Not Too Shabby II",   "Win 10 battles",            false },
        { "battle_100",     "Not Too Shabby III",  "Win 100 battles",           false },
        { "bone_seelcat",   "What Are You Doing?", "Feed a Bone to a SeelCat",  false },
        { "inactive_30",    "Are You There?",       "Be inactive for 30 minutes",false },
        { "all_minigames",  "Completionist",        "Play all 3 minigames",      false },
        { "proud_parent",   "Proud Parent",         "Bring your pet to Adult",   false },
        { "temper_tantrum", "Temper Tantrum",       "Pet is sleeping AND hungry at the same time", false },
        { "crown_hat",      "Here Yee Here Yee",   "Obtain the Crown Hat",      false },
    };
}

// ── Internal unlock helper ────────────────────────────────────────────────
QString Achievements::unlock(const QString &key) {
    for (Achievement &a : m_achievements) {
        if (a.key == key && !a.unlocked) {
            a.unlocked = true;
            return a.title; // newly unlocked
        }
    }
    return QString(); // already had it or not found
}

// ── Check methods ─────────────────────────────────────────────────────────

QList<QString> Achievements::onBattleWon(int totalWins) {
    QList<QString> unlocked;
    if (totalWins >= 1)   { QString t = unlock("battle_1");   if (!t.isEmpty()) unlocked << t; }
    if (totalWins >= 10)  { QString t = unlock("battle_10");  if (!t.isEmpty()) unlocked << t; }
    if (totalWins >= 100) { QString t = unlock("battle_100"); if (!t.isEmpty()) unlocked << t; }
    return unlocked;
}

QList<QString> Achievements::onFedBone(const QString &petType) {
    QList<QString> unlocked;
    if (petType == "SeelCat") {
        QString t = unlock("bone_seelcat");
        if (!t.isEmpty()) unlocked << t;
    }
    return unlocked;
}

QList<QString> Achievements::onInactive() {
    QList<QString> unlocked;
    QString t = unlock("inactive_30");
    if (!t.isEmpty()) unlocked << t;
    return unlocked;
}

QList<QString> Achievements::onMinigamePlayed(int gamesPlayed) {
    QList<QString> unlocked;
    if (gamesPlayed >= 3) {
        QString t = unlock("all_minigames");
        if (!t.isEmpty()) unlocked << t;
    }
    return unlocked;
}

QList<QString> Achievements::onAgeChanged(const QString &ageGroup) {
    QList<QString> unlocked;
    if (ageGroup == "Adult") {
        QString t = unlock("proud_parent");
        if (!t.isEmpty()) unlocked << t;
    }
    return unlocked;
}

QList<QString> Achievements::onTemperTantrum(bool sleeping, bool angry) {
    QList<QString> unlocked;
    if (sleeping && angry) {
        QString t = unlock("temper_tantrum");
        if (!t.isEmpty()) unlocked << t;
    }
    return unlocked;
}

QList<QString> Achievements::onCrownHatEquipped() {
    QList<QString> unlocked;
    QString t = unlock("crown_hat");
    if (!t.isEmpty()) unlocked << t;
    return unlocked;
}

// ── JSON ──────────────────────────────────────────────────────────────────

QJsonObject Achievements::toJson() const {
    QJsonObject json;
    for (const Achievement &a : m_achievements)
        json[a.key] = a.unlocked;
    return json;
}

void Achievements::fromJson(const QJsonObject &json) {
    for (Achievement &a : m_achievements) {
        if (json.contains(a.key))
            a.unlocked = json[a.key].toBool();
    }
}
