#pragma once
#include <QString>
#include <QVector>
#include <QStringList>

enum class Role { Student, Teacher, Admin };

struct Grade {
    QString subject;
    int grade;
    QString comment;
};

struct User {
    QString fullName;
    QString username;
    Role role;
    int classNumber;     // номер класса
    QString classLetter; // буква класса
    QVector<Grade> grades;
};

class DataStore {
public:
    QVector<User> m_users;

    QVector<User>& allUsers() { return m_users; }

    void addUser(const User& u) { m_users.append(u); }
    void deleteUser(const QString& username) {
        for(int i=0;i<m_users.size();i++){
            if(m_users[i].username==username) { m_users.remove(i); return; }
        }
    }

    QStringList allClasses() const {
        QStringList classes;
        for(auto& u : m_users){
            if(u.role==Role::Student){
                QString c = QString::number(u.classNumber) + u.classLetter;
                if(!classes.contains(c)) classes.append(c);
            }
        }
        return classes;
    }

    QStringList studentsInClass(const QString& className) const {
        QStringList list;
        for(auto& u : m_users){
            if(u.role==Role::Student){
                QString c = QString::number(u.classNumber)+u.classLetter;
                if(c==className) list.append(u.fullName);
            }
        }
        return list;
    }
};
