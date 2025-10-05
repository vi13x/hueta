#include "datastore.h"
#include "studentview.h"
#include "teacherview.h"
#include "adminview.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

DataStore& DataStore::getInstance() {
    static DataStore instance;
    return instance;
}

bool DataStore::addUser(std::shared_ptr<User> user) {
    if (findUser(user->getUsername())) {
        return false; // User already exists
    }
    users.append(user);
    return true;
}

std::shared_ptr<User> DataStore::findUser(const QString& username) {
    for (auto& user : users) {
        if (user->getUsername() == username) {
            return user;
        }
    }
    return nullptr;
}

bool DataStore::authenticateUser(const QString& username, const QString& password) {
    auto user = findUser(username);
    return user && user->getPassword() == password;
}

QList<std::shared_ptr<User>> DataStore::getAllUsers() const {
    return users;
}

QList<std::shared_ptr<Student>> DataStore::getStudents() const {
    QList<std::shared_ptr<Student>> students;
    for (auto& user : users) {
        if (user->getRole() == UserRole::Student) {
            auto student = std::dynamic_pointer_cast<Student>(user);
            if (student) {
                students.append(student);
            }
        }
    }
    return students;
}

std::shared_ptr<Student> DataStore::findStudent(const QString& username) {
    auto user = findUser(username);
    if (user && user->getRole() == UserRole::Student) {
        return std::dynamic_pointer_cast<Student>(user);
    }
    return nullptr;
}

QList<std::shared_ptr<Teacher>> DataStore::getTeachers() const {
    QList<std::shared_ptr<Teacher>> teachers;
    for (auto& user : users) {
        if (user->getRole() == UserRole::Teacher) {
            auto teacher = std::dynamic_pointer_cast<Teacher>(user);
            if (teacher) {
                teachers.append(teacher);
            }
        }
    }
    return teachers;
}

std::shared_ptr<Teacher> DataStore::findTeacher(const QString& username) {
    auto user = findUser(username);
    if (user && user->getRole() == UserRole::Teacher) {
        return std::dynamic_pointer_cast<Teacher>(user);
    }
    return nullptr;
}

QList<std::shared_ptr<Admin>> DataStore::getAdmins() const {
    QList<std::shared_ptr<Admin>> admins;
    for (auto& user : users) {
        if (user->getRole() == UserRole::Admin) {
            auto admin = std::dynamic_pointer_cast<Admin>(user);
            if (admin) {
                admins.append(admin);
            }
        }
    }
    return admins;
}

std::shared_ptr<Admin> DataStore::findAdmin(const QString& username) {
    auto user = findUser(username);
    if (user && user->getRole() == UserRole::Admin) {
        return std::dynamic_pointer_cast<Admin>(user);
    }
    return nullptr;
}

void DataStore::addMark(const QString& studentUsername, const QString& subject, int mark) {
    marks[studentUsername][subject].append(mark);
}

QMap<QString, QList<int>> DataStore::getStudentMarks(const QString& studentUsername) const {
    return marks.value(studentUsername);
}

QMap<QString, QMap<QString, QList<int>>> DataStore::getAllMarks() const {
    return marks;
}

void DataStore::saveData() {
    // Simple file-based storage
    QFile file("data.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (auto& user : users) {
            out << user->getUsername() << "|" 
                << user->getPassword() << "|" 
                << static_cast<int>(user->getRole()) << "\n";
        }
    }
}

void DataStore::loadData() {
    QFile file("data.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split("|");
            if (parts.size() == 3) {
                QString username = parts[0];
                QString password = parts[1];
                UserRole role = static_cast<UserRole>(parts[2].toInt());
                
                std::shared_ptr<User> user;
                switch (role) {
                    case UserRole::Student:
                        user = std::make_shared<Student>(username, password);
                        break;
                    case UserRole::Teacher:
                        user = std::make_shared<Teacher>(username, password);
                        break;
                    case UserRole::Admin:
                        user = std::make_shared<Admin>(username, password);
                        break;
                }
                if (user) {
                    addUser(user);
                }
            }
        }
    }
}

void DataStore::initDemoData() {
    // Add demo users
    addUser(std::make_shared<Admin>("admin", "admin123"));
    addUser(std::make_shared<Teacher>("teacher1", "teacher123"));
    addUser(std::make_shared<Teacher>("teacher2", "teacher123"));
    addUser(std::make_shared<Student>("student1", "student123"));
    addUser(std::make_shared<Student>("student2", "student123"));
    addUser(std::make_shared<Student>("student3", "student123"));
    
    // Add demo marks
    addMark("student1", "Математика", 5);
    addMark("student1", "Математика", 4);
    addMark("student1", "Физика", 5);
    addMark("student2", "Математика", 3);
    addMark("student2", "Физика", 4);
    addMark("student3", "Математика", 5);
    addMark("student3", "Химия", 4);
}
