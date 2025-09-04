#ifndef USER_H
#define USER_H

#include <QString>
#include <QJsonObject>

enum class Role { Student=0, Teacher=1, Admin=2 };

struct User {
    QString fullName;
    QString username;
    QString passwordHash; // для логина
    Role role;
    int classId;          // класс студента
    QVector<Grade> grades; // оценки
};


    QJsonObject toJson() const {
        return QJsonObject{
            {"username", username},
            {"passwordHash", passwordHash},
            {"role", (int)role},
            {"fullName", fullName},
            {"classId", classId}
        };
    }

    static User fromJson(const QJsonObject &o) {
        User u;
        u.username = o.value("username").toString();
        u.passwordHash = o.value("passwordHash").toString();
        u.role = (Role)o.value("role").toInt();
        u.fullName = o.value("fullName").toString();
        u.classId = o.value("classId").toString();
        return u;
    }
};

#endif
