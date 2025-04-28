// user.h
#ifndef USER_H
#define USER_H

#include <QString>
#include <QDate>
#include <QVector>
#include <QDateTime>

struct GameScore {
    int score;
    QDateTime playedAt;

    GameScore() : score(0) {}
    GameScore(int s, const QDateTime &t) : score(s), playedAt(t) {}
};

class User
{
public:
    // Constructor
    User();

    // Getters
    QString getFirstName() const;
    QString getLastName() const;
    QString getFullName() const;
    QDate getDateOfBirth() const;
    QString getGender() const;
    QString getUsername() const;
    QString getPassword() const;
    QString getProfilePicture() const;
    bool getIsGuest() const;
    QVector<GameScore> getScoreHistory() const;
    int getBestScore() const;
    bool isBirthday() const;

    // Setters
    void setFirstName(const QString &value);
    void setLastName(const QString &value);
    void setDateOfBirth(const QDate &value);
    void setGender(const QString &value);
    void setUsername(const QString &value);
    void setPassword(const QString &value);
    void setProfilePicture(const QString &value);
    void setIsGuest(bool value);

    // Other methods
    void addScore(int score);

    QString firstName;
    QString lastName;
    QDate dateOfBirth;
    QString gender;
    QString username;
    QString password;
    QString profilePicture;
    bool isGuest;
    QVector<GameScore> scoreHistory;
};

#endif // USER_H
