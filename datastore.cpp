#include "datastore.h"
#include <QDir>
#include <QJsonArray>
#include <QCryptographicHash>

DataStore::DataStore(QObject* parent): QObject(parent) {
    m_path = QDir::currentPath() + "/data.json";
}

bool DataStore::load() {
    QFile f(m_path);
    if(!f.exists()) return true;
    if(!f.open(QIODevice::ReadOnly)) return false;
    const auto doc = QJsonDocument::fromJson(f.readAll());
    f.close();

    auto o = doc.object();
    m_users.clear();
    m_marks.clear();
    m_absents.clear();
    m_schedule.clear();

    for (const auto &uo: o["users"].toArray()) {
        User u = User::fromJson(uo.toObject());
        m_users.insert(u.username, u);
    }

    for (const auto &mo: o["marks"].toArray()) {
        auto obj = mo.toObject();
        Mark m;
        m.subject = obj["subject"].toString();
        m.value = obj["value"].toInt();
        m.date = QDate::fromString(obj["date"].toString(), Qt::ISODate);
        QString student = obj["student"].toString();
        m_marks.insert(student, m);
    }

    for (const auto &ao: o["absents"].toArray()) {
        auto obj = ao.toObject();
        QString student = obj["student"].toString();
        QSet<QDate> set;
        for (const auto &d: obj["dates"].toArray()) {
            set.insert(QDate::fromString(d.toString(), Qt::ISODate));
        }
        m_absents.insert(student, set);
    }

    for (const auto &so: o["schedule"].toArray()) {
        auto obj = so.toObject();
        int wd = obj["weekday"].toInt();
        QStringList lessons;
        for (const auto &l: obj["lessons"].toArray()) lessons << l.toString();
        m_schedule.insert(wd, lessons);
    }

    emit changed();
    return true;
}

bool DataStore::save() const {
    QJsonObject o;

    QJsonArray usersArr;
    for (const auto &u: m_users) usersArr.append(u.toJson());
    o["users"] = usersArr;

    QJsonArray marksArr;
    for (auto it = m_marks.begin(); it != m_marks.end(); ++it) {
        QJsonObject obj {
            {"student", it.key()},
            {"subject", it->subject},
            {"value", it->value},
            {"date", it->date.toString(Qt::ISODate)}
        };
        marksArr.append(obj);
    }
    o["marks"] = marksArr;

    QJsonArray absArr;
    for (auto it = m_absents.begin(); it != m_absents.end(); ++it) {
        QJsonArray dates;
        for (const auto &d: it.value()) dates.append(d.toString(Qt::ISODate));
        absArr.append(QJsonObject{{"student", it.key()}, {"dates", dates}});
    }
    o["absents"] = absArr;

    QJsonArray sch;
    for (auto it = m_schedule.begin(); it != m_schedule.end(); ++it) {
        QJsonArray lessons;
        for (const auto &l: it.value()) lessons.append(l);
        sch.append(QJsonObject{{"weekday", it.key()}, {"lessons", lessons}});
    }
    o["schedule"] = sch;

    QFile f(m_path);
    if(!f.open(QIODevice::WriteOnly)) return false;
    f.write(QJsonDocument(o).toJson());
    f.close();
    return true;
}

// ======== Users ========
bool DataStore::addUser(const User& u) {
    if (m_users.contains(u.username)) return false;
    m_users.insert(u.username, u);
    emit changed();
    return save();
}

bool DataStore::removeUser(const QString& username) {
    bool ok = m_users.remove(username);
    emit changed();
    save();
    return ok;
}

bool DataStore::userExists(const QString& username) const {
    return m_users.contains(username);
}

User DataStore::getUser(const QString& username) const {
    return m_users.value(username, User{});
}

QList<User> DataStore::allUsers() const {
    return m_users.values();
}

// ======== Auth ========
QString DataStore::hash(const QString& password) {
    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}

bool DataStore::verify(const QString& username, const QString& password, User* out) const {
    if (!m_users.contains(username)) return false;
    auto u = m_users.value(username);
    if (u.passwordHash == hash(password)) {
        if (out) *out = u;
        return true;
    }
    return false;
}

// ======== Marks ========
void DataStore::addMark(const QString& student, const QString& subject, int value, const QDate& date) {
    m_marks.insert(student, Mark{subject, value, date});
    emit changed();
    save();
}

QVector<Mark> DataStore::getMarks(const QString& student) const {
    return m_marks.values(student).toVector();
}

// ======== Absents ========
void DataStore::setAbsent(const QString& student, const QDate& date, bool absent) {
    if (absent) m_absents[student].insert(date);
    else m_absents[student].remove(date);
    emit changed();
    save();
}

bool DataStore::isAbsent(const QString& student, const QDate& date) const {
    return m_absents.value(student).contains(date);
}

// ======== Schedule ========
QStringList DataStore::getSchedule(int weekday) const {
    return m_schedule.value(weekday);
}

void DataStore::setSchedule(int weekday, const QStringList& lessons) {
    m_schedule[weekday] = lessons;
    emit changed();
    save();
}
