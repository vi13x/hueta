#include "datastore.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDate>

DataStore::DataStore() {}

QString DataStore::studentsFile() { return QDir::currentPath() + "/students.txt"; }
QString DataStore::teachersFile() { return QDir::currentPath() + "/teachers.txt"; }
QString DataStore::adminsFile() { return QDir::currentPath() + "/admins.txt"; }
QString DataStore::scheduleFile() { return QDir::currentPath() + "/schedule.txt"; }
QString DataStore::gradesFile() { return QDir::currentPath() + "/grades.txt"; }

QVector<User> DataStore::loadStudents() {
    QVector<User> out;
    QFile f(studentsFile());
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return out;
    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        // format username:password
        auto parts = line.split(':');
        if (parts.size() >= 2) {
            User u; u.username = parts[0]; u.password = parts[1]; u.role = "student";
            out.append(u);
        }
    }
    return out;
}

bool DataStore::addStudent(const User &u) {
    // check exists
    auto students = loadStudents();
    for (auto &s : students) if (s.username == u.username) return false;
    QFile f(studentsFile());
    if (!f.open(QIODevice::Append | QIODevice::Text)) return false;
    QTextStream out(&f);
    out << u.username << ":" << u.password << "\n";
    return true;
}

bool DataStore::checkStudentCredentials(const QString &username, const QString &password) {
    auto students = loadStudents();
    for (auto &s : students) if (s.username==username && s.password==password) return true;
    return false;
}

static bool checkCredentialsFile(const QString &path, const QString &username, const QString &password) {
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        auto parts = line.split(':');
        if (parts.size() >= 2) {
            if (parts[0]==username && parts[1]==password) return true;
        }
    }
    return false;
}

bool DataStore::checkTeacherCredentials(const QString &username, const QString &password) {
    return checkCredentialsFile(teachersFile(), username, password);
}
bool DataStore::checkAdminCredentials(const QString &username, const QString &password) {
    return checkCredentialsFile(adminsFile(), username, password);
}

bool DataStore::addTeacher(const QString &username, const QString &password) {
    QFile f(teachersFile());
    if (!f.open(QIODevice::Append | QIODevice::Text)) return false;
    QTextStream out(&f);
    out << username << ":" << password << "\n";
    return true;
}
bool DataStore::removeTeacher(const QString &username) {
    QFile f(teachersFile());
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    QVector<QString> lines;
    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.startsWith(username + ":")) lines.append(line);
    }
    f.close();
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return false;
    QTextStream out(&f);
    for (auto &l : lines) out << l << "\n";
    return true;
}

bool DataStore::addAdmin(const QString &username, const QString &password) {
    QFile f(adminsFile());
    if (!f.open(QIODevice::Append | QIODevice::Text)) return false;
    QTextStream out(&f);
    out << username << ":" << password << "\n";
    return true;
}
bool DataStore::removeAdmin(const QString &username) {
    QFile f(adminsFile());
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    QVector<QString> lines;
    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.startsWith(username + ":")) lines.append(line);
    }
    f.close();
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return false;
    QTextStream out(&f);
    for (auto &l : lines) out << l << "\n";
    return true;
}

QVector<QString> DataStore::loadSchedule() {
    QVector<QString> out;
    QFile f(scheduleFile());
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return out;
    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine();
        out.append(line);
    }
    return out;
}

bool DataStore::saveSchedule(const QVector<QString> &lines) {
    QFile f(scheduleFile());
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return false;
    QTextStream out(&f);
    for (auto &l : lines) out << l << "\n";
    return true;
}

QVector<QString> DataStore::loadGrades() {
    QVector<QString> out;
    QFile f(gradesFile());
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return out;
    QTextStream in(&f);
    while (!in.atEnd()) out.append(in.readLine());
    return out;
}

bool DataStore::addGrade(const QString &className, const QString &student, const QString &subject, const QString &date, const QString &grade) {
    QFile f(gradesFile());
    if (!f.open(QIODevice::Append | QIODevice::Text)) return false;
    QTextStream out(&f);
    out << className << "|" << student << "|" << subject << "|" << date << "|" << grade << "\n";
    return true;
}

bool DataStore::overwriteGrades(const QVector<QString> &lines) {
    QFile f(gradesFile());
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return false;
    QTextStream out(&f);
    for (auto &l : lines) out << l << "\n";
    return true;
}
