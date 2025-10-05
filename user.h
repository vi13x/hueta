#ifndef USER_H
#define USER_H

#include <QString>
#include <QStringList>

enum class UserRole {
    Student,
    Teacher,
    Admin
};

class User {
public:
    User(const QString& username, const QString& password, UserRole role);
    virtual ~User() = default;

    QString getUsername() const;
    QString getPassword() const;
    UserRole getRole() const;
    QString getRoleString() const;

    void setUsername(const QString& username);
    void setPassword(const QString& password);
    void setRole(UserRole role);

    virtual QStringList getPermissions() const = 0;

protected:
    QString username;
    QString password;
    UserRole role;
};

#endif // USER_H
