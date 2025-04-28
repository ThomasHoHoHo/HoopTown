#ifndef USERDATABASE_H
#define USERDATABASE_H

#include <QObject>
#include <QHash>
#include <QVector>
#include "user.h"

class UserDatabase : public QObject
{
    Q_OBJECT

public:
    static UserDatabase* instance();

    bool addUser(const User &user);
    bool authenticateUser(const QString &username, const QString &password, User &user);
    bool updateUser(const User &user);
    bool userExists(const QString &username);
    int getGlobalBestScore() const;

    explicit UserDatabase(QObject *parent = nullptr);
    ~UserDatabase();

    static UserDatabase *m_instance;
    QHash<QString, User> m_users;  // username -> User
    QString m_dbFilePath;

    void loadDatabase();
    void saveDatabase();
};

#endif // USERDATABASE_H
