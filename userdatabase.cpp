#include "userdatabase.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCryptographicHash>

UserDatabase* UserDatabase::m_instance = nullptr;

UserDatabase* UserDatabase::instance()
{
    if (!m_instance) {
        m_instance = new UserDatabase();
    }
    return m_instance;
}

UserDatabase::UserDatabase(QObject *parent) : QObject(parent)
{
    m_dbFilePath = "data/users.json";
    loadDatabase();
}

UserDatabase::~UserDatabase()
{
    saveDatabase();
}

bool UserDatabase::addUser(const User &user)
{
    if (m_users.contains(user.username)) {
        return false;
    }

    m_users[user.username] = user;
    saveDatabase();
    return true;
}

bool UserDatabase::authenticateUser(const QString &username, const QString &password, User &user)
{
    if (!m_users.contains(username)) {
        return false;
    }

    User storedUser = m_users[username];
    if (storedUser.password == password) {
        user = storedUser;
        return true;
    }

    return false;
}

bool UserDatabase::updateUser(const User &user)
{
    if (!m_users.contains(user.username)) {
        return false;
    }

    m_users[user.username] = user;
    saveDatabase();
    return true;
}

bool UserDatabase::userExists(const QString &username)
{
    return m_users.contains(username);
}

int UserDatabase::getGlobalBestScore() const
{
    int globalBest = 0;

    for (const User &user : m_users) {
        int userBest = user.getBestScore();
        if (userBest > globalBest) {
            globalBest = userBest;
        }
    }

    return globalBest;
}

void UserDatabase::loadDatabase()
{
    QFile file(m_dbFilePath);
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isArray()) {
        QJsonArray userArray = doc.array();

        for (const QJsonValue &userValue : userArray) {
            QJsonObject userObj = userValue.toObject();

            User user;
            user.firstName = userObj["firstName"].toString();
            user.lastName = userObj["lastName"].toString();
            user.dateOfBirth = QDate::fromString(userObj["dateOfBirth"].toString(), "yyyy-MM-dd");
            user.gender = userObj["gender"].toString();
            user.username = userObj["username"].toString();
            user.password = userObj["password"].toString();
            user.profilePicture = userObj["profilePicture"].toString();

            // Load score history
            QJsonArray scoresArray = userObj["scoreHistory"].toArray();
            for (const QJsonValue &scoreValue : scoresArray) {
                QJsonObject scoreObj = scoreValue.toObject();
                GameScore score;
                score.score = scoreObj["score"].toInt();
                score.playedAt = QDateTime::fromString(scoreObj["playedAt"].toString(), Qt::ISODate);
                user.scoreHistory.append(score);
            }

            m_users[user.username] = user;
        }
    }

    file.close();
}

void UserDatabase::saveDatabase()
{
    QFile file(m_dbFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }

    QJsonArray userArray;

    for (const User &user : m_users) {
        if (user.isGuest) {
            continue;  // Don't save guest users
        }

        QJsonObject userObj;
        userObj["firstName"] = user.firstName;
        userObj["lastName"] = user.lastName;
        userObj["dateOfBirth"] = user.dateOfBirth.toString("yyyy-MM-dd");
        userObj["gender"] = user.gender;
        userObj["username"] = user.username;
        userObj["password"] = user.password;
        userObj["profilePicture"] = user.profilePicture;

        // Save score history
        QJsonArray scoresArray;
        for (const GameScore &score : user.scoreHistory) {
            QJsonObject scoreObj;
            scoreObj["score"] = score.score;
            scoreObj["playedAt"] = score.playedAt.toString(Qt::ISODate);
            scoresArray.append(scoreObj);
        }
        userObj["scoreHistory"] = scoresArray;

        userArray.append(userObj);
    }

    QJsonDocument doc(userArray);
    file.write(doc.toJson());
    file.close();
}
