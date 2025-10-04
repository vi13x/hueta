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
QString DataStore::classesFile() { return QDir::currentPath() + "/classes.txt"; }

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

// Classes and students management
QVector<QString> DataStore::getClasses() {
    QVector<QString> classes;
    QFile f(classesFile());
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Если файл не существует, создаем базовые классы
        classes = {"1A", "1B", "2A", "2B", "3A", "3B"};
        QFile createFile(classesFile());
        if (createFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&createFile);
            for (const QString &cls : classes) {
                out << cls << "\n";
            }
        }
        return classes;
    }
    
    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) classes.append(line);
    }
    return classes;
}

QVector<QString> DataStore::getStudentsForClass(const QString &className) {
    QVector<QString> students;
    QFile f(studentsFile());
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return students;
    
    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        
        // Формат: username:password:class
        QStringList parts = line.split(':');
        if (parts.size() >= 3 && parts[2] == className) {
            students.append(parts[0]);
        }
    }
    
    // Если нет студентов для класса, создаем демо-данные
    if (students.isEmpty()) {
        if (className == "1A") {
            students = {"Иванов Иван", "Петров Петр", "Сидоров Сидор", "Козлова Анна"};
        } else if (className == "1B") {
            students = {"Анна Смирнова", "Ольга Волкова", "Мария Новикова"};
        } else if (className == "2A") {
            students = {"Алексей Морозов", "Дмитрий Лебедев", "Елена Соколова"};
        } else if (className == "2B") {
            students = {"Николай Попов", "Татьяна Федорова"};
        } else if (className == "3A") {
            students = {"Андрей Морозов", "Екатерина Волкова", "Сергей Новиков"};
        } else if (className == "3B") {
            students = {"Наталья Соколова", "Владимир Лебедев"};
        }
    }
    
    return students;
}

bool DataStore::addStudentToClass(const QString &className, const QString &studentName) {
    QFile f(studentsFile());
    if (!f.open(QIODevice::Append | QIODevice::Text)) return false;
    QTextStream out(&f);
    out << studentName << ":password123:" << className << "\n";
    return true;
}

bool DataStore::removeStudentFromClass(const QString &className, const QString &studentName) {
    QFile f(studentsFile());
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    
    QVector<QString> lines;
    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(':');
        if (parts.size() >= 3 && parts[0] == studentName && parts[2] == className) {
            continue; // Пропускаем эту строку
        }
        lines.append(line);
    }
    f.close();
    
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return false;
    QTextStream out(&f);
    for (const QString &line : lines) {
        out << line << "\n";
    }
    return true;
}

// Schedule generation
void DataStore::generateDefaultSchedule() {
    QVector<QString> schedule = {
        "Понедельник: Математика (9:00-10:30), Русский язык (11:00-12:30), Физика (13:30-15:00)",
        "Вторник: История (9:00-10:30), Литература (11:00-12:30), Химия (13:30-15:00)",
        "Среда: География (9:00-10:30), Биология (11:00-12:30), Информатика (13:30-15:00)",
        "Четверг: Английский язык (9:00-10:30), Обществознание (11:00-12:30), Физкультура (13:30-15:00)",
        "Пятница: Математика (9:00-10:30), Русский язык (11:00-12:30), Искусство (13:30-15:00)",
        "Суббота: Выходной",
        "Воскресенье: Выходной"
    };
    
    saveSchedule(schedule);
}

QVector<QString> DataStore::getSubjects() {
    return {"Математика", "Русский язык", "Физика", "История", "Литература", 
            "Химия", "География", "Биология", "Информатика", "Английский язык", 
            "Обществознание", "Физкультура", "Искусство"};
}

QVector<QString> DataStore::getTeachers() {
    QVector<QString> teachers;
    QFile f(teachersFile());
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Создаем демо-учителей
        teachers = {"Иванова А.И.", "Петров П.П.", "Сидорова С.С.", "Козлов К.К.", "Волкова В.В."};
        QFile createFile(teachersFile());
        if (createFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&createFile);
            for (const QString &teacher : teachers) {
                out << teacher << ":teacher123\n";
            }
        }
        return teachers;
    }
    
    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        QStringList parts = line.split(':');
        if (parts.size() >= 1) {
            teachers.append(parts[0]);
        }
    }
    return teachers;
}
