// user.cpp
#include "user.h"

// Constructor
User::User() : isGuest(false) {}

// Getters
QString User::getFirstName() const
{
    return firstName;
}

QString User::getLastName() const
{
    return lastName;
}

QString User::getFullName() const
{
    return firstName + " " + lastName;
}

QDate User::getDateOfBirth() const
{
    return dateOfBirth;
}

QString User::getGender() const
{
    return gender;
}

QString User::getUsername() const
{
    return username;
}

QString User::getPassword() const
{
    return password;
}

QString User::getProfilePicture() const
{
    return profilePicture;
}

bool User::getIsGuest() const
{
    return isGuest;
}

QVector<GameScore> User::getScoreHistory() const
{
    return scoreHistory;
}

int User::getBestScore() const
{
    int best = 0;
    for (const GameScore &score : scoreHistory) {
        if (score.score > best) {
            best = score.score;
        }
    }
    return best;
}

bool User::isBirthday() const
{
    QDate currentDate = QDate::currentDate();
    return dateOfBirth.day() == currentDate.day() &&
           dateOfBirth.month() == currentDate.month();
}

// Setters
void User::setFirstName(const QString &value)
{
    firstName = value;
}

void User::setLastName(const QString &value)
{
    lastName = value;
}

void User::setDateOfBirth(const QDate &value)
{
    dateOfBirth = value;
}

void User::setGender(const QString &value)
{
    gender = value;
}

void User::setUsername(const QString &value)
{
    username = value;
}

void User::setPassword(const QString &value)
{
    password = value;
}

void User::setProfilePicture(const QString &value)
{
    profilePicture = value;
}

void User::setIsGuest(bool value)
{
    isGuest = value;
}

// Other methods
void User::addScore(int score)
{
    scoreHistory.append(GameScore(score, QDateTime::currentDateTime()));
}
