#include "user.h"

User::User(const QString& username, const QString& password, UserRole role)
    : username(username), password(password), role(role) {
}

QString User::getUsername() const {
    return username;
}

QString User::getPassword() const {
    return password;
}

UserRole User::getRole() const {
    return role;
}

QString User::getRoleString() const {
    switch (role) {
        case UserRole::Student:
            return "Студент";
        case UserRole::Teacher:
            return "Преподаватель";
        case UserRole::Admin:
            return "Администратор";
        default:
            return "Неизвестно";
    }
}

void User::setUsername(const QString& username) {
    this->username = username;
}

void User::setPassword(const QString& password) {
    this->password = password;
}

void User::setRole(UserRole role) {
    this->role = role;
}
