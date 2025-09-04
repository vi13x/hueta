#ifndef DATASTORE_H
#define DATASTORE_H

#include "user.h"
#include <QObject>
#include <QMap>
#include <QVector>
#include <QDate>
#include <QJsonDocument>
#include <QFile>
#include <QSet>

struct Mark {
    QString subject;
    int value = 0;   // 1-10
    QDate date;
};

class DataStore : public QObject {
    Q_OBJECT
public:
    explicit DataStore(QObject* parent=nullptr);

    bool load();
    bool save() const;

    // Пользователи
    bool addUser(const User& u);
    bool removeUser(const QString& username);
    bool userExists(const QString& username) const;
    User getUser(const QString& username) const;
    QList<User> allUsers() const;

    // Аутентификация
    static QString hash(const QString& password);
    bool verify(const QString& username, const QString& password, User* out=nullptr) const;

    // Оценки / пропуски (для учителей)
    void addMark(const QString& student, const QString& subject, int value, const QDate& date);
    QVector<Mark> getMarks(const QString& student) const;

    void setAbsent(const QString& student, const QDate& date, bool absent);
    bool isAbsent(const QString& student, const QDate& date) const;

    // Расписание (для администратора)
    QStringList getSchedule(int weekday) const; // 1=Пн
    void setSchedule(int weekday, const QStringList& lessons);

    QString dataPath() const { return m_path; }

signals:
    void changed();

private:
    QString m_path;
    QMap<QString, User> m_users; // username -> User
    QMultiMap<QString, Mark> m_marks; // student -> оценки
    QMap<QString, QSet<QDate>> m_absents; // student -> дни отсутствия
    QMap<int, QStringList> m_schedule; // день недели -> уроки
};

#endif
