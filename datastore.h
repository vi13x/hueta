#pragma once
#include <QObject>
#include <QString>
#include <QList>
#include <QVector>
#include <QDate>
#include <QStringList>
#include <QSet>
#include <QRandomGenerator>

enum class Role { Student, Teacher, Admin };

struct Grade {
    QString subject;
    int mark;
    QString comment;
    QDate date;
};

struct User {
    QString fullName;
    QString username;
    QString passwordHash;
    Role role;
    int classId;           // номер класса
    QVector<Grade> grades; // оценки
};

class DataStore : public QObject {
    Q_OBJECT
public:
    explicit DataStore(QObject* parent = nullptr);

    bool load();
    bool save() const;

    bool addUser(const User&);
    bool removeUser(const QString&);
    bool userExists(const QString&) const;
    User getUser(const QString&) const;
    QList<User> allUsers() const;

    QString hash(const QString&);
    bool verify(const QString& username, const QString& password, User* u = nullptr) const;

    void addMark(const QString& username, const QString& subject, int mark, const QDate& date);
    int getMarks(const QString& username) const;

    void setAbsent(const QString&, const QDate&, bool);
    bool isAbsent(const QString&, const QDate&) const;

    QStringList getSchedule(int classId) const;
    void setSchedule(int classId, const QStringList& schedule);

    QStringList allClasses() const;
    QStringList studentsInClass(const QString& className) const;

signals:
    void changed();

private:
    QList<User> m_users;
};
